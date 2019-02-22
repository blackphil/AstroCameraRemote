#include "SonyAlphaRemote_Sender.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QTimer>
#include "AstroBase.h"

#include "SonyAlphaRemote_Helper.h"

namespace SonyAlphaRemote
{

QJsonDocument Sender::handleReply(QNetworkReply *reply) const
{
    Q_ASSERT(reply);
    if(!reply)
    {
        return QJsonDocument(noReplyError);
    }

    return QJsonDocument::fromJson(reply->readAll());
}

Sender::Sender(QObject* parent)
    : QObject(parent)
    , manager(new QNetworkAccessManager(this))
    , replyDelay(2000)
    , timeoutTimer(NULL)
    , postViewImageReply(NULL)
{
//    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    noReplyError = QJsonObject
    {
        { "id", 1 },
        { "version", "1.0" },
        { "params", "no reply" }
    };

}

void Sender::finished(QNetworkReply *reply)
{
    if(timeoutTimer)
    {
        timeoutTimer->stop();
        delete timeoutTimer;
        timeoutTimer = NULL;
    }

    QJsonDocument json = handleReply(reply);
    Q_EMIT replyReceived(json);
    Q_EMIT replyReceivedQml(json.toJson());
}

QNetworkReply* Sender::privateSend(const QByteArray &buffer)
{
    QNetworkRequest request(QUrl("http://192.168.122.1:8080/sony/camera"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json-rpc"));
    int length = buffer.size();
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(length));

    QNetworkReply* reply = manager->post(request, buffer);

    timeoutTimer = new QTimer(this);
    connect(timeoutTimer, SIGNAL(timeout()), this, SIGNAL(replyTimeout()));
    timeoutTimer->start(replyDelay);

    return reply;

}

void Sender::handlePostViewImageReply()
{
    QByteArray buffer = postViewImageReply->readAll();
    Q_EMIT loadedPostViewImage(buffer);
}

void Sender::loadPostViewImage(QString urlStr)
{
    QUrl url(urlStr);
    QNetworkRequest postViewImageRequest;//(QUrl(url));
    postViewImageRequest.setUrl(url);
    postViewImageRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    postViewImageReply =  manager->get(postViewImageRequest);
    connect(postViewImageReply, SIGNAL(finished()), this, SLOT(handlePostViewImageReply()));

}


void Sender::send(Json::Command* cmd)
{
    QNetworkReply* reply = privateSend(cmd->getJson().toJson());
    if(reply)
        cmd->setReply(reply);
    else
        AB_ERR("no reply");
}

} //namespace SonyAlphaRemote
