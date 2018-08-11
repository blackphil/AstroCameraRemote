#ifndef BATCHPROCESS_VISUAL_TASK_H
#define BATCHPROCESS_VISUAL_TASK_H

#include "batchprocess_global.h"

#include <QObject>

#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>

namespace BatchProcess {
namespace Visual {

class Task;

class BATCHPROCESSSHARED_EXPORT Pin : public QObject
{
    Q_OBJECT

public :
    enum Direction
    {
        Direction_Up
        , Direction_Down
        , Direction_Left
        , Direction_Right
    };

private :

    Task* task;

    QGraphicsEllipseItem* point;
    QGraphicsLineItem* line;

    QGraphicsTextItem* title;

    Direction direction;

public :
    Pin(const QString& title, Task* task);
    QString getTitle() const;
    void setTitle(const QString &value);

    void move(const QPointF& pos);

    Direction getDirection() const;
    void setDirection(const Direction &value);
};

class BATCHPROCESSSHARED_EXPORT Task : public QObject
{

    Q_OBJECT

    QGraphicsScene* scene;
    QGraphicsRectItem* body;

    QList<Pin*> inputs;
    QList<Pin*> outputs;

public:
    Task(QGraphicsScene* scene, QObject* parent);

    void setInputs(const QStringList& inputs);
    void setOutputs(const QStringList& outputs);
    QGraphicsScene *getScene() const;
};

} // namespace Visual
} // namespace BatchProcess

#endif // BATCHPROCESS_VISUAL_TASK_H
