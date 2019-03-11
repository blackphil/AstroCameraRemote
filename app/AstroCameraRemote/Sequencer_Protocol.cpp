#include "Sequencer_Protocol.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

namespace Sequencer {

Properties Protocol::getProperties() const
{
    return properties;
}

void Protocol::setProperties(const Properties &value)
{
    properties = value;
}

Protocol::Protocol(QObject *parent) : QObject(parent)
{

}

void Protocol::start()
{
    startTime = QDateTime::currentDateTime();
}

void Protocol::shotFinished(QString url, int index, int numShots)
{
    Q_UNUSED(numShots);

    PhotoShot fs(index, url);
    fs.timeStamp = QDateTime::currentDateTime();

    photoShots << fs;
}

void Protocol::sequenceFinished()
{
    QFileInfo protocolFileInfo(QDir::home(), "Protocol.xml");
    QFile f(protocolFileInfo.absoluteFilePath());
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QXmlStreamWriter writer(&f);
    writer.setAutoFormatting(true);

    serializeXml(writer);

    f.close();
}

void Protocol::serializeXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement("Protocol");
    writer.writeAttribute("startTime", startTime.toString("yyyy-MM-ddThh:mm:ss.zzz"));
    properties.serializeXml(writer);

    Q_FOREACH(const PhotoShot& fs, photoShots)
    {
        fs.serializeXml(writer);
    }
    writer.writeEndElement();
}

void Protocol::deSerializeXml(QXmlStreamReader &reader)
{
    while(!reader.atEnd() && reader.readNextStartElement())
    {
        if(reader.name() == "Protocol")
        {
            startTime = QDateTime::fromString(reader.attributes().value("startTime").toString(), "yyyy-MM-ddThh:mm:ss.zzz");
        }
        else if(reader.name() == "Properties")
        {
            properties.deSerializeXml(reader);
        }
        else if(reader.name() == "PhotoShot")
        {
            PhotoShot ps;
            ps.deSerializeXml(reader);
            photoShots << ps;
        }
    }
}

Protocol::PhotoShot::PhotoShot()
    : index(-1)
{
}

Protocol::PhotoShot::PhotoShot(int index, QString fileName)
    : index(index)
    , fileName(fileName)
{

}

void Protocol::PhotoShot::serializeXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement("PhotoShot");
    writer.writeAttribute("index", QString::number(index));
    writer.writeAttribute("fileName", fileName);
    writer.writeAttribute("timeStamp", timeStamp.toString("yyyy-MM-ddThh:mm:ss.zzz"));
    writer.writeEndElement();
}

void Protocol::PhotoShot::deSerializeXml(QXmlStreamReader &reader)
{
    if(reader.isStartElement() && reader.name() == "PhotoShot")
    {
        index = reader.attributes().value("index").toInt();
        fileName = reader.attributes().value("fileName").toString();
        timeStamp = QDateTime::fromString(reader.attributes().value("timeStamp").toString(), "yyyy-MM-ddThh:mm:ss.zzz");
    }
}



} // namespace Sequencer
