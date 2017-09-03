#ifndef LIVEVIEW_READERTHREAD_H
#define LIVEVIEW_READERTHREAD_H

#include <QObject>

#include "LiveView_Reader.h"
#include "LiveView_ThreadInfo.h"

namespace LiveView {

class ReaderThread : public QThread
{

    Q_OBJECT

    Reader* reader;
    QString url;
    bool running;
    ThreadInfo* threadInfo;


public :
    ReaderThread(const QString& url,ThreadInfo* threadInfo, QObject* parent = 0);
    ~ReaderThread();

private :
    void run();

Q_SIGNALS :
    void newPayload(PayloadPtr payload);
};



} // namespace LiveView

#endif // LIVEVIEW_READERTHREAD_H
