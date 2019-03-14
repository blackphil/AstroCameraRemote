#ifndef POSTVIEW_POSTVIEW_WIDGET_H
#define POSTVIEW_POSTVIEW_WIDGET_H

#include <QWidget>
#include <QByteArray>
#include <QDir>

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
    int refImageIndex;
    bool enabled;

    StarTrack::GraphicsScene* starTrackScene;

    void resizeEvent(QResizeEvent* re);

public:
    explicit Widget(QWidget *parent = Q_NULLPTR);
    ~Widget();

    StarTrack::GraphicsScene *getStarTrackScene() const;

    bool getEnabled() const;
    void setEnabled(bool value);

public Q_SLOTS :
    void updatePostViewImage(const QByteArray &data);
    void updatePostViewImage(const QPixmap &pixmap);
    void updatePostView();

    void newInfo(const PostView::Info& info);

private Q_SLOTS :

    void reset();

    void newHfdValue(StarTrack::StarInfoPtr starInfo);

    void setImage(int index);

    void on_postViewFwd_clicked();
    void on_postViewBwd_clicked();


    void on_loadTestDataBtn_clicked();

    void on_openFilesBtn_clicked();

    void loadFiles(const QStringList& files);

    void on_clearBtn_clicked();

    void on_setAsRefImg_clicked();

    void on_refImg_clicked();

    void on_firstImg_clicked();

    void on_latestImg_clicked();

private:
    Ui::Widget *ui;
};


} // namespace PostView
#endif // POSTVIEW_POSTVIEW_WIDGET_H
