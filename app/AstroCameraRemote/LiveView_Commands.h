#ifndef LIVEVIEW_COMMAND_H
#define LIVEVIEW_COMMAND_H

#include "Json_Command.h"


namespace LiveView {

class StartLiveView : public Json::Command
{
    Q_OBJECT

    void handleReply(const QJsonDocument& replyJson) override;
Q_SIGNALS:
    void newLiveViewUrl(QString);

public :
    StartLiveView(QObject* parent = nullptr);

};




class StopLiveView : public Json::Command
{
    Q_OBJECT

    void handleReply(const QJsonDocument& replyJson) override;

Q_SIGNALS:
    void liveViewStopped();

public :
    StopLiveView(QObject* parent = nullptr);

};



} // namespace LiveView

#endif // LIVEVIEW_COMMAND_H
