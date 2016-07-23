#include "SonyAlphaRemote_Sequencer_StateBase.h"

#include "SonyAlphaRemote_Helper.h"

namespace SonyAlphaRemote {
namespace Sequencer {

StateBase::StateBase(const QString& entryMessage, const QString& exitMessage, QTimer* t)
    :entryMessage(entryMessage)
    , exitMessage(exitMessage)
    , t(t)
{

}

StateBase::~StateBase()
{

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
    }
}

void StateBase::onExit(QEvent*)
{
    if(!exitMessage.isEmpty())
    {
        SAR_INF(exitMessage);
        Q_EMIT message(exitMessage);
    }
}

} // namespace Sequencer
} // namespace SonyAlphaRemote
