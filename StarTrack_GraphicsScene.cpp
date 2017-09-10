#include "StarTrack_GraphicsScene.h"

#include "StarTrack_Marker.h"
#include "SonyAlphaRemote_Helper.h"
#include "hfd/Hfd_Calculator.h"
#include "StarTrack_Settings.h"


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

GraphicsScene::GraphicsScene(QObject* parent)
    : QGraphicsScene(parent)
    , marker(new Marker(this, this))
    , enabled(true)
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
    if(!enabled)
        return;

    imageLayer->setPixmap(pixmap);
    newMark(marker->getRect());
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

void GraphicsScene::newMark(QRectF rect)
{
    if(!enabled)
        return;
    if(rect.isNull())
        return;

    QPixmap pixmap = imageLayer->pixmap();
    if(pixmap.isNull())
        return;

    SAR_INF("BEGIN pixmap(" << pixmap.size().width() << ", " << pixmap.size().height() << ")");

    static int imgCount = 0;


    Hfd::Calculator hfd;

    QImage star = pixmap.copy(rect.toRect()).toImage();
    double mean = hfd.meanValue(star);
    QImage scaledStar = hfd.scaledImage(star, mean);


    marker->centerStar(scaledStar);

    star = imageLayer->pixmap().copy(marker->getRect().toRect()).toImage();
    mean = hfd.meanValue(star);
    scaledStar = hfd.scaledImage(star, mean);

    helper::debugSaveImage(scaledStar, QString("scaled_%0").arg(imgCount++));

    float outerDiameter = qMin(rect.width(), rect.height());
    float hfdValue = hfd.calcHfd(scaledStar, qRound(outerDiameter));
    hfdValue = outerDiameter / hfdValue;

    marker->setInfo(QString("hfd(%0)").arg(hfdValue));

    Q_EMIT starCentered(star);
    Q_EMIT newHfdValue(hfdValue);

    SAR_INF("END");

}




} // namespace StarTrack
