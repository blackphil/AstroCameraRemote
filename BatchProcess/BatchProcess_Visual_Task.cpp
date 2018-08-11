#include "BatchProcess_Visual_Task.h"

#include <QGraphicsScene>

#define PIN_DISTANCE_HALF 20.
#define PIN_DISTANCE PIN_DISTANCE_HALF * 2.
#define PIN_LENGTH 20.
#define PIN_POINT_SIZE 8.

#define TASK_BODY_HEIGHT_MIN 200.
#define TASK_BODY_WIDTH_MIN 100.

namespace BatchProcess {
namespace Visual {

Pin::Direction Pin::getDirection() const
{
    return direction;
}

void Pin::setDirection(const Direction &value)
{
    direction = value;
}

Pin::Pin(const QString &title, Task *task)
    : QObject(task)
    , direction(Direction_Left)
{
    line = task->getScene()->addLine(0, 0., 0., 0.);
    point = task->getScene()->addEllipse(0, 0., PIN_POINT_SIZE, PIN_POINT_SIZE);
    this->title = task->getScene()->addText(title);
    //this->title->setPos(point->pos());


}

QString Pin::getTitle() const
{
    return title->toPlainText();
}

void Pin::setTitle(const QString &value)
{
    title->setPlainText(value);
}

void Pin::move(const QPointF &pos)
{
    QPointF to = pos;
    to.setX(pos.x()-PIN_LENGTH);
    line->setLine(QLineF(to, pos));
    point->setPos(QPointF(to.x() - PIN_POINT_SIZE, to.y() - PIN_POINT_SIZE/2.));
    title->setPos(QPointF(to.x(), to.y() - title->boundingRect().height()));
}

QGraphicsScene *Task::getScene() const
{
    return scene;
}

Task::Task(QGraphicsScene *scene, QObject *parent)
    : QObject(parent)
    , scene(scene)
{

    body = scene->addRect(10, 10, TASK_BODY_WIDTH_MIN, TASK_BODY_HEIGHT_MIN);

    setInputs(QStringList() << "A" << "B" << "C");
}

void Task::setInputs(const QStringList& inputs)
{
    QPointF pinPos = QPointF(body->rect().left(), body->rect().top() + PIN_DISTANCE_HALF);
    qDeleteAll(this->inputs);
    this->inputs.clear();
    foreach(QString input, inputs)
    {
        Pin* p = new Pin(input, this);
        p->move(pinPos);
        pinPos.setY(pinPos.y() + PIN_DISTANCE);
        this->inputs << p;
    }


}

} // namespace Visual
} // namespace BatchProcess
