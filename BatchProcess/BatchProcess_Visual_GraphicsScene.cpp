#include "BatchProcess_Visual_GraphicsScene.h"

#include "BatchProcess_Visual_TaskBox.h"
#include "BatchProcess_Task.h"

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

} // namespace Visual
} // namespace BatchProcess
