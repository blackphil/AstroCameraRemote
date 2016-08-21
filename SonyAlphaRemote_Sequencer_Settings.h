#ifndef SONYALPHAREMOTE_SHOOTSEQUENCE_H
#define SONYALPHAREMOTE_SHOOTSEQUENCE_H

#include <QString>
#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include <QSharedPointer>

#include "SonyAlphaRemote_Settings.h"


namespace SonyAlphaRemote {

class Sender;

namespace Sequencer {

class Settings : public Setting
{
    Q_OBJECT

    QString shutterSpeed;
    QString iso;
    int shutterSpeedBulb;
    int shutterspeedBulbUnit;
    int startDelay;
    int startDelayUnit;
    int pause;
    int pauseUnit;
    int numShots;

public:
    Settings(Setting* parent);
    ~Settings();

    void addChildSetting(Setting* child);
    void load();
    void save();

    QString getShutterSpeed() const;
    void setShutterSpeed(const QString &value);
    QString getIso() const;
    void setIso(const QString &value);
    int getShutterSpeedBulb() const;
    void setShutterSpeedBulb(int value);
    int getStartDelay() const;
    void setStartDelay(int value);
    int getPause() const;
    void setPause(int value);
    int getNumShots() const;
    void setNumShots(int value);
    int getShutterspeedBulbUnit() const;
    void setShutterspeedBulbUnit(int value);
    int getStartDelayUnit() const;
    void setStartDelayUnit(int value);
    int getPauseUnit() const;
    void setPauseUnit(int value);
};

} //namespace Sequencer
} // namespace SonyAlphaRemote


#endif // SONYALPHAREMOTE_SHOOTSEQUENCE_H
