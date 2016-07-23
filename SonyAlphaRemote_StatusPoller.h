#ifndef SONYALPHAREMOTE_STATUSPOLLER_H
#define SONYALPHAREMOTE_STATUSPOLLER_H

#include <QObject>
#include <QTimer>

#include "SonyAlphaRemote_Sender.h"
#include "SonyAlphaRemote_Json_Command.h"

namespace SonyAlphaRemote {

class StatusPoller : public QObject
{
    Q_OBJECT

    Sender* sender;
    QTimer* trigger;
    QString cameraStatus;
    Json::GetEvent* getEvent;
    Json::AwaitTakePicture* awaitTakePicture;

    bool pollPostViewPictureUrlEnabled;
    bool waitingForEventReply;

public:
    explicit StatusPoller(Sender* sender, QObject *parent = 0);

    void start(double interval);
    void stop();
    bool isActive() const;



    QString getCameraStatus() const;

    bool getPollPostViewPictureUrlEnabled() const;
    void setPollPostViewPictureUrlEnabled(bool value);

    void simCamReady();

Q_SIGNALS:
    void statusChanged(QString);
    void haveNewPostViewPictureUrl(QString);

private Q_SLOTS :
    void poll();
    void handleEventReply();
    void handleEventError();
    void handleAwaitPicReply(const QString& potUrl);
    void handleAraitPicError();
};

} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_STATUSPOLLER_H
