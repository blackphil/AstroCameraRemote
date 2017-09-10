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

public:
    GraphicsScene(QObject* parent);
    ~GraphicsScene();

    bool getEnabled() const;
    void setEnabled(bool value);

Q_SIGNALS :
    void starCentered(const QImage&);
    void newHfdValue(float);

public Q_SLOTS :
    void updateBackground(const QPixmap& pixmap);
    void updateMarker();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private Q_SLOTS :
    void newMark(QRectF rect);

};

} // namespace StarTrack

#endif // STARTRACK_GRAPHICSSCENE_H
