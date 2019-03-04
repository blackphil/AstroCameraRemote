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

    if(!enabled || trackers.isEmpty())
        return;

    if(publishScaledImage)
        Q_EMIT starCentered(trackers[0]->getScaledStar());
    else
        Q_EMIT starCentered(trackers[0]->getCurrentStar());

}

bool GraphicsScene::grabImages()
{
    QPixmap pixmap = imageLayer->pixmap();
    if(pixmap.isNull())
        return false;

    foreach(TrackerPtr t, trackers)
        t->update(pixmap);
    return true;
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

    foreach(TrackerPtr tracker, trackers)
    {
        tracker->update(imageLayer->pixmap());
    }
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!enabled)
        return;
    marker->start(event->scenePos());
    if(trackers.isEmpty())
        trackers << TrackerPtr(new Tracker());
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

    trackers.last()->setRect(marker->getRect());

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
    if(trackers.isEmpty())
        return;

    trackers[0]->setRect(marker->getRect());
    StarInfoPtr info(new StarInfo());

    if(!grabImages())
        return;


    QRectF rect = trackers[0]->getRect();
    marker->update(rect);

    info->window = rect.size();
    info->centerPosition = rect.center();

    if(publishScaledImage)
        Q_EMIT starCentered(trackers[0]->getScaledStar());
    else
        Q_EMIT starCentered(trackers[0]->getCurrentStar());


    info->hfd = trackers[0]->getHfd();
    marker->setInfo(QString("hfd(%0)").arg(info->hfd));

    Q_EMIT newHfdValue(info);

    AB_INF("END");

}

} // namespace StarTrack
