#include "SonyAlphaRemote_Sequencer_StateFinish.h"

namespace SonyAlphaRemote {
namespace Sequencer {

StateFinish::StateFinish()
    : StateBase(tr("finished"), "", NULL)
{
    setObjectName("StateFinish");
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
