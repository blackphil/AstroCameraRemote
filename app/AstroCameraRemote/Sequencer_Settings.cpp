#include "Sequencer_Settings.h"

#include "Sender.h"
#include "Json_Command.h"
#include "AstroBase.h"

#include <QRegularExpression>

namespace Sequencer {

#if 0
QString Settings::getShutterSpeed() const
{
    return properties.shutterSpeed;
}

void Settings::setShutterSpeed(const QString &value)
{
    properties.shutterSpeed = value;
}

QString Settings::getIso() const
{
    return properties.iso;
}

void Settings::setIso(const QString &value)
{
    properties.iso = value;
}

int Settings::getShutterSpeedBulb() const
{
    return properties.shutterSpeedBulb;
}

void Settings::setShutterSpeedBulb(int value)
{
    properties.shutterSpeedBulb = value;
}

int Settings::getStartDelay() const
{
    return properties.startDelay;
}

void Settings::setStartDelay(int value)
{
    properties.startDelay = value;
}

int Settings::getPause() const
{
    return properties.pause;
}

void Settings::setPause(int value)
{
    properties.pause = value;
}

int Settings::getNumShots() const
{
    return properties.numShots;
}

void Settings::setNumShots(int value)
{
    properties.numShots = value;
}

int Settings::getShutterspeedBulbUnit() const
{
    return properties.shutterSpeedBulbUnit;
}

void Settings::setShutterspeedBulbUnit(int value)
{
    properties.shutterSpeedBulbUnit = value;
}

int Settings::getStartDelayUnit() const
{
    return properties.startDelayUnit;
}

void Settings::setStartDelayUnit(int value)
{
    properties.startDelayUnit = value;
}

int Settings::getPauseUnit() const
{
    return properties.pauseUnit;
}

void Settings::setPauseUnit(int value)
{
    properties.pauseUnit = value;
}

#endif

Properties::Properties()
    : shutterSpeed("BULB")
    , iso("800")
    , shutterSpeedBulb(1000)
    , shutterSpeedBulbUnit(2)
    , startDelay(0)
    , startDelayUnit(2)
    , pause(1000)
    , pauseUnit(2)
    , numShots(1)
{}

void Properties::serializeXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement("Properties");
    writer.writeAttribute("shutterSpeed", shutterSpeed);
    writer.writeAttribute("iso", iso);
    writer.writeAttribute("shutterSpeedBulb", QString::number(shutterSpeedBulb));
    writer.writeAttribute("shutterSpeedBulbUnit", QString::number(shutterSpeedBulbUnit));
    writer.writeAttribute("startDelay", QString::number(startDelay));
    writer.writeAttribute("startDelayUnit", QString::number(startDelayUnit));
    writer.writeAttribute("pause", QString::number(pause));
    writer.writeAttribute("pauseUnit", QString::number(pauseUnit));
    writer.writeAttribute("numShots", QString::number(numShots));
    writer.writeEndElement();
}

void Properties::deSerializeXml(QXmlStreamReader &reader)
{
    if(reader.isStartElement() && reader.name() == "Properties")
    {
        shutterSpeed = reader.attributes().value("shutterSpeed").toString();
        iso = reader.attributes().value("iso").toString();
        shutterSpeedBulbUnit = reader.attributes().value("shutterSpeedBulbUnit").toInt();
        startDelay = reader.attributes().value("startDelay").toInt();
        startDelayUnit = reader.attributes().value("startDelayUnit").toInt();
        pause = reader.attributes().value("pause").toInt();
        pauseUnit = reader.attributes().value("pauseUnit").toInt();
        numShots = reader.attributes().value("numShots").toInt();
    }
}

int Properties::getShutterSpeedInMilliseconds() const
{
    if("BULB" == shutterSpeed)
    {
        return shutterSpeedBulb;
    }
    else
    {
        QRegularExpressionMatch match1 = QRegularExpression("1/(\\d+)").match(shutterSpeed);
        QRegularExpressionMatch match2 = QRegularExpression("(\\d+)\"").match(shutterSpeed);
        int milliSeconds = 0;
        if(match1.hasMatch())
            milliSeconds = static_cast<int>(1000. / match1.captured(1).toDouble());
        else if(match2.hasMatch())
            milliSeconds = static_cast<int>(1000. * match2.captured(1).toDouble());

        return milliSeconds;

    }
}

bool Properties::isBulb() const
{
    return shutterSpeed == "BULB" ? true : false;
}

#if 0
Properties Settings::getProperties() const
{
    return properties;
}

void Settings::setProperties(const Properties &value)
{
    properties = value;
}

Settings::Settings(Setting *parent)
    : Setting(parent)
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
    qSettings->setValue("shutterSpeed", properties.shutterSpeed);
    qSettings->setValue("iso", properties.iso);
    qSettings->setValue("shutterSpeedBulb", properties.shutterSpeedBulb);
    qSettings->setValue("shutterSpeedBulbUnit", properties.shutterSpeedBulbUnit);
    qSettings->setValue("startDelay", properties.startDelay);
    qSettings->setValue("startDelayUnit", properties.startDelayUnit);
    qSettings->setValue("pause", properties.pause);
    qSettings->setValue("pauseUnit", properties.pauseUnit);
    qSettings->setValue("numShots", properties.numShots);
    qSettings->endGroup();
}

void Settings::load()
{
    qSettings->beginGroup(objectName());
    properties.shutterSpeed = qSettings->value("shutterSpeed", "BULB").toString();
    properties.iso = qSettings->value("iso", "800").toString();
    properties.shutterSpeedBulb = qSettings->value("shutterSpeedBulb", 1000).toInt();
    properties.shutterSpeedBulbUnit = qSettings->value("shutterSpeedBulbUnit", 2).toInt();
    properties.startDelay = qSettings->value("startDelay", 0).toInt();
    properties.startDelayUnit = qSettings->value("startDelayUnit", 2).toInt();
    properties.pause = qSettings->value("pause", 1000).toInt();
    properties.pauseUnit = qSettings->value("pauseUnit", 2).toInt();
    properties.numShots = qSettings->value("numShots", 1).toInt();
    qSettings->endGroup();
}
#endif
} //namespace Sequencer
