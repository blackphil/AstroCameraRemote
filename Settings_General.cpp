#include "Settings_General.h"


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
    lenrEnabled = value;
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
}

} // namespace Settings
