#include "StarTrack_Tracker.h"

#include <QPixmap>

#include "hfd/Hfd_Calculator.h"
#include "AstroBase.h"

namespace StarTrack {

Tracker::Tracker()
    : hfd(0)
{

}

void Tracker::setRect(const QRectF &r)
{
    currentRect = r;
    currentStar = QImage();
    scaledStar = QImage();
}

QRectF Tracker::getRect() const
{
    return currentRect;
}

bool Tracker::update(const QPixmap& pm)
{
    if(pm.isNull())
        return false;

    static const qreal epsilon = 1;

    Hfd::Calculator hfdCalculcator;
    QRectF newRect = currentRect;
    QPointF currentCenter = currentRect.center();
    QPointF newCenter;

    int dbgRunsNeeded = 0;
    do
    {
        currentRect = newRect;
        currentCenter = newRect.center();

        currentStar = pm.copy(currentRect.toRect()).toImage();
        double mean = hfdCalculcator.meanValue(currentStar);
        scaledStar = hfdCalculcator.scaledImage(currentStar, mean);

        newRect = centerStar();
        if(newRect.isNull())
            return false;

        newCenter = newRect.center();

        dbgRunsNeeded++;

        AB_DBG("run(" << dbgRunsNeeded << "), newCenter(" << newCenter.x() << "," << newCenter.y() << "), manhattenLength(" << (currentCenter-newCenter).manhattanLength() << ")");

    }
    while((currentCenter-newCenter).manhattanLength() > epsilon);


    qreal outerDiameter = qMin(newRect.width(), newRect.height());
    qreal hfdValue = hfdCalculcator.calcHfd(scaledStar, qRound(outerDiameter));
    hfd = outerDiameter / hfdValue;
    return true;

}

namespace helper
{
bool checkSize_(const QSize& a, const QSize& b)
{
    return a.width() == b.width() && a.height() == b.height();
}
}
QImage Tracker::getCurrentStar() const
{
    return currentStar;
}

QImage Tracker::getScaledStar() const
{
    return scaledStar;
}

qreal Tracker::getHfd() const
{
    return hfd;
}

QRectF Tracker::centerStar()
{
    if(scaledStar.isNull())
        return QRectF();

    QRectF rect = currentRect;

    if(!helper::checkSize_(rect.size().toSize(), scaledStar.size()))
    {
        AB_WRN("centering star failed! Sizes of start image and marker rect are different: "
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

    AB_INF("count-black: " << c);


    QPointF newCenter = rect.topLeft() + newCenterLocal;

#if 0
    AB_INF("topLeft(" << rect.topLeft().x() << ", " << rect.topLeft().y() << ")");
    AB_INF("bottomRight(" << rect.bottomRight().x() << ", " << rect.bottomRight().y() << ")");
    AB_INF("center(" << rect.center().x() << ", " << rect.center().y() << ")");
    AB_INF("newcenterLocal(" << newCenterLocal.x() << ", " << newCenterLocal.y() << ")");
    AB_INF("newcenter(" << newCenter.x() << ", " << newCenter.y() << ")");
#endif

    QPointF toMove = newCenter - currentCenter;
    Q_UNUSED(toMove)
//    AB_INF("toMove(" << toMove.x() << ", " << toMove.y() << ")");

    QRectF newRect(newCenter.x() - (rect.width()/2.), newCenter.y() - (rect.height()/2.), rect.width(), rect.height());

//    AB_INF("END");

    return newRect;
}

Tracker::Tracker(const Tracker &)
{

}

} // namespace StarTrack
