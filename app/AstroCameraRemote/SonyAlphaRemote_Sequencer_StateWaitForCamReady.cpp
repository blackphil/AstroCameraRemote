#include "SonyAlphaRemote_Sequencer_StateWaitForCamReady.h"


namespace Sequencer {

StateWaitForCamReady::StateWaitForCamReady(Sender* sender, int i, int numShots)
    : StateBase(tr("wait for camera IDLE mode (%0/%1)").arg(i).arg(numShots), "", NULL)
    , sender(sender)
    , i(i)
    , numShots(numShots)

{
    setObjectName("StateWaitForCamReady");
    connect(&awaitTakePicture, SIGNAL(havePostViewUrl(QString)), this, SLOT(updatePostViewInfo(QString)));
}


void StateWaitForCamReady::onExit(QEvent *event)
{
    Q_UNUSED(event);
    sender->send(&awaitTakePicture);
}

void StateWaitForCamReady::updatePostViewInfo(QString url)
{
    Q_EMIT havePostViewUrl(url, i, numShots);
}

} // namespace Sequencer

