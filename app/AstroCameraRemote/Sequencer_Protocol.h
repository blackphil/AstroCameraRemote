#ifndef SEQUENCER_PROTOCOL_H
#define SEQUENCER_PROTOCOL_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Sequencer_Settings.h"

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

    QDateTime startTime;
    bool stashed;

    QString subject;

public:
    explicit Protocol(QObject *parent = nullptr);

    void serializeXml(QXmlStreamWriter& writer) const;
    void deSerializeXml(QXmlStreamReader& reader);

    const Properties &getProperties() const;
    void setProperties(const Properties &value);

    bool getStashed() const;
    void setStashed(bool value);

    QDateTime getStartTime() const;
    int getNumShotsFinished() const;

    static QString getProtocolPath(bool createIfNotExists = false);
    static QString getStashPath(bool createIfNotExists = false);

    QString getSubject() const;
    void setSubject(const QString &value);

    QString getFilePath() const;

public Q_SLOTS :
    void start();
    void shotFinished(QString url, int index, int numShots);
    void stop();

    void deleteFile();

};

} // namespace Sequencer

#endif // SEQUENCER_PROTOCOL_H
