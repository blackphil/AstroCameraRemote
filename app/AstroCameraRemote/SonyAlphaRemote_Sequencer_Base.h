#ifndef SONYALPHAREMOTE_SEQUENCER_BASE_H
#define SONYALPHAREMOTE_SEQUENCER_BASE_H

#include <QObject>
#include <QStateMachine>
#include <QState>

#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_StatusPoller.h"


namespace SonyAlphaRemote {
namespace Sequencer {

class StateBase;

class Base : public QObject
{
    Q_OBJECT

protected :
    QPointer<StatusPoller> statusPoller;
    QPointer<Sender> sender;
    QStateMachine* stateMachine;

    int count;
    int numShots;

    virtual void handleStarted() = 0;

protected Q_SLOTS :
    virtual void handleStopped() = 0;
    virtual void handleCameraStatus(QString status);

    void addState(StateBase* s);

public:
    explicit Base(StatusPoller* statusPoller, Sender* sender, QObject *parent = 0);
    virtual ~Base();

    int getNumShots() const;
    void setNumShots(int value);
    bool isRunning() const;

    virtual int calculateSequenceDuration() const = 0;

    static int calculateSequenceDuration(int startDelay, int shutterSpeed, int pauseDelay, int numShots);

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


};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SEQUENCER_BASE_H
