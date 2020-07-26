#include "Sender.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QTimer>
#include "AstroBase.h"

#include "Helper.h"


Sender* Sender::instance(nullptr);

Sender* Sender::create(QObject *parent)
{
    Q_ASSERT(!instance);
    if(instance)
       delete instance;

    instance = new Sender(parent);
    return instance;
}

Sender* Sender::get()
{
    return instance;
}

QString Sender::getHost() const
{
    return host;
}

void Sender::setHost(const QString &value)
{
    host = value;
}

int Sender::getPort() const
{
    return port;
}

void Sender::setPort(int value)
{
    port = value;
}

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
    , timeoutTimer(nullptr)
{
//    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    noReplyError = QJsonObject
    {
        { "id", 1 },
        { "version", "1.0" },
        { "params", "no reply" }
    };

}

QUrl Sender::createUrl(const QString &subPath) const
{
    return QString("http://%0:%1%2").arg(host).arg(port).arg(subPath);
}

Sender::~Sender()
{
    instance = nullptr;
}

void Sender::finished(QNetworkReply *reply)
{
    if(timeoutTimer)
    {
        timeoutTimer->stop();
        delete timeoutTimer;
        timeoutTimer = nullptr;
    }

    QJsonDocument json = handleReply(reply);
    Q_EMIT replyReceived(json);
    Q_EMIT replyReceivedQml(json.toJson());
}

QNetworkReply* Sender::privateSend(const QByteArray &buffer)
{
    QNetworkRequest request(createUrl("/sony/camera"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json-rpc"));
    int length = buffer.size();
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(length));

    QNetworkReply* reply = manager->post(request, buffer);

    timeoutTimer = new QTimer(this);
    connect(timeoutTimer, SIGNAL(timeout()), this, SIGNAL(replyTimeout()));
    timeoutTimer->start(replyDelay);

    return reply;

}

void Sender::loadPostViewImage(QString urlStr)
{
    QUrl url(urlStr);
    QNetworkRequest postViewImageRequest;//(QUrl(url));
    postViewImageRequest.setUrl(url);
    postViewImageRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));

    auto reply = manager->get(postViewImageRequest);
    connect(reply, &QNetworkReply::finished, this, [this, reply](){
        reply->deleteLater();
        auto data = reply->readAll();
        Q_ASSERT(!data.isEmpty());
        if(!data.isEmpty())
            Q_EMIT loadedPostViewImage(data);
        else {
            AB_ERR("no data received after loading post view image");
        }
    });
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), [](auto code){
        AB_ERR("reply error during loading post view image:" << code);
    });


}


void Sender::send(Json::Command* cmd)
{
    QNetworkReply* reply = privateSend(cmd->getJson());
    if(reply)
        cmd->setReply(reply);
    else
        AB_ERR("no reply");
}

