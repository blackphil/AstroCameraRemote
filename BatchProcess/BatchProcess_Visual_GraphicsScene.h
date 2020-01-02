#ifndef BATCHPROCESS_VISUAL_GRAPHICSSCENE_H
#define BATCHPROCESS_VISUAL_GRAPHICSSCENE_H

#include "BatchProcess_Visual_TaskBox.h"
#include "BatchProcess_Visual_Pin.h"
#include "BatchProcess_Visual_Connection.h"

#include <QGraphicsScene>
#include <QList>


namespace BatchProcess {

namespace Visual {


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

    QList<TaskBoxPtr> tasks;
    QList<ConnectionPtr> connections;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    PinPtr grippedPin;

public:
    GraphicsScene(QObject* parent);
    ~GraphicsScene();

public Q_SLOTS :
    void addTask(TaskPtr t);
    void connectPins(PinPtr a, PinPtr b);
};

} // namespace Visual
} // namespace BatchProcess

#endif // BATCHPROCESS_VISUAL_GRAPHICSSCENE_H
