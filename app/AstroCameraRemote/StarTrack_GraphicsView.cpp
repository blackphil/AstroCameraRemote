#include "StarTrack_GraphicsView.h"

namespace StarTrack {

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView (parent)
{

}

void GraphicsView::resizeEvent(QResizeEvent *re)
{
    fitInView(0, 0, 200, 200, Qt::KeepAspectRatio);
}

} // namespace StarTrack
