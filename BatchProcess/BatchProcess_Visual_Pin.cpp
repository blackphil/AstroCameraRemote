#include "BatchProcess_Visual_Pin.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "BatchProcess.h"
#include "BatchProcess_Visual_TaskBox.h"
#include "AstroBase.h"



namespace BatchProcess {
namespace Visual {

QRectF Pin::boundingRect() const
{
    return childrenBoundingRect();
}

void Pin::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if(0)
    {
        painter->fillRect(boundingRect(), QBrush(Qt::blue));
        painter->setPen(QColor(Qt::red));
        painter->drawEllipse(pinPos(), 10, 10);
    }

}

void Pin::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    AB_DBG("VISIT");
    if(signal->edit())
        privateUpdate();

    QGraphicsObject::mouseDoubleClickEvent(event);
}

Pin::Pin(SignalPtr signal, TaskBox *task)
    : QGraphicsObject(task)
    , taskBox(task)
    , signal(signal)
    , direction(Direction_Left)
{
    line = task->getScene()->addLine(0, 0., 0., 0.);
    line->setParentItem(this);
    point = task->getScene()->addEllipse(0, 0., PIN_POINT_SIZE, PIN_POINT_SIZE);
    point->setParentItem(this);
    this->title = task->getScene()->addText(signal->getTitle());
    this->title->setParentItem(this);

    setAcceptDrops(true);

    connect(this, SIGNAL(xChanged(qreal)), this, SLOT(onPosChange(qreal)));
    connect(this, SIGNAL(yChanged(qreal)), this, SLOT(onPosChange(qreal)));
    connect(this, SIGNAL(zChanged(qreal)), this, SLOT(onPosChange(qreal)));

}

void Pin::move(int pos, Direction direction)
{
    position = pos;
    this->direction = direction;
    privateUpdate();
}

SignalPtr Pin::getSignal() const
{
    return signal;
}

QPointF Pin::pinPos() const
{
    QPointF p = point->pos();
    p.setX(p.x()+point->rect().width()/2.);
    p.setY(p.y()+point->rect().height()/2.);

    return p;
}

void Pin::privateUpdate()
{
    QPointF pixelPos;
    QRectF bounding = taskBox->bodyRect();
    QPointF offsetLine, offsetPoint, offsetText;

    title->setPlainText(signal->getTitle());

    double pinLength = qMax(PIN_LENGTH, title->boundingRect().width());

    switch(direction)
    {
    case Direction_Left :
        pixelPos = QPointF(bounding.left(), bounding.top()+PIN_DISTANCE_HALF+(position*PIN_DISTANCE));
        offsetLine.setX(-pinLength);
        offsetPoint = QPointF(-pinLength-PIN_POINT_SIZE, -PIN_POINT_SIZE/2.);
        offsetText = QPointF(offsetLine.x(), -title->boundingRect().height());
        break;
    case Direction_Right :
        pixelPos = QPointF(bounding.right(), bounding.top()+PIN_DISTANCE_HALF+(position*PIN_DISTANCE));
        offsetLine.setX(+pinLength);
        offsetPoint = QPointF(+pinLength, -PIN_POINT_SIZE/2.);
        offsetText.setY(-title->boundingRect().height());
        break;
    case Direction_Up :
        pixelPos = QPointF(bounding.left()+PIN_DISTANCE_HALF+(position*PIN_DISTANCE), bounding.top());
        offsetLine.setY(-pinLength);
        offsetPoint.setY(-pinLength-PIN_POINT_SIZE);
        offsetText.setX(+title->boundingRect().width());
        break;
    case Direction_Down :
        pixelPos = QPointF(bounding.left()+PIN_DISTANCE_HALF+(position*PIN_DISTANCE), bounding.bottom());
        offsetLine.setY(+pinLength);
        offsetPoint.setY(+pinLength+PIN_POINT_SIZE);
        offsetText.setX(+title->boundingRect().width());
        break;
    }

    line->setLine(QLineF(pixelPos+offsetLine, pixelPos));
    point->setPos(pixelPos+offsetPoint);
    title->setPos(pixelPos+offsetText);
}

void Pin::onPosChange(qreal)
{
    AB_DBG("VISIT");
    Q_EMIT pinPosChanged(pinPos());
}


bool Pin::touchedPin(const QPointF &p) const
{
//    AB_DBG("VISIT" << signal->getName());
    QPointF pointPos = point->mapToScene(QPointF());
    QPointF distanceToPin = p - pointPos;
    qreal length = distanceToPin.manhattanLength();
//    AB_DBG("scene(" << pointPos.x() << "," << pointPos.y() << "), point(" << p.x() << p.y() << ")");
//    AB_DBG("length(" << length << ")");
    if(length <= PIN_POINT_SIZE*1.5)
    {
        AB_DBG("clicked on the pin" << signal->getName());
        return true;
    }

    return false;
}
} // namespace Visual
} // namespace BatchProcess
