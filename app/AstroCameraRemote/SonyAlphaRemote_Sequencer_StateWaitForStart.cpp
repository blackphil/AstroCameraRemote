#include "SonyAlphaRemote_Sequencer_StateWaitForStart.h"


namespace Sequencer {

StateWaitForStart::StateWaitForStart(QTimer *t)
    : StateBase(tr("wait for starting sequence (%0 milliseconds)").arg(t->interval()), "", t)
{
    setObjectName("StateWaitForStart");
}



} // namespace Sequencer

