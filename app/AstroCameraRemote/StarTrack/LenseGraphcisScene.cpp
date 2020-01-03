#include "LenseGraphcisScene.h"

#include "StarInfo.h"

#include "AstroBase.h"
#include <QPen>
#include <QFont>

namespace StarTrack {

LenseGraphcisScene::LenseGraphcisScene(QObject* parent)
    : QGraphicsScene(parent)
    , star(nullptr)
{
    setSceneRect(0, 0, 200, 200);
    star = addPixmap(QPixmap());
    QFont f;
    f.fromString("MS Shell Dlg 2");
    f.setPointSize(20);
    f.setBold(true);
    hfd = addSimpleText("nan", f);

    QPen p(Qt::red);
    p.setWidthF(1.5);
    hfd->setPen(p);
    hfd->setBrush(Qt::transparent);

}

QImage LenseGraphcisScene::getStar() const
{
    return star->pixmap().toImage();
}

StarInfoPtr LenseGraphcisScene::getStarInfo() const
{
    return starInfo;
}

void LenseGraphcisScene::updateStar(const QImage &image)
{
    Q_ASSERT(!image.isNull());
    if(image.isNull())
    {
        AB_ERR("image is null");
        return;
    }
    AB_INF("bounding(" << width() << ", " << height() << ")");
    star->setPixmap(
                QPixmap::fromImage(
                    image.scaled(
                        static_cast<int>(std::round(width()))
                        , static_cast<int>(std::round(height()))
                        , Qt::KeepAspectRatio)));
}

void LenseGraphcisScene::updateHfd(StarInfoPtr starInfo)
{
    this->starInfo = starInfo;
    if(starInfo)
        this->hfd->setText(QString::number(static_cast<double>(starInfo->hfd), 'g', 3));
}

void LenseGraphcisScene::changeHfdFontPointSize(int size)
{
    QFont f = hfd->font();
    f.setPointSize(size);
    hfd->setFont(f);
    updateHfd(starInfo);
}

} // namespace StarTrack
