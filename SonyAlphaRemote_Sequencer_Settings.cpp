#include "SonyAlphaRemote_Sequencer_Settings.h"

#include "SonyAlphaRemote_Sender.h"
#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_Helper.h"

namespace SonyAlphaRemote {

namespace Sequencer {

enum ApplyStatusFlags
{
      Apply_ShutterSpeed = 1
    , Apply_IsoSpeedRate = 2
};

Settings::Settings(const QString& name, QObject* parent)
    : QObject(parent)
{
    SAR_INF("Sequencer::Settings ctor");

    setObjectName(name);

    connect(&shutterSpeed, SIGNAL(confirmed()), this, SLOT(confirmedShutterSpeed()));
    connect(&isoSpeedRate, SIGNAL(confirmed()), this, SLOT(confirmedIsoSpeedRate()));
}

Settings::~Settings()
{
    SAR_INF("Sequencer::Settings dtor");
}

bool Settings::apply(Sender *sender)
{
    applyStatus = ~0;

    sender->send(&shutterSpeed);
    sender->send(&isoSpeedRate);

    QMutexLocker l(&confirmMutex);
    while(0 != applyStatus)
    {
        if(!confirmCondition.wait(&confirmMutex, 10 * 1000))
        {
            Q_EMIT error(tr("apply shoot sequence settings: waiting for confirmation timet out"));
            return false;
        }
    }

    return true;
}




void Settings::confirmedShutterSpeed() { applyStatus &= ~Apply_ShutterSpeed; confirmCondition.wakeAll(); }
void Settings::confirmedIsoSpeedRate() { applyStatus &= ~Apply_IsoSpeedRate; confirmCondition.wakeAll(); }





QString Settings::getShutterSpeed() const
{
    return shutterSpeed.getShutterSpeed();
}

void Settings::setShutterSpeed(const QString &value)
{
    shutterSpeed.setShutterSpeed(value);
}

int Settings::getBulbShutterSpeed() const
{
    return bulbShutterSpeed;
}

void Settings::setBulbShutterSpeed(int value)
{
    bulbShutterSpeed = value;
}

QString Settings::getIsoSpeedRate() const
{
    return isoSpeedRate.getIsoSpeedRate();
}

void Settings::setIsoSpeedRate(const QString &value)
{
    isoSpeedRate.setIsoSpeedRate(value);
}

int Settings::getPauseDelay() const
{
    return pauseDelay;
}

void Settings::setPauseDelay(int value)
{
    pauseDelay = value;
}

int Settings::getStartDelay() const
{
    return startDelay;
}

void Settings::setStartDelay(int value)
{
    startDelay = value;
}

int Settings::getNumShots() const
{
    return numShots;
}

void Settings::setNumShots(int value)
{
    numShots = value;
}

} //namespace Sequencer
} // namespace SonyAlphaRemote
