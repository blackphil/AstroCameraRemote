#ifndef SONYALPHAREMOTE_STATUSPOLLER_H
#define SONYALPHAREMOTE_STATUSPOLLER_H

#include <QObject>
#include <QTimer>

#include "SonyAlphaRemote_Sender.h"
#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_Json_GetEvent.h"
#include "SonyAlphaRemote_BatteryInfo.h"

namespace SonyAlphaRemote {

class   StatusPoller : public QObject
{
    Q_OBJECT

    Sender* sender;
    QTimer* trigger;
    QString cameraStatus;
    BatteryInfo batteryInfo;
    Json::GetEvent* getEvent;

    bool waitingForEventReply;

    bool getEnumeratedOption(int index, const QString& type, QJsonArray status, const QString& currentName, const QString& candidatesName, QString& current, QStringList& candidates) const;
    void getBatteryInfo(QJsonArray status);

public:
    explicit StatusPoller(Sender* sender, QObject *parent = 0);

    void start(double interval);
    void stop();
    bool isActive() const;



    QString getCameraStatus() const;
    BatteryInfo getBatteryInfo() const;


    void simCamReady();

Q_SIGNALS:
    void statusChanged(QString);
    void haveNewPostViewPictureUrl(QString);

    void isoSpeedRatesChanged(QStringList, QString);
    void shutterSpeedsChanged(QStringList, QString);
    void stillQualityChanged(QStringList, QString);

    void batteryStatusChanged();

private Q_SLOTS :
    void poll();
    void handleEventReply();
    void handleEventError();

};

} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_STATUSPOLLER_H
