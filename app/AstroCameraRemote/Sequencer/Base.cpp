#include "Base.h"

#include "AstroBase.h"
#include "../Settings.h"
#include "../Settings_General.h"
#include "StateBase.h"


namespace Sequencer {

Base::Base(QObject *parent)
  : QObject(parent)
  , stateMachine(nullptr)
  , numShots(0)
  , startIndex(0)
  , numShotsFinished(0)
{
    connect(StatusPoller::get(), SIGNAL(statusChanged(QString)), this, SLOT(handleCameraStatus(QString)));
    connect(this, SIGNAL(havePostViewUrl(QString, int, int)), this, SLOT(shotFinished()));
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
    int factor = 1;
    if(GeneralSettings::getLenrEnabled())
        factor = 2;

    return startDelay + (factor * shutterSpeed + pauseDelay) * numShots - pauseDelay;
}

void Base::start()
{
    if(stateMachine)
    {
        if(stateMachine->active())
        {
            AB_ERR("cannot start, sequence still active");
            return;
        }

        delete stateMachine;
    }

    stateMachine = new QStateMachine(this);
    connect(stateMachine, SIGNAL(stopped()), this, SIGNAL(stopped()));
    connect(stateMachine, SIGNAL(finished()), this, SIGNAL(stopped()));

    handleStarted();

    stateMachine->start();
}

void Base::stop()
{
    stateMachine->stop();
    handleStopped();
    Q_EMIT updateStatus(tr("Sequence \"%0\" stopped").arg(objectName()));
}

void Base::shotFinished()
{
    numShotsFinished++;
}

int Base::getStartIndex() const
{
    return startIndex;
}

void Base::setStartIndex(int value)
{
    startIndex = value;
}

int Base::getNumShotsFinished() const
{
    return numShotsFinished;
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

