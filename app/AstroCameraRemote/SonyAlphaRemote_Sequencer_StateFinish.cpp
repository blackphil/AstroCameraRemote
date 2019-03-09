#include "SonyAlphaRemote_Sequencer_StateFinish.h"


namespace Sequencer {

StateFinish::StateFinish()
    : StateBase(tr("finished"), "", NULL)
{
    setObjectName("StateFinish");
}

} // namespace Sequencer

