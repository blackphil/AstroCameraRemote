#ifndef BATTERYINFO_H
#define BATTERYINFO_H

#include <QString>
#include <QJsonObject>

class BatteryInfo
{

public:
    enum Status
    {
        Status_active
        , Status_inactive
        , Status_unknown
    };
    enum AdditionalStatus
    {
        AdditionalStatus_batteryNearEnd
        , AdditionalStatus_charging
        , AdditionalStatus_Nothing
    };

private :

    QString id;
    Status status;
    AdditionalStatus additionalStatus;

    int levelNumber;
    int levelDenom;
    QString description;

    static QString styleNormal;
    static QString styleLow;
    static QString styleCritical;


public:
    BatteryInfo();
    BatteryInfo(QJsonObject obj);
    QString getId() const;
    Status getStatus() const;
    AdditionalStatus getAdditionalStatus() const;
    int getLevelNumber() const;
    int getLevelDenom() const;
    QString getDescription() const;

    static QString getStyleNormal() { return styleNormal; }
    static QString getStyleLow() { return styleLow; }
    static QString getStyleCritical() { return styleCritical; }
};


#endif // BATTERYINFO_H
