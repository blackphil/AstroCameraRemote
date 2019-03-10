#ifndef SEQUENCER_STATEBULBSHOOTING_H
#define SEQUENCER_STATEBULBSHOOTING_H

#include "Sequencer_StateBase.h"
#include "Json_Command.h"
#include "Sender.h"


namespace Sequencer {

class StateBulbShooting : public StateBase
{
    Q_OBJECT

    Json::StartBulbShooting startBulbShooting;
    Json::StopBulbShooting stopBulbShooting;

public :
    StateBulbShooting(QTimer* t, quint32 i, quint32 maxCount);

protected :
    void onEntry(QEvent* event);
    void onExit(QEvent* event);

};

} // namespace Sequencer

#endif // SEQUENCER_STATEBULBSHOOTING_H
