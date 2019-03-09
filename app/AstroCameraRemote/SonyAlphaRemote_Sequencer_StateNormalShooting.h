#ifndef SEQUENCER_STATENORMALSHOOTING_H
#define SEQUENCER_STATENORMALSHOOTING_H

#include "SonyAlphaRemote_Sequencer_StateBase.h"
#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_Sender.h"


namespace Sequencer {

class StateNormalShooting : public StateBase
{

    Json::ActTakePicture actTakePicture;
    QPointer<Sender> sender;

public:
    StateNormalShooting(Sender *sender, quint32 i, quint32 maxCount);

protected :
    void onEntry(QEvent* ev);
};

} // namespace Sequencer


#endif // SEQUENCER_STATENORMALSHOOTING_H
