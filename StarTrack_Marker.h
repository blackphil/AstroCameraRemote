#ifndef STARTRACK_MARKER_H
#define STARTRACK_MARKER_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>
#include <QPoint>
#include <QPen>

namespace StarTrack {

class Marker : public QObject
{
    Q_OBJECT

    QGraphicsScene* scene;
    QGraphicsRectItem* rectItem;
    QGraphicsSimpleTextItem* info;
    QPen rectPen;
    QPointF startPos;

    enum Status
    {
        Status_Idle
        , Status_Moving
        , Status_Finished
    };

    Status status;

Q_SIGNALS :
    void newMark(QRectF);

public:
    Marker(QGraphicsScene* scene, QObject* parent);

    QRectF getRect() const;

public Q_SLOTS :
    void start(const QPointF &pos);
    void finish(const QPointF& pos);
    void mouseMoved(const QPointF& pos);

    void setInfo(const QString& text);

    void centerStar(const QImage& scaledStar);
};

} // namespace StarTrack

#endif // STARTRACK_MARKER_H
