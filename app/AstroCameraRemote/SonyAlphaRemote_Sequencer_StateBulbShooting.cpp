#include "SonyAlphaRemote_Sequencer_StateBulbShooting.h"


#include "SonyAlphaRemote_Sender.h"


namespace Sequencer {

StateBulbShooting::StateBulbShooting(QTimer* t, quint32 i, quint32 maxCount)
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

