#ifndef SEQUENCER_BASE_H
#define SEQUENCER_BASE_H

#include <QObject>
#include <QStateMachine>
#include <QState>

#include "Json_Command.h"
#include "StatusPoller.h"



namespace Sequencer {

class StateBase;

class Base : public QObject
{
    Q_OBJECT

protected :
    QStateMachine* stateMachine;

    int numShots;
    int startIndex;
    int numShotsFinished;

    virtual void handleStarted() = 0;

protected Q_SLOTS :
    virtual void handleStopped() = 0;
    virtual void handleCameraStatus(QString status);

    void addState(StateBase* s);

public:
    explicit Base(QObject *parent = nullptr);
    virtual ~Base();

    int getNumShots() const;
    void setNumShots(int value);
    bool isRunning() const;

    virtual int calculateSequenceDuration() const = 0;
    virtual void setStartDelay(int value) = 0;
    virtual void setShutterSpeed(int value) = 0;
    virtual void setPauseDelay(int value) = 0;

    static int calculateSequenceDuration(int startDelay, int shutterSpeed, int pauseDelay, int numShots);

    int getStartIndex() const;
    void setStartIndex(int value);

    int getNumShotsFinished() const;

Q_SIGNALS :
    void started();
    void stopped();
    void statusMessage(QString);
    void countChanged(int);
    void cameraReady();
    void havePostViewUrl(QString, int, int);

    void updateStatus(QString);


public Q_SLOTS :
    void start();
    void stop();

    void shotFinished();


};

} // namespace Sequencer


#endif // SEQUENCER_BASE_H
