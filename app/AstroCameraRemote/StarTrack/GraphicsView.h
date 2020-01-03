#ifndef STARTRACK_GRAPHICSVIEW_H
#define STARTRACK_GRAPHICSVIEW_H

#include <QGraphicsView>


namespace StarTrack {

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget* parent = nullptr);

protected :
    void resizeEvent(QResizeEvent*);
};

} // namespace StarTrack

#endif // STARTRACK_GRAPHICSVIEW_H
