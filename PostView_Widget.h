#ifndef POSTVIEW_POSTVIEW_WIDGET_H
#define POSTVIEW_POSTVIEW_WIDGET_H

#include <QWidget>
#include <QByteArray>

#include "PostView_Info.h"


namespace StarTrack {
    class GraphicsScene;
}

namespace PostView {

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    QList<Info> imageStack;
    int cursor;

    StarTrack::GraphicsScene* starTrackScene;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    StarTrack::GraphicsScene *getStarTrackScene() const;

public Q_SLOTS :
    void updatePostViewImage(const QByteArray& data);
    void updatePostViewImage(const QPixmap& pixmap);
    void updatePostView();

    void newInfo(const PostView::Info& info);

private Q_SLOTS :
    void on_postViewFwd_clicked();
    void on_postViewBwd_clicked();


    void on_loadTestDataBtn_clicked();

private:
    Ui::Widget *ui;
};


} // namespace PostView
#endif // POSTVIEW_POSTVIEW_WIDGET_H
