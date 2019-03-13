#ifndef SHOOTSEQUENCE_H
#define SHOOTSEQUENCE_H

#include <QString>
#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include <QSharedPointer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Settings.h"


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

#if 0
class Settings : public Setting
{
    Q_OBJECT

    Properties properties;


public:
    Settings(Setting* parent);
    ~Settings();

    void load();
    void save();

    QString getShutterSpeed() const;
    void setShutterSpeed(const QString &value);
    QString getIso() const;
    void setIso(const QString &value);
    int getShutterSpeedBulb() const;
    void setShutterSpeedBulb(int value);
    int getStartDelay() const;
    void setStartDelay(int value);
    int getPause() const;
    void setPause(int value);
    int getNumShots() const;
    void setNumShots(int value);
    int getShutterspeedBulbUnit() const;
    void setShutterspeedBulbUnit(int value);
    int getStartDelayUnit() const;
    void setStartDelayUnit(int value);
    int getPauseUnit() const;
    void setPauseUnit(int value);
    Properties getProperties() const;
    void setProperties(const Properties &value);
};
#endif
} //namespace Sequencer

#endif // SHOOTSEQUENCE_H
