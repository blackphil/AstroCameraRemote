#include "GetEvent.h"

#include <QJsonDocument>


namespace Json {

QJsonArray GetEvent::getStatus() const
{
    return status;
}

void GetEvent::setCallbackImmedialetyEnabled(bool yes)
{

    json.insert("params", QJsonArray { !yes });
}

bool GetEvent::isCallbackImmedialetyEnabled() const
{
    return !json.value("params").toBool();
}

GetEvent::GetEvent(QObject* parent)
    : Command(parent)
    , callbackImmedialetyEnabled(true)
{
    setObjectName("GetEvent");

    json =
    {
        { "method", "getEvent" },
        { "id", 1 },
        { "version", "1.2" }
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

