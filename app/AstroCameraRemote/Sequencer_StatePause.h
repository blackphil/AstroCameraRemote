#ifndef SEQUENCER_STATEPAUSE_H
#define SEQUENCER_STATEPAUSE_H

#include "Sequencer_StateBase.h"



namespace Sequencer {

class StatePause : public StateBase
{
    Q_OBJECT
public:
    StatePause(QTimer* t);
};

} // namespace Sequencer


#endif // SEQUENCER_STATEPAUSE_H
