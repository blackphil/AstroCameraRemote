#ifndef SEQUENCER_PROPERTIES_H
#define SEQUENCER_PROPERTIES_H

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace Sequencer {

struct Properties
{
    Properties();
    QString shutterSpeed;
    QString iso;
    int shutterSpeedBulb;
    int shutterSpeedBulbUnit;
    int startDelay;
    int startDelayUnit;
    int pause;
    int pauseUnit;
    int numShots;

    void serializeXml(QXmlStreamWriter& writer) const;
    void deSerializeXml(QXmlStreamReader& reader);

    int getShutterSpeedInMilliseconds() const;
    bool isBulb() const;

};

} //namespace Sequencer

#endif // SEQUENCER_PROPERTIES_H
