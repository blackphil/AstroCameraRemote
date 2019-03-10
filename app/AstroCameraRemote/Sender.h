#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>

#include "Json_Command.h"

class Sender : public QObject
{
    Q_OBJECT

    static Sender* instance;

    QNetworkAccessManager* manager;
    QJsonObject noReplyError;
    uint replyDelay;
    QTimer* timeoutTimer;
    QNetworkReply* postViewImageReply;

    QJsonDocument handleReply(QNetworkReply* reply) const;

    explicit Sender(QObject* parent = nullptr);
public:

    ~Sender();

    static Sender* create(QObject* parent);
    static Sender* get();

Q_SIGNALS :
    void replyReceived(QJsonDocument reply);
    void replyReceivedQml(QByteArray reply);
    void replyTimeout();
    void loadedPostViewImage(QByteArray);

private Q_SLOTS :
    void finished(QNetworkReply* reply);
    QNetworkReply *privateSend(const QByteArray& buffer);
    void handlePostViewImageReply();

public Q_SLOTS :
    void send(Json::Command *cmd);
    void loadPostViewImage(QString urlStr);


};


#endif // SENDER_H
