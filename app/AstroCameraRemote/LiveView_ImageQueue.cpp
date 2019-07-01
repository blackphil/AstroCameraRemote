#include "LiveView_ImageQueue.h"

#include <QMutexLocker>

namespace LiveView {

ImageQueue::ImageQueue(QObject *parent)
    : QObject { parent }
{

}

PayloadPtr ImageQueue::pop()
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);

    return 0 < q.size() ? q.takeFirst() : nullptr;
}

void ImageQueue::push(PayloadPtr data)
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);

    while(1 < q.size())
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
