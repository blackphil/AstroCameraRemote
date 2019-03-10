#include "Sequencer_Settings.h"

#include "Sender.h"
#include "Json_Command.h"
#include "AstroBase.h"


namespace Sequencer {


QString Settings::getShutterSpeed() const
{
    return shutterSpeed;
}

void Settings::setShutterSpeed(const QString &value)
{
    shutterSpeed = value;
}

QString Settings::getIso() const
{
    return iso;
}

void Settings::setIso(const QString &value)
{
    iso = value;
}

int Settings::getShutterSpeedBulb() const
{
    return shutterSpeedBulb;
}

void Settings::setShutterSpeedBulb(int value)
{
    shutterSpeedBulb = value;
}

int Settings::getStartDelay() const
{
    return startDelay;
}

void Settings::setStartDelay(int value)
{
    startDelay = value;
}

int Settings::getPause() const
{
    return pause;
}

void Settings::setPause(int value)
{
    pause = value;
}

int Settings::getNumShots() const
{
    return numShots;
}

void Settings::setNumShots(int value)
{
    numShots = value;
}

int Settings::getShutterspeedBulbUnit() const
{
    return shutterspeedBulbUnit;
}

void Settings::setShutterspeedBulbUnit(int value)
{
    shutterspeedBulbUnit = value;
}

int Settings::getStartDelayUnit() const
{
    return startDelayUnit;
}

void Settings::setStartDelayUnit(int value)
{
    startDelayUnit = value;
}

int Settings::getPauseUnit() const
{
    return pauseUnit;
}

void Settings::setPauseUnit(int value)
{
    pauseUnit = value;
}

Settings::Settings(Setting *parent)
    : Setting(parent)
    , shutterSpeed("BULB")
    , iso("800")
    , shutterSpeedBulb(1000)
    , shutterspeedBulbUnit(2)
    , startDelay(0)
    , startDelayUnit(2)
    , pause(1000)
    , pauseUnit(2)
    , numShots(1)
{
    AB_INF("Sequencer::Settings ctor");
}

Settings::~Settings()
{
    AB_INF("Sequencer::Settings dtor");
}

void Settings::save()
{
    qSettings->beginGroup(objectName());
    qSettings->setValue("shutterSpeed", shutterSpeed);
    qSettings->setValue("iso", iso);
    qSettings->setValue("shutterSpeedBulb", shutterSpeedBulb);
    qSettings->setValue("shutterSpeedBulbUnit", shutterspeedBulbUnit);
    qSettings->setValue("startDelay", startDelay);
    qSettings->setValue("startDelayUnit", startDelayUnit);
    qSettings->setValue("pause", pause);
    qSettings->setValue("pauseUnit", pauseUnit);
    qSettings->setValue("numShots", numShots);
    qSettings->endGroup();
}

void Settings::load()
{
    qSettings->beginGroup(objectName());
    shutterSpeed = qSettings->value("shutterSpeed", "BULB").toString();
    iso = qSettings->value("iso", "800").toString();
    shutterSpeedBulb = qSettings->value("shutterSpeedBulb", 1000).toInt();
    shutterspeedBulbUnit = qSettings->value("shutterSpeedBulbUnit", 2).toInt();
    startDelay = qSettings->value("startDelay", 0).toInt();
    startDelayUnit = qSettings->value("startDelayUnit", 2).toInt();
    pause = qSettings->value("pause", 1000).toInt();
    pauseUnit = qSettings->value("pauseUnit", 2).toInt();
    numShots = qSettings->value("numShots", 1).toInt();
    qSettings->endGroup();
}

} //namespace Sequencer
