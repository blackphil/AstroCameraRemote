#ifndef BATCHPROCESS_VISUAL_TASKBOX_H
#define BATCHPROCESS_VISUAL_TASKBOX_H

#include "batchprocess_global.h"



#include <QGraphicsObject>
#include <QSharedDataPointer>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPointer>
#include <QGraphicsProxyWidget>

namespace BatchProcess {

class Task;
typedef QPointer<Task> TaskPtr;

class Signal;
typedef QSharedPointer<Signal> SignalPtr;

namespace Visual {

class TaskBox;
typedef QPointer<TaskBox> TaskBoxPtr;

class BATCHPROCESSSHARED_EXPORT Pin : public QGraphicsObject
{

public :
    enum Direction
    {
        Direction_Up
        , Direction_Down
        , Direction_Left
        , Direction_Right
    };

private :

    QPointer<TaskBox> taskBox;
    SignalPtr signal;

    QGraphicsEllipseItem* point;
    QGraphicsLineItem* line;

    QGraphicsTextItem* title;

    int position;
    Direction direction;

    void privateUpdate();

protected:

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

public :
    Pin(SignalPtr signal, TaskBox* taskBox);

    void move(int pos, Direction direction);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

};
typedef QPointer<Pin> PinPtr;

class BATCHPROCESSSHARED_EXPORT TaskBox : public QGraphicsObject
{

  //  Q_OBJECT

    QGraphicsScene* scene;
    QGraphicsRectItem* body;

    QList<PinPtr> inputs;
    QList<PinPtr> outputs;

    QGraphicsTextItem* title;

    QPointF mousePos;
    Qt::MouseButton mouseButton;

    void addPin(SignalPtr s, bool isInput);
    void addInputPin(SignalPtr s);
    void addOutputPin(SignalPtr s);

protected :

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
#if 0
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
#endif
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

public:
    TaskBox(QGraphicsScene* scene, const QString& title = "<unknown>");
    ~TaskBox();

    void setTask(TaskPtr task);
    QGraphicsScene *getScene() const;

    QRectF boundingRect() const;
    QRectF bodyRect() const;
    QString getTitle() const;
    void setTitle(const QString &value);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

typedef QPointer<TaskBox> TaskBoxPtr;

} // namespace Visual
} // namespace BatchProcess

#endif // BATCHPROCESS_VISUAL_TASKBOX_H
