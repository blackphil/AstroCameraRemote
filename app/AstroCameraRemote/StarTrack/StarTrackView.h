#ifndef StarTrack_STARTRACKVIEW_H
#define StarTrack_STARTRACKVIEW_H

#include <QWidget>
#include "StarInfo.h"

namespace Ui {
	class StarTrack_StarTrackView;
}

namespace StarTrack {

class LenseGraphcisScene;

class StarTrackView : public QWidget
{
    Q_OBJECT

    LenseGraphcisScene* lense;

public:
    explicit StarTrackView(QWidget *parent = nullptr, bool fullScreen = false);
    ~StarTrackView();

    StarInfoPtr getStarInfo() const;
    QImage getStar() const;

Q_SIGNALS :
    void trackingEnabledStatusToggled(bool);
    void updateMarker();
    void toggleFullScreen(bool);
    void selectNextStar();
    void selectPreviousStar();

public Q_SLOTS :

    void updateStar(const QImage &img);
    void updateHfdValue(StarInfoPtr hfd);
    void fullScreenToggled(bool yes);
    void applyStatusFrom(StarTrackView* other);

private slots:
    void on_markerModusCombobox_activated(int index);

    void on_markerFixedRectSpinbox_editingFinished();

    void on_hfdFontPointSize_valueChanged(int arg1);

    void on_prev_clicked();

    void on_next_clicked();

private:
   ::Ui::StarTrack_StarTrackView *ui;

    void keyPressEvent(QKeyEvent* ke);
};


} // namespace StarTrack
#endif // StarTrack_STARTRACKVIEW_H
