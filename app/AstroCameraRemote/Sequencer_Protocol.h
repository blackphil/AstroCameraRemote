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

public:
    explicit Protocol(QObject *parent = nullptr);

    void serializeXml(QXmlStreamWriter& writer) const;
    void deSerializeXml(QXmlStreamReader& reader);

    Properties getProperties() const;
    void setProperties(const Properties &value);

public Q_SLOTS :
    void start();
    void shotFinished(QString url, int index, int numShots);
    void sequenceFinished();

};

} // namespace Sequencer

#endif // SEQUENCER_PROTOCOL_H
