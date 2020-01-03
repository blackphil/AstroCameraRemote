#ifndef LIVEVIEW_IMAGEQUEUE_H
#define LIVEVIEW_IMAGEQUEUE_H

#include <QQueue>
#include <QByteArray>
#include <QMutex>
#include <QObject>
#include <QSharedPointer>

namespace LiveView {


struct Payload
{
    QByteArray payload;
    QByteArray paddingData;

    Payload(const QByteArray& payload, const QByteArray& paddingData)
        : payload { payload }
        , paddingData { paddingData }
    {

    }
};
typedef QSharedPointer<Payload> PayloadPtr;

class ImageQueue : public QObject
{
    Q_OBJECT

public :
    ImageQueue(QObject* parent = nullptr);

private :

    QQueue<PayloadPtr> q;
    mutable QMutex m;

public :
    PayloadPtr pop();
    int size() const;

public Q_SLOTS :
    void push(PayloadPtr data);
    void clear();

};

} // namespace LiveView

#endif // LIVEVIEW_IMAGEQUEUE_H
