#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>


class Settings;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    Settings* settings;


public:
    explicit SettingsDialog(Settings* settings, QWidget *parent = nullptr);
    ~SettingsDialog();


private Q_SLOTS :
    void on_buttonBox_accepted();
    void on_starTrackMarkerModus_activated(int index);

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
