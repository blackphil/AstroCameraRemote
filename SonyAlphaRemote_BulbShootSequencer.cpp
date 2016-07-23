#include "SonyAlphaRemote_BulbShootSequencer.h"
#include "SonyAlphaRemote_Helper.h"

//#define DRY_RUN

namespace SonyAlphaRemote {

StateWaitForStart::StateWaitForStart(QTimer* t) : t(t) {}

void StateWaitForStart::onEntry(QEvent*)
{
    QString msg = tr("wait for starting BULB sequence (%0 milliseconds)").arg(t->interval());
    SAR_INF(msg);
    Q_EMIT message(msg);
    t->start();
}

StateShooting::StateShooting(Sender* sender, QTimer* t, quint32 i, quint32 maxCount)
        : t(t)
        , sender(sender)
        , i(i)
        , maxCount(maxCount)
    {}

void StateShooting::onEntry(QEvent*)
{
    QString msg = tr("start shoot (%0/%1)").arg(i).arg(maxCount);
    SAR_INF(msg);
    Q_EMIT message(msg);

#ifndef DRY_RUN
    if(!sender.isNull())
        sender.data()->send(&startBulbShooting);
#endif
    t->start();
}

void StateShooting::onExit(QEvent*)
{
    QString msg = tr("stop shoot (%0/%1)").arg(i).arg(maxCount);
    SAR_INF(msg);
    Q_EMIT message(msg);
#ifndef DRY_RUN
    if(!sender.isNull())
        sender.data()->send(&stopBulbShooting);
#endif
}

StateWaitForCamReady::StateWaitForCamReady(Sender* sender, int i, int numShots)
    : awaitTakePicture(new Json::AwaitTakePicture(this))
    , sender(sender)
    , i(i)
    , numShots(numShots)
{
    connect(awaitTakePicture, SIGNAL(havePostViewUrl(QString)), this, SIGNAL(updatePostViewInfo(QString)));
}

void StateWaitForCamReady::onEntry(QEvent*)
{
    QString msg = tr("wait for camera IDLE mode");
    SAR_INF(msg);
    Q_EMIT message(msg);
}

void StateWaitForCamReady::onExit(QEvent *event)
{
    sender->send(awaitTakePicture);
}

void StateWaitForCamReady::updatePostViewInfo(QString url)
{
    Q_EMIT havePostViewUrl(url, i, numShots);
}


StatePause::StatePause(QTimer* t) : t(t) {}

void StatePause::onEntry(QEvent*)
{
    QString msg = tr("pause");
    SAR_INF(msg);
    Q_EMIT message(msg);
    t->start();
}


void StateFinish::onEntry(QEvent *event)
{
    QString msg = tr("finished");
    SAR_INF(msg);
    Q_EMIT message(msg);
}

BulbShootSequencer::BulbShootSequencer(StatusPoller *statusPoller, Sender* sender, QObject *parent)
    : QObject(parent)
    , statusPoller(statusPoller)
    , sender(sender)
    , stateMachine(NULL)
    , count(0)
    , shutterSpeedTm(new QTimer(this))
    , pauseDelayTm(new QTimer(this))
    , startDelayTm(new QTimer(this))
    , numShots(3)
{
    connect(statusPoller, SIGNAL(statusChanged(QString)), this, SLOT(handleCameraStatus(QString)));
    shutterSpeedTm->setSingleShot(true);
    pauseDelayTm->setSingleShot(true);
    startDelayTm->setSingleShot(true);
    shutterSpeedTm->setInterval(1000);
    pauseDelayTm->setInterval(1000);
    startDelayTm->setInterval(1000);


}

void BulbShootSequencer::start()
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
    stateMachine->addState(waitForStart);
    stateMachine->setInitialState(waitForStart);


    QState* prevState = waitForStart;
    QTimer* currentTimer = startDelayTm;
    for(quint32 i=0; i<numShots; i++)
    {
        StateShooting* shooting = new StateShooting(sender, shutterSpeedTm, i+1, numShots);
        connect(shooting, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
        stateMachine->addState(shooting);

        prevState->addTransition(currentTimer, SIGNAL(timeout()), shooting);

        StateWaitForCamReady* waitForCamReady = new StateWaitForCamReady(sender, i+1, numShots);
        connect(waitForCamReady, SIGNAL(message(QString)), this, SIGNAL(statusMessage(QString)));
        connect(waitForCamReady, SIGNAL(havePostViewUrl(QString, int, int)), this, SIGNAL(havePostViewUrl(QString, int, int)));
        stateMachine->addState(waitForCamReady);
        shooting->addTransition(shutterSpeedTm, SIGNAL(timeout()), waitForCamReady);

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

void BulbShootSequencer::handleCameraStatus(QString status)
{
    if(status == "IDLE")
        Q_EMIT cameraReady();
}

int BulbShootSequencer::getNumShots() const
{
    return numShots;
}

void BulbShootSequencer::setNumShots(int value)
{
    numShots = value;
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

bool BulbShootSequencer::isRunning() const
{
    return stateMachine && stateMachine->isRunning();
}

int BulbShootSequencer::calculateSequenceDuration() const
{
    return startDelayTm->interval() + (2 * shutterSpeedTm->interval() + pauseDelayTm->interval()) * numShots;
}



} // namespace SonyAlphaRemote
