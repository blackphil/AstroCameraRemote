#ifndef LIVEVIEW_LIVEVIEW_WIDGET_H
#define LIVEVIEW_LIVEVIEW_WIDGET_H

#include <QWidget>
#include <QThread>
#include <QQueue>
#include <QByteArray>

#include "LiveView_Commands.h"
#include "LiveView_Reader.h"
#include "SonyAlphaRemote_Sender.h"
#include "LiveView_ThreadInfo.h"
#include "LiveView_ReaderThread.h"

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
    SonyAlphaRemote::Sender* sender;
    ThreadInfo threadInfo;



    ReaderThread* readerThread;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void setSender(SonyAlphaRemote::Sender *value);


public Q_SLOTS :
    void start();
    void stop();

    void updateLiveViewImage(const PayloadPtr &data);


private Q_SLOTS :
    void startReaderThread(QString url);
    void stopReaderThread();
};


} // namespace LiveView
#endif // LIVEVIEW_LIVEVIEW_WIDGET_H
