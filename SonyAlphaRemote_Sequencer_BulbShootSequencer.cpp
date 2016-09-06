#include "SonyAlphaRemote_Sequencer_BulbShootSequencer.h"
#include "SonyAlphaRemote_Helper.h"

#include "SonyAlphaRemote_Sequencer_StateWaitForStart.h"
#include "SonyAlphaRemote_Sequencer_StateBulbShooting.h"
#include "SonyAlphaRemote_Sequencer_StateNormalShooting.h"
#include "SonyAlphaRemote_Sequencer_StateWaitForCamReady.h"
#include "SonyAlphaRemote_Sequencer_StatePause.h"
#include "SonyAlphaRemote_Sequencer_StateFinish.h"

namespace SonyAlphaRemote {
namespace Sequencer {


BulbShootSequencer::BulbShootSequencer(StatusPoller *statusPoller, Sender* sender, QObject *parent)
    : Base(statusPoller, sender, parent)
    , shutterSpeedTm(new QTimer(this))
    , pauseDelayTm(new QTimer(this))
    , startDelayTm(new QTimer(this))
{
    setObjectName(tr("normal shoot"));
    shutterSpeedTm->setSingleShot(true);
    pauseDelayTm->setSingleShot(true);
    startDelayTm->setSingleShot(true);
    shutterSpeedTm->setInterval(1000);
    pauseDelayTm->setInterval(1000);
    startDelayTm->setInterval(1000);
}

void BulbShootSequencer::handleStarted()
{

    StateWaitForStart* waitForStart = new StateWaitForStart(startDelayTm);
    connect(waitForStart, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
    connect(stateMachine, SIGNAL(started()), this, SIGNAL(started()));
    connect(stateMachine, SIGNAL(stopped()), this, SLOT(handleStopped()));
    connect(stateMachine, SIGNAL(finished()), this, SLOT(handleStopped()));

    addState(waitForStart);
    stateMachine->setInitialState(waitForStart);


    QState* prevState = waitForStart;
    QTimer* currentTimer = startDelayTm;
    for(int i=0; i<numShots; i++)
    {
        StateBulbShooting* shooting = new StateBulbShooting(sender, shutterSpeedTm, i+1, numShots);
        connect(shooting, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
        addState(shooting);

        prevState->addTransition(currentTimer, SIGNAL(timeout()), shooting);

        StateWaitForCamReady* waitForCamReady = new StateWaitForCamReady(sender, i+1, numShots);
        connect(waitForCamReady, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
        connect(waitForCamReady, SIGNAL(havePostViewUrl(QString, int, int)), this, SIGNAL(havePostViewUrl(QString, int, int)));
        addState(waitForCamReady);
        shooting->addTransition(shutterSpeedTm, SIGNAL(timeout()), waitForCamReady);

        StatePause* pause = new StatePause(pauseDelayTm);
        addState(pause);
        connect(pause, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
        waitForCamReady->addTransition(this, SIGNAL(cameraReady()), pause);


        prevState = pause;
        currentTimer = pauseDelayTm;
    }

    StateFinish* finish = new StateFinish();
    connect(finish, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
    prevState->addTransition(finish);
    addState(finish);
    QFinalState* done = new QFinalState();
    finish->addTransition(done);
    stateMachine->addState(done);


}

int BulbShootSequencer::getStartDelay() const
{
    return startDelayTm->interval();
}
void BulbShootSequencer::setStartDelay(int value)
{
    startDelayTm->setInterval(value);
}

int BulbShootSequencer::getShutterSpeed() const
{
    return shutterSpeedTm->interval();
}
void BulbShootSequencer::setShutterSpeed(int value)
{
    shutterSpeedTm->setInterval(value);
}

int BulbShootSequencer::getPauseDelay() const
{
    return pauseDelayTm->interval();
}
void BulbShootSequencer::setPauseDelay(int value)
{
    pauseDelayTm->setInterval(value);
}

int BulbShootSequencer::calculateSequenceDuration() const
{
    return Base::calculateSequenceDuration(startDelayTm->interval(), shutterSpeedTm->interval(), pauseDelayTm->interval(), numShots);
}

void BulbShootSequencer::handleStopped()
{
    sender->send(&stopBulbShooting);

    if(shutterSpeedTm)
        shutterSpeedTm->stop();

    if(pauseDelayTm)
        pauseDelayTm->stop();

    if(startDelayTm)
        startDelayTm->stop();
}


} // namespace Sequencer
} // namespace SonyAlphaRemote
