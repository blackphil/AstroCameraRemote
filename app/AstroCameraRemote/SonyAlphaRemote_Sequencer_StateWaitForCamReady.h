#ifndef SEQUENCER_STATEWAITFORCAMREADY_H
#define SEQUENCER_STATEWAITFORCAMREADY_H

#include "SonyAlphaRemote_Sequencer_StateBase.h"
#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_Sender.h"



namespace Sequencer {

class StateWaitForCamReady: public StateBase
{
    Q_OBJECT

    Json::AwaitTakePicture awaitTakePicture;
    Sender* sender;
    int i, numShots;


public :
    StateWaitForCamReady(Sender *sender, int i, int numShots);

protected :
    void onExit(QEvent *event);

private Q_SLOTS:
    void updatePostViewInfo(QString url);

Q_SIGNALS :
    void havePostViewUrl(QString, int, int);

};
} // namespace Sequencer


#endif // SEQUENCER_STATEWAITFORCAMREADY_H
