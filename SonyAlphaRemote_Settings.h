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
    Setting(Setting *parent = 0);

public :
    virtual ~Setting() {}

public Q_SLOTS :
    virtual void save() = 0;
    virtual void load() = 0;
};



class Settings : public Setting
{
    Q_OBJECT

    QSettings settings;

    QList<Setting*> childSettings;

public :
    Settings(QObject* parent = 0);
    QSettings* getQSettings() { return &settings; }

    void add(Setting* s);

public:
    void save();
    void load();
};



} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SETTINGS_H
