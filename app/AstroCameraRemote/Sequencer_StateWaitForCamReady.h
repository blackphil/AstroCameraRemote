#ifndef SEQUENCER_STATEWAITFORCAMREADY_H
#define SEQUENCER_STATEWAITFORCAMREADY_H

#include "Sequencer_StateBase.h"
#include "Json_Command.h"
#include "Sender.h"



namespace Sequencer {

class StateWaitForCamReady: public StateBase
{
    Q_OBJECT

    Json::AwaitTakePicture awaitTakePicture;
    int i, numShots;


public :
    StateWaitForCamReady(int i, int numShots);

protected :
    void onExit(QEvent *event);

private Q_SLOTS:
    void updatePostViewInfo(QString url);

Q_SIGNALS :
    void havePostViewUrl(QString, int, int);

};
} // namespace Sequencer


#endif // SEQUENCER_STATEWAITFORCAMREADY_H
