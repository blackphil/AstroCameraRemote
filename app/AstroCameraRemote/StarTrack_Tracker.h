#ifndef STARTRACK_TRACKER_H
#define STARTRACK_TRACKER_H

#include <QRectF>
#include <QImage>
#include <QSharedPointer>

class QPixmap;


namespace StarTrack {

class Tracker
{

    QRectF currentRect;
    QImage currentStar, scaledStar;

    qreal hfd;

    QRectF centerStar();

    Tracker(const Tracker&);

public:
    Tracker();

    void setRect(const QRectF& r);
    QRectF getRect() const;
    bool update(const QPixmap& pm);
    QImage getCurrentStar() const;
    QImage getScaledStar() const;
    qreal getHfd() const;
};

typedef QSharedPointer<Tracker> TrackerPtr;

} // namespace StarTrack

#endif // STARTRACK_TRACKER_H
