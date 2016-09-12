#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>


class ToolBar : public QToolBar
{
    Q_OBJECT

    QMainWindow* mw;

    bool init;
    QPoint lastPos;

public:
    ToolBar(QMainWindow* mw);

protected :
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // TOOLBAR_H
