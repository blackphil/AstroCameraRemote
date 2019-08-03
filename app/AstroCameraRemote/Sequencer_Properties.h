#ifndef SEQUENCER_PROPERTIES_H
#define SEQUENCER_PROPERTIES_H

#include <QString>
#include <QDomElement>
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
    void deSerializeXml(QDomElement xml);

    int getShutterSpeedInMilliseconds() const;
    bool isBulb() const;

};

} //namespace Sequencer

#endif // SEQUENCER_PROPERTIES_H
