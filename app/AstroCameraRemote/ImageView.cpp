#include "ImageView.h"

#include <QResizeEvent>
#include <QtMath>

#include <AstroBase/AstroBase>

ImageView::ImageView(QWidget* parent)
    : QGraphicsView { parent }
    , zoomFactorBbase { 1.0015 }
{
    viewport()->installEventFilter(this);
    setMouseTracking(true);
}

void ImageView::gentleZoom(double factor)
{
    AB_DBG("viewport size:" << viewport()->size().width() << "," << viewport()->size().height());
    AB_DBG("   scene size:" << sceneRect().size().width() << "," << sceneRect().size().height());
    AB_DBG("   frame size:" << frameSize().width() << "," << frameSize().height());
    scale(factor, factor);
    centerOn(targetScenePos);

    QPointF delta_viewport_pos
    {
        targetViewportPos - QPointF(viewport()->width() / 2.0,
                                    viewport()->height() / 2.0)
    };

    QPointF viewport_center
    {
        mapFromScene(targetScenePos) - delta_viewport_pos
    };

    centerOn(mapToScene(viewport_center.toPoint()));
    Q_EMIT zoomed(true);
}


void ImageView::setZoomFactorBase(double value)
{
    zoomFactorBbase = value;
}

void ImageView::zoom1to1()
{
    gentleZoom(1./transform().m11());
}

void ImageView::zoomIn()
{
    gentleZoom(1.2);
}

void ImageView::zoomOut()
{
    gentleZoom(0.84);
}

bool ImageView::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event { static_cast<QMouseEvent*>(event) };

        if(QPointF delta { targetViewportPos - mouse_event->pos() }; qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            targetViewportPos = mouse_event->pos();
            targetScenePos = mapToScene(mouse_event->pos());
        }
    }
    else if (event->type() == QEvent::Wheel)
    {
        if(QWheelEvent* wheel_event { static_cast<QWheelEvent*>(event) }
                ; wheel_event->modifiers().testFlag(Qt::ControlModifier) && wheel_event->angleDelta().y() != 0) //ToDo: Check Qt6 Porting
        {
            double angle = wheel_event->angleDelta().y();
            double factor = qPow(zoomFactorBbase, angle);
            gentleZoom(factor);
            return true;
        }
    }

    Q_UNUSED(object)
    return false;
}

void ImageView::fitToWindow(bool yes)
{
    if(!yes)
        return;

    QTransform t = transform();

    QSizeF absFactors(viewport()->width()/sceneRect().width(), viewport()->height()/sceneRect().height());
    qreal absFactor = qMin(absFactors.width(), absFactors.height()) / t.m11();

    t.scale(absFactor, absFactor);
    setTransform(t);

    Q_EMIT zoomed(false);
}

