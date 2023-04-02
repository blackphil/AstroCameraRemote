#ifndef BATTERYINFO_H
#define BATTERYINFO_H

#include <QString>
#include <QJsonObject>

class BatteryInfo
{

public:
    enum Status
    {
        Active
        , Inactive
        , Unknown
    };
    enum class AdditionalStatus
    {
        BatteryNearEnd
        , Charging
        , Nothing
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
