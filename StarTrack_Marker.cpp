#include "StarTrack_Marker.h"

#include <QBrush>
#include <QRect>

#include "SonyAlphaRemote_Helper.h"


namespace StarTrack {

Marker::Marker(QGraphicsScene* scene, QObject *parent)
    : QObject(parent)
    , scene(scene)
    , rectItem(NULL)
    , info(NULL)
    , rectPen(QPen(QBrush(Qt::green), 1))
    , status(Status_Idle)
{

}

QRectF Marker::getRect() const
{
    if(rectItem)
        return rectItem->rect();
    return QRectF();
}

void Marker::start(const QPointF &pos)
{
    if(rectItem)
        delete rectItem;

    rectItem = scene->addRect(pos.x(), pos.y(), 1, 1, rectPen);
    rectItem->setZValue(2);
    status = Status_Moving;

    startPos = pos;


}

void Marker::finish(const QPointF &pos)
{
    Q_UNUSED(pos)
    Q_EMIT newMark(getRect());
}

void Marker::mouseMoved(const QPointF &pos)
{
    QRectF current = rectItem->rect();
    switch(status)
    {
    case Status_Idle :
        break;
    case Status_Moving :
        current.setLeft(qMin(startPos.x(), pos.x()));
        current.setRight(qMax(startPos.x(), pos.x()));
        current.setTop(qMin(startPos.y(), pos.y()));
        current.setBottom(qMax(startPos.y(), pos.y()));
        rectItem->setRect(current);
        break;
    case Status_Finished :
        break;
    }


}

void Marker::setInfo(const QString &text)
{
    if(!info)
    {
        info = scene->addSimpleText(text);
        info->setBrush(QBrush(Qt::green));
    }
    else
        info->setText(text);

    info->setPos(rectItem->rect().topLeft() + QPoint(0, -info->boundingRect().height()));
}

void Marker::centerStar(const QImage &scaledStar)
{
    if(!rectItem)
        return;

    QRectF rect = rectItem->rect();
    QPointF currentCenter(rect.center());

    int brightestPixel = 0;
    QPointF newCenterLocal = currentCenter - rect.topLeft();
    if(0)
    {
        for(int y=0; y<scaledStar.height(); y++)
        {
            for(int x=0; x<scaledStar.width(); x++)
            {
                int pixel = qGray(scaledStar.pixel(x, y));
                if(pixel >= brightestPixel)
                {
                    brightestPixel = pixel;
                    newCenterLocal = QPoint(x, y);
                }
            }
        }
    }
    else
    {

        double c = 0;
        double x = 0, y = 0;
        for(int i=0; i<rect.height(); i++)
        {
            for(int j=0; j<rect.width(); j++)
            {
                int pixel = qGray(scaledStar.pixel(j, i));
                if(pixel>10)
                {
                    c=c+1;
                    x=x+j;
                    y=y+i;
                }
            }
        }
        if(c>0)
        {
            newCenterLocal = QPointF(x/c, y/c);
        }
        SAR_INF("count-black: " << c);
    }

    QPointF newCenter = rect.topLeft() + newCenterLocal;

    SAR_INF("topLeft(" << rect.topLeft().x() << ", " << rect.topLeft().y() << ")");
    SAR_INF("bottomRight(" << rect.bottomRight().x() << ", " << rect.bottomRight().y() << ")");
    SAR_INF("center(" << rect.center().x() << ", " << rect.center().y() << ")");
    SAR_INF("newcenterLocal(" << newCenterLocal.x() << ", " << newCenterLocal.y() << ")");
    SAR_INF("newcenter(" << newCenter.x() << ", " << newCenter.y() << ")");

    QPointF toMove = newCenter - currentCenter;
    SAR_INF("toMove(" << toMove.x() << ", " << toMove.y() << ")");

    QRectF newRect(newCenter.x() - (rect.width()/2.), newCenter.y() - (rect.height()/2.), rect.width(), rect.height());
    rectItem->setRect(newRect);
}

} // namespace StarTrack
