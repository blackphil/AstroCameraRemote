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

#include <algorithm>

#include "AstroBase_Exception.h"

namespace Sequencer {



int Protocol::compareTo(const Protocol *rhs) const
{
    if(objectName > rhs->objectName)
        return 1;

    if(objectName != rhs->objectName)
        return -1;

    return 0;
}

const Protocol::PhotoShotMap &Protocol::getPhotoShots() const
{
    return photoShots;
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
    return photoShots[currentPhotoShotType].count();
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

QString Protocol::getObjectName() const
{
    return objectName;
}

void Protocol::setObjectName(const QString &value)
{
    objectName = value;
}

QString Protocol::getFilePath() const
{
    QString dataPath = getProtocolPath(true);
    QDir dataDir(dataPath);
    Q_ASSERT(dataDir.exists());


    QString fileName = QString("%0.xml").arg(objectName.trimmed());
    fileName.replace(QRegularExpression("[/\\\\:\\*\\?<>\\\"]"), "_");
    QFileInfo protocolFileInfo(QDir(dataPath), fileName);

    return protocolFileInfo.absoluteFilePath();
}

static int objCount(0);


QList<QRectF> Protocol::getReferenceMarkers() const
{
    return markers;
}

PhotoShot::Type Protocol::getCurrentPhotoShotType() const
{
    return currentPhotoShotType;
}

void Protocol::setCurrentPhotoShotType(const PhotoShot::Type &value)
{
    currentPhotoShotType = value;
}

Protocol::Protocol(const QString& name, QObject *parent)
    : QObject(parent)
    , status(Status_Stopped)
    , objectName(name)
    , currentPhotoShotType(PhotoShot::Type::Focusing)
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

    PhotoShot s { index, url, currentPhotoShotType };
    s.timeStamp = QDateTime::currentDateTime();
    photoShots[s.type] << s;

}

void Protocol::havePostViewImage(const QByteArray &data)
{
    Q_ASSERT(!photoShots[currentPhotoShotType].isEmpty());
    if(photoShots[currentPhotoShotType].isEmpty())
        return;

    if(Status_Stopped == status)
        return;


    PhotoShot& current = photoShots[currentPhotoShotType].last();
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
    QFile f(getFilePath());
    f.open(QIODevice::WriteOnly | QIODevice::Text);

    QXmlStreamWriter writer(&f);
    writer.setAutoFormatting(true);

    serializeXml(writer);

    f.close();
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
    writer.writeAttribute("object", objectName);
    writer.writeAttribute("startTime", startTime.toString("yyyy-MM-ddThh:mm:ss.zzz"));
    writer.writeAttribute("currentType", PhotoShot::typeToString(currentPhotoShotType));

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

    for(auto shots : photoShots)
    {
        for(auto s : shots)
        {
            writer.writeStartElement(PhotoShot::typeToString(s.type));
            s.serializeXml(writer);
            writer.writeEndElement();
        }
    }
    writer.writeEndElement();
}

void Protocol::deSerializeXml(const QByteArray &data)
{
    QString errMsg;
    int errLine, errCol;

    QDomDocument doc;
    if(!doc.setContent(data, &errMsg, &errLine, &errCol))
    {
        throw AstroBase::Exception(tr("Error reading protocol at line %0, column %1: %2")
                                   .arg(errLine).arg(errCol).arg(errMsg));
    }

    QDomElement rootEl = doc.childNodes().at(0).toElement();
    startTime = QDateTime::fromString(rootEl.attribute("startTime"), "yyyy-MM-ddThh:mm:ss.zzz");
    objectName = rootEl.attribute("object");
    currentPhotoShotType = PhotoShot::typeFromString(rootEl.attribute("currentType"));

    QDomNodeList markerNodes { doc.elementsByTagName("Marker") };
    for(int i=0; i<markerNodes.count(); i++)
    {
        QDomElement markerEl { markerNodes.at(i).toElement() };
        if(QStringList values { markerEl.attribute("rect").split(";") }; values.count() == 4)
        {
            if(QRectF f { values[0].toDouble(), values[1].toDouble(), values[2].toDouble(), values[3].toDouble() }; !f.isNull())
            {
                markers << std::move(f);
                continue;
            }
        }
        throw AstroBase::Exception(tr("Invalid marker data at line %0, column %1").arg(markerEl.lineNumber()).arg(markerEl.columnNumber()));
    }

    if(QDomNodeList propertyNodes { doc.elementsByTagName("Properties") }; propertyNodes.count() > 0)
    {
        properties.deSerializeXml(propertyNodes.at(0).toElement());
    }

    for(PhotoShot::Type t : PhotoShot::AllTypes)
    {
        QDomNodeList nodes = doc.elementsByTagName(PhotoShot::typeToString(t));
        for(int i=0; i<nodes.count(); i++)
        {
            QDomNodeList photoShotNodes = nodes.at(i).toElement().elementsByTagName("PhotoShot");
            for(int ii=0; ii<photoShotNodes.count(); ii++)
            {
                PhotoShot ps(t);
                ps.deSerializeXml(photoShotNodes.at(ii).toElement());
                photoShots[ps.type] << std::move(ps);
            }
        }
    }
}



} // namespace Sequencer
