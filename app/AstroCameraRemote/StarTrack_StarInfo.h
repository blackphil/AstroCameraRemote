#ifndef STARTRACK_STARINFO_H
#define STARTRACK_STARINFO_H

#include <QSizeF>
#include <QPointF>
#include <QSharedPointer>


namespace StarTrack {

struct StarInfo
{
    QSizeF window;
    QPointF centerPosition;
    qreal hfd;
    int index;

    StarInfo();
};

typedef QSharedPointer<StarInfo> StarInfoPtr;

} // namespace StarTrack

#endif // STARTRACK_STARINFO_H
