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
{
    ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);
    currentName = qSettings.value("currentSettings", "").toString();
}

QStringList SettingsManager::getSettingsNames() const
{
    ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);
    return qSettings.childGroups();
}


bool SettingsManager::rename(const QString &oldName, const QString &newName)
{
//    if(!remove(oldName))
//        return false;
//    if(!add(newName))
//        return false;

//    if(currentName == oldName)
//        setCurrent(newName);

    return true;
}

bool SettingsManager::renameCurrent(const QString &newName)
{
//    if(currentName.isEmpty() || currentName == newName)
//        return false;
//    return rename(currentName, newName);
    return true;
}

void SettingsManager::setCurrent(const QString &name)
{
    currentName = name;
    handleCurrentChanged();
}

bool SettingsManager::remove(QString name)
{
    {
        ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);

        if(!qSettings.childGroups().contains(name))
            return false;

        qSettings.remove(name);

    }

    if(name == currentName)
    {
        QStringList list = getSettingsNames();
        if(list.isEmpty())
            currentName = "";
        else
            currentName = list.first();
        handleCurrentChanged();
    }


    Q_EMIT removed(name);
    return true;
}

bool SettingsManager::removeCurrent()
{
    return remove(currentName);
}


void SettingsManager::handleCurrentChanged()
{
    ScopedQSettingsGroup seqGrp("Sequencer", qSettings); Q_UNUSED(seqGrp);
    qSettings.setValue("currentSettings", currentName);
    Q_EMIT currentChanged(currentName);
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
