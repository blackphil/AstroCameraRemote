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

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , startLiveView(new StartLiveView(this))
    , stopLiveView(new StopLiveView(this))
    , sender(NULL)
    , readerThread(NULL)

{
    ui->setupUi(this);
    connect(startLiveView, SIGNAL(newLiveViewUrl(QString)), this, SLOT(startReaderThread(QString)));
    connect(stopLiveView, SIGNAL(liveViewStopped()), this, SLOT(stopReaderThread()));
}

Widget::~Widget()
{
    SAR_INF("dtor");
    if(threadInfo.running)
    {
        stopReaderThread();
    }
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

void Widget::updateLiveViewImage(const PayloadPtr &data)
{
    static float fps = 0;
    static int frameCount = 0;

    static QTime lastTimeStamp = QTime::currentTime();

    QTime now = QTime::currentTime();


    fps = (float)(1000.0 / (float)lastTimeStamp.msecsTo(now));

    lastTimeStamp = now;
    Info metaInfo;
    metaInfo.setFps(fps);
    metaInfo.setFrameCount(frameCount++);
    ui->metaInfo->setText(metaInfo.toHtml());

    QPixmap pixmap = QPixmap::fromImage(QImage::fromData(data->payload, "JPG"));
    ui->liveViewImage->setPixmap(pixmap);

}

void Widget::startReaderThread(QString url)
{
    if(readerThread)
    {
        SAR_ERR("reader thread already exists!");
        return;
    }


    readerThread = new ReaderThread(url, &threadInfo, this);
    connect(readerThread, SIGNAL(newPayload(PayloadPtr)), this, SLOT(updateLiveViewImage(PayloadPtr)));

    threadInfo.running = true;
    readerThread->start(QThread::LowestPriority);


    SAR_INF("started reader thread URL(" << url << ")");

}

void Widget::stopReaderThread()
{
    threadInfo.running = false;
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
