#include "SonyAlphaRemote_Settings.h"
#include <QPointer>

namespace SonyAlphaRemote {


Setting::Setting(Setting* parent)
    : QObject(parent)
    , qSettings(NULL == parent ? NULL : parent->qSettings)
{
}

QPointer<Settings> Settings::instance(0);

Settings::Settings(QObject *parent)
{
    Q_ASSERT(parent);
    QObject::setParent(parent);
    Setting::qSettings = &settings;
    instance = this;
}

Settings* Settings::getInstance()
{
    return instance;
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
