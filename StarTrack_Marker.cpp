#include "StarTrack_Marker.h"

#include <QBrush>
#include <QRect>

#include "SonyAlphaRemote_Helper.h"


namespace StarTrack {

Marker::Marker(QGraphicsScene* scene, QObject *parent)
    : QObject(parent)
    , scene(scene)
    , rectItem(NULL)
    , rectPen(QPen(QBrush(Qt::green), 1))
    , status(Status_Idle)
{

}

QRectF Marker::getRect() const
{
    return rectItem->rect();
}

void Marker::start(const QPointF &pos)
{
    if(rectItem)
        delete rectItem;

    rectItem = scene->addRect(pos.x(), pos.y(), 1, 1, rectPen);
    rectItem->setZValue(2);
    status = Status_Moving;

    startPos = pos;


}

void Marker::finish(const QPointF &pos)
{
    Q_UNUSED(pos)
}

void Marker::mouseMoved(const QPointF &pos)
{
    QRectF current = rectItem->rect();
    switch(status)
    {
    case Status_Idle :
        break;
    case Status_Moving :
        current.setLeft(qMin(startPos.x(), pos.x()));
        current.setRight(qMax(startPos.x(), pos.x()));
        current.setTop(qMin(startPos.y(), pos.y()));
        current.setBottom(qMax(startPos.y(), pos.y()));
        rectItem->setRect(current);
        break;
    case Status_Finished :
        break;
    }


}

} // namespace StarTrack
