#ifndef SONYALPHAREMOTE_SETTINGS_H
#define SONYALPHAREMOTE_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QList>
#include <QSpinBox>
#include <QComboBox>
#include "SonyAlphaRemote_TimeUnitButton.h"

namespace SonyAlphaRemote {

class Settings;
class Setting : public QObject
{
    Q_OBJECT

protected :
    QSettings* qSettings;
    Setting(Setting *parent);

Q_SIGNALS:
    void settingChanged();

public :
    virtual ~Setting();

public Q_SLOTS :
    virtual void save() = 0;
    virtual void load() = 0;
};



class Settings : public Setting
{
    Q_OBJECT

    static QPointer<Settings> instance;

    QSettings settings;

    QList<Setting*> childSettings;

public :
    Settings(QObject* parent);
    QSettings* getQSettings() { return &settings; }

    void add(Setting* s);

    Setting* getSettingByName(const QString& name);

    static Settings* getInstance();

public:
    void save();
    void load();
};



} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SETTINGS_H
