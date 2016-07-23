#include "SonyAlphaRemote_Sequencer_StatePause.h"

namespace SonyAlphaRemote {
namespace Sequencer {

StatePause::StatePause(QTimer *t)
    : StateBase(tr("pause"), "", t)
{

}

} // namespace Sequencer
} // namespace SonyAlphaRemote
