#include "StarTrack_LenseGraphcisScene.h"

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
    hfd->setPen(QPen(Qt::red));
}

QImage LenseGraphcisScene::getStar() const
{
    return star->pixmap().toImage();
}

float LenseGraphcisScene::getHfd() const
{
    return hfd->text().toFloat();
}

void LenseGraphcisScene::updateStar(const QImage &image)
{
    AB_INF("bounding(" << width() << ", " << height() << ")");
    star->setPixmap(QPixmap::fromImage(image.scaled(width(), height(), Qt::KeepAspectRatio)));
}

void LenseGraphcisScene::updateHfd(const float& hfd)
{
    this->hfd->setText(QString::number(static_cast<double>(hfd), 'g', 3));
}

} // namespace StarTrack
