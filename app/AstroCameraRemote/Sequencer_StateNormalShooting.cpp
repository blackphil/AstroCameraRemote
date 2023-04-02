#include "Sequencer_StateNormalShooting.h"


namespace Sequencer {

StateNormalShooting::StateNormalShooting(int i, int maxCount)
    : StateBase(tr("take shoot (%0/%1").arg(i+1).arg(maxCount), "", nullptr)
{
    setObjectName("StateNormalShooting");
}

void StateNormalShooting::onEntry(QEvent *ev)
{
#ifndef DRY_RUN
    Sender::get()->send(&actTakePicture);
#endif

    StateBase::onEntry(ev);
}

} // namespace Sequencer

