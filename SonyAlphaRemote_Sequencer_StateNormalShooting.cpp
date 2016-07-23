#include "SonyAlphaRemote_Sequencer_StateNormalShooting.h"

namespace SonyAlphaRemote {
namespace Sequencer {

StateNormalShooting::StateNormalShooting()
    : StateBase(tr("trigger shoot"), "", NULL)
{
    setObjectName("StateNormalShooting");
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
