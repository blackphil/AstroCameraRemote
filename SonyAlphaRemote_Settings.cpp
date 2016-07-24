#include "SonyAlphaRemote_Settings.h"

namespace SonyAlphaRemote {


Setting::Setting(QSettings *settings, const QString &namePath)
    : settings(settings)
    , namePath(namePath)
{

}

QVariant Setting::getValue(const QVariant &defaultValue) const
{
    return settings->value(namePath, defaultValue);
}

void Setting::valueChanged()
{
    updateValue();
}


void Settings::registerSpinBox(QSpinBox* spinBox, const QString& namePath, int defaultValue)
{
    registeredSettings << new SpinBoxSetting(&settings, namePath, spinBox, defaultValue);
}

void Settings::registerComboBox(QComboBox *comboBox, const QString &namePath, const QString& defaultValue)
{
    registeredSettings << new ComboBoxSetting(&settings, namePath, comboBox, defaultValue);
}

SpinBoxSetting::SpinBoxSetting(QSettings* settings, const QString& namePath, QSpinBox* spinBox, int defaultValue)
    : Setting(settings, namePath)
    , spinBox(spinBox)
{
    connect(spinBox, SIGNAL(editingFinished()), this, SLOT(valueChanged()));
    spinBox->setValue(settings->value(namePath, defaultValue).toInt());
}

void SpinBoxSetting::updateValue()
{
    settings->setValue(namePath, spinBox->value());
}

ComboBoxSetting::ComboBoxSetting(QSettings* settings, const QString& namePath, QComboBox* comboBox, const QString &defaultValue)
    : Setting(settings, namePath)
    , comboBox(comboBox)
{
    connect(comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(valueChanged()));
    comboBox->setCurrentText(settings->value(namePath, defaultValue).toString());
}

void ComboBoxSetting::updateValue()
{
    settings->setValue(namePath, comboBox->currentText());
}



} // namespace SonyAlphaRemote
