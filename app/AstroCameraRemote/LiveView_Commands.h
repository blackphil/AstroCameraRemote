#ifndef SONYALPHAREMOTE_LIVEVIEW_COMMAND_H
#define SONYALPHAREMOTE_LIVEVIEW_COMMAND_H

#include <SonyAlphaRemote_Json_Command.h>


namespace LiveView {

class StartLiveView : public SonyAlphaRemote::Json::Command
{
    Q_OBJECT

    void handleReply(const QJsonDocument& replyJson);
Q_SIGNALS:
    void newLiveViewUrl(QString);

public :
    StartLiveView(QObject* parent = nullptr);
    QJsonDocument getJson() const;
};




class StopLiveView : public SonyAlphaRemote::Json::Command
{
    Q_OBJECT

    void handleReply(const QJsonDocument& replyJson);

Q_SIGNALS:
    void liveViewStopped();

public :
    StopLiveView(QObject* parent = nullptr);
    QJsonDocument getJson() const;
};



} // namespace LiveView

#endif // SONYALPHAREMOTE_LIVEVIEW_COMMAND_H
