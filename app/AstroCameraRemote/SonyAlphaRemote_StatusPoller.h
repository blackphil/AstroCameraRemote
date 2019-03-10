#ifndef STATUSPOLLER_H
#define STATUSPOLLER_H

#include <QObject>
#include <QTimer>

#include "SonyAlphaRemote_Sender.h"
#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_Json_GetEvent.h"
#include "SonyAlphaRemote_BatteryInfo.h"



class StatusPoller : public QObject
{
    Q_OBJECT

    static StatusPoller* instance;

    QTimer* trigger;
    QString cameraStatus;
    BatteryInfo batteryInfo;
    Json::GetEvent* getEvent;

    bool waitingForEventReply;

    bool getEnumeratedOption(int index, const QString& type, QJsonArray status, const QString& currentName, const QString& candidatesName, QString& current, QStringList& candidates) const;
    void getBatteryInfo(QJsonArray status);

    explicit StatusPoller(QObject *parent = nullptr);
public:

    ~StatusPoller();

    void start(double interval);
    void stop();
    bool isActive() const;

    static StatusPoller* create(QObject* parent);
    static StatusPoller* get();



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



#endif // STATUSPOLLER_H
