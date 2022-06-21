#include "Sequencer_Base.h"

#include <AstroBase/AstroBase>
#include "Settings.h"
#include "Settings_General.h"
#include "Sequencer_StateBase.h"


namespace Sequencer {

Base::Base(QObject *parent)
  : QObject(parent)
  , stateMachine(nullptr)
  , numShots(0)
  , startIndex(0)
  , numShotsFinished(0)
{
    connect(StatusPoller::get(), &StatusPoller::statusChanged, this, &Base::handleCameraStatus);
    connect(this, &Base::havePostViewUrl, this, &Base::shotFinished);
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
    connect(stateMachine, &QStateMachine::stopped, this, &Base::stopped);
    connect(stateMachine, &QStateMachine::finished, this, &Base::stopped);

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
    connect(s, &StateBase::updateStatus, this, &Base::updateStatus);
}


} // namespace Sequencer

