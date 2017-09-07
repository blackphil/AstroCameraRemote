#include "StarTrack_GraphicsScene.h"

#include "StarTrack_Marker.h"
#include "SonyAlphaRemote_Helper.h"

#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include <QDir>
#include <QFileInfo>

namespace StarTrack {

GraphicsScene::GraphicsScene(QObject* parent)
    : QGraphicsScene(parent)
    , marker(new Marker(this, this))
{
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
    qDeleteAll(graphicsItemBucket);
}

void GraphicsScene::updateBackground(const QPixmap &pixmap)
{
    imageLayer->setPixmap(pixmap);
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

    QPixmap star = imageLayer->pixmap().copy(marker->getRect().toRect());
    QFile imageFile(QFileInfo(QDir::temp(), "star.jpg").absoluteFilePath());
    imageFile.open(QIODevice::WriteOnly);
    star.toImage().save(&imageFile, "JPG", 100);
}



} // namespace StarTrack
