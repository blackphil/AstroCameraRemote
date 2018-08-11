#include "BatchProcess_Visual_GraphicsScene.h"

#include "BatchProcess_Visual_Task.h"

namespace BatchProcess {
namespace Visual {

GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    addTask(new Task(this, this));
}

GraphicsScene::~GraphicsScene()
{
    qDeleteAll(tasks);
}

void GraphicsScene::addTask(Task *t)
{
    tasks << t;
}

} // namespace Visual
} // namespace BatchProcess
