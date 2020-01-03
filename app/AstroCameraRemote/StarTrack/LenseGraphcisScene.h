#ifndef STARTRACK_LENSEGRAPHCISSCENE_H
#define STARTRACK_LENSEGRAPHCISSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>

#include "StarInfo.h"

namespace StarTrack {


class LenseGraphcisScene : public QGraphicsScene
{
    Q_OBJECT

    StarInfoPtr starInfo;

    QGraphicsPixmapItem* star;
    QGraphicsSimpleTextItem* hfd;

public:
    LenseGraphcisScene(QObject* parent);

    QImage getStar() const;
    StarInfoPtr getStarInfo() const;


public Q_SLOTS :
    void updateStar(const QImage& image);
    void updateHfd(StarInfoPtr starInfo);
    void changeHfdFontPointSize(int size);

};

} // namespace StarTrack

#endif // STARTRACK_LENSEGRAPHCISSCENE_H
