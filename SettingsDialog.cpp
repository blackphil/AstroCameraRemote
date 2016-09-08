#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QSettings>

#include "Settings_General.h"



SettingsDialog::SettingsDialog(Settings::General* settings,QWidget *parent)
    : QDialog(parent)
    , settings(settings)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    Q_ASSERT(settings);
    if(settings)
    {
        settings->load();
        ui->lenrCheckbox->setChecked(settings->getLenrEnabled());
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    Q_ASSERT(settings);
    if(!settings)
        return;

    settings->setLenrEnabled(ui->lenrCheckbox->isChecked());
    settings->save();
}
