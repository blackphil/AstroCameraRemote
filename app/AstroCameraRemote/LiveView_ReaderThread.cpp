#include "LiveView_ReaderThread.h"
#include "SonyAlphaRemote_Helper.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QList>

namespace LiveView {

ReaderThreadBase::ReaderThreadBase(QObject* parent)
    : QThread(parent)
{}

ReaderThreadBase::~ReaderThreadBase()
{}

ReaderThread::ReaderThread(const QString &url, QObject *parent)
    : ReaderThreadBase(parent)
    , reader(nullptr)
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
    reader = nullptr;
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
    reader = nullptr;

    return;

}

DummyReaderThread::DummyReaderThread(QObject* parent)
    : ReaderThreadBase (parent)
    , cursor(0)
{
}

void DummyReaderThread::emitPayload()
{
    Q_ASSERT(0 <= cursor && cursor < payloadItems.count());
    if(0 > cursor || cursor >= payloadItems.count())
        return;

    Q_EMIT newPayload(payloadItems[cursor]);
    cursor = (cursor+1) % payloadItems.count();
}

void DummyReaderThread::run()
{

    QTimer* trigger = new QTimer();
    connect(trigger, SIGNAL(timeout()), this, SLOT(emitPayload()));


    QDir dir = QDir(":/hfd/sequence");
    QStringList entries = dir.entryList(QDir::Files, QDir::Time);
    for(int i=0; i<entries.count(); i++)
    {
        QString fp = QFileInfo(dir, entries[i]).absoluteFilePath();
        QFile f(fp);
        f.open(QIODevice::ReadOnly);
        QByteArray data = f.readAll();
        QByteArray paddingData; //not used
        payloadItems << LiveView::PayloadPtr(new LiveView::Payload(data, paddingData));
    }

    trigger->start(50);

    QThread::run();

    delete trigger;

}


} // namespace LiveView
