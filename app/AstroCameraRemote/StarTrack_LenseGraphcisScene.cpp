#include "StarTrack_LenseGraphcisScene.h"

#include "AstroBase.h"

namespace StarTrack {

LenseGraphcisScene::LenseGraphcisScene(QObject* parent)
    : QGraphicsScene(parent)
    , star(NULL)
{
    setSceneRect(0, 0, 200, 200);
    star = addPixmap(QPixmap());
}

void LenseGraphcisScene::updateStar(const QImage &image)
{
    AB_INF("bounding(" << width() << ", " << height() << ")");
    star->setPixmap(QPixmap::fromImage(image.scaled(width(), height(), Qt::KeepAspectRatio)));
}

} // namespace StarTrack
