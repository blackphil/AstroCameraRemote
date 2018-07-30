#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace SonyAlphaRemote
{
class Settings;
}

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    SonyAlphaRemote::Settings* settings;


public:
    explicit SettingsDialog(SonyAlphaRemote::Settings* settings, QWidget *parent = 0);
    ~SettingsDialog();


private Q_SLOTS :
    void on_buttonBox_accepted();
    void on_starTrackMarkerModus_activated(int index);

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
