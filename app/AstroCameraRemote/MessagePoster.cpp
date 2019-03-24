#include "MessagePoster.h"




MessagePoster::MessagePoster()
    : msgHandler(nullptr)
{
}

MessagePoster::~MessagePoster()
{

}

void MessagePoster::setMsgHandler(MessageHandler *value)
{
    msgHandler = value;
}

MessageHandler *MessagePoster::getMessageHandler() const
{
    return msgHandler;
}

void MessagePoster::infoMessage(QString msg)
{
    if(msgHandler)
        msgHandler->infoMessage(msg);
}

void MessagePoster::warningMessage(QString msg)
{
    if(msgHandler)
        msgHandler->warningMessage(msg);
}

void MessagePoster::errorMessage(QString msg)
{
    if(msgHandler)
        msgHandler->errorMessage(msg);
}
