#ifndef SEQUENCER_PHOTOSHOT_H
#define SEQUENCER_PHOTOSHOT_H

#include "EasyExif_Exif.h"

#include <QDateTime>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace Sequencer {

struct PhotoShot
{
    enum class Type
    {
        Undefined
        , Focusing
        , Light
        , Dark
        , Flat
        , DarkFlat
        , numTypes
    } type;

    constexpr static Type AllTypes[] { Type::Focusing, Type::Light, Type::Dark, Type::Flat, Type::DarkFlat };

    static QString typeToString(Type t);
    static Type typeFromString(const QString& t);

    PhotoShot();
    PhotoShot(int index, QString fileName, Type t);
    int index;
    QDateTime timeStamp;
    QString fileName;

    EasyExif::EXIFInfo exif;

    void serializeXml(QXmlStreamWriter& writer) const;
    void deSerializeXml(QXmlStreamReader& reader);

    void serializeExif(QXmlStreamWriter& writer) const;
    void deSerializeExif(QXmlStreamReader& reader);
};


} // namespace Sequencer

#endif // SEQUENCER_PHOTOSHOT_H
