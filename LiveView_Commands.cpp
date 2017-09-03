#include "LiveView_Commands.h"

#include "SonyAlphaRemote_Helper.h"

namespace LiveView {



StartLiveView::StartLiveView(QObject* parent)
    : SonyAlphaRemote::Json::Command(parent)
{
    setObjectName("StartLiveView");
}

QJsonDocument StartLiveView::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "startLiveview";
    command["params"] = QJsonArray();

    return QJsonDocument(command);
}

void StartLiveView::handleReply(const QJsonDocument& replyJson)
{
    QJsonObject obj = replyJson.object();
    if(obj.isEmpty() || !obj.contains("result") || !obj["result"].isArray())
    {
        SAR_ERR("startLiveview: invalid reply!");
        return;
    }

    QJsonArray result = obj["result"].toArray();
    if(1 > result.count())
    {
        SAR_ERR("startLiveview: invalid reply!");
        return;
    }

    QString url = result[0].toString();
    Q_EMIT newLiveViewUrl(url);
}





StopLiveView::StopLiveView(QObject* parent)
    : Command(parent)
{
    setObjectName("StopLiveView");
}

QJsonDocument StopLiveView::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "stopLiveview";
    command["params"] = QJsonArray();

    return QJsonDocument(command);
}

void StopLiveView::handleReply(const QJsonDocument &replyJson)
{
    SonyAlphaRemote::Json::Command::handleReply(replyJson);
    Q_EMIT liveViewStopped();
}


} // namespace LiveView

