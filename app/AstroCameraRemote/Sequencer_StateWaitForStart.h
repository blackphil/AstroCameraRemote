#ifndef SEQUENCER_STATEWAITFORSTART_H
#define SEQUENCER_STATEWAITFORSTART_H

#include "Sequencer_StateBase.h"


namespace Sequencer {

class StateWaitForStart : public StateBase
{
    Q_OBJECT

    QPointer<QTimer> t;
public :
    StateWaitForStart(QTimer* t);

};

} // namespace Sequencer
#endif // SEQUENCER_STATEWAITFORSTART_H
