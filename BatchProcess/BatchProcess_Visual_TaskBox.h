#ifndef BATCHPROCESS_VISUAL_TASKBOX_H
#define BATCHPROCESS_VISUAL_TASKBOX_H

#include "batchprocess_global.h"

#include "BatchProcess_Signal.h"
#include "BatchProcess_Task.h"

#include <QGraphicsObject>
#include <QSharedDataPointer>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPointer>
#include <QGraphicsProxyWidget>

namespace BatchProcess {


class Signal;
typedef QPointer<Signal> SignalPtr;

namespace Visual {

class TaskBox;
typedef QPointer<TaskBox> TaskBoxPtr;

class Pin;
typedef QPointer<Pin> PinPtr;

class BATCHPROCESSSHARED_EXPORT TaskBox : public QGraphicsObject
{

    Q_OBJECT

    TaskPtr task;

    QGraphicsScene* scene;
    QGraphicsRectItem* body;

    QList<PinPtr> inputs;
    QList<PinPtr> outputs;

    QGraphicsTextItem* title;
    QGraphicsTextItem* info;

    QPointF mousePos;
    Qt::MouseButton mouseButton;

    void addPin(SignalPtr s, bool isInput);
    void addInputPin(SignalPtr s);
    void addOutputPin(SignalPtr s);

Q_SIGNALS :
    void moved(QPointF);

private Q_SLOTS :
    void statusChanged(Task::Status);
    void infoUpdate(QString text);
    void errorUpdate(QString text);

protected :

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
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


    PinPtr touchedPin(const QPointF& p, Signal::Direction directionFilter) const;
};

typedef QPointer<TaskBox> TaskBoxPtr;

} // namespace Visual
} // namespace BatchProcess

#endif // BATCHPROCESS_VISUAL_TASKBOX_H
