#include "ToolBar.h"

#include <QMainWindow>
#include <QMouseEvent>

ToolBar::ToolBar(QMainWindow* mw)
    : QToolBar(mw)
    , mw(mw)
    , init(true)
{

}

void ToolBar::mousePressEvent(QMouseEvent *event)
{
    init = true;
    lastPos = event->globalPosition().toPoint();
}

void ToolBar::mouseMoveEvent(QMouseEvent *event)
{
    QPoint nextWindowPos = mw->pos() + event->globalPosition().toPoint() - lastPos;
    mw->move(nextWindowPos.x(), nextWindowPos.y());
    init = false;
    lastPos = event->globalPosition().toPoint();
    mw->showNormal();
}
