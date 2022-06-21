#include "BatchProcess_Visual_GraphicsScene.h"

#include "BatchProcess_Visual_TaskBox.h"
#include "BatchProcess_Task.h"
#include "BatchProcess_Visual_Pin.h"
#include "BatchProcess_Visual_Connection.h"

#include <AstroBase/AstroBase>

#include <QGraphicsSceneMouseEvent>

#define RASTER_MARGIN 10

namespace BatchProcess {
namespace Visual {

GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::addTask(TaskPtr task)
{
    TaskBoxPtr taskBox(new TaskBox(this, task->getTitle()));
    taskBox->setTask(task);
    this->addItem(taskBox.data());

    QPointF pos(RASTER_MARGIN, RASTER_MARGIN);
    foreach(TaskBoxPtr tb, tasks)
    {
        pos.setX(pos.x() + tb->boundingRect().width() + RASTER_MARGIN);
    }

    taskBox->setPos(pos);
    tasks << taskBox;
}

void GraphicsScene::connectPins(PinPtr a, PinPtr b)
{
    a->getSignal()->connectToSignal(b->getSignal());
    ConnectionPtr c = new Connection(a, b);
    AB_DBG("connected" << a->getSignal()->getTitle() << "to"<< b->getSignal()->getTitle());
    this->addItem(c);

    connect(a->getTaskBox(), SIGNAL(moved(QPointF)), c, SLOT(pinPosChanged()));
    connect(b->getTaskBox(), SIGNAL(moved(QPointF)), c, SLOT(pinPosChanged()));

    connections << c;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    AB_DBG("VISIT");

    foreach(TaskBoxPtr tb, tasks)
    {
        PinPtr pin = tb->touchedPin(event->scenePos(), Signal::Direction_In);
        if(pin)
        {
//            AB_DBG("touched input:" << pin->getSignal()->getTitle());
            grippedPin = pin;
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    AB_DBG("VISIT");

    foreach(TaskBoxPtr tb, tasks)
    {
        PinPtr pin = tb->touchedPin(event->scenePos(), Signal::Direction_Out);
        if(pin)
        {
//            AB_DBG("touched output:" << pin->getSignal()->getTitle());
            connectPins(grippedPin, pin);
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

} // namespace Visual
} // namespace BatchProcess
