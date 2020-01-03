#include "StatePause.h"


namespace Sequencer {

StatePause::StatePause(QTimer *t)
    : StateBase(tr("pause"), "", t)
{
    setObjectName("StatePause");
}

} // namespace Sequencer

