#ifndef BULBSHOOTSEQUENCER_H
#define BULBSHOOTSEQUENCER_H

#include <QObject>
#include <QTimer>

#include "Base.h"

namespace Sequencer {



class BulbShootSequencer : public Sequencer::Base
{
    Q_OBJECT


private :
    QTimer* shutterSpeedTm;
    QTimer* pauseDelayTm;
    QTimer* startDelayTm;

    Json::StopBulbShooting stopBulbShooting;

    void handleStarted();
    void handleStopped();

public:


    explicit BulbShootSequencer(QObject *parent = nullptr);

    int getStartDelay() const;
    void setStartDelay(int value);

    int getShutterSpeed() const;
    void setShutterSpeed(int value);

    int getPauseDelay() const;
    void setPauseDelay(int value);

    int calculateSequenceDuration() const;

};

} // namespace Sequencer

#endif // BULBSHOOTSEQUENCER_H
