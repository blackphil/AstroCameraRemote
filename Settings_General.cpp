#include "Settings_General.h"
#include "SonyAlphaRemote_Settings.h"

namespace Settings {

QString General::groupName("General");
bool General::lenrEnabledDefault(true);
QString General::lenrEnabledName("lenrEnabled");

bool General::getLenrEnabled() const
{
    return lenrEnabled;
}

void General::setLenrEnabled(bool value)
{
    if(lenrEnabled != value)
    {
        lenrEnabled = value;
        Q_EMIT settingChanged();
    }
}

General *General::getInstance()
{
    foreach(QObject* child, SonyAlphaRemote::Settings::getInstance()->children())
    {
        General* me = qobject_cast<General*>(child);
        if(me)
            return me;
    }
    return NULL;
}

General::General(Setting *parent)
    : Setting(parent)
    , lenrEnabled(lenrEnabledDefault)
{

}

void General::save()
{
    qSettings->beginGroup(groupName);
    qSettings->setValue(lenrEnabledName, lenrEnabled);
    qSettings->endGroup();
}

void General::load()
{
    qSettings->beginGroup(groupName);
    lenrEnabled = qSettings->value(lenrEnabledName, lenrEnabledDefault).toBool();
    qSettings->endGroup();

    Q_EMIT settingChanged();
}

} // namespace Settings
