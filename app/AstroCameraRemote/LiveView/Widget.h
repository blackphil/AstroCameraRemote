#ifndef LIVEVIEW_LIVEVIEW_WIDGET_H
#define LIVEVIEW_LIVEVIEW_WIDGET_H

#include <QWidget>
#include <QThread>
#include <QQueue>
#include <QByteArray>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "Commands.h"
#include "Reader.h"
#include "../Sender.h"
#include "ImageQueue.h"
#include "ReaderThread.h"
#include "Settings.h"
#include "../StarTrack/GraphicsScene.h"

namespace Ui {
	class LiveView_Widget;
}

namespace LiveView {





class Widget : public QWidget
{
    Q_OBJECT

   ::Ui::LiveView_Widget *ui;

    StartLiveView* startLiveView;
    StopLiveView* stopLiveView;

    ImageQueue* imageQueue;
    QTimer* pollImageTimer;

    ReaderThreadBase* readerThread;
    QTime lastTimeStamp;
    StarTrack::GraphicsScene* starTrackScene;

    int frameCount;

    bool debugModeEnabled;
    QString url;

    int calcFps();

    void resizeEvent(QResizeEvent* re);

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    StarTrack::GraphicsScene *getStarTrackScene() const;

public Q_SLOTS :
    void start();
    void stop();

    void updateLiveViewImage();


private Q_SLOTS :
    void startReaderThread(const QString &url);
    void startReaderThread();
    void stopReaderThread();

    void on_fpsSpinBox_valueChanged(int fps);
    void on_debugModeCb_toggled(bool checked);
    void on_setRefBtn_clicked();
};


} // namespace LiveView
#endif // LIVEVIEW_LIVEVIEW_WIDGET_H
