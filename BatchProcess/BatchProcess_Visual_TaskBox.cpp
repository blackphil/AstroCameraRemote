#include "BatchProcess_Visual_TaskBox.h"

#include "AstroBase.h"

#include <QGraphicsScene>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "BatchProcess.h"
#include "BatchProcess_Task.h"
#include "BatchProcess_Signal.h"
#include "BatchProcess_Visual_Pin.h"

namespace BatchProcess {
namespace Visual {


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

PinPtr TaskBox::touchedPin(const QPointF &p, Signal::Direction directionFilter) const
{
    if(directionFilter & Signal::Direction_In)
    {
        foreach(PinPtr pin, inputs)
        {
            if(pin->touchedPin(p))
                return pin;
        }
    }

    if(directionFilter & Signal::Direction_Out)
    {
        foreach(PinPtr pin, outputs)
        {
            if(pin->touchedPin(p))
                return pin;
        }
    }

    return PinPtr();
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

    info = scene->addText(tr("INFO"));
    info->setHtml("<b>INFO</b>");
    info->setParentItem(this);

}

TaskBox::~TaskBox()
{
}

void TaskBox::infoUpdate(QString text)
{
    info->setHtml(QString("<p style=\"font-weight:bold\">%0</p>").arg(text));
}

void TaskBox::errorUpdate(QString text)
{
    info->setHtml(QString("<p style=\"font-weight:bold; color:red\">%0</p>").arg(text));
}

void TaskBox::setTask(TaskPtr task)
{
    if(this->task)
    {
        disconnect(this->task, SIGNAL(statusChanged(Task::Status)), this, SLOT(statusChanged(Task::Status)));
        disconnect(this->task, SIGNAL(infoUpdate(QString)), this, SLOT(infoUpdate(QString)));
        disconnect(this->task, SIGNAL(errorUpdate(QStirng)), this, SLOT(errorUpdate(QString)));
    }
    this->task = task;
    if(!this->task)
        return;

    connect(this->task, SIGNAL(statusChanged(Task::Status)), this, SLOT(statusChanged(Task::Status)));
    connect(this->task, SIGNAL(infoUpdate(QString)), this, SLOT(infoUpdate(QString)));
    connect(this->task, SIGNAL(errorUpdate(QString)), this, SLOT(errorUpdate(QString)));

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

void TaskBox::statusChanged(Task::Status status)
{
    AB_DBG("status changed for" << title->toPlainText() << ":" << status);
    switch(status)
    {
    case Task::Status_Idle :
        body->setPen(QPen(Qt::black));
        break;
    case Task::Status_Execution :
        body->setPen(QPen(Qt::blue));
        break;
    case Task::Status_Error :
        body->setPen(QPen(Qt::red));
        break;
    }

    body->update();
}

void TaskBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    task->edit();
}

void TaskBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
}

void TaskBox::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    AB_DBG("VISIT");
    QPointF d = event->scenePos() - event->lastScenePos();
    moveBy(d.x(), d.y());
    Q_EMIT moved(d);
}


} // namespace Visual
} // namespace BatchProcess
