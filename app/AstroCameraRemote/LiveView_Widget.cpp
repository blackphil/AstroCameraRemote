#include "LiveView_Widget.h"
#include "ui_LiveView_Widget.h"
#include <AstroBase/AstroBase>
#include "LiveView_Info.h"

#include <QMutexLocker>
#include <QTime>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>



namespace LiveView {




Widget::Widget(QWidget *parent)
    : QWidget { parent }
    , ui { new Ui::Widget }
    , startLiveView { new StartLiveView { this }  }
    , stopLiveView { new StopLiveView { this }  }
    , imageQueue { new ImageQueue { this }  }
    , pollImageTimer { new QTimer { this }  }
    , readerThread { nullptr }
    , lastTimeStamp { QTime::currentTime() }
    , starTrackScene { new StarTrack::GraphicsScene { this }  }
    , frameCount { 0 }
    , debugModeEnabled { false }
{
    AB_INF("ctor");
    ui->setupUi(this);

    ui->graphicsView->setScene(starTrackScene);
    starTrackScene->setEnabled(false);

    ui->zoomControl->setup(ui->graphicsView);

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
    ui->graphicsView->fitToWindow(ui->zoomControl->fitInWindowEnabled());
    starTrackScene->setEnabled(true);
    Sender::get()->send(startLiveView);
}

void Widget::stop()
{
    starTrackScene->setEnabled(false);
    Sender::get()->send(stopLiveView);
    stopReaderThread();
}

StarTrack::GraphicsScene *Widget::getStarTrackScene() const
{
    return starTrackScene;
}

int Widget::calcFps()
{

    QTime now { QTime::currentTime() };
    int fps { qRound(1000.0f / lastTimeStamp.msecsTo(now)) };
    lastTimeStamp = now;

    return fps;
}

void Widget::resizeEvent(QResizeEvent *re)
{
    Q_UNUSED(re)

    if(ui->zoomControl->fitInWindowEnabled())
    {
        ui->graphicsView->fitToWindow(true);
    }
    QWidget::resizeEvent(re);
}

void Widget::updateLiveViewImage()
{

    if(PayloadPtr data { imageQueue->pop() }; data != nullptr)
    {


        Info metaInfo;
        metaInfo.setFps(calcFps());
        metaInfo.setFrameCount(frameCount++);
        ui->metaInfo->setText(metaInfo.toHtml());

        QPixmap pixmap { QPixmap::fromImage(QImage::fromData(data->payload, "JPG")) };
        Q_ASSERT(!pixmap.isNull());
        starTrackScene->updateBackground(pixmap);
    }
}

void Widget::startReaderThread(const QString& url)
{
    this->url = url;
    if(!debugModeEnabled)
        startReaderThread();
}

void Widget::startReaderThread()
{
    if(readerThread)
    {
        readerThread->quit();
        readerThread->wait(30 * 1000);
        delete readerThread;
    }

    if(debugModeEnabled)
        readerThread = new DummyReaderThread(this);
    else if(!url.isEmpty())
        readerThread = new ReaderThread(url, this);

    if(!readerThread)
        return;

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

void Widget::on_fpsSpinBox_valueChanged(int fps)
{
    pollImageTimer->setInterval(1000 / fps);
    Settings::setFps(fps);
}

void Widget::on_debugModeCb_toggled(bool checked)
{
    debugModeEnabled = checked;
    stopReaderThread();
    startReaderThread();
}

void Widget::on_setRefBtn_clicked()
{
    starTrackScene->setReference();
}

} // namespace LiveView




