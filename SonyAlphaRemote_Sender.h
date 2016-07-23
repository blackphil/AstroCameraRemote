#ifndef SONYALPHAREMOTE_SENDER_H
#define SONYALPHAREMOTE_SENDER_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>

#include "SonyAlphaRemote_Json_Command.h"

namespace SonyAlphaRemote
{

class Sender : public QObject
{
    Q_OBJECT

    QNetworkAccessManager* manager;
    QJsonObject noReplyError;
    uint replyDelay;
    QTimer* timeoutTimer;
    QNetworkReply* postViewImageReply;

    QJsonDocument handleReply(QNetworkReply* reply) const;

public:
    explicit Sender(QObject* parent = 0);

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

} //namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SENDER_H
