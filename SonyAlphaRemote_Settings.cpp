#include "SonyAlphaRemote_Settings.h"

namespace SonyAlphaRemote {


Setting::Setting(QSettings *settings, const QString &namePath, const QVariant& defaultValue)
    : settings(settings)
    , namePath(namePath)
    , activeSequencerSettings("default")
    , defaultValue(defaultValue)
{

}

void Setting::setValue(QString path, const QVariant &value)
{
    QString resolvedPath = path.replace("<sequencer-settings-name>", activeSequencerSettings);
    settings->setValue(resolvedPath, value);
}

QVariant Setting::getValue() const
{
    return settings->value(QString(namePath).replace("<sequencer-settings-name>", activeSequencerSettings), defaultValue);
}

void Setting::valueChanged()
{
    updateValue();
}

void Setting::activeSequencerSettingsChanged(const QString &name)
{
    activeSequencerSettings = name;
}

void Setting::save()
{
    setValue(namePath, getValue());
}


void Settings::registerSpinBox(QSpinBox* spinBox, const QString& namePath, int defaultValue)
{
    registeredSettings << new SpinBoxSetting(&settings, namePath, spinBox, defaultValue);
    connect(
                this, SIGNAL(activeSequencerSettingsChanged(QString))
                , registeredSettings.last(), SLOT(activeSequencerSettingsChanged(QString)));

}

void Settings::registerDoubleSpinBox(QDoubleSpinBox* spinBox, const QString& namePath, double defaultValue)
{
    registeredSettings << new DoubleSpinBoxSetting(&settings, namePath, spinBox, defaultValue);
    connect(
                this, SIGNAL(activeSequencerSettingsChanged(QString))
                , registeredSettings.last(), SLOT(activeSequencerSettingsChanged(QString)));
}

void Settings::registerComboBox(QComboBox *comboBox, const QString &namePath, const QString& defaultValue)
{
    registeredSettings << new ComboBoxSetting(&settings, namePath, comboBox, defaultValue);
    connect(
                this, SIGNAL(activeSequencerSettingsChanged(QString))
                , registeredSettings.last(), SLOT(activeSequencerSettingsChanged(QString)));
}

void Settings::registerTimeUnitButton(TimeUnitButton *button, const QString &namePath, TimeUnitButton::Unit defaultValue)
{
    registeredSettings << new TimeUnitButtonSetting(&settings, namePath, button, defaultValue);
    connect(
                this, SIGNAL(activeSequencerSettingsChanged(QString))
                , registeredSettings.last(), SLOT(activeSequencerSettingsChanged(QString)));
}

void Settings::save()
{
    foreach(Setting* s, registeredSettings)
    {
        s->save();
    }
}

void Settings::load()
{
    foreach(Setting* s, registeredSettings)
    {
        s->load();
    }
}

SpinBoxSetting::SpinBoxSetting(QSettings* settings, const QString& namePath, QSpinBox* spinBox, int defaultValue)
    : Setting(settings, namePath, defaultValue)
    , spinBox(spinBox)
{
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
    spinBox->setValue(getValue().toInt());
}

void SpinBoxSetting::updateValue()
{
    setValue(namePath, spinBox->value());
}

void SpinBoxSetting::load()
{
    spinBox->setValue(getValue().toInt());
}

DoubleSpinBoxSetting::DoubleSpinBoxSetting(QSettings* settings, const QString& namePath, QDoubleSpinBox* spinBox, double defaultValue)
    : Setting(settings, namePath, defaultValue)
    , spinBox(spinBox)
{
    connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(valueChanged()));
    double v = getValue().toDouble();
    spinBox->setMaximum(9999999.);
    spinBox->setValue(v);
}

void DoubleSpinBoxSetting::updateValue()
{
    setValue(namePath, spinBox->value());
}

void DoubleSpinBoxSetting::load()
{
    spinBox->setValue(getValue().toDouble());
}

ComboBoxSetting::ComboBoxSetting(QSettings* settings, const QString& namePath, QComboBox* comboBox, const QString &defaultValue)
    : Setting(settings, namePath, defaultValue)
    , comboBox(comboBox)
{
    connect(comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(valueChanged()));
    comboBox->setCurrentText(getValue().toString());
}

void ComboBoxSetting::updateValue()
{
    setValue(namePath, comboBox->currentText());
}

void ComboBoxSetting::load()
{
    comboBox->setCurrentText(getValue().toString());
}

TimeUnitButtonSetting::TimeUnitButtonSetting(QSettings *settings, const QString &namePath, TimeUnitButton *button, TimeUnitButton::Unit defaultValue)
    : Setting(settings, namePath, defaultValue)
    , button(button)
{
    connect(button, SIGNAL(unitChanged(Unit)), this, SLOT(valueChanged()));
    button->setCurrentUnit((TimeUnitButton::Unit)getValue().toInt());
}

void TimeUnitButtonSetting::updateValue()
{
    setValue(namePath, button->currentUnit());
}

void TimeUnitButtonSetting::load()
{
    button->setCurrentUnit((TimeUnitButton::Unit)getValue().toInt());
}



} // namespace SonyAlphaRemote
