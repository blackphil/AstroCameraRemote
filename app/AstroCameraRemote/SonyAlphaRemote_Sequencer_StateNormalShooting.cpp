#include "SonyAlphaRemote_Sequencer_StateNormalShooting.h"

namespace SonyAlphaRemote {
namespace Sequencer {

StateNormalShooting::StateNormalShooting(Sender* sender, quint32 i, quint32 maxCount)
    : StateBase(tr("take shoot (%0/%1").arg(i).arg(maxCount), "", NULL)
    , sender(sender)
{
    setObjectName("StateNormalShooting");
}

void StateNormalShooting::onEntry(QEvent *ev)
{
#ifndef DRY_RUN
    if(!sender.isNull())
        sender.data()->send(&actTakePicture);
#endif

    StateBase::onEntry(ev);
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
