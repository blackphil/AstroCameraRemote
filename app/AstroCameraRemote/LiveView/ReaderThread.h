#ifndef LIVEVIEW_READERTHREAD_H
#define LIVEVIEW_READERTHREAD_H

#include <QObject>
#include <QTimer>

#include "Reader.h"
#include "ImageQueue.h"

namespace LiveView {

class ReaderThreadBase : public QThread
{
    Q_OBJECT

public :
    ReaderThreadBase(QObject* parent = nullptr);
    virtual ~ReaderThreadBase();

Q_SIGNALS :
    void newPayload(PayloadPtr);
};

class ReaderThread : public ReaderThreadBase
{

    Reader* reader;
    QString url;


public :
    ReaderThread(const QString& url, QObject* parent = nullptr);
    ~ReaderThread();

private :
    void run();

};

class DummyReaderThread : public ReaderThreadBase
{
    Q_OBJECT

    QList<LiveView::PayloadPtr> payloadItems;
    int cursor;


public :
    DummyReaderThread(QObject* parent = nullptr);

private Q_SLOTS :
    void emitPayload();

private :
    void run();

};



} // namespace LiveView

#endif // LIVEVIEW_READERTHREAD_H
