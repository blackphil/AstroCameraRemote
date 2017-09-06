#ifndef LIVEVIEW_LIVEVIEW_WIDGET_H
#define LIVEVIEW_LIVEVIEW_WIDGET_H

#include <QWidget>
#include <QThread>
#include <QQueue>
#include <QByteArray>
#include <QTimer>

#include "LiveView_Commands.h"
#include "LiveView_Reader.h"
#include "SonyAlphaRemote_Sender.h"
#include "LiveView_ImageQueue.h"
#include "LiveView_ReaderThread.h"
#include "LiveView_Settings.h"

namespace LiveView {

namespace Ui {
class Widget;
}



class Widget : public QWidget
{
    Q_OBJECT

    Ui::Widget *ui;

    Settings* settings;

    StartLiveView* startLiveView;
    StopLiveView* stopLiveView;
    SonyAlphaRemote::Sender* sender;

    ImageQueue* imageQueue;
    QTimer* pollImageTimer;

    ReaderThread* readerThread;
    QTime lastTimeStamp;
    int frameCount;

    float calcFps();

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void setSender(SonyAlphaRemote::Sender *value);


    void setSettings(Settings *value);

public Q_SLOTS :
    void start();
    void stop();

    void updateLiveViewImage();


private Q_SLOTS :
    void startReaderThread(QString url);
    void stopReaderThread();
    void on_fpsSpinBox_valueChanged(double fps);
    void updateSettings();
};


} // namespace LiveView
#endif // LIVEVIEW_LIVEVIEW_WIDGET_H
