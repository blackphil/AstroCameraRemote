#ifndef STARTRACK_GRAPHICSSCENE_H
#define STARTRACK_GRAPHICSSCENE_H

#include <QGraphicsScene>

namespace StarTrack {

class Marker;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

    QGraphicsPixmapItem* imageLayer;
    StarTrack::Marker* marker;

public:
    GraphicsScene(QObject* parent);
    ~GraphicsScene();

public Q_SLOTS :
    void updateBackground(const QPixmap& pixmap);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private Q_SLOTS :
    void newMark(QRectF rect);

};

} // namespace StarTrack

#endif // STARTRACK_GRAPHICSSCENE_H
