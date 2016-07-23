#ifndef SONYALPHAREMOTE_SEQUENCER_STATEWAITFORCAMREADY_H
#define SONYALPHAREMOTE_SEQUENCER_STATEWAITFORCAMREADY_H

#include "SonyAlphaRemote_Sequencer_StateBase.h"
#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_Sender.h"


namespace SonyAlphaRemote {
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
    void message(QString);
    void havePostViewUrl(QString, int, int);

};
} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SEQUENCER_STATEWAITFORCAMREADY_H
