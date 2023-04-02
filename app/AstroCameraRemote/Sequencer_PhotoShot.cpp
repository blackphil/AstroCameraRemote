#include "Sequencer_PhotoShot.h"

namespace Sequencer {

namespace helper
{
static const QMap<PhotoShot::Type, QString> typeStringMap
{
    { PhotoShot::Type::Undefined, "Undefined" }
    , { PhotoShot::Type::Focusing , "Focusing"  }
    , { PhotoShot::Type::Light    , "Light"     }
    , { PhotoShot::Type::Dark     , "Dark"      }
    , { PhotoShot::Type::Flat     , "Flat"      }
    , { PhotoShot::Type::DarkFlat , "DarkFlat"  }
};

}

PhotoShot::PhotoShot(int index, QString fileName, Type t)
    : type(t)
    , index(index)
    , fileName(fileName)
{
}

void PhotoShot::serializeXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement("PhotoShot");
    writer.writeAttribute("type", typeToString(type));
    writer.writeAttribute("index", QString::number(index));
    writer.writeAttribute("fileName", fileName);
    writer.writeAttribute("timeStamp", timeStamp.toString("yyyy-MM-ddThh:mm:ss.zzz"));

    serializeExif(writer);
    writer.writeEndElement();
}

void PhotoShot::deSerializeXml(QDomElement el)
{
    type = typeFromString(el.attribute("type"));
    index = el.attribute("index").toInt();
    fileName = el.attribute("fileName");
    timeStamp = QDateTime::fromString(el.attribute("timeStamp"), "yyyy-MM-ddThh:mm:ss.zzz");

    if(QDomNodeList exifNodes { el.elementsByTagName("Exif") }; !exifNodes.isEmpty())
    {
        deSerializeExif(exifNodes.at(0).toElement());
    }
}


QString PhotoShot::typeToString(Type t)
{
    return helper::typeStringMap[t];
}

PhotoShot::Type PhotoShot::typeFromString(const QString& t)
{
    for(QMap<PhotoShot::Type, QString>::ConstIterator it=helper::typeStringMap.begin(); it!=helper::typeStringMap.end(); ++it)
    {
        if(it.value() == t)
            return it.key();
    }

    return Type::Undefined;
}

PhotoShot::PhotoShot(Type t)
    : type(t)
    , index(0)
{

}

void PhotoShot::serializeExif(QXmlStreamWriter &writer) const
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

void PhotoShot::deSerializeExif(QDomElement el)
{

    exif.valid = true;
    exif.ExposureTime = el.attribute("exposureTime").toDouble();
    exif.ISOSpeedRatings = el.attribute("isoSpeedRatings").toUShort();

    if(QDomNodeList dtNodes { el.elementsByTagName("DateTime") }; !dtNodes.isEmpty())
    {
        QDomElement dtEl = dtNodes.at(0).toElement();
        exif.DateTime           = dtEl.attribute("changed").toStdString();
        exif.DateTimeOriginal   = dtEl.attribute("original").toStdString();
        exif.DateTimeDigitized  = dtEl.attribute("digitized").toStdString();
        exif.SubSecTimeOriginal = dtEl.attribute("subSecOriginal").toStdString();
    }

}


} // namespace Sequencer
