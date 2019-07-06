#include "Sequencer_Protocol.h"

#include "AstroBase.h"

#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QProcess>
#include <QStringList>
#include <QMap>

#include <Windows.h>

namespace Sequencer {


namespace helper
{
const QMap<Protocol::Type, QString>& typeStringMap()
{
    static QMap<Protocol::Type, QString> map;
    if(map.isEmpty())
    {
        map[Protocol::Type_Focusing] = "Focusing (no protocol)";
        map[Protocol::Type_Light   ] = "Light";
        map[Protocol::Type_Dark    ] = "Dark";
        map[Protocol::Type_Flat    ] = "Flat";
    }
    return map;
}

const QMap<Protocol::ColorChannel, QString>& colorChannelStringMap()
{
    static QMap<Protocol::ColorChannel, QString> map;
    if(map.isEmpty())
    {
        map[Protocol::Color_RGB      ] = "RGB"      ;
        map[Protocol::Color_Luminance] = "Luminance";
        map[Protocol::Color_Red      ] = "Red"      ;
        map[Protocol::Color_Green    ] = "Green"    ;
        map[Protocol::Color_Blue     ] = "Blue"     ;

    }

    return map;
}
}

QString Protocol::typeToString(Type t)
{
    if(helper::typeStringMap().contains(t))
        return helper::typeStringMap()[t];
    return "Undefined";
}

Protocol::Type typeFromString(const QString& t)
{
    for(QMap<Protocol::Type, QString>::ConstIterator it=helper::typeStringMap().begin(); it!=helper::typeStringMap().end(); ++it)
    {
        if(it.value() == t)
            return it.key();
    }

    return Protocol::NumTypes;
}


QString Protocol::colorChannelToString(ColorChannel c)
{
    if(helper::colorChannelStringMap().contains(c))
        return helper::colorChannelStringMap()[c];
    return "undefined";
}

Protocol::ColorChannel Protocol::colorChannelFromString(const QString& c)
{
    for(QMap<Protocol::ColorChannel, QString>::ConstIterator it=helper::colorChannelStringMap().begin(); it!=helper::colorChannelStringMap().end(); ++it)
    {
        if(it.value() == c)
            return it.key();
    }

    return Protocol::NumColorChannels;
}

Protocol::ColorChannel Protocol::getColorChannel() const
{
    return colorChannel;
}

void Protocol::setColorChannel(const ColorChannel &value)
{
    colorChannel = value;
}

int Protocol::compareTo(const Protocol *rhs) const
{
    if(subject > rhs->subject)
        return 1;

    if(subject != rhs->subject)
        return -1;

    if(type > rhs->type)
        return 1;

    if(type != rhs->type)
        return -1;

    if(colorChannel > rhs->colorChannel)
        return 1;

    if(colorChannel != rhs->colorChannel)
        return -1;

    return 0;
}

const QList<Protocol::PhotoShot> &Protocol::getPhotoShots() const
{
    return photoShots;
}

void Protocol::PhotoShot::serializeExif(QXmlStreamWriter &writer) const
{
    if(!exif.isValid())
        return;

    writer.writeStartElement("Exif");
    writer.writeAttribute("exposureTime", QString::number(exif.ExposureTime));
    writer.writeAttribute("isoSpeedRatings", QString::number(exif.ISOSpeedRatings));

    writer.writeStartElement("DateTime");
    writer.writeAttribute("changed", QString::fromStdString(exif.DateTime));
    writer.writeAttribute("original", QString::fromStdString(exif.DateTimeOriginal));
    writer.writeAttribute("digitized", QString::fromStdString(exif.DateTimeDigitized));
    writer.writeAttribute("subSecOriginal", QString::fromStdString(exif.SubSecTimeOriginal));
    writer.writeEndElement();
    writer.writeEndElement();
}

void Protocol::PhotoShot::deSerializeExif(QXmlStreamReader &reader)
{
    if(!reader.isStartElement() || reader.name() != "Exif")
        return;

    while(!reader.atEnd())
    {
        if(reader.isStartElement())
        {
            if(reader.name() == "Exif")
            {
                exif.valid = true;
                exif.ExposureTime = reader.attributes().value("exposureTime").toDouble();
                exif.ISOSpeedRatings = reader.attributes().value("isoSpeedRatings").toUShort();
            }
            else if(reader.name() == ("DateTime"))
            {
                exif.DateTime = reader.attributes().value("changed").toString().toStdString();
                exif.DateTimeOriginal = reader.attributes().value("original").toString().toStdString();
                exif.DateTimeDigitized = reader.attributes().value("digitized").toString().toStdString();
                exif.SubSecTimeOriginal = reader.attributes().value("subSecOriginal").toString().toStdString();
            }
        }
        else if(reader.isEndElement())
        {
            if(reader.name() == "Exif")
                return;
        }

        reader.readNext();
    }

}


const Properties& Protocol::getProperties() const
{
    return properties;
}

void Protocol::setProperties(const Properties &value)
{
    properties = value;
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
    QString dataPath = getProtocolPath(true);
    QDir dataDir(dataPath);
    Q_ASSERT(dataDir.exists());


    QString fileName = QString("%0.xml").arg(subject.trimmed());
    fileName.replace(QRegularExpression("[/\\\\:\\*\\?<>\\\"]"), "_");
    QFileInfo protocolFileInfo(QDir(dataPath), fileName);

    return protocolFileInfo.absoluteFilePath();
}

static int objCount(0);


QList<QRectF> Protocol::getReferenceMarkers() const
{
    return markers;
}

Protocol::Type Protocol::getType() const
{
    return type;
}

void Protocol::setType(const Type &value)
{
    type = value;
}

Protocol::Protocol(QObject *parent)
    : QObject(parent)
    , status(Status_Stopped)
{
    objCount++;
    AB_DBG("CTOR(" << objCount << ")");
}

Protocol::~Protocol()
{
    objCount--;
    AB_DBG("DTOR(" << objCount << ")");
}

bool Protocol::isRecording() const
{
    return status != Status_Stopped ? true : false;
}

void Protocol::start()
{
    status = Status_Recording;
    if(startTime.isNull())
        startTime = QDateTime::currentDateTime();
    save();
}

void Protocol::havePostViewUrl(QString url, int index, int numShots)
{
    Q_UNUSED(numShots);

    if(Status_Stopped == status)
        return;

    PhotoShot fs(index, url);
    fs.timeStamp = QDateTime::currentDateTime();

    photoShots << fs;

}

void Protocol::havePostViewImage(const QByteArray &data)
{
    Q_ASSERT(!photoShots.isEmpty());
    if(photoShots.isEmpty())
        return;

    if(Status_Stopped == status)
        return;


    PhotoShot& current = photoShots.last();
    Q_ASSERT(!current.exif.isValid());
    if(current.exif.isValid())
    {
        AB_WRN("Cannot get exif data for two times!");
        return;
    }

    const unsigned char* rawData = reinterpret_cast<const unsigned char*>(data.data());
    unsigned int rawLength = static_cast<unsigned int>(data.length());
    current.exif.parseFrom(rawData, rawLength);

    save();


    if(Status_Stopping == status)
        status = Status_Stopped;


}

void Protocol::setReferenceMarkers(const QList<QRectF> &markers)
{
    this->markers = markers;
    save();
}

void Protocol::cleanUpMarkers()
{
    markers.clear();
    save();
}

void Protocol::stop()
{
    status = Status_Stopping;
}

void Protocol::save()
{
    if(Type_Focusing != type)
    {
        QFile f(getFilePath());
        f.open(QIODevice::WriteOnly | QIODevice::Text);
        QXmlStreamWriter writer(&f);
        writer.setAutoFormatting(true);

        serializeXml(writer);

        f.close();
    }

}

bool Protocol::deleteFile()
{
    if(Status_Stopped != status)
    {
        AB_WRN("Cannot delete protocol file while the protocol is recording");
        return false;
    }

    QFileInfo fi = getFilePath();
    if(!fi.absoluteDir().remove(fi.fileName()))
    {
        AB_DBG(QString("Removing protocol file \"%0\" failed!").arg(fi.absoluteFilePath()));
        return false;
    }

    return true;
}

void Protocol::serializeXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement("Protocol");
    writer.writeAttribute("subject", subject);
    writer.writeAttribute("type", QString::number(type));
    writer.writeAttribute("colorChannel", QString::number(colorChannel));
    writer.writeAttribute("startTime", startTime.toString("yyyy-MM-ddThh:mm:ss.zzz"));

