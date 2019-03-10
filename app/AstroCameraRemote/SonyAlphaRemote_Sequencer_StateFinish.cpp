#include "SonyAlphaRemote_Sequencer_StateFinish.h"


namespace Sequencer {

StateFinish::StateFinish()
    : StateBase(tr("finished"), "", nullptr)
{
    setObjectName("StateFinish");
}

} // namespace Sequencer

