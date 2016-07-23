#include "SonyAlphaRemote_Sequencer_StatePause.h"

namespace SonyAlphaRemote {
namespace Sequencer {

StatePause::StatePause(QTimer *t)
    : StateBase(tr("pause"), "", t)
{
    setObjectName("StatePause");
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
