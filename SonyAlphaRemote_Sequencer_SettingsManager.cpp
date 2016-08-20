#include "SonyAlphaRemote_Sequencer_SettingsManager.h"

namespace SonyAlphaRemote {
namespace Sequencer {

class ScopedQSettingsGroup
{
    QSettings& s;

public :

    ScopedQSettingsGroup(const QString& name, QSettings& s)
        : s(s)
    {
        s.beginGroup(name);
    }


    ~ScopedQSettingsGroup() { s.endGroup(); }
};

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
    , current(new Settings())
{
    ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);
    QString activeSettingsName = qSettings.value("currentSettings", "").toString();
    current->setObjectName(activeSettingsName);
    get(current);
}

QStringList SettingsManager::getSettingsNames() const
{
    ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);
    return qSettings.childGroups();
}

void SettingsManager::set(SettingsPtr settings)
{
    ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);
    ScopedQSettingsGroup setGrp(settings->objectName(), qSettings); Q_UNUSED(setGrp);


    qSettings.setValue("ShutterSpeed", settings->getShutterSpeed());
    qSettings.setValue("BulbShutterSpeed", settings->getBulbShutterSpeed());
    qSettings.setValue("IsoSpeedRate", settings->getIsoSpeedRate());
    qSettings.setValue("PauseDelay", settings->getPauseDelay());
    qSettings.setValue("StartDelay", settings->getStartDelay());
    qSettings.setValue("NumShots", settings->getNumShots());

}

bool SettingsManager::rename(const QString &oldName, const QString &newName)
{

    SettingsPtr s(new Settings(oldName));
    if(!get(s))
        return false;
    if(!remove(oldName))
        return false;

    s->setObjectName(newName);
    set(s);

    return true;
}

bool SettingsManager::renameCurrent(const QString &newName)
{
    if(!current || current->objectName().isEmpty())
        return false;

    return rename(current->objectName(), newName);
}

void SettingsManager::setCurrent(const QString &name)
{
    current = SettingsPtr(new Settings(name));
    get(current);
    handleCurrentChanged();
}

bool SettingsManager::remove(const QString &name)
{
    ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);

    if(!qSettings.contains(name))
        return false;

    qSettings.remove(name);

    if(name == current->objectName())
    {
        current = SettingsPtr(new Settings());
        handleCurrentChanged();
    }

    return true;
}

bool SettingsManager::removeCurrent()
{
    return remove(current->objectName());
}

bool SettingsManager::get(SettingsPtr settings) const
{

    ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);

    QString name = settings->objectName();
    if(name.isEmpty() || !qSettings.contains(name))
        return false;

    ScopedQSettingsGroup setGrp(settings->objectName(), qSettings); Q_UNUSED(setGrp);

    settings->setShutterSpeed    (qSettings.value("ShutterSpeed"    , QVariant()).toString());
    settings->setBulbShutterSpeed(qSettings.value("BulbShutterSpeed", QVariant()).toInt());
    settings->setIsoSpeedRate    (qSettings.value("IsoSpeedRate"    , QVariant()).toString());
    settings->setPauseDelay      (qSettings.value("PauseDelay"      , QVariant()).toInt());
    settings->setStartDelay      (qSettings.value("StartDelay"      , QVariant()).toInt());
    settings->setNumShots        (qSettings.value("NumShots"        , QVariant()).toInt());

    return true;
}

void SettingsManager::handleCurrentChanged()
{
    if(current)
    {
        ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);
        qSettings.setValue("currentSettings", current->objectName());
    }
    Q_EMIT currentChanged(current);
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
