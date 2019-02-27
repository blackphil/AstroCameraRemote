#include "LiveView_Widget.h"
#include "ui_LiveView_Widget.h"
#include "AstroBase.h"
#include "LiveView_Info.h"

#include <QMutexLocker>
#include <QTime>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>

using namespace SonyAlphaRemote;

namespace LiveView {


void Widget::setSender(SonyAlphaRemote::Sender *value)
{
    sender = value;
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , startLiveView(new StartLiveView(this))
    , stopLiveView(new StopLiveView(this))
    , sender(Q_NULLPTR)
    , imageQueue(new ImageQueue(this))
    , pollImageTimer(new QTimer(this))
    , readerThread(Q_NULLPTR)
    , lastTimeStamp(QTime::currentTime())
    , starTrackScene(new StarTrack::GraphicsScene(this))
    , frameCount(0)

{
    AB_INF("ctor");
    ui->setupUi(this);

    ui->graphicsView->setScene(starTrackScene);

    connect(startLiveView, SIGNAL(newLiveViewUrl(QString)), this, SLOT(startReaderThread(QString)));
    connect(stopLiveView, SIGNAL(liveViewStopped()), this, SLOT(stopReaderThread()));
    connect(pollImageTimer, SIGNAL(timeout()), this, SLOT(updateLiveViewImage()));
}

Widget::~Widget()
{
    AB_INF("dtor");
    stopReaderThread();
    delete ui;
}

void Widget::start()
{
    Q_ASSERT(sender);
    if(!sender)
        AB_ERR("no sender available!!!");
    sender->send(startLiveView);
}

void Widget::stop()
{
    Q_ASSERT(sender);
    if(!sender)
        AB_ERR("no sender available!!!");
    sender->send(stopLiveView);
    stopReaderThread();
}

StarTrack::GraphicsScene *Widget::getStarTrackScene() const
{
    return starTrackScene;
}

int Widget::calcFps()
{

    float fps = 0;
    QTime now = QTime::currentTime();
    fps = 1000.0f / lastTimeStamp.msecsTo(now);
    lastTimeStamp = QTime::currentTime();

    return qRound(fps);
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
    starTrackScene->updateBackground(pixmap);
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


    pollImageTimer->start(1000 / Settings::getFps());

    AB_INF("started reader thread URL(" << url << ")");

}

void Widget::stopReaderThread()
{
    pollImageTimer->stop();

    if(readerThread)
    {
        readerThread->quit();
        if(!readerThread->wait(30 * 1000))
            AB_WRN("could not stop reader thread!");
        else
            AB_INF("stopped reader thread");

        delete readerThread;
        readerThread = nullptr;

    }
    else
    {
        AB_ERR("no reader thread exists!");
    }

}

void LiveView::Widget::on_fpsSpinBox_valueChanged(int fps)
{
    pollImageTimer->setInterval(1000 / fps);
    Settings::setFps(fps);
}

} // namespace LiveView


