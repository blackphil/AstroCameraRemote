#ifndef SONYALPHAREMOTE_BATTERYINFO_H
#define SONYALPHAREMOTE_BATTERYINFO_H

#include <QString>
#include <QJsonObject>

namespace SonyAlphaRemote {

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
    static QString styleDanger;


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
    static QString getStyleDanger() { return styleDanger; }
};

} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_BATTERYINFO_H
