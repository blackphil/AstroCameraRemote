#ifndef SEQUENCER_PROPERTIES_H
#define SEQUENCER_PROPERTIES_H

#include <QString>
#include <QDomElement>
#include <QXmlStreamWriter>

#include "../TimeUnit.h"

namespace Sequencer {

struct Properties
{
    Properties();

    QString shutterSpeed;
    QString iso;
    int shutterSpeedBulb;
    TimeUnit::Unit shutterSpeedBulbUnit;
    int startDelay;
    TimeUnit::Unit startDelayUnit;
    int pause;
    TimeUnit::Unit pauseUnit;
    int numShots;

    void serializeXml(QXmlStreamWriter& writer) const;
    void deSerializeXml(QDomElement xml);

    int getShutterSpeedInMilliseconds() const;
    bool isBulb() const;


};

} //namespace Sequencer

#endif // SEQUENCER_PROPERTIES_H
