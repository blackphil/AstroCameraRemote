#ifndef BATCHPROCESS_VISUAL_GRAPHICSSCENE_H
#define BATCHPROCESS_VISUAL_GRAPHICSSCENE_H

#include "batchprocess_global.h"

#include <QGraphicsScene>
#include <QList>


namespace BatchProcess {
namespace Visual {

class Task;

class BATCHPROCESSSHARED_EXPORT GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

    QList<Task*> tasks;

public:
    GraphicsScene(QObject* parent);
    ~GraphicsScene();

public Q_SLOTS :
    void addTask(Task* t);
};

} // namespace Visual
} // namespace BatchProcess

#endif // BATCHPROCESS_VISUAL_GRAPHICSSCENE_H
