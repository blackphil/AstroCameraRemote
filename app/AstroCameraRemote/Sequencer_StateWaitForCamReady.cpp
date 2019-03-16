#include "Sequencer_StateWaitForCamReady.h"

#include "AstroBase.h"

namespace Sequencer {

StateWaitForCamReady::StateWaitForCamReady(int i, int numShots)
    : StateBase(tr("wait for camera IDLE mode (%0/%1)").arg(i+1).arg(numShots), "", nullptr)
    , i(i)
    , numShots(numShots)

{
    setObjectName("StateWaitForCamReady");
    connect(&awaitTakePicture, SIGNAL(havePostViewUrl(QString)), this, SLOT(updatePostViewInfo(QString)));
}

void StateWaitForCamReady::onExit(QEvent *event)
{
    Q_UNUSED(event);
    Sender::get()->send(&awaitTakePicture);
    StateBase::onExit(event);
}

void StateWaitForCamReady::updatePostViewInfo(QString url)
{
    Q_EMIT havePostViewUrl(url, i, numShots);
}

} // namespace Sequencer

