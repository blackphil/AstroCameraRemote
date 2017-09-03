#include "LiveView_ThreadInfo.h"

#include <QMutexLocker>

namespace LiveView {

QByteArray ThreadInfo::JpegQueue::pop()
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);

    QByteArray data;
    if(0 < q.size())
        return q.takeFirst();

    return QByteArray();
}

void ThreadInfo::JpegQueue::push(QByteArray data)
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);

    if(2 == q.size())
        q.pop_front();

    q.push_back(data);
}

int ThreadInfo::JpegQueue::size() const
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);
    return q.size();
}

void ThreadInfo::JpegQueue::clear()
{
    QMutexLocker lock(&m);
    Q_UNUSED(lock);
    q.clear();
}

} // namespace LiveView
