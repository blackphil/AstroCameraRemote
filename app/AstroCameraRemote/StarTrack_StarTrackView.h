#ifndef StarTrack_STARTRACKVIEW_H
#define StarTrack_STARTRACKVIEW_H

#include <QWidget>

namespace StarTrack {

class LenseGraphcisScene;

namespace Ui {
class StarTrackView;
}

class StarTrackView : public QWidget
{
    Q_OBJECT

    LenseGraphcisScene* lense;

public:
    explicit StarTrackView(QWidget *parent = nullptr, bool fullScreen = false);
    ~StarTrackView();

    float getHfdValue() const;
    QImage getStar() const;

Q_SIGNALS :
    void trackingEnabledStatusToggled(bool);
    void updateMarker();
    void toggleFullScreen(bool);

public Q_SLOTS :

    void updateStar(const QImage &img);
    void updateHfdValue(float hfd);
    void fullScreenToggled(bool yes);
    void applyStatusFrom(StarTrackView* other);

private slots:
    void on_markerModusCombobox_activated(int index);

    void on_markerFixedRectSpinbox_editingFinished();

    void on_hfdFontPointSize_valueChanged(int arg1);

private:
    Ui::StarTrackView *ui;

    void keyPressEvent(QKeyEvent* ke);
};


} // namespace StarTrack
#endif // StarTrack_STARTRACKVIEW_H