    if(!markers.isEmpty())
    {
        for(auto m : markers)
        {
            writer.writeStartElement("Marker");
            writer.writeAttribute(
                        "rect"
                        , QString("%0;%1;%2;%3")
                        .arg(m.x())
                        .arg(m.y())
                        .arg(m.width())
                        .arg(m.height()));
            writer.writeEndElement();
        }
    }

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
                type = static_cast<Type>(reader.attributes().value("type").toInt());
                colorChannel = static_cast<ColorChannel>(reader.attributes().value("colorChannel").toInt());
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
            else if(reader.name() == "Marker")
            {
                QStringList values = reader.attributes().value("rect").toString().split(";");
                Q_ASSERT(values.count() == 4);
                if(values.count() == 4)
                {
                    QRectF m(values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble());
                    if(!m.isNull())
                        markers << m;
                }

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

    serializeExif(writer);
    writer.writeEndElement();
}

void Protocol::PhotoShot::deSerializeXml(QXmlStreamReader &reader)
{
    if(!reader.isStartElement() || reader.name() != "PhotoShot")
        return;

    while(!reader.atEnd())
    {
        if(reader.isStartElement())
        {
            if(reader.name() == "PhotoShot")
            {
                index = reader.attributes().value("index").toInt();
                fileName = reader.attributes().value("fileName").toString();
                timeStamp = QDateTime::fromString(reader.attributes().value("timeStamp").toString(), "yyyy-MM-ddThh:mm:ss.zzz");
            }
            else if(reader.name() == "Exif")
            {
                deSerializeExif(reader);
            }
        }
        else if(reader.isEndElement())
        {
            if(reader.name() == "PhotoShot")
                return;
        }

        reader.readNext();
    }
}


} // namespace Sequencer