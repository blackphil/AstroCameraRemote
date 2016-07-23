#include "SonyAlphaRemote_Sequencer_StateWaitForStart.h"

namespace SonyAlphaRemote {
namespace Sequencer {

StateWaitForStart::StateWaitForStart(QTimer *t)
    : StateBase(tr("wait for starting BULB sequence (%0 milliseconds)").arg(t->interval()), "", t)
{
    setObjectName("StateWaitForStart");
}



} // namespace Sequencer
} // namespace SonyAlphaRemote
