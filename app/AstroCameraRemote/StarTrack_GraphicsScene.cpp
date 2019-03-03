#include "StarTrack_GraphicsScene.h"

#include "StarTrack_Marker.h"
#include "AstroBase.h"
#include "hfd/Hfd_Calculator.h"
#include "StarTrack_Settings.h"
#include "StarTrack_StarInfo.h"


#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include <QDir>
#include <QFileInfo>

namespace StarTrack {


bool GraphicsScene::getEnabled() const
{
    return enabled;
}

void GraphicsScene::setEnabled(bool value)
{
    enabled = value;
}

bool GraphicsScene::getPublishScaledImage() const
{
    return publishScaledImage;
}

void GraphicsScene::setPublishScaledImage(bool value)
{
    if(publishScaledImage == value)
        return;
    publishScaledImage = value;

    if(!enabled)
        return;

    if(publishScaledImage)
        Q_EMIT starCentered(scaledStar);
    else
        Q_EMIT starCentered(star);

}

bool GraphicsScene::grabImages(const QRectF& rect)
{
    QPixmap pixmap = imageLayer->pixmap();
    if(pixmap.isNull())
        return false;

    Hfd::Calculator hfd;
    star = pixmap.copy(rect.toRect()).toImage();
    double mean = hfd.meanValue(star);
    scaledStar = hfd.scaledImage(star, mean);

    return true;
}

qreal GraphicsScene::calcHfd(const QRectF& rect) const
{
    Hfd::Calculator hfd;
    qreal outerDiameter = qMin(rect.width(), rect.height());
    qreal hfdValue = hfd.calcHfd(scaledStar, qRound(outerDiameter));
    hfdValue = outerDiameter / hfdValue;
    return hfdValue;
}

GraphicsScene::GraphicsScene(QObject* parent)
    : QGraphicsScene(parent)
    , marker(new Marker(this, this))
    , enabled(true)
    , publishScaledImage(Settings::getPublishScaledImage())
{


    connect(marker, SIGNAL(newMark()), this, SLOT(newMark()));
    connect(this, SIGNAL(starTrackingEnabled(bool)), marker, SLOT(setTracking(bool)));

    QRect defaultRect(0, 0, 808, 540);

    setSceneRect(defaultRect);

    QFile defaultImage(":/images/LiveView_NoImage.jpg");
    defaultImage.open(QIODevice::ReadOnly);
    QByteArray imageData = defaultImage.readAll();

    imageLayer = addPixmap(QPixmap::fromImage(QImage::fromData(imageData, "JPG").scaled(defaultRect.size())));
    imageLayer->setZValue(0);

}

GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::updateBackground(const QPixmap &pixmap)
{
    if(!enabled)
        return;

    imageLayer->setPixmap(pixmap);
    newMark();
}

void GraphicsScene::updateMarker()
{
    if(!enabled)
        return;
    marker->update();
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!enabled)
        return;
    marker->start(event->scenePos());
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!enabled)
        return;
    marker->mouseMoved(event->scenePos());
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!enabled)
        return;
    marker->finish(event->scenePos());

}

namespace helper
{
void debugSaveImage(const QImage& img, const QString& prefix)
{
    QFile imageFile(QString("testimages/%0_star.jpg").arg(prefix));
    imageFile.open(QIODevice::WriteOnly);
    img.save(&imageFile, "JPG", 100);
}
}

void GraphicsScene::newMark()
{
    if(!enabled)
        return;

    StarInfoPtr info(new StarInfo());
    QRectF rect = marker->getRect();

    if(!grabImages(rect))
        return;

    rect = marker->centerStar(scaledStar);
    info->window = rect.size();
    info->centerPosition = rect.center();

    if(!grabImages(rect))
        return;

    if(publishScaledImage)
        Q_EMIT starCentered(scaledStar);
    else
        Q_EMIT starCentered(star);


    info->hfd = calcHfd(marker->getRect());
    marker->setInfo(QString("hfd(%0)").arg(info->hfd));

    Q_EMIT newHfdValue(info);

    AB_INF("END");

}

} // namespace StarTrack
