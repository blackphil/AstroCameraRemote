#ifndef NormalShootSequencer_H
#define NormalShootSequencer_H

#include <QObject>
#include <QTimer>
#include <QStateMachine>
#include <QState>
#include <QByteArray>

#include "Sequencer_Base.h"


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


    explicit NormalShootSequencer(QObject *parent = nullptr);

    int getStartDelay() const;
    void setStartDelay(int value);

    int getPauseDelay() const;
    void setPauseDelay(int value);

    int calculateSequenceDuration() const;

    int getShutterSpeed() const;
    void setShutterSpeed(int value);


};

} // namespace Sequencer


#endif // NormalShootSequencer_H
