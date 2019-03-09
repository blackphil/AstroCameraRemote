#ifndef LIVEVIEW_LIVEVIEW_WIDGET_H
#define LIVEVIEW_LIVEVIEW_WIDGET_H

#include <QWidget>
#include <QThread>
#include <QQueue>
#include <QByteArray>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "LiveView_Commands.h"
#include "LiveView_Reader.h"
#include "SonyAlphaRemote_Sender.h"
#include "LiveView_ImageQueue.h"
#include "LiveView_ReaderThread.h"
#include "LiveView_Settings.h"
#include "StarTrack_GraphicsScene.h"

namespace LiveView {

namespace Ui {
class Widget;
}



class Widget : public QWidget
{
    Q_OBJECT

    Ui::Widget *ui;

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

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    StarTrack::GraphicsScene *getStarTrackScene() const;

public Q_SLOTS :
    void start();
    void stop();

    void updateLiveViewImage();


private Q_SLOTS :
    void startReaderThread(QString url);
    void startReaderThread();
    void stopReaderThread();

    void on_fpsSpinBox_valueChanged(int fps);
    void on_debugModeCb_toggled(bool checked);
    void on_setRefBtn_clicked();
};


} // namespace LiveView
#endif // LIVEVIEW_LIVEVIEW_WIDGET_H
