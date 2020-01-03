#include "Commands.h"

#include "AstroBase.h"

namespace LiveView {



StartLiveView::StartLiveView(QObject* parent)
    : Json::Command { parent }
{
    setObjectName("StartLiveView");
    json["method"] = "startLiveview";
    json["params"] = QJsonArray{};
}


void StartLiveView::handleReply(const QJsonDocument& replyJson)
{
    QJsonObject obj = replyJson.object();
    if(obj.isEmpty() || !obj.contains("result") || !obj["result"].isArray())
    {
        AB_ERR("startLiveview: invalid reply!");
        return;
    }

    QJsonArray result = obj["result"].toArray();
    if(1 > result.count())
    {
        AB_ERR("startLiveview: invalid reply!");
        return;
    }

    QString url = result[0].toString();
    Q_EMIT newLiveViewUrl(url);
}





StopLiveView::StopLiveView(QObject* parent)
    : Command(parent)
{
    setObjectName("StopLiveView");
    json["method"] = "stopLiveview";
    json["params"] = QJsonArray {};
}

void StopLiveView::handleReply(const QJsonDocument &replyJson)
{
    Json::Command::handleReply(replyJson);
    Q_EMIT liveViewStopped();
}


} // namespace LiveView

