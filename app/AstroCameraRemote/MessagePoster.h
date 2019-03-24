#ifndef MESSAGEPOSTER_H
#define MESSAGEPOSTER_H

#include <QString>


class MessageHandler
{

public :
    explicit MessageHandler() {}
    virtual ~MessageHandler() {}

public :
    virtual void infoMessage(QString) = 0;
    virtual void warningMessage(QString) = 0;
    virtual void errorMessage(QString) = 0;
};

class MessagePoster
{
    MessageHandler* msgHandler;


public:
    explicit MessagePoster();
    virtual ~MessagePoster();

    void infoMessage(QString msg);
    void warningMessage(QString msg);
    void errorMessage(QString);

    virtual void setMsgHandler(MessageHandler *value);
    MessageHandler* getMessageHandler() const;
};


#endif // MESSAGEPOSTER_H
