#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>

#include "Json/Command.h"

class Sender : public QObject
{
    Q_OBJECT

    static Sender* instance;

    QString host;
    int port;

    QNetworkAccessManager* manager;
    QJsonObject noReplyError;
    int replyDelay;
    QTimer* timeoutTimer;
    QNetworkReply* postViewImageReply;

    QJsonDocument handleReply(QNetworkReply* reply) const;

    explicit Sender(QObject* parent = nullptr);

    QUrl createUrl(const QString& subPath) const;
public:

    ~Sender();

    static Sender* create(QObject* parent);
    static Sender* get();

    QString getHost() const;
    void setHost(const QString &value);

    int getPort() const;
    void setPort(int value);

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
