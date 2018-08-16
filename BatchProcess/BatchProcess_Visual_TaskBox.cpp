#include "BatchProcess_Visual_TaskBox.h"

#include "AstroBase.h"

#include <QGraphicsScene>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "BatchProcess_Task.h"
#include "BatchProcess_Signal.h"
#include "BatchProcess_TaskWidget.h"

#define PIN_DISTANCE_HALF 20.
#define PIN_DISTANCE PIN_DISTANCE_HALF * 2.
#define PIN_LENGTH 20.
#define PIN_POINT_SIZE 8.

#define TASK_BODY_HEIGHT_MIN PIN_DISTANCE
#define TASK_BODY_WIDTH_MIN PIN_DISTANCE

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

}

void Pin::move(int pos, Direction direction)
{
    position = pos;
    this->direction = direction;
    privateUpdate();
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

QGraphicsScene *TaskBox::getScene() const
{
    return scene;
}

QRectF TaskBox::boundingRect() const
{
    QRectF b = body->boundingRect().united(title->boundingRect());
    return b;
}

QRectF TaskBox::bodyRect() const
{
    return body->boundingRect();
}

void TaskBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if(0)
    {
        painter->fillRect(boundingRect(), QBrush(Qt::magenta));
    }
}

TaskBox::TaskBox(QGraphicsScene *scene, const QString &title)
    : scene(scene)
    , mouseButton(Qt::NoButton)
{
    setFlag(QGraphicsItem::ItemNegativeZStacksBehindParent);
    body = scene->addRect(0, 0, TASK_BODY_WIDTH_MIN, TASK_BODY_HEIGHT_MIN);
    body->setParentItem(this);
    this->title = scene->addText(title);
    this->title->setParentItem(this);

    QRectF bodyRect = body->boundingRect();
    this->title->setPos(bodyRect.left(), bodyRect.top()-this->title->boundingRect().height());


}

TaskBox::~TaskBox()
{
}

void TaskBox::setTask(TaskPtr task)
{
    QList<SignalPtr> signals_;
    task->getInputs(signals_);
    foreach(SignalPtr in, signals_)
        addInputPin(in);

    signals_.clear();

    task->getOutputs(signals_);
    foreach(SignalPtr out, signals_)
        addOutputPin(out);


}

QString TaskBox::getTitle() const
{
    return title->toPlainText();
}

void TaskBox::setTitle(const QString &value)
{
    title->setPlainText(value);
}

void TaskBox::addPin(SignalPtr s, bool isInput)
{
    PinPtr p(new Pin(s, this));
    scene->addItem(p);
    p->setParentItem(this);

    if(isInput)
        inputs << p;
    else
        outputs << p;

    int numVerticalPins = qMax(inputs.count(), outputs.count());
    QRectF bodyRect = body->rect();
    QSizeF bodySize = bodyRect.size();
    bodySize.setHeight(numVerticalPins*PIN_DISTANCE);
    bodyRect.setSize(bodySize);
    body->setRect(bodyRect);
    this->title->setPos(bodyRect.left(), bodyRect.top()-this->title->boundingRect().height());

    if(isInput)
    {
        p->move(inputs.count()-1, Pin::Direction_Left);
    }
    else
    {
        p->move(outputs.count()-1, Pin::Direction_Right);
    }

}

void TaskBox::addInputPin(SignalPtr in)
{
    addPin(in, true);
}

void TaskBox::addOutputPin(SignalPtr out)
{
    addPin(out, false);
}

void TaskBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    AB_DBG("VISIT");
    //QGraphicsObject::mousePressEvent(event);
//    event->accept();
}
#if 0

void TaskBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    AB_DBG("VISIT");
    //    QGraphicsObject::mouseReleaseEvent(event);
    event->accept();
}
#endif

void TaskBox::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    AB_DBG("VISIT");
    QPointF d = event->scenePos() - event->lastScenePos();
    moveBy(d.x(), d.y());
    // QGraphicsObject::mouseMoveEvent(event);
}


} // namespace Visual
} // namespace BatchProcess
