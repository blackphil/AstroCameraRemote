#include "Sequencer_Protocol.h"

#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <Windows.h>

namespace Sequencer {

const Properties& Protocol::getProperties() const
{
    return properties;
}

void Protocol::setProperties(const Properties &value)
{
    properties = value;
}

bool Protocol::getStashed() const
{
    return stashed;
}

void Protocol::setStashed(bool value)
{
    stashed = value;
}

QDateTime Protocol::getStartTime() const
{
    return startTime;
}

int Protocol::getNumShotsFinished() const
{
    return photoShots.count();
}

QString Protocol::getProtocolPath(bool createIfNotExists)
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if(dataPath.isEmpty())
        dataPath = QDir::tempPath();

    dataPath += "/protocols";


    QDir dataDir(dataPath);
    if(!dataDir.exists() && createIfNotExists)
    {
        QDir().mkpath(dataPath);
    }

    return dataPath;
}

QString Protocol::getStashPath(bool createIfNotExists)
{

    QString dataPath = getProtocolPath(createIfNotExists);
    dataPath += "/stashed";

    QDir dataDir(dataPath);
    if(!dataDir.exists() && createIfNotExists)
    {
        QDir().mkpath(dataPath);
    }

    return dataPath;
}

QString Protocol::getSubject() const
{
    return subject;
}

void Protocol::setSubject(const QString &value)
{
    subject = value;
}

QString Protocol::getFilePath() const
{
    QString dataPath;
    if(stashed)
        dataPath = getStashPath(true);
    else
        dataPath = getProtocolPath(true);

    QDir dataDir(dataPath);
    Q_ASSERT(dataDir.exists());


    QString fileName = QString("%0_%1.xml").arg(subject.trimmed()).arg(startTime.toString("yyyy-MM-ddThhmmss"));
    fileName.replace(QRegularExpression("[/\\\\:\\*\\?<>\\\"]"), "_");
    QFileInfo protocolFileInfo(QDir(dataPath), QString("%0_%1.xml")
                               .arg(fileName)
                               .arg(startTime.toString("yyyy-MM-ddThhmmss")));

    return protocolFileInfo.absoluteFilePath();
}

Protocol::Protocol(QObject *parent)
    : QObject(parent)
    , stashed(false)
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

void Protocol::stop()
{

    QFile f(getFilePath());
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QXmlStreamWriter writer(&f);
    writer.setAutoFormatting(true);

    serializeXml(writer);

    f.close();
}

void Protocol::deleteFile()
{
    DeleteFileA(getFilePath().toStdString().c_str());
}

void Protocol::serializeXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement("Protocol");
    writer.writeAttribute("subject", subject);
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
    while(!reader.atEnd())
    {
        switch(reader.readNext())
        {
        case QXmlStreamReader::StartElement :
            if(reader.name() == "Protocol")
            {
                startTime = QDateTime::fromString(reader.attributes().value("startTime").toString(), "yyyy-MM-ddThh:mm:ss.zzz");
                subject = reader.attributes().value("subject").toString();
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
            break;
        default :
            break;
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
