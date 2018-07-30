#include "LiveView_ReaderThread.h"
#include "SonyAlphaRemote_Helper.h"

namespace LiveView {

ReaderThread::ReaderThread(const QString &url, QObject *parent)
    : QThread(parent)
    , reader(NULL)
    , url(url)
{
    setObjectName("ReaderThread");
}

ReaderThread::~ReaderThread()
{
    if(reader)
    {
        reader->close();
        delete reader;
    }
    reader = NULL;
}

void ReaderThread::run()
{
    if(reader)
    {
        reader->close();
        delete reader;
    }

    reader = new Reader();
    connect(reader, SIGNAL(newPayload(PayloadPtr)), this, SIGNAL(newPayload(PayloadPtr)));
    reader->open(url);

    QThread::run();

    reader->close();
    delete reader;
    reader = NULL;

    return;

}


} // namespace LiveView
