#ifndef MESSAGEPOSTER_H
#define MESSAGEPOSTER_H

#include <QObject>
#include <QString>

class MessageHandler;
class MessagePoster;

class MessageManager : public QObject
{
    Q_OBJECT

    static MessageManager mgr;
    ~MessageManager() {}
    MessageManager(const MessageManager&) {}
    MessageManager() {}

    QList<MessageHandler*> handlers;
    QList<MessagePoster*> posters;

public Q_SLOTS :
    void error(const QString& err);
    void warning(const QString& wrn);
    void info(const QString& msg);

public :

    static MessageManager* get();

    static bool registerHandler(MessageHandler* handler);
    static bool registerPoster(MessagePoster* poster);

    static bool unregisterHandler(MessageHandler* handler);
    static bool unregisterPoster(MessagePoster* poster);
};


class MessageHandler
{

public :
    explicit MessageHandler();
    virtual ~MessageHandler();

public :
    virtual void error(const QString&) = 0;
    virtual void warning(const QString&) = 0;
    virtual void info(const QString&) = 0;
};

class MessagePoster : public QObject
{
    Q_OBJECT

Q_SIGNALS :
    void error(QString);
    void warning(QString);
    void info(QString);

public:
    explicit MessagePoster(QObject* parent = nullptr);
    ~MessagePoster();
};


#endif // MESSAGEPOSTER_H
