#include "BatchProcess_Visual_Connection.h"

#include <QPainter>
#include <QGraphicsScene>

#include "AstroBase.h"

namespace BatchProcess {
namespace Visual {

Connection::Connection(PinPtr from, PinPtr to)
    : from(from)
    , to(to)
{
    line = from->scene()->addLine(QLineF(from->mapToScene(from->pinPos()), to->mapToScene(to->pinPos())));
    line->setParentItem(this);
}

void Connection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF Connection::boundingRect() const
{
    QPointF fromPos = from->mapToScene(from->pinPos());
    QPointF toPos = to->mapToScene(to->pinPos());

    QPointF topLeft(qMin(fromPos.x(), toPos.x()), qMin(fromPos.y(), toPos.y()));
    QPointF bottomRight(qMax(fromPos.x(), toPos.x()), qMax(fromPos.y(), toPos.y()));

    return QRectF(topLeft, bottomRight);
}

void Connection::pinPosChanged()
{
//    AB_DBG("VISIT");
    line->setLine(QLineF(from->mapToScene(from->pinPos()), to->mapToScene(to->pinPos())));
}

} // namespace Visual
} // namespace BatchProcess
