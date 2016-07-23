#include "SonyAlphaRemote_Sequencer_StateBulbShooting.h"

//#define DRY_RUN


namespace SonyAlphaRemote {
namespace Sequencer {

StateBulbShooting::StateBulbShooting(Sender* sender, QTimer* t, quint32 i, quint32 maxCount)
    : StateBase(
          tr("start shoot (%0/%1)").arg(i).arg(maxCount)
          , tr("stop shoot (%0/%1)").arg(i).arg(maxCount)
          , t)
    , sender(sender)
{
    setObjectName("StateBulbShooting");
}


void StateBulbShooting::onEntry(QEvent* event)
{

#ifndef DRY_RUN
    if(!sender.isNull())
        sender.data()->send(&startBulbShooting);
#endif

    StateBase::onEntry(event);
}

void StateBulbShooting::onExit(QEvent* event)
{
#ifndef DRY_RUN
    if(!sender.isNull())
        sender.data()->send(&stopBulbShooting);
#endif

    StateBase::onExit(event);
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
