#ifndef STARTRACK_GRAPHICSSCENE_H
#define STARTRACK_GRAPHICSSCENE_H

#include <QGraphicsScene>

namespace StarTrack {

class Marker;
class Settings;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

    QGraphicsPixmapItem* imageLayer;
    Marker* marker;

    bool enabled;
    bool publishScaledImage;

    QImage star, scaledStar;

    bool grabImages(const QRectF &rect);
    double calcHfd(const QRectF &rect) const;

public:
    GraphicsScene(QObject* parent);
    ~GraphicsScene();

    bool getEnabled() const;
    void setEnabled(bool value);

    bool getPublishScaledImage() const;
    void setPublishScaledImage(bool value);

Q_SIGNALS :
    void starCentered(const QImage&);
    void newHfdValue(float);
    void starTrackingEnabled(bool yes);

public Q_SLOTS :
    void updateBackground(const QPixmap& pixmap);
    void updateMarker();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private Q_SLOTS :
    void newMark();

};

} // namespace StarTrack

#endif // STARTRACK_GRAPHICSSCENE_H
