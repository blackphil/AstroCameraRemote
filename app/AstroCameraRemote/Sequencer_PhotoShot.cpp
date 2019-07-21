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

PhotoShot::PhotoShot()
    : type(Type::Undefined)
    , index(-1)
{
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

void PhotoShot::deSerializeXml(QXmlStreamReader &reader)
{
    if(!reader.isStartElement() || reader.name() != "PhotoShot")
        return;

    while(!reader.atEnd())
    {
        if(reader.isStartElement())
        {
            if(reader.name() == "PhotoShot")
            {
                type = typeFromString(reader.attributes().value("type").toString());
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

void PhotoShot::deSerializeExif(QXmlStreamReader &reader)
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


} // namespace Sequencer
