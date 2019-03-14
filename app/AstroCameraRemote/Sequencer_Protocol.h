#ifndef SEQUENCER_PROTOCOL_H
#define SEQUENCER_PROTOCOL_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Sequencer_Properties.h"

namespace Sequencer {

class Protocol : public QObject
{
    Q_OBJECT

    Properties properties;

    struct PhotoShot
    {
        PhotoShot();
        PhotoShot(int index, QString fileName);
        int index;
        QDateTime timeStamp;
        QString fileName;

        void serializeXml(QXmlStreamWriter& writer) const;
        void deSerializeXml(QXmlStreamReader& reader);
    };
    QList<PhotoShot> photoShots;

    QList<QRectF> markers;

    QDateTime startTime;

    QString subject;

    bool recording;

public:
    explicit Protocol(QObject *parent = nullptr);
    ~Protocol();

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

    bool getRecording() const;

    QList<QRectF> getReferenceMarkers() const;

public Q_SLOTS :
    void start();
    void shotFinished(QString url, int index, int numShots);
    void setReferenceMarkers(const QList<QRectF>& markers);
    void cleanUpMarkers();
    void stop();

    bool deleteFile();

};

} // namespace Sequencer

#endif // SEQUENCER_PROTOCOL_H
