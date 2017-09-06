#include "LiveView_Widget.h"
#include "ui_LiveView_Widget.h"
#include "SonyAlphaRemote_Helper.h"
#include "LiveView_Info.h"

#include <QMutexLocker>
#include <QTime>

using namespace SonyAlphaRemote;

namespace LiveView {


void Widget::setSender(SonyAlphaRemote::Sender *value)
{
    sender = value;
}

void Widget::setSettings(Settings *value)
{
    if(settings)
        disconnect(settings, SIGNAL(settingChanged()), this, SLOT(updateSettings()));
    settings = value;
    if(settings)
    connect(settings, SIGNAL(settingChanged()), this, SLOT(updateSettings()));
}

void Widget::updateSettings()
{
    if(!settings)
        return;

    float fps = settings->getFps();
    SAR_INF("fps: " << fps);
    pollImageTimer->setInterval(1000 / fps);
    ui->fpsSpinBox->setValue(fps);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , settings(NULL)
    , startLiveView(new StartLiveView(this))
    , stopLiveView(new StopLiveView(this))
    , sender(NULL)
    , imageQueue(new ImageQueue(this))
    , pollImageTimer(new QTimer(this))
    , readerThread(NULL)
    , lastTimeStamp(QTime::currentTime())
    , frameCount(0)

{
    ui->setupUi(this);
    connect(startLiveView, SIGNAL(newLiveViewUrl(QString)), this, SLOT(startReaderThread(QString)));
    connect(stopLiveView, SIGNAL(liveViewStopped()), this, SLOT(stopReaderThread()));
    connect(pollImageTimer, SIGNAL(timeout()), this, SLOT(updateLiveViewImage()));
}

Widget::~Widget()
{
    SAR_INF("dtor");
    stopReaderThread();
    delete ui;
}

void Widget::start()
{
    Q_ASSERT(sender);
    if(!sender)
        SAR_ERR("no sender available!!!");
    sender->send(startLiveView);
}

void Widget::stop()
{
    Q_ASSERT(sender);
    if(!sender)
        SAR_ERR("no sender available!!!");
    sender->send(stopLiveView);
}

float Widget::calcFps()
{

    float fps = 0;
    QTime now = QTime::currentTime();
    fps = (float)(1000.0 / (float)lastTimeStamp.msecsTo(now));
    lastTimeStamp = QTime::currentTime();

    return fps;
}

void Widget::updateLiveViewImage()
{

    PayloadPtr data = imageQueue->pop();
    if(!data)
        return;


    Info metaInfo;
    metaInfo.setFps(calcFps());
    metaInfo.setFrameCount(frameCount++);
    ui->metaInfo->setText(metaInfo.toHtml());

    QPixmap pixmap = QPixmap::fromImage(QImage::fromData(data->payload, "JPG"));
    ui->liveViewImage->setPixmap(pixmap);

}

void Widget::startReaderThread(QString url)
{
    if(readerThread)
    {
        readerThread->quit();
        readerThread->wait(30 * 1000);
        delete readerThread;
    }


    readerThread = new ReaderThread(url, this);
    connect(readerThread, SIGNAL(newPayload(PayloadPtr)), imageQueue, SLOT(push(PayloadPtr)));

    readerThread->start();

    Q_ASSERT(settings);
    if(settings)
        pollImageTimer->start(1000 / settings->getFps());
    else
        pollImageTimer->start(200);

    SAR_INF("started reader thread URL(" << url << ")");

}

void Widget::stopReaderThread()
{
    pollImageTimer->stop();

    if(readerThread)
    {
        readerThread->quit();
        if(!readerThread->wait(30 * 1000))
            SAR_WRN("could not stop reader thread!");
        else
            SAR_INF("stopped reader thread");

        delete readerThread;
        readerThread = 0;

    }
    else
    {
        SAR_ERR("no reader thread exists!");
    }

}

} // namespace LiveView

void LiveView::Widget::on_fpsSpinBox_valueChanged(double fps)
{
    pollImageTimer->setInterval(1000 / fps);
    Q_ASSERT(settings);
    if(settings)
        settings->setFps(fps);
}
