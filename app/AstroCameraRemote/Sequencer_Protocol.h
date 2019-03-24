#ifndef SEQUENCER_PROTOCOL_H
#define SEQUENCER_PROTOCOL_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Sequencer_Properties.h"
#include "EasyExif_Exif.h"

namespace Sequencer {

class Protocol : public QObject
{
    Q_OBJECT

public :
    enum Type
    {
        Type_Focusing
        , Type_Light
        , Type_Dark
        , Type_Flat
        , NumTypes
    };

    enum ColorChannel
    {
        Color_RGB
        , Color_Luminance
        , Color_Red
        , Color_Green
        , Color_Blue
        , NumColorChannels
    };

    static QString typeToString(Type t);
    static Type typeFromString(const QString& t);

    static QString colorChannelToString(ColorChannel c);
    static ColorChannel colorChannelFromString(const QString& c);

    struct PhotoShot
    {
        PhotoShot();
        PhotoShot(int index, QString fileName);
        int index;
        QDateTime timeStamp;
        QString fileName;

        EasyExif::EXIFInfo exif;

        void serializeXml(QXmlStreamWriter& writer) const;
        void deSerializeXml(QXmlStreamReader& reader);

        void serializeExif(QXmlStreamWriter& writer) const;
        void deSerializeExif(QXmlStreamReader& reader);
    };

private :

    enum Status
    {
          Status_Stopped
        , Status_Recording
        , Status_Stopping
    };

    Status status;



    Type type;
    ColorChannel colorChannel;
    Properties properties;
    QList<PhotoShot> photoShots;
    QList<QRectF> markers;
    QDateTime startTime;
    QString subject;


public:
    explicit Protocol(QObject *parent = nullptr);
    ~Protocol();

    bool isRecording() const;

    void serializeXml(QXmlStreamWriter& writer) const;
    void deSerializeXml(QXmlStreamReader& reader);

    const Properties &getProperties() const;
    void setProperties(const Properties &value);

    QDateTime getStartTime() const;
    int getNumShotsFinished() const;

    static QString getProtocolPath(bool createIfNotExists = false);

    QString getSubject() const;
    void setSubject(const QString &value);

    QString getFilePath() const;

    QList<QRectF> getReferenceMarkers() const;

    Type getType() const;
    void setType(const Type &value);

    const QList<PhotoShot>& getPhotoShots() const;

    ColorChannel getColorChannel() const;
    void setColorChannel(const ColorChannel &value);

public Q_SLOTS :
    void start();
    void havePostViewUrl(QString url, int index, int numShots);
    void havePostViewImage(const QByteArray& data);
    void setReferenceMarkers(const QList<QRectF>& markers);
    void cleanUpMarkers();
    void stop();
    void save();

    bool deleteFile();

};

} // namespace Sequencer

#endif // SEQUENCER_PROTOCOL_H
