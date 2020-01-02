#ifndef BATCHPROCESS_CONNECTION_H
#define BATCHPROCESS_CONNECTION_H

#include "BatchProcess_Visual_Pin.h"

#include <QGraphicsObject>
#include <QSharedPointer>

namespace BatchProcess {
namespace Visual {


class Connection : public QGraphicsObject
{
    Q_OBJECT

    PinPtr from, to;

    QGraphicsLineItem* line;

public:
    Connection(PinPtr from, PinPtr to);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

public Q_SLOTS :
    void pinPosChanged();

};
typedef QPointer<Connection> ConnectionPtr;

} // namespace Visual
} // namespace BatchProcess

#endif // BATCHPROCESS_CONNECTION_H
