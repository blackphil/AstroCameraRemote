#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Settings
{
class General;
}

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    Settings::General* settings;


public:
    explicit SettingsDialog(Settings::General* settings, QWidget *parent = 0);
    ~SettingsDialog();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
