#include "SonyAlphaRemote_Json_GetEvent.h"

#include <QJsonDocument>


namespace Json {

QJsonArray GetEvent::getStatus() const
{
    return status;
}

void GetEvent::setCallbackImmedialetyEnabled(bool yes)
{
    callbackImmedialetyEnabled = yes;
}

bool GetEvent::isCallbackImmedialetyEnabled() const
{
    return callbackImmedialetyEnabled;
}

GetEvent::GetEvent(QObject* parent)
    : Command(parent)
    , callbackImmedialetyEnabled(true)
{
    setObjectName("GetEvent");
}

QJsonDocument GetEvent::getJson() const
{
    QJsonDocument json(getBase());
//    AB_INF(QString(json.toJson()));
    return json;

}

QJsonObject GetEvent::getBase() const
{
    return QJsonObject
    {
        { "method", "getEvent" },
        { "id", 1 },
        { "version", "1.2" },
        { "params", QJsonArray { !callbackImmedialetyEnabled } }
    };


}

void GetEvent::handleReply(const QJsonDocument &replyJson)
{
    if(replyJson.isObject())
    {
        if(replyJson.object().contains("result"))
        {
            status = replyJson.object()["result"].toArray();
            Q_EMIT confirmed();
            return;
        }

        if(replyJson.object().contains("error"))
        {
            if(2 == replyJson.object()["error"].toArray()[0].toInt())
            {
                Q_EMIT declined();
            }
        }
    }
    handleError(replyJson, tr("invalid reply"));
}

} // namespace Json

