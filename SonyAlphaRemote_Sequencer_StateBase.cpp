#include "SonyAlphaRemote_Sequencer_StateBase.h"

#include "SonyAlphaRemote_Helper.h"

#include <QFile>

#define MSG_MAX_LEN 40

namespace SonyAlphaRemote {
namespace Sequencer {


StateBase::StateBase(const QString& entryMessage, const QString& exitMessage, QTimer* t)
    :entryMessage(entryMessage)
    , exitMessage(exitMessage)
    , t(t)
    , countDownTimer(NULL != t ? new QTimer(this) : NULL)
    , countDown(NULL != t ? t->interval() / 1000 : 0)
{

    QFile countdownMsgFile(":/sequencer/Countdown_Msg.html");
    countdownMsgFile.open(QIODevice::ReadOnly | QIODevice::Text);
    countdownMsg = countdownMsgFile.readAll();

    QFile normalMsgFile(":/sequencer/Normal_Msg.html");
    normalMsgFile.open(QIODevice::ReadOnly | QIODevice::Text);
    normalMsg = normalMsgFile.readAll();

    if(countDownTimer)
        connect(countDownTimer, SIGNAL(timeout()), this, SLOT(triggerCountDown()));
}

StateBase::~StateBase()
{
    SAR_INF("[DTOR] state: " << objectName());
}


void StateBase::onEntry(QEvent*)
{
    if(!entryMessage.isEmpty())
    {
        SAR_INF(entryMessage);
        Q_EMIT message(entryMessage);
    }

    if(t)
    {
        t->start();
        countDownTimer->start(1000);
    }
    else
        Q_EMIT updateStatus(normalMsg.arg(entryMessage.left(MSG_MAX_LEN)).arg(exitMessage.left(MSG_MAX_LEN)));
}

void StateBase::onExit(QEvent*)
{
    if(!exitMessage.isEmpty())
    {
        SAR_INF(exitMessage);
        Q_EMIT message(exitMessage);
    }

    if(countDownTimer)
        countDownTimer->stop();
    else
        Q_EMIT updateStatus(normalMsg.arg(entryMessage.left(MSG_MAX_LEN)).arg(exitMessage.left(MSG_MAX_LEN)));
}

void StateBase::triggerCountDown()
{
    countDown--;
    QString msg = countdownMsg
            .arg(entryMessage.left(MSG_MAX_LEN))
            .arg(exitMessage.left(MSG_MAX_LEN))
            .arg(countDown);

    Q_EMIT updateStatus(msg);
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
