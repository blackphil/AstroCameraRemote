#ifndef LIVEVIEW_READERTHREAD_H
#define LIVEVIEW_READERTHREAD_H

#include <QObject>

#include "LiveView_Reader.h"
#include "LiveView_ImageQueue.h"

namespace LiveView {

class ReaderThread : public QThread
{

    Q_OBJECT

    Reader* reader;
    QString url;


public :
    ReaderThread(const QString& url, QObject* parent = 0);
    ~ReaderThread();

private :
    void run();

Q_SIGNALS :
    void newPayload(PayloadPtr payload);
};



} // namespace LiveView

#endif // LIVEVIEW_READERTHREAD_H
