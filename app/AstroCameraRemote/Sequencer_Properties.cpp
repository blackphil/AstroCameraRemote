#include "Sequencer_Properties.h"

#include "AstroBase.h"

#include <QRegularExpression>

namespace Sequencer {


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

void Properties::deSerializeXml(QDomElement xml)
{

    shutterSpeed         = xml.attribute("shutterSpeed");
    iso                  = xml.attribute("iso");
    shutterSpeedBulbUnit = xml.attribute("shutterSpeedBulbUnit").toInt();
    startDelay           = xml.attribute("startDelay").toInt();
    startDelayUnit       = xml.attribute("startDelayUnit").toInt();
    pause                = xml.attribute("pause").toInt();
    pauseUnit            = xml.attribute("pauseUnit").toInt();
    numShots             = xml.attribute("numShots").toInt();

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


} //namespace Sequencer
