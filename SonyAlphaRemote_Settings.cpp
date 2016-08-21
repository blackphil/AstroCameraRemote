#include "SonyAlphaRemote_Settings.h"

namespace SonyAlphaRemote {


Setting::Setting(Setting* parent)
    : QObject(parent)
    , qSettings(NULL == parent ? NULL : parent->qSettings)
{
}


Settings::Settings(QObject *parent)
{
    QObject::setParent(parent);
    Setting::qSettings = &settings;
}

void Settings::add(Setting *s)
{
    if(childSettings.contains(s))
        return;
    childSettings << s;
}


void Settings::save()
{
    foreach(Setting* s, childSettings)
    {
        s->save();
    }
}

void Settings::load()
{
    foreach(Setting* s, childSettings)
    {
        s->load();
    }
}


} // namespace SonyAlphaRemote
