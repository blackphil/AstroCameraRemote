#ifndef SEQUENCER_PHOTOSHOT_H
#define SEQUENCER_PHOTOSHOT_H

#include <EasyExif/Exif>

#include <QDateTime>
#include <QDomElement>
#include <QXmlStreamWriter>

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

    PhotoShot(Type t);
    PhotoShot(int index, QString fileName, Type t);
    int index;
    QDateTime timeStamp;
    QString fileName;

    EasyExif::EXIFInfo exif;

    void serializeXml(QXmlStreamWriter& writer) const;
    void deSerializeXml(QDomElement el);

    void serializeExif(QXmlStreamWriter& writer) const;
    void deSerializeExif(QDomElement el);
};


} // namespace Sequencer

#endif // SEQUENCER_PHOTOSHOT_H
