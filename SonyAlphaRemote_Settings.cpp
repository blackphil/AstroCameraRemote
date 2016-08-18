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

void Settings::registerDoubleSpinBox(QDoubleSpinBox* spinBox, const QString& namePath, double defaultValue)
{
    registeredSettings << new DoubleSpinBoxSetting(&settings, namePath, spinBox, defaultValue);
}

void Settings::registerComboBox(QComboBox *comboBox, const QString &namePath, const QString& defaultValue)
{
    registeredSettings << new ComboBoxSetting(&settings, namePath, comboBox, defaultValue);
}

void Settings::registerTimeUnitButton(TimeUnitButton *button, const QString &namePath, TimeUnitButton::Unit defaultValue)
{
    registeredSettings << new TimeUnitButtonSetting(&settings, namePath, button, defaultValue);
}

SpinBoxSetting::SpinBoxSetting(QSettings* settings, const QString& namePath, QSpinBox* spinBox, int defaultValue)
    : Setting(settings, namePath)
    , spinBox(spinBox)
{
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
    spinBox->setValue(settings->value(namePath, defaultValue).toInt());
}

void SpinBoxSetting::updateValue()
{
    settings->setValue(namePath, spinBox->value());
}

DoubleSpinBoxSetting::DoubleSpinBoxSetting(QSettings* settings, const QString& namePath, QDoubleSpinBox* spinBox, double defaultValue)
    : Setting(settings, namePath)
    , spinBox(spinBox)
{
    connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(valueChanged()));
    double v = settings->value(namePath, defaultValue).toDouble();
    spinBox->setMaximum(9999999.);
    spinBox->setValue(v);
}

void DoubleSpinBoxSetting::updateValue()
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

TimeUnitButtonSetting::TimeUnitButtonSetting(QSettings *settings, const QString &namePath, TimeUnitButton *button, TimeUnitButton::Unit defaultValue)
    : Setting(settings, namePath)
    , button(button)
{
    connect(button, SIGNAL(unitChanged(Unit)), this, SLOT(valueChanged()));
    button->setCurrentUnit((TimeUnitButton::Unit)settings->value(namePath, defaultValue).toInt());
}

void TimeUnitButtonSetting::updateValue()
{
    settings->setValue(namePath, button->currentUnit());
}



} // namespace SonyAlphaRemote
