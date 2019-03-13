#include "Settings.h"

#include <QPointer>

#include "AstroBase.h"


Setting::Setting(Setting* parent)
    : QObject(parent)
    , qSettings(nullptr == parent ? nullptr : parent->qSettings)
{
}

Setting::~Setting()
{
    AB_INF("dtor: " << objectName());
}

QPointer<Settings> Settings::instance(nullptr);

Settings::Settings(QObject *parent)
    : Setting(nullptr)
{
    Q_ASSERT(nullptr == instance);

    setObjectName("SonyAlphaRemote");
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
    AB_INF("add setting " << s->objectName());

    if(childSettings.contains(s))
        return;
    childSettings << s;
}

Setting *Settings::getSettingByName(const QString &name)
{
    foreach(Setting* child, childSettings)
    {
        if(child->objectName() == name)
            return child;
    }

    return nullptr;
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
