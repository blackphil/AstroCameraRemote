#ifndef SONYALPHAREMOTE_SEQUENCER_STATEBULBSHOOTING_H
#define SONYALPHAREMOTE_SEQUENCER_STATEBULBSHOOTING_H

#include "SonyAlphaRemote_Sequencer_StateBase.h"
#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_Sender.h"


namespace SonyAlphaRemote {
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
Q_SIGNALS :
    void message(QString);
};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SEQUENCER_STATEBULBSHOOTING_H
