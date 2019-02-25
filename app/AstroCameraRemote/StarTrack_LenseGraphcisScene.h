#ifndef STARTRACK_LENSEGRAPHCISSCENE_H
#define STARTRACK_LENSEGRAPHCISSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>

namespace StarTrack {

class LenseGraphcisScene : public QGraphicsScene
{
    Q_OBJECT

    QGraphicsPixmapItem* star;
    QGraphicsSimpleTextItem* hfd;

public:
    LenseGraphcisScene(QObject* parent);

    QImage getStar() const;
    float getHfd() const;


public Q_SLOTS :
    void updateStar(const QImage& image);
    void updateHfd(const float& hfd);

};

} // namespace StarTrack

#endif // STARTRACK_LENSEGRAPHCISSCENE_H
