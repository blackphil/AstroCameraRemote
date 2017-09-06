#include "LiveView_ImageQueue.h"

#include <QMutexLocker>

namespace LiveView {

ImageQueue::ImageQueue(QObject *parent)
    : QObject(parent)
{

}

PayloadPtr ImageQueue::pop()
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);

    if(0 < q.size())
        return q.takeFirst();

    return PayloadPtr();
}

void ImageQueue::push(PayloadPtr data)
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);

    if(2 == q.size())
        q.pop_front();

    q.push_back(data);
}

int ImageQueue::size() const
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);
    return q.size();
}

void ImageQueue::clear()
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);
    q.clear();
}

} // namespace LiveView
