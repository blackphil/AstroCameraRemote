#include "MessagePoster.h"


MessageManager MessageManager::mgr{};

MessageManager* MessageManager::get()
{
    return &mgr;
}

void MessageManager::error(const QString& err)
{    
    for(MessageHandler* handler : handlers)
        handler->error(err);
}

void MessageManager::warning(const QString& wrn)
{
    for(MessageHandler* handler : handlers)
        handler->warning(wrn);
}

void MessageManager::info(const QString& msg)
{
    for(MessageHandler* handler : handlers)
        handler->info(msg);
}

bool MessageManager::registerHandler(MessageHandler* handler)
{
    if(!mgr.handlers.contains(handler))
    {
        mgr.handlers << handler;
        return true;
    }
    else {
        return false;
    }
}

bool MessageManager::registerPoster(MessagePoster* poster)
{
    if(!mgr.posters.contains(poster))
    {
        mgr.connect(poster, SIGNAL(error(QString)), &mgr, SLOT(error(QString)));
        mgr.connect(poster, SIGNAL(warning(QString)), &mgr, SLOT(warning(QString)));
        mgr.connect(poster, SIGNAL(info(QString)), &mgr, SLOT(info(QString)));
        mgr.posters << poster;
        return true;
    }
    else {
        return false;
    }
}

bool MessageManager::unregisterHandler(MessageHandler* handler)
{
    if(mgr.handlers.contains(handler))
    {
        mgr.handlers.removeAll(handler);
        return true;
    }
    else {
        return false;
    }
}

bool MessageManager::unregisterPoster(MessagePoster* poster)
{
    if(mgr.posters.contains(poster))
    {
        mgr.disconnect(poster, nullptr, &mgr, nullptr);
        mgr.posters.removeAll(poster);
        return true;
    }
    else {
        return false;
    }
}

MessageHandler::MessageHandler()
{
    MessageManager::get()->registerHandler(this);
}

MessageHandler::~MessageHandler()
{
    MessageManager::get()->unregisterHandler(this);
}


MessagePoster::MessagePoster(QObject *parent)
    : QObject (parent)
{
    MessageManager::get()->registerPoster(this);
}

MessagePoster::~MessagePoster()
{
    MessageManager::get()->unregisterPoster(this);
}
