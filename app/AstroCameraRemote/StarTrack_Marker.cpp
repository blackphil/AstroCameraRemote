#include "StarTrack_Marker.h"

#include <QBrush>
#include <QRect>

#include "AstroBase.h"
#include "StarTrack_Settings.h"
#include "StarTrack_GraphicsScene.h"

namespace StarTrack {


Marker::Marker(GraphicsScene *scene, QObject *parent)
    : QObject(parent)
    , scene(scene)
    , rectItem(Q_NULLPTR)
    , info(Q_NULLPTR)
    , pen(QPen(QBrush(Qt::green), 1))
    , tracking(true )
    , status(Status_Idle)
    , isSelected(false)
{

    connect(this, SIGNAL(newMark()), scene, SLOT(newMark()));
    connect(scene, SIGNAL(starTrackingEnabled(bool)), this, SLOT(setTracking(bool)));

    rectItem = scene->addRect(QRectF(), pen);
    rectItem->setZValue(2);

    for(int i=0; i<2; i++)
    {
        crosshair[i] = scene->addLine(QLineF(), pen);
        crosshair[i]->setZValue(2);
    }

    info = scene->addSimpleText("");
    info->setBrush(QBrush(Qt::green));
    info->setZValue(2);

    lineFromRef = scene->addLine(QLineF(), QPen(QBrush(Qt::red), 2));
    lineFromRef->setZValue(3);
}

QRectF Marker::getRect() const
{
    if(rectItem)
        return rectItem->rect();
    return QRectF();
}

namespace helper
{
void logRect(const QRectF& r, const QString& context)
{
    Q_UNUSED(r)
    Q_UNUSED(context)
    AB_INF(
                context <<
                ": center(" << r.center().x() << ", " << r.center().y() << ")"
                ", size(" << r.width() << ", " << r.height() << ")");
}
}

bool Marker::getTracking() const
{
    return tracking;
}

const Tracker &Marker::getTracker() const
{
    return tracker;
}

void Marker::setTracking(bool value)
{
    tracking = value;
}

bool Marker::update(const QRectF& r)
{
    if(r == rectItem->rect())
        return false;

    rectItem->setRect(r);

    helper::logRect(r, "bounding");
    auto w = r.width() / 4.0;
    auto h = r.height() / 4.0;
    QRectF chr = r.marginsRemoved(QMarginsF(w, h, w, h));

    AB_INF("update marker ...");
    helper::logRect(chr, "crosshair");

    crosshair[0]->setLine(QLineF(chr.center().x(), chr.top(), chr.center().x(), chr.bottom()));
    crosshair[1]->setLine(QLineF(chr.left(), chr.center().y(), chr.right(), chr.center().y()));

    if(!referencePos.isNull())
    {
        QPointF currentCenter = r.center();
        lineFromRef->setLine(QLineF(referencePos.x(), referencePos.y(), currentCenter.x(), currentCenter.y()));
    }

    info->setPos(rectItem->rect().topLeft() + QPointF(0, -info->boundingRect().height()));

    return true;
}

void Marker::start(const QPointF &pos)
{

    QRectF r;
    switch(Settings::getMarkerModus())
    {
    case Modus_FixedRect :
    {
        auto rectSize = Settings::getFixedRectSize();
        auto rectSizeHalf = rectSize / 2.;
        r = QRectF(pos.x() - rectSizeHalf, pos.y() - rectSizeHalf, rectSize, rectSize);
        break;
    }
    case Modus_Rubberband :
        r = QRectF(pos.x(), pos.y(), 1, 1);
        break;
    }

    startPos = pos;
    rectItem->setRect(r);

    status = Status_Moving;
}

void Marker::finish()
{
    tracker.setRect(getRect());
    Q_EMIT newMark();

}

void Marker::mouseMoved(const QPointF &pos)
{
    if(Status_Moving != status)
        return;

    QRectF r = rectItem->rect();

    switch(Settings::getMarkerModus())
    {
    case Modus_FixedRect :
    {
        qreal rectSize = Settings::getFixedRectSize();
        qreal rectSizeHalf = rectSize / 2.;
        r = QRectF(pos.x() - rectSizeHalf, pos.y() - rectSizeHalf, rectSize, rectSize);
        break;
    }
    case Modus_Rubberband :
        r.setLeft(qMin(startPos.x(), pos.x()));
        r.setRight(qMax(startPos.x(), pos.x()));
        r.setTop(qMin(startPos.y(), pos.y()));
        r.setBottom(qMax(startPos.y(), pos.y()));
        break;
    }


    update(r);

}

void Marker::setInfo(const QString &text)
{
    info->setText(text);
}

QPointF Marker::getReferencePos() const
{
    return referencePos;
}

void Marker::setReferencePos()
{
    referencePos = tracker.getRect().center();
}

bool Marker::getIsSelected() const
{
    return isSelected;
}

void Marker::setIsSelected(bool value)
{
    isSelected = value;
    if(isSelected)
    {
        rectItem->setPen(QPen(Qt::yellow, 1));
        crosshair[0]->setPen(QPen(Qt::yellow, 1));
        crosshair[1]->setPen(QPen(Qt::yellow, 1));
        info->setBrush(Qt::yellow);
    }
    else
    {
        rectItem->setPen(QPen(Qt::green, 1));
        crosshair[0]->setPen(QPen(Qt::green, 1));
        crosshair[1]->setPen(QPen(Qt::green, 1));
        info->setBrush(Qt::green);
    }
}

void Marker::update()
{
    QRectF r = getRect();
    
    switch(Settings::getMarkerModus())
    {
    case Marker::Modus_Rubberband :
        break;
    case Marker::Modus_FixedRect :
    {
        qreal size = Settings::getFixedRectSize();
        qreal sizeHalf = size / 2.;
        QPointF center = r.center();
        r.setTopLeft(QPointF(center.x()-sizeHalf, center.y()-sizeHalf));
        r.setSize(QSizeF(size, size));
        break;
    }
    }

    if(update(r))
        Q_EMIT newMark();
}

void Marker::update(const QPixmap& image)
{
    tracker.update(image);
    update(tracker.getRect());
    setInfo(QString("hfd(%0)").arg(tracker.getHfd()));
}


} // namespace StarTrack
