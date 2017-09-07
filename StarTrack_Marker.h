#ifndef STARTRACK_MARKER_H
#define STARTRACK_MARKER_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPoint>
#include <QPen>

namespace StarTrack {

class Marker : public QObject
{
    Q_OBJECT

    QGraphicsScene* scene;
    QGraphicsRectItem* rectItem;
    QPen rectPen;
    QPointF startPos;

    enum Status
    {
        Status_Idle
        , Status_Moving
        , Status_Finished
    };

    Status status;

public:
    Marker(QGraphicsScene* scene, QObject* parent);

    QRectF getRect() const;

public Q_SLOTS :
    void start(const QPointF &pos);
    void finish(const QPointF& pos);
    void mouseMoved(const QPointF& pos);
};

} // namespace StarTrack

#endif // STARTRACK_MARKER_H
