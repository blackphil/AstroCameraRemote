#ifndef SONYALPHAREMOTE_SETTINGS_H
#define SONYALPHAREMOTE_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QList>
#include <QSpinBox>
#include <QComboBox>
#include "SonyAlphaRemote_TimeUnitButton.h"

namespace SonyAlphaRemote {

class Setting : public QObject
{
    Q_OBJECT

protected :
    QSettings* settings;
    QString namePath;
    QString activeSequencerSettings;
    QVariant defaultValue;

    Setting(QSettings* settings, const QString& namePath, const QVariant& defaultValue);
    virtual void updateValue() = 0;

    void setValue(QString path, const QVariant& value);

public :
    virtual ~Setting() {}
    QVariant getValue() const;

public Q_SLOTS :
    void valueChanged();
    void activeSequencerSettingsChanged(const QString& name);
    void save();
    virtual void load() = 0;
};


class SpinBoxSetting : public Setting
{
    QSpinBox* spinBox;
public :
    SpinBoxSetting(QSettings* settings, const QString& namePath, QSpinBox* spinBox, int defaultValue);
private :
    void updateValue();
    void load();
};

class DoubleSpinBoxSetting : public Setting
{
    QDoubleSpinBox* spinBox;
public :
    DoubleSpinBoxSetting(QSettings* settings, const QString& namePath, QDoubleSpinBox* spinBox, double defaultValue);
private :
    void updateValue();
    void load();
};


class ComboBoxSetting : public Setting
{
    QComboBox* comboBox;
public :
    ComboBoxSetting(QSettings* settings, const QString& namePath, QComboBox* comboBox, const QString& defaultValue);
private :
    void updateValue();
    void load();
};

class TimeUnitButtonSetting : public Setting
{
    TimeUnitButton* button;
public :
    TimeUnitButtonSetting(QSettings* settings, const QString& namePath, TimeUnitButton* button, TimeUnitButton::Unit defaultValue);
private :
    void updateValue();
    void load();
};


class Settings : public QObject
{
    Q_OBJECT

    QSettings settings;
    QList<Setting*> registeredSettings;

Q_SIGNALS:
    void activeSequencerSettingsChanged(QString);

public:
    void registerSpinBox(QSpinBox* spinBox, const QString& namePath, int defaultValue);
    void registerDoubleSpinBox(QDoubleSpinBox* spinBox, const QString& namePath, double defaultValue);
    void registerComboBox(QComboBox* comboBox, const QString& namePath, const QString &defaultValue);
    void registerTimeUnitButton(TimeUnitButton* button, const QString& namePath, TimeUnitButton::Unit defaultValue);

    void save();
    void load();
};



} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SETTINGS_H
