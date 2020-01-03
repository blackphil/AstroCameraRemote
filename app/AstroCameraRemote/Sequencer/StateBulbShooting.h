#ifndef SEQUENCER_STATEBULBSHOOTING_H
#define SEQUENCER_STATEBULBSHOOTING_H

#include "StateBase.h"
#include "../Json/Command.h"
#include "../Sender.h"


namespace Sequencer {

class StateBulbShooting : public StateBase
{
    Q_OBJECT

    Json::StartBulbShooting startBulbShooting;
    Json::StopBulbShooting stopBulbShooting;

public :
    StateBulbShooting(QTimer* t, int i, int maxCount);

protected :
    void onEntry(QEvent* event);
    void onExit(QEvent* event);

};

} // namespace Sequencer

#endif // SEQUENCER_STATEBULBSHOOTING_H
