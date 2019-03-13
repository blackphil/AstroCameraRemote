#ifndef LIVEVIEW_COMMAND_H
#define LIVEVIEW_COMMAND_H

#include <Json_Command.h>


namespace LiveView {

class StartLiveView : public Json::Command
{
    Q_OBJECT

    void handleReply(const QJsonDocument& replyJson);
Q_SIGNALS:
    void newLiveViewUrl(QString);

public :
    StartLiveView(QObject* parent = nullptr);
    QJsonDocument getJson() const;
};




class StopLiveView : public Json::Command
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

#endif // LIVEVIEW_COMMAND_H
