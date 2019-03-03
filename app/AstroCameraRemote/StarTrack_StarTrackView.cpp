#include "StarTrack_StarTrackView.h"
#include "ui_StarTrack_StarTrackView.h"

#include "StarTrack_Marker.h"
#include "StarTrack_LenseGraphcisScene.h"
#include "StarTrack_Settings.h"
#include "StarTrack_StarInfo.h"

#include "AstroBase.h"

#include <QKeyEvent>

namespace StarTrack {

static int objCount(0);

StarTrackView::StarTrackView(QWidget *parent, bool fullScreen)
    : QWidget(parent)
    , lense(new LenseGraphcisScene(this))
    , ui(new Ui::StarTrackView)
{
    objCount++;
    AB_DBG("CTOR" << objCount);

    ui->setupUi(this);
    ui->toggleFullscreenBtn->setChecked(fullScreen);

    connect(ui->toggleFullscreenBtn, SIGNAL(toggled(bool)), this, SIGNAL(toggleFullScreen(bool)));

    connect(ui->trackStarEnabledCheckbox, SIGNAL(toggled(bool)), this, SIGNAL(trackingEnabledStatusToggled(bool)));

    StarTrack::Marker::Modus modus = Settings::getMarkerModus();
    ui->markerModusCombobox->setCurrentIndex(modus);
    ui->markerFixedRectSpinbox->setValue(static_cast<int>(Settings::getFixedRectSize()));
    if(Marker::Modus_FixedRect == modus)
        ui->markerFixedRectSpinbox->setEnabled(true);
    else
        ui->markerFixedRectSpinbox->setEnabled(false);
    ui->scaledImageCheckbox->setChecked(Settings::getPublishScaledImage());
    ui->starTrackView->setScene(lense);

    this->lense->changeHfdFontPointSize(Settings::getHfdDisplayFontSize());
}

StarTrackView::~StarTrackView()
{
    objCount--;
    AB_DBG("DTOR" << objCount);
    delete ui;
}

StarInfoPtr StarTrackView::getStarInfo() const
{
    return lense->getStarInfo();
}

QImage StarTrackView::getStar() const
{
    return lense->getStar();
}

void StarTrackView::updateStar(const QImage& img)
{
    lense->updateStar(img);
}

void StarTrackView::updateHfdValue(StarInfoPtr hfd)
{
    lense->updateHfd(hfd);
}

void StarTrackView::fullScreenToggled(bool yes)
{
    ui->toggleFullscreenBtn->blockSignals(true);
    ui->toggleFullscreenBtn->setChecked(yes);
    ui->toggleFullscreenBtn->blockSignals(false);
}

void StarTrackView::applyStatusFrom(StarTrackView *other)
{
    updateHfdValue(other->getStarInfo());
    updateStar(other->getStar());

    lense->changeHfdFontPointSize(Settings::getHfdDisplayFontSize());
    ui->hfdFontPointSize->setValue(Settings::getHfdDisplayFontSize());
    ui->settingsGroupBox->setDisabled(isFullScreen());
}

void StarTrackView::on_markerModusCombobox_activated(int index)
{
    StarTrack::Marker::Modus modus = static_cast<StarTrack::Marker::Modus>(index);
    StarTrack::Settings::setMarkerModus(modus);
    if(StarTrack::Marker::Modus_FixedRect == modus)
        ui->markerFixedRectSpinbox->setEnabled(true);
    else
        ui->markerFixedRectSpinbox->setEnabled(false);

    Q_EMIT updateMarker();
}



void StarTrackView::on_markerFixedRectSpinbox_editingFinished()
{
    qreal newVal = ui->markerFixedRectSpinbox->value();
    qreal currentVal = StarTrack::Settings::getFixedRectSize();
    AB_INF("new(" << newVal << "), current(" << currentVal << ")");
    if(AB_EQUAL(newVal, currentVal))
        return;
    StarTrack::Settings::setFixedRectSize(newVal);
    Q_EMIT updateMarker();
}

void StarTrackView::keyPressEvent(QKeyEvent *ke)
{
    if(ke->key() == Qt::Key_Escape && isFullScreen())
        Q_EMIT toggleFullScreen(false);

    QWidget::keyPressEvent(ke);
}

void StarTrackView::on_hfdFontPointSize_valueChanged(int v)
{
    lense->changeHfdFontPointSize(v);
    Settings::setHfdDisplayFontSize(v);
}

} // namespace StarTrack


