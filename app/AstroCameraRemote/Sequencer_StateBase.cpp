#include "Sequencer_StateBase.h"

#include "AstroBase.h"

#include <QFile>
#include <QStateMachine>

#define MSG_MAX_LEN 40

namespace Sequencer {


StateBase::StateBase(const QString& entryMessage, const QString& exitMessage, QTimer* t)
    : entryMessage(entryMessage)
    , exitMessage(exitMessage)
    , t(t)
    , countDownTimer(nullptr != t ? new QTimer(this) : nullptr)
    , countDown(nullptr != t ? t->interval() / 1000 : 0)
{

    QFile countdownMsgFile(":/sequencer/Countdown_Msg.html");
    countdownMsgFile.open(QIODevice::ReadOnly | QIODevice::Text);
    countdownMsg = countdownMsgFile.readAll();

    QFile normalMsgFile(":/sequencer/Normal_Msg.html");
    normalMsgFile.open(QIODevice::ReadOnly | QIODevice::Text);
    normalMsg = normalMsgFile.readAll();

    if(countDownTimer)
    {
        connect(countDownTimer, SIGNAL(timeout()), this, SLOT(triggerCountDown()));
    }
}

StateBase::~StateBase()
{
    AB_INF("[DTOR] state: " << objectName());
}


void StateBase::onEntry(QEvent*)
{   
    if(!entryMessage.isEmpty())
    {
        AB_INF(entryMessage);
        Q_EMIT message(entryMessage);
    }

    if(t)
    {
        t->start();
        if(countDownTimer)
        {
            connect(machine(), SIGNAL(stopped()), countDownTimer, SLOT(stop()));
            countDownTimer->start(1000);
        }
    }
    else
        Q_EMIT updateStatus(normalMsg.arg(entryMessage.left(MSG_MAX_LEN)).arg(exitMessage.left(MSG_MAX_LEN)));
}

void StateBase::onExit(QEvent*)
{
    if(!exitMessage.isEmpty())
    {
        AB_INF(exitMessage);
        Q_EMIT message(exitMessage);
    }

    if(countDownTimer)
    {
        countDownTimer->stop();
        disconnect(machine(), SIGNAL(stopped()), countDownTimer, SLOT(stop()));
    }
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
