#include "GraphicsScene.h"

#include "Marker.h"
#include "AstroBase.h"
#include "../hfd/Hfd_Calculator.h"
#include "Settings.h"
#include "StarInfo.h"


#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QThread>
#include <QGraphicsView>
#include <QMessageBox>

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
    , imageLayer(nullptr)
    , selectedMarker(nullptr)
    , enabled(true)
    , publishScaledImage(Settings::getPublishScaledImage())
{


    QRect defaultRect(0, 0, 808, 540);

    setSceneRect(defaultRect);

    QFile defaultImage(":/images/LiveView/NoImage.jpg");
    defaultImage.open(QIODevice::ReadOnly);
    QByteArray imageData = defaultImage.readAll();

    if(QImage img { QImage::fromData(imageData, "JPG") }; !img.isNull())
    {
        imageLayer = addPixmap(QPixmap::fromImage(img.scaled(defaultRect.size())));
        imageLayer->setZValue(0);
    }

}

GraphicsScene::~GraphicsScene()
{
    qDeleteAll(markers);
}

void GraphicsScene::updateBackground(const QPixmap &pixmap)
{
//    AB_DBG("VISIT (thread:" << QThread::currentThread()->objectName() << "[" << QThread::currentThreadId() << "]");
    if(!enabled || pixmap.isNull())
        return;

    imageLayer->setPixmap(pixmap.scaledToHeight(sceneRect().toRect().height()));
    newMark();
}

void GraphicsScene::updateMarker()
{
    AB_DBG("VISIT (thread:" << QThread::currentThread()->objectName() << "[" << QThread::currentThreadId() << "]");
    if(!enabled)
        return;

    QPixmap pm = imageLayer->pixmap();

    for(auto marker : markers)
        marker->update(pm);
}

void GraphicsScene::setReference()
{
    QList<QRectF> refRects;
    for(auto marker : markers)
    {
        marker->setReferencePos();
        refRects << marker->getRect();
    }

    Q_EMIT newReferenceMarkers(refRects);
}

void GraphicsScene::unsetReference()
{
    for(auto marker : markers)
        marker->unsetReferencePos();
}

void GraphicsScene::removeSelectedMarker()
{
    if(!selectedMarker)
        return;

    QWidget* mainWidget = views().first();
    do
    {
        if(mainWidget->parentWidget())
            mainWidget = mainWidget->parentWidget();
        else
            break;
    }
    while(mainWidget);

    Q_ASSERT(mainWidget);
    if(!mainWidget)
        return;

    if(QMessageBox::Yes == QMessageBox::question(mainWidget, tr("Remote star tracker"), tr("Remove star tracker?")))
    {
        markers.removeAll(selectedMarker);
        selectedMarker->deleteLater();
        if(!markers.isEmpty())
            setSelectedMarker(markers.last());
        else
            setSelectedMarker(nullptr);

    }
}

void GraphicsScene::cleanUpMarkers()
{
    setSelectedMarker(nullptr);
    qDeleteAll(markers);
    markers.clear();
}

void GraphicsScene::applyReferenceMarkers(const QList<QRectF> &refMarkers)
{
    cleanUpMarkers();
    for(auto m : refMarkers)
    {
        Marker* marker = new Marker(this, this);
        marker->forceRect(m);
        marker->setReferencePos();
        markers << marker;
    }
}

void GraphicsScene::selectNextStar()
{
    if(markers.isEmpty())
        return;

    Q_ASSERT(selectedMarker);
    if(!selectedMarker)
        return;

    int index = markers.indexOf(selectedMarker);
    Q_ASSERT(0 <= index && index < markers.count());
    if(0 > index || index >= markers.count())
        return;

    index++;
    if(markers.count() == index)
        index = 0;
    setSelectedMarker(markers[index]);
}

void GraphicsScene::selectPreviousStar()
{
    if(markers.isEmpty())
        return;

    Q_ASSERT(selectedMarker);
    if(!selectedMarker)
        return;

    int index = markers.indexOf(selectedMarker);
    Q_ASSERT(0 <= index && index < markers.count());
    if(0 > index || index >= markers.count())
        return;

    index--;
    if(0 > index)
        index = markers.count()-1;
    setSelectedMarker(markers[index]);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!enabled)
        return;

    if(markers.isEmpty() || event->modifiers().testFlag(Qt::ControlModifier))
    {
        Marker* m = new Marker(this, this);
        markers << m;
        setSelectedMarker(m);
    }
    else
        setSelectedMarker(event->scenePos());

    if(selectedMarker)
        selectedMarker->start(event->scenePos());
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!enabled || selectedMarker == nullptr)
        return;
    if(event->buttons().testFlag(Qt::LeftButton) && event->modifiers() == 0)
        selectedMarker->mouseMoved(event->scenePos());
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if(!enabled || selectedMarker == nullptr)
        return;
    selectedMarker->finish();
}

void GraphicsScene::keyPressEvent(QKeyEvent *event)
{
    if(Qt::Key_Delete == event->key())
        removeSelectedMarker();

    QGraphicsScene::keyPressEvent(event);
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
//    AB_DBG("VISIT (thread:" << QThread::currentThread()->objectName() << "[" << QThread::currentThreadId() << "]");
    if(!enabled)
        return;

    QPixmap pixmap = imageLayer->pixmap();
    if(pixmap.isNull())
        return;

    for(auto m : markers)
        m->update(pixmap);

    if(selectedMarker == nullptr)
    {
//        AB_DBG("No selected marker");
        return;
    }

    if(selectedMarker->getTracker().getRect().isNull())
    {
        AB_DBG("Selected marker doesn't track any star for now");
        return;
    }

    AB_DBG("Handle selected marker's star tracked ...");

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

void GraphicsScene::setSelectedMarker(const QPointF &pos)
{

    for(auto m : markers)
    {
        if(m->getRect().contains(pos))
        {
            setSelectedMarker(m);
            return;
        }
    }

    setSelectedMarker(nullptr);

}

void GraphicsScene::setSelectedMarker(Marker *m)
{
    if(selectedMarker)
        selectedMarker->setIsSelected(false);

    selectedMarker = m;

    if(selectedMarker)
        selectedMarker->setIsSelected(true);
}


} // namespace StarTrack
