#ifndef SONYALPHAREMOTE_SHOOTSEQUENCE_H
#define SONYALPHAREMOTE_SHOOTSEQUENCE_H

#include <QString>
#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include <QSharedPointer>

#include "SonyAlphaRemote_Json_Command.h"

#if 0
namespace SonyAlphaRemote {

class Sender;

namespace Sequencer {

class Settings : public QObject
{
    Q_OBJECT

    Json::SetShutterSpeed shutterSpeed;
    Json::SetIsoSpeedRate isoSpeedRate;

    int bulbShutterSpeed;
    int pauseDelay;
    int startDelay;
    int numShots;

    int applyStatus;
    QMutex confirmMutex;
    QWaitCondition confirmCondition;


Q_SIGNALS:
    void error(QString);

private Q_SLOTS :
    void confirmedShutterSpeed();
    void confirmedIsoSpeedRate();

public:
    Settings(const QString& name = "", QObject* parent = 0);
    ~Settings();

    bool apply(Sender* sender);

    QString getShutterSpeed() const;
    void setShutterSpeed(const QString &value);
    int getBulbShutterSpeed() const;
    void setBulbShutterSpeed(int value);
    QString getIsoSpeedRate() const;
    void setIsoSpeedRate(const QString &value);
    int getPauseDelay() const;
    void setPauseDelay(int value);
    int getStartDelay() const;
    void setStartDelay(int value);
    int getNumShots() const;
    void setNumShots(int value);
};

typedef QSharedPointer<Settings> SettingsPtr;

} //namespace Sequencer
} // namespace SonyAlphaRemote

#endif

#endif // SONYALPHAREMOTE_SHOOTSEQUENCE_H
