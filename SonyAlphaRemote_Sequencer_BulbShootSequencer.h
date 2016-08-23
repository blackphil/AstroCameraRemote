#ifndef SONYALPHAREMOTE_BULBSHOOTSEQUENCER_H
#define SONYALPHAREMOTE_BULBSHOOTSEQUENCER_H

#include <QObject>
#include <QTimer>

#include "SonyAlphaRemote_Sequencer_Base.h"

namespace SonyAlphaRemote {
namespace Sequencer {



class BulbShootSequencer : public Base
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


    explicit BulbShootSequencer(StatusPoller* statusPoller, Sender* sender, QObject *parent = 0);

    int getStartDelay() const;
    void setStartDelay(int value);

    int getShutterSpeed() const;
    void setShutterSpeed(int value);

    int getPauseDelay() const;
    void setPauseDelay(int value);

    int calculateSequenceDuration() const;

};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_BULBSHOOTSEQUENCER_H
