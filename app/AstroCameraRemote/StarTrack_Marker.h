#ifndef STARTRACK_MARKER_H
#define STARTRACK_MARKER_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include <QPoint>
#include <QPen>

#include "StarTrack_GraphicsScene.h"
#include "StarTrack_Tracker.h"

namespace StarTrack {
class Marker : public QObject
{
    Q_OBJECT

public :
    enum Status
    {
        Status_Idle
        , Status_Moving
        , Status_Finished
    };

    enum Modus
    {
        Modus_Rubberband
        , Modus_FixedRect
    };

private :

    Tracker tracker;

    QPointF referencePos;

    QGraphicsLineItem* lineFromRef;

    GraphicsScene* scene;
    QGraphicsRectItem* rectItem;
    QGraphicsLineItem* crosshair[2];
    QGraphicsSimpleTextItem* info;
    QPen pen;
    QPointF startPos;
    bool tracking;

    Status status;

    bool isSelected;
    bool haveStar;

    void update();
    bool update(const QRectF &r);

Q_SIGNALS :
    void newMark();

public:
    Marker(GraphicsScene* scene, QObject* parent);
    ~Marker();

    QRectF getRect() const;


    bool getTracking() const;

    const Tracker& getTracker() const;



    QPointF getReferencePos() const;
    void setReferencePos();

    bool getIsSelected() const;
    void setIsSelected(bool value);

public Q_SLOTS :
    void setTracking(bool value);
    void start(const QPointF &pos);
    void finish();
    void mouseMoved(const QPointF& pos);

    void setInfo(const QString& text);

    void update(const QPixmap& image);

};

} // namespace StarTrack

#endif // STARTRACK_MARKER_H
