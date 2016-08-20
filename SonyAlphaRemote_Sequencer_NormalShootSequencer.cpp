#include "SonyAlphaRemote_Sequencer_NormalShootSequencer.h"
#include "SonyAlphaRemote_Helper.h"

#include "SonyAlphaRemote_Sequencer_StateWaitForStart.h"
#include "SonyAlphaRemote_Sequencer_StateNormalShooting.h"
#include "SonyAlphaRemote_Sequencer_StateWaitForCamReady.h"
#include "SonyAlphaRemote_Sequencer_StatePause.h"
#include "SonyAlphaRemote_Sequencer_StateFinish.h"

namespace SonyAlphaRemote {
namespace Sequencer {




NormalShootSequencer::NormalShootSequencer(StatusPoller *statusPoller, Sender* sender, QObject *parent)
    : QObject(parent)
    , statusPoller(statusPoller)
    , sender(sender)
    , stateMachine(NULL)
    , count(0)
    , pauseDelayTm(new QTimer(this))
    , startDelayTm(new QTimer(this))
    , numShots(3)
    , shutterSpeed(0)
{
    connect(statusPoller, SIGNAL(statusChanged(QString)), this, SLOT(handleCameraStatus(QString)));
    pauseDelayTm->setSingleShot(true);
    startDelayTm->setSingleShot(true);
    pauseDelayTm->setInterval(1000);
    startDelayTm->setInterval(1000);


}

void NormalShootSequencer::start()
{
    if(stateMachine)
    {
        if(stateMachine->active())
        {
            SAR_ERR("cannot start, sequence still active");
            return;
        }

        delete stateMachine;
    }

    stateMachine = new QStateMachine(this);

    StateWaitForStart* waitForStart = new StateWaitForStart(startDelayTm);
    connect(waitForStart, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
    connect(stateMachine, SIGNAL(started()), this, SIGNAL(started()));
    connect(stateMachine, SIGNAL(stopped()), this, SLOT(handleStopped()));
    connect(stateMachine, SIGNAL(finished()), this, SLOT(handleStopped()));

    stateMachine->addState(waitForStart);
    stateMachine->setInitialState(waitForStart);


    QState* prevState = waitForStart;
    QTimer* currentTimer = startDelayTm;
    for(int i=0; i<numShots; i++)
    {
        StateNormalShooting* shooting = new StateNormalShooting(sender, i+1, numShots);
        connect(shooting, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
        stateMachine->addState(shooting);

        prevState->addTransition(currentTimer, SIGNAL(timeout()), shooting);

        StateWaitForCamReady* waitForCamReady = new StateWaitForCamReady(sender, i+1, numShots);
        connect(waitForCamReady, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
        connect(waitForCamReady, SIGNAL(havePostViewUrl(QString, int, int)), this, SIGNAL(havePostViewUrl(QString, int, int)));
        stateMachine->addState(waitForCamReady);
        shooting->addTransition(waitForCamReady);

        StatePause* pause = new StatePause(pauseDelayTm);
        stateMachine->addState(pause);
        connect(pause, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
        waitForCamReady->addTransition(this, SIGNAL(cameraReady()), pause);


        prevState = pause;
        currentTimer = pauseDelayTm;
    }

    StateFinish* finish = new StateFinish();
    connect(finish, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
    prevState->addTransition(finish);
    stateMachine->addState(finish);
    QFinalState* done = new QFinalState();
    finish->addTransition(done);
    stateMachine->addState(done);


    stateMachine->start();
}

void NormalShootSequencer::stop()
{
    stateMachine->stop();
    handleStopped();
}

void NormalShootSequencer::handleCameraStatus(QString status)
{
    if(status == "IDLE")
        Q_EMIT cameraReady();
}

int NormalShootSequencer::getNumShots() const
{
    return numShots;
}

void NormalShootSequencer::setNumShots(int value)
{
    numShots = value;
}

int NormalShootSequencer::getStartDelay() const
{
    return startDelayTm->interval();
}
void NormalShootSequencer::setStartDelay(int value)
{
    startDelayTm->setInterval(value);
}

int NormalShootSequencer::getPauseDelay() const
{
    return pauseDelayTm->interval();
}
void NormalShootSequencer::setPauseDelay(int value)
{
    pauseDelayTm->setInterval(value);
}

bool NormalShootSequencer::isRunning() const
{
    return stateMachine && stateMachine->isRunning();
}

int NormalShootSequencer::calculateSequenceDuration() const
{
    return calculateSequenceDuration(startDelayTm->interval(), shutterSpeed, pauseDelayTm->interval(), numShots);
}

int NormalShootSequencer::calculateSequenceDuration(int startDelay, int shutterSpeed, int pauseDelay, int numShots)
{
    return startDelay + (2* shutterSpeed + pauseDelay) * numShots - pauseDelay;
}

void NormalShootSequencer::handleStopped()
{

    if(pauseDelayTm)
        pauseDelayTm->stop();

    if(startDelayTm)
        startDelayTm->stop();

    Q_EMIT stopped();
}

int NormalShootSequencer::getShutterSpeed() const
{
    return shutterSpeed;
}

void NormalShootSequencer::setShutterSpeed(int value)
{
    shutterSpeed = value;
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
