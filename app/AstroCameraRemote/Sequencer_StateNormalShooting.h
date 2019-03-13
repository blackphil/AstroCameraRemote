#ifndef SEQUENCER_STATENORMALSHOOTING_H
#define SEQUENCER_STATENORMALSHOOTING_H

#include "Sequencer_StateBase.h"
#include "Json_Command.h"
#include "Sender.h"


namespace Sequencer {

class StateNormalShooting : public StateBase
{

    Json::ActTakePicture actTakePicture;

public:
    StateNormalShooting(int i, int maxCount);

protected :
    void onEntry(QEvent* ev);
};

} // namespace Sequencer


#endif // SEQUENCER_STATENORMALSHOOTING_H
