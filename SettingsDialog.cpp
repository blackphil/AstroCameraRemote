#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QSettings>

#include "SonyAlphaRemote_Settings.h"
#include "Settings_General.h"
#include "StarTrack_Settings.h"


SettingsDialog::SettingsDialog(SonyAlphaRemote::Settings *settings, QWidget *parent)
    : QDialog(parent)
    , settings(settings)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    settings->save();
    settings->load();

//    Settings::General* general = qobject_cast<Settings::General*>(settings->getSettingByName(Settings::General::getName()));
//    StarTrack::Settings* starTrack = qobject_cast<StarTrack::Settings*>(settings->getSettingByName(StarTrack::Settings::getName()));

//    ui->lenrCheckbox->setChecked(general->getLenrEnabled());

//    ui->starTrackMarkerModus->setCurrentIndex(starTrack->getMarkerModus());
//    ui->starTrackFixedBoundingRectSize->setValue(starTrack->getFixedRectSize());
//    ui->starTrackFixedBoundingRectSize->setEnabled(starTrack->getMarkerModus() == StarTrack::Marker::Modus_FixedRect ? true : false);

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
//    Settings::General* general = qobject_cast<Settings::General*>(settings->getSettingByName(Settings::General::getName()));
//    StarTrack::Settings* starTrack = qobject_cast<StarTrack::Settings*>(settings->getSettingByName(StarTrack::Settings::getName()));


//    general->setLenrEnabled(ui->lenrCheckbox->isChecked());
//    starTrack->setFixedRectSize(ui->starTrackFixedBoundingRectSize->value());
//    starTrack->setMarkerModus((StarTrack::Marker::Modus)ui->starTrackMarkerModus->currentIndex());

//    settings->save();
}

void SettingsDialog::on_starTrackMarkerModus_activated(int index)
{
    ui->starTrackFixedBoundingRectSize->setEnabled(index == StarTrack::Marker::Modus_FixedRect ? true : false);
}
