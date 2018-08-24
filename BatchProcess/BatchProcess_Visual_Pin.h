#ifndef BATCHPROCESS_VISUAL_PIN_H
#define BATCHPROCESS_VISUAL_PIN_H

#include "batchprocess_global.h"

#include "BatchProcess_Signal.h"

#include <QGraphicsObject>
#include <QPointer>

namespace BatchProcess {
namespace Visual {

class TaskBox;
typedef QPointer<TaskBox> TaskBoxPtr;

class BATCHPROCESSSHARED_EXPORT Pin : public QGraphicsObject
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

    TaskBoxPtr taskBox;
    SignalPtr signal;

    QGraphicsEllipseItem* point;
    QGraphicsLineItem* line;

    QGraphicsTextItem* title;

    int position;
    Direction direction;


Q_SIGNALS :
    void pinPosChanged(QPointF);

private Q_SLOTS :
    void onPosChange(qreal);
    void privateUpdate();

protected:

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

public :
    Pin(SignalPtr signal, TaskBox* taskBox);

    void move(int pos, Direction direction);

    bool touchedPin(const QPointF& p) const;
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    SignalPtr getSignal() const;

    QPointF pinPos() const;

    TaskBoxPtr getTaskBox() const { return taskBox; }
};
typedef QPointer<Pin> PinPtr;

} // namespace Visual
} // namespace BatchProcess

#endif // BATCHPROCESS_VISUAL_PIN_H
