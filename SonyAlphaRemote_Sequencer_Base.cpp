#include "SonyAlphaRemote_Sequencer_Base.h"

#include "SonyAlphaRemote_Helper.h"

#include "SonyAlphaRemote_Sequencer_StateBase.h"

namespace SonyAlphaRemote {
namespace Sequencer {

Base::Base(StatusPoller *statusPoller, Sender *sender, QObject *parent)
  : QObject(parent)
  , statusPoller(statusPoller)
  , sender(sender)
  , stateMachine(NULL)
  , count(0)
  , numShots(0)
{
    connect(statusPoller, SIGNAL(statusChanged(QString)), this, SLOT(handleCameraStatus(QString)));
}

Base::~Base()
{

}

int Base::getNumShots() const
{
    return numShots;
}

void Base::setNumShots(int value)
{
    numShots = value;
}

bool Base::isRunning() const
{
    return stateMachine && stateMachine->isRunning();
}

int Base::calculateSequenceDuration(int startDelay, int shutterSpeed, int pauseDelay, int numShots)
{
    return startDelay + (2* shutterSpeed + pauseDelay) * numShots - pauseDelay;
}

void Base::start()
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
    connect(stateMachine, SIGNAL(stopped()), this, SIGNAL(stopped()));

    handleStarted();

    stateMachine->start();
}

void Base::stop()
{
    stateMachine->stop();
    handleStopped();
}

void Base::handleCameraStatus(QString status)
{
    if(status == "IDLE")
        Q_EMIT cameraReady();
}

void Base::addState(StateBase *s)
{
    stateMachine->addState(s);
    connect(s, SIGNAL(updateStatus(QString)), this, SIGNAL(updateStatus(QString)));
}


} // namespace Sequencer
} // namespace SonyAlphaRemote
