#include "Sequencer_SettingsManager.h"


#include "AstroBase.h"

namespace Sequencer {

#if 0

SettingsManager::SettingsManager(Setting* parent)
    : Setting(parent)
    , current(nullptr)
{
    setObjectName(getName());
}

void SettingsManager::load()
{
    qDeleteAll(settings);
    settings.clear();

    qSettings->beginGroup(objectName());



    QString currentName = qSettings->value("currentSettings", "default").toString();
    QStringList availableSettings = qSettings->childGroups();

    foreach(QString name, availableSettings)
    {
        AB_INF("loading sequencer setting: " << name);
        Sequencer::Settings* s = new Sequencer::Settings(this);
        s->setObjectName(name);
        s->load();
        settings.insert(name, s);
        if(s->objectName() == currentName)
        {
            current = s;
        }
    }

    qSettings->endGroup();


    if(settings.isEmpty())
    {
        Sequencer::Settings* defaultSettings(new Sequencer::Settings(this));
        defaultSettings->setObjectName("default");
        settings[defaultSettings->objectName()] = defaultSettings;
        current = defaultSettings;
    }

    if(!current)
        current = settings.begin().value();

    Q_EMIT currentChanged(current->objectName(), settings.keys());
}

void SettingsManager::save()
{
    qSettings->beginGroup(objectName());

    if(current)
        qSettings->setValue("currentSettings", current->objectName());

    for(QHash<QString, Sequencer::Settings*>::ConstIterator it=settings.constBegin(); it!=settings.constEnd(); ++it)
    {
        (*it)->save();
    }

    qSettings->endGroup();

}

void SettingsManager::add(Settings *s)
{
    Q_ASSERT(s != current);
    if(s == current)
        return;

    if(settings.contains(s->objectName()))
    {
        settings[s->objectName()]->deleteLater();
    }
    settings[s->objectName()] = s;
}

QStringList SettingsManager::getSettingsNames() const
{
    return settings.keys();
}

QString SettingsManager::getName()
{
    return "Sequence";
}

void SettingsManager::setCurrent(const QString &name)
{
    if(!settings.contains(name))
        return;

    if(current == settings[name])
        return;

    current = settings[name];

    Q_EMIT currentChanged(name, settings.keys());
}

bool SettingsManager::remove(QString name)
{
    if(!settings.contains(name))
        return false;

    Sequencer::Settings* toRemove = settings[name];
    toRemove->deleteLater();
    settings.remove(name);

    if(toRemove == current)
    {
        setCurrent(settings.begin().key());
    }

    Q_EMIT removed(name);
    return true;
}

bool SettingsManager::removeCurrent()
{
    if(!current)
        return false;
    return remove(current->objectName());
}

void SettingsManager::setShutterSpeed(const QString &value)
{
    current->setShutterSpeed(value);
}

void SettingsManager::setIso(const QString &value)
{
    current->setIso(value);
}

void SettingsManager::setShutterSpeedBulb(int value)
{
    current->setShutterSpeedBulb(value);
}

void SettingsManager::setShutterSpeedBulbUnit(int value)
{
    current->setShutterspeedBulbUnit(value);
}

void SettingsManager::setStartDelay(int value)
{
    current->setStartDelay(value);
}

void SettingsManager::setStartDelayUnit(int value)
{
    current->setStartDelayUnit(value);
}

void SettingsManager::setPause(int value)
{
    current->setPause(value);
}

void SettingsManager::setPauseUnit(int value)
{
    current->setPauseUnit(value);
}

void SettingsManager::setNumShots(int value)
{
    current->setNumShots(value);
}

#endif
} // namespace Sequencer
