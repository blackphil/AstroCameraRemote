#ifndef JSON_GETEVENT_H
#define JSON_GETEVENT_H

#include "SonyAlphaRemote_Json_Command.h"



namespace Json {

class GetEvent : public Command
{
    QJsonArray status;
    bool callbackImmedialetyEnabled;

    Q_OBJECT

    QJsonObject getBase() const;
    void handleReply(const QJsonDocument &replyJson);

public :
    GetEvent(QObject* parent = nullptr);
    QJsonDocument getJson() const;
    QJsonArray getStatus() const;

    void setCallbackImmedialetyEnabled(bool yes);
    bool isCallbackImmedialetyEnabled() const;
};


} // namespace Json


#endif // JSON_GETEVENT_H
