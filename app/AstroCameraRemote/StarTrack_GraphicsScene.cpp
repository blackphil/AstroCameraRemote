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

    if(!enabled || selectedMarker == nullptr)
        return;

    if(publishScaledImage)
        Q_EMIT starCentered(selectedMarker->getTracker().getScaledStar());
    else
        Q_EMIT starCentered(selectedMarker->getTracker().getCurrentStar());

}


GraphicsScene::GraphicsScene(QObject* parent)
    : QGraphicsScene(parent)
    , selectedMarker(nullptr)
    , enabled(true)
    , publishScaledImage(Settings::getPublishScaledImage())
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

    QPixmap pm = imageLayer->pixmap();

    foreach(Marker* marker, markers)
    {
        marker->update(pm);
    }
}

void GraphicsScene::setReference()
{
    foreach(Marker* marker, markers)
        marker->setReferencePos();
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!enabled)
        return;

    selectedMarker = nullptr;

    if(markers.isEmpty() || event->modifiers().testFlag(Qt::ControlModifier))
    {
        selectedMarker = new Marker(this, this);
        markers << selectedMarker;
    }
    else
    {
        foreach(Marker* m, markers)
        {
            if(m->getRect().contains(event->scenePos()))
            {
                selectedMarker = m;
                break;
            }
        }
    }

    if(!selectedMarker)
        return;

    selectedMarker->start(event->scenePos());
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!enabled || selectedMarker == nullptr)
        return;
    selectedMarker->mouseMoved(event->scenePos());
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if(!enabled || selectedMarker == nullptr)
        return;
    selectedMarker->finish();
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

    QPixmap pixmap = imageLayer->pixmap();
    if(pixmap.isNull())
        return;

    foreach(Marker* m, markers)
    {
        m->update(pixmap);
    }

    if(selectedMarker == nullptr)
        return;

    StarInfoPtr info(new StarInfo());

    QRectF rect = selectedMarker->getRect();
    info->window = rect.size();
    info->centerPosition = rect.center();

    if(publishScaledImage)
        Q_EMIT starCentered(selectedMarker->getTracker().getScaledStar());
    else
        Q_EMIT starCentered(selectedMarker->getTracker().getCurrentStar());


    info->hfd = selectedMarker->getTracker().getHfd();

    Q_EMIT newHfdValue(info);


}

} // namespace StarTrack
