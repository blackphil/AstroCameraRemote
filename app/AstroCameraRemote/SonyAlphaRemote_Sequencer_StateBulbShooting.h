#ifndef SEQUENCER_STATEBULBSHOOTING_H
#define SEQUENCER_STATEBULBSHOOTING_H

#include "SonyAlphaRemote_Sequencer_StateBase.h"
#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_Sender.h"


namespace Sequencer {

class StateBulbShooting : public StateBase
{
    Q_OBJECT

    Json::StartBulbShooting startBulbShooting;
    Json::StopBulbShooting stopBulbShooting;

    QPointer<Sender> sender;


public :
    StateBulbShooting(Sender* sender, QTimer* t, quint32 i, quint32 maxCount);

protected :
    void onEntry(QEvent* event);
    void onExit(QEvent* event);

};

} // namespace Sequencer

#endif // SEQUENCER_STATEBULBSHOOTING_H
