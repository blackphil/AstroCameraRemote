#ifndef STARTRACK_GRAPHICSSCENE_H
#define STARTRACK_GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QList>

#include "StarTrack_Tracker.h"
#include "StarTrack_StarInfo.h"

namespace StarTrack {

class Marker;
class Settings;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

    QGraphicsPixmapItem* imageLayer;
    Marker* selectedMarker;

    bool enabled;
    bool publishScaledImage;

    QList<Marker*> markers;


public:
    GraphicsScene(QObject* parent);
    ~GraphicsScene();

    bool getEnabled() const;
    void setEnabled(bool value);

    bool getPublishScaledImage() const;
    void setPublishScaledImage(bool value);

Q_SIGNALS :
    void starCentered(const QImage&);
    void newHfdValue(StarInfoPtr);
    void starTrackingEnabled(bool);
    void newReferenceMarkers(const QList<QRectF>&);

public Q_SLOTS :
    void updateBackground(const QPixmap& pixmap);
    void updateMarker();
    void setReference();
    void unsetReference();
    void removeSelectedMarker();
    void cleanUpMarkers();
    void applyReferenceMarkers(const QList<QRectF>& refMarkers);
    void selectNextStar();
    void selectPreviousStar();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent* event);

private Q_SLOTS :
    void newMark();
    void setSelectedMarker(const QPointF& pos);
    void setSelectedMarker(Marker* m);

};

} // namespace StarTrack

#endif // STARTRACK_GRAPHICSSCENE_H
