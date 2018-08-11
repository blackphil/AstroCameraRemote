#include "StarTrack_Marker.h"

#include <QBrush>
#include <QRect>

#include "SonyAlphaRemote_Helper.h"
#include "StarTrack_Settings.h"

namespace StarTrack {


Marker::Marker(GraphicsScene *scene, QObject *parent)
    : QObject(parent)
    , scene(scene)
    , rectItem(Q_NULLPTR)
    , info(Q_NULLPTR)
    , rectPen(QPen(QBrush(Qt::green), 1))
    , crosshairPen(QPen(QBrush(Qt::green), 1))
    , tracking(true )
    , status(Status_Idle)
{

    rectItem = scene->addRect(QRectF(), rectPen);
    rectItem->setZValue(2);

    for(int i=0; i<2; i++)
    {
        crosshair[i] = scene->addLine(QLineF(), crosshairPen);
        crosshair[i]->setZValue(2);
    }

    info = scene->addSimpleText("");
    info->setBrush(QBrush(Qt::green));
    info->setZValue(2);
}

QRectF Marker::getRect() const
{
    if(rectItem)
        return rectItem->rect();
    return QRectF();
}

namespace helper
{
void logRect(const QRectF& r, const QString& context)
{
    Q_UNUSED(r)
    Q_UNUSED(context)
    SAR_INF(
                context <<
                ": center(" << r.center().x() << ", " << r.center().y() << ")"
                ", size(" << r.width() << ", " << r.height() << ")");
}
}

bool Marker::getTracking() const
{
    return tracking;
}

void Marker::setTracking(bool value)
{
    tracking = value;
}

bool Marker::update(const QRectF& r)
{
    if(r == rectItem->rect())
        return false;

    rectItem->setRect(r);

    helper::logRect(r, "bounding");
    auto w = r.width() / 4.0;
    auto h = r.height() / 4.0;
    QRectF chr = r.marginsRemoved(QMarginsF(w, h, w, h));

    SAR_INF("update marker ...");
    helper::logRect(chr, "crosshair");

    crosshair[0]->setLine(QLineF(chr.center().x(), chr.top(), chr.center().x(), chr.bottom()));
    crosshair[1]->setLine(QLineF(chr.left(), chr.center().y(), chr.right(), chr.center().y()));

    info->setPos(rectItem->rect().topLeft() + QPointF(0, -info->boundingRect().height()));

    return true;
}

void Marker::start(const QPointF &pos)
{

    QRectF r;
    switch(Settings::getMarkerModus())
    {
    case Modus_FixedRect :
    {
        auto rectSize = Settings::getFixedRectSize();
        auto rectSizeHalf = rectSize / 2.;
        r = QRectF(pos.x() - rectSizeHalf, pos.y() - rectSizeHalf, rectSize, rectSize);
        break;
    }
    case Modus_Rubberband :
        r = QRectF(pos.x(), pos.y(), 1, 1);
        break;
    }

    startPos = pos;
    rectItem->setRect(r);

    status = Status_Moving;
}

void Marker::finish(const QPointF &pos)
{
    Q_UNUSED(pos)
    Q_EMIT newMark();
}

void Marker::mouseMoved(const QPointF &pos)
{
    if(Status_Moving != status)
        return;

    QRectF r = rectItem->rect();

    switch(Settings::getMarkerModus())
    {
    case Modus_FixedRect :
    {
        qreal rectSize = Settings::getFixedRectSize();
        qreal rectSizeHalf = rectSize / 2.;
        r = QRectF(pos.x() - rectSizeHalf, pos.y() - rectSizeHalf, rectSize, rectSize);
        break;
    }
    case Modus_Rubberband :
        r.setLeft(qMin(startPos.x(), pos.x()));
        r.setRight(qMax(startPos.x(), pos.x()));
        r.setTop(qMin(startPos.y(), pos.y()));
        r.setBottom(qMax(startPos.y(), pos.y()));
        break;
    }


    update(r);

}

void Marker::setInfo(const QString &text)
{
    info->setText(text);
}

namespace helper
{
bool checkSize(const QSize& a, const QSize& b)
{
    return a.width() == b.width() && a.height() == b.height();
}
}
QRectF Marker::centerStar(const QImage &scaledStar)
{
    if(scaledStar.isNull())
        return QRectF();

    QRectF rect = rectItem->rect();
    if(!tracking)
        return rect;

    if(!helper::checkSize(rect.size().toSize(), scaledStar.size()))
    {
        SAR_WRN("centering star failed! Sizes of start image and marker rect are different: "
                << "image(" << scaledStar.width() << ", " << scaledStar.height() << ")"
                << "marker(" << rect.width() << ", " << rect.height() << ")");
        return QRectF();
    }

    QPointF currentCenter(rect.center());

    QPointF newCenterLocal = currentCenter - rect.topLeft();

    qreal c = 0;
    qreal x = 0, y = 0;
    for(auto i=0.; i<rect.height(); i+=0.1)
    {
        for(auto j=0.; j<rect.width(); j+=0.1)
        {
            auto pixel = qGray(scaledStar.pixel(static_cast<int>(j), static_cast<int>(i))) / 255.;
            c += pixel;
            x += j * pixel;
            y += i * pixel;
        }
    }
    if(c>0)
        newCenterLocal = QPointF(x/c, y/c);

    SAR_INF("count-black: " << c);


    QPointF newCenter = rect.topLeft() + newCenterLocal;

    SAR_INF("topLeft(" << rect.topLeft().x() << ", " << rect.topLeft().y() << ")");
    SAR_INF("bottomRight(" << rect.bottomRight().x() << ", " << rect.bottomRight().y() << ")");
    SAR_INF("center(" << rect.center().x() << ", " << rect.center().y() << ")");
    SAR_INF("newcenterLocal(" << newCenterLocal.x() << ", " << newCenterLocal.y() << ")");
    SAR_INF("newcenter(" << newCenter.x() << ", " << newCenter.y() << ")");

    QPointF toMove = newCenter - currentCenter;
    Q_UNUSED(toMove)
    SAR_INF("toMove(" << toMove.x() << ", " << toMove.y() << ")");

    QRectF newRect(newCenter.x() - (rect.width()/2.), newCenter.y() - (rect.height()/2.), rect.width(), rect.height());
    update(newRect);

    SAR_INF("END");

    return newRect;
}

void Marker::update()
{
    QRectF r = getRect();

    switch(Settings::getMarkerModus())
    {
    case Marker::Modus_Rubberband :
        break;
    case Marker::Modus_FixedRect :
    {
        qreal size = Settings::getFixedRectSize();
        qreal sizeHalf = size / 2.;
        QPointF center = r.center();
        r.setTopLeft(QPointF(center.x()-sizeHalf, center.y()-sizeHalf));
        r.setSize(QSizeF(size, size));
        break;
    }
    }

    if(update(r))
        Q_EMIT newMark();
}


} // namespace StarTrack
