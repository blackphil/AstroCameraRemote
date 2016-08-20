#ifndef SONYALPHAREMOTE_SEQUENCER_SETTINGSMANAGER_H
#define SONYALPHAREMOTE_SEQUENCER_SETTINGSMANAGER_H


#include <QObject>
#include <QSettings>


#include "SonyAlphaRemote_Sequencer_Settings.h"

namespace SonyAlphaRemote {
namespace Sequencer {

class SettingsManager : public QObject
{
    Q_OBJECT

    mutable QSettings qSettings;


    SettingsPtr current;

public:
    explicit SettingsManager(QObject *parent = 0);

    QStringList getSettingsNames() const;
    bool get(SettingsPtr settings) const;

    void handleCurrentChanged();

Q_SIGNALS :
    void currentChanged(SonyAlphaRemote::Sequencer::SettingsPtr s);

public Q_SLOTS :
    void set(SonyAlphaRemote::Sequencer::SettingsPtr settings);
    bool rename(const QString& oldName, const QString& newName);
    bool renameCurrent(const QString& newName);
    bool remove(const QString& name);
    bool removeCurrent();
    void setCurrent(const QString& name);

};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SEQUENCER_SETTINGSMANAGER_H
