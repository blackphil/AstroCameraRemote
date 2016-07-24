#ifndef SONYALPHAREMOTE_SETTINGS_H
#define SONYALPHAREMOTE_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QList>
#include <QSpinBox>
#include <QComboBox>

namespace SonyAlphaRemote {

class Setting : public QObject
{
    Q_OBJECT

protected :
    QSettings* settings;
    QString namePath;

    Setting(QSettings* settings, const QString& namePath);
    virtual void updateValue() = 0;

public :
    virtual ~Setting() {}
    QVariant getValue(const QVariant& defaultValue = QVariant()) const;

public Q_SLOTS :
    void valueChanged();
};


class SpinBoxSetting : public Setting
{
    QSpinBox* spinBox;
public :
    SpinBoxSetting(QSettings* settings, const QString& namePath, QSpinBox* spinBox, int defaultValue);
private :
    void updateValue();
};

class ComboBoxSetting : public Setting
{
    QComboBox* comboBox;
public :
    ComboBoxSetting(QSettings* settings, const QString& namePath, QComboBox* comboBox, const QString& defaultValue);
private :
    void updateValue();
};


class Settings
{
    QSettings settings;
    QList<Setting*> registeredSettings;
public:
    void registerSpinBox(QSpinBox* spinBox, const QString& namePath, int defaultValue);
    void registerComboBox(QComboBox* comboBox, const QString& namePath, const QString &defaultValue);
};



} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SETTINGS_H
