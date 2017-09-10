#ifndef STARTRACK_LENSEGRAPHCISSCENE_H
#define STARTRACK_LENSEGRAPHCISSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>


namespace StarTrack {

class LenseGraphcisScene : public QGraphicsScene
{
    Q_OBJECT

    QGraphicsPixmapItem* star;

public:
    LenseGraphcisScene(QObject* parent);

public Q_SLOTS :
    void updateStar(const QImage& image);
};

} // namespace StarTrack

#endif // STARTRACK_LENSEGRAPHCISSCENE_H
