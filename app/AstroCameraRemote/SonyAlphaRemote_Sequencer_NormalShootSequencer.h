#ifndef SONYALPHAREMOTE_NormalShootSequencer_H
#define SONYALPHAREMOTE_NormalShootSequencer_H

#include <QObject>
#include <QTimer>
#include <QStateMachine>
#include <QState>
#include <QByteArray>

#include "SonyAlphaRemote_Sequencer_Base.h"

namespace SonyAlphaRemote {
namespace Sequencer {


class NormalShootSequencer : public Base
{
    Q_OBJECT


private :
    QTimer* pauseDelayTm;
    QTimer* startDelayTm;
    int shutterSpeed;

    void handleStarted();
    void handleStopped();

public:


    explicit NormalShootSequencer(StatusPoller* statusPoller, Sender* sender, QObject *parent = 0);

    int getStartDelay() const;
    void setStartDelay(int value);

    int getPauseDelay() const;
    void setPauseDelay(int value);

    int calculateSequenceDuration() const;

    int getShutterSpeed() const;
    void setShutterSpeed(int value);


};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_NormalShootSequencer_H
