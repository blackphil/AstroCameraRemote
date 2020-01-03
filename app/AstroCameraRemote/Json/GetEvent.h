#ifndef JSON_GETEVENT_H
#define JSON_GETEVENT_H

#include "Command.h"



namespace Json {

class GetEvent : public Command
{
    QJsonArray status;
    bool callbackImmedialetyEnabled;

    Q_OBJECT

    void handleReply(const QJsonDocument &replyJson) override;

public :
    GetEvent(QObject* parent = nullptr);
    QJsonArray getStatus() const;

    void setCallbackImmedialetyEnabled(bool yes);
    bool isCallbackImmedialetyEnabled() const;
};


} // namespace Json


#endif // JSON_GETEVENT_H
