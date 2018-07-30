#ifndef SONYALPHAREMOTE_JSON_GETEVENT_H
#define SONYALPHAREMOTE_JSON_GETEVENT_H

#include "SonyAlphaRemote_Json_Command.h"


namespace SonyAlphaRemote {
namespace Json {

class GetEvent : public Command
{
    QJsonArray status;
    bool callbackImmedialetyEnabled;

    Q_OBJECT

    QJsonObject getBase() const;
    void handleReply(const QJsonDocument &replyJson);

public :
    GetEvent(QObject* parent = 0);
    QJsonDocument getJson() const;
    QJsonArray getStatus() const;

    void setCallbackImmedialetyEnabled(bool yes);
    bool isCallbackImmedialetyEnabled() const;
};


} // namespace Json
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_JSON_GETEVENT_H
