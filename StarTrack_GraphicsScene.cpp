#include "StarTrack_GraphicsScene.h"

#include "StarTrack_Marker.h"
#include "SonyAlphaRemote_Helper.h"
#include "hfd/Hfd_Calculator.h"

#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include <QDir>
#include <QFileInfo>

namespace StarTrack {

GraphicsScene::GraphicsScene(QObject* parent)
    : QGraphicsScene(parent)
    , marker(new Marker(this, this))
{

    connect(marker, SIGNAL(newMark(QRectF)), this, SLOT(newMark(QRectF)));

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
    imageLayer->setPixmap(pixmap);
    newMark(marker->getRect().toRect());
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    marker->start(event->scenePos());
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    marker->mouseMoved(event->scenePos());
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
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

void GraphicsScene::newMark(QRectF rect)
{
    if(rect.isNull())
        return;

    static int imgCount = 0;


    Hfd::Calculator hfd;

    double mean = hfd.meanValue(imageLayer->pixmap().toImage());
    QImage star = imageLayer->pixmap().copy(rect.toRect()).toImage();
    QImage scaledStar = hfd.scaledImage(star, mean);


    marker->centerStar(scaledStar);

    star = imageLayer->pixmap().copy(marker->getRect().toRect()).toImage();
    scaledStar = hfd.scaledImage(star, mean);

    helper::debugSaveImage(scaledStar, QString("scaled_%0").arg(imgCount++));

    float hfdValue = hfd.calcHfd(scaledStar, qRound(qMin(rect.width(), rect.height())));

    marker->setInfo(QString("hfd(%0)").arg(hfdValue));

}




} // namespace StarTrack