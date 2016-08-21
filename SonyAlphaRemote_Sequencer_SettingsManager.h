#ifndef SONYALPHAREMOTE_SEQUENCER_SETTINGSMANAGER_H
#define SONYALPHAREMOTE_SEQUENCER_SETTINGSMANAGER_H


#include <QObject>
#include <QHash>

#include "SonyAlphaRemote_Settings.h"
#include "SonyAlphaRemote_Sequencer_Settings.h"

namespace SonyAlphaRemote {
namespace Sequencer {

class SettingsManager : public Setting
{
    Q_OBJECT

    Sequencer::Settings* current;
    QHash<QString, Sequencer::Settings*> settings;

public:
    explicit SettingsManager(Setting *parent);

    QStringList getSettingsNames() const;
    Sequencer::Settings* getCurrent() const { return current; }

Q_SIGNALS :
    void currentChanged(QString, QStringList);
    void removed(QString);

public Q_SLOTS :
    void load();
    void save();

    void add(Sequencer::Settings* s);
    bool remove(QString name);
    bool removeCurrent();
    void setCurrent(const QString& name);

    void setShutterSpeed(const QString &value);
    void setIso(const QString &value);

    void setShutterSpeedBulb(int value);
    void setShutterSpeedBulbUnit(int value);

    void setStartDelay(int value);
    void setStartDelayUnit(int value);

    void setPause(int value);
    void setPauseUnit(int value);

    void setNumShots(int value);

};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SEQUENCER_SETTINGSMANAGER_H
