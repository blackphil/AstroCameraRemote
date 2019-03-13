#include "Sequencer_StateBulbShooting.h"


#include "Sender.h"


namespace Sequencer {

StateBulbShooting::StateBulbShooting(QTimer* t, int i, int maxCount)
    : StateBase(
          tr("start shoot (%0/%1)").arg(i).arg(maxCount)
          , tr("stop shoot (%0/%1)").arg(i).arg(maxCount)
          , t)
{
    setObjectName("StateBulbShooting");
}


void StateBulbShooting::onEntry(QEvent* event)
{

#ifndef DRY_RUN
    Sender::get()->send(&startBulbShooting);
#endif

    StateBase::onEntry(event);
}

void StateBulbShooting::onExit(QEvent* event)
{
#ifndef DRY_RUN
    Sender::get()->send(&stopBulbShooting);
#endif

    StateBase::onExit(event);
}

} // namespace Sequencer

