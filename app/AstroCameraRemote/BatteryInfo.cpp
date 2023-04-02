#include "BatteryInfo.h"



QString BatteryInfo::styleNormal   { "QProgressBar::chunk {background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 255, 0, 255), stop:1 rgba(0, 127, 0, 255))};"                         };
QString BatteryInfo::styleLow      { "QProgressBar::chunk {background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 0, 255), stop:1 rgba(127, 127, 0, 255))};"                     };
QString BatteryInfo::styleCritical { "QProgressBar {border-bottom-left-radius: 7px;background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:1 rgba(127, 0, 0, 255))};" };

QString BatteryInfo::getId() const
{
    return id;
}

BatteryInfo::Status BatteryInfo::getStatus() const
{
    return status;
}

BatteryInfo::AdditionalStatus BatteryInfo::getAdditionalStatus() const
{
    return additionalStatus;
}

int BatteryInfo::getLevelNumber() const
{
    return levelNumber;
}

int BatteryInfo::getLevelDenom() const
{
    return levelDenom;
}

QString BatteryInfo::getDescription() const
{
    return description;
}

BatteryInfo::BatteryInfo()
    : status(Unknown)
    , additionalStatus(AdditionalStatus::Nothing)
    , levelNumber(0)
    , levelDenom(0)
{

}

BatteryInfo::BatteryInfo(QJsonObject obj)
    : id(obj["batteryID"].toString())
    , status { Status::Unknown }
    , additionalStatus { AdditionalStatus::Nothing }
    , levelNumber{obj["levelNumer"].toInt()}
    , levelDenom{obj["levelDenom"].toInt()}
    , description{obj["description"].toString()}
{
    QString statusStr { obj["status"].toString() };
    if(statusStr == "active")
        status = Status::Active;
    else if(statusStr ==  "inactive")
        status = Status::Inactive;
    else
        status = Status::Unknown;

    QString additionalStatusStr { obj["additionalStatus"].toString() };
    if(additionalStatusStr == "batteryNearEnd")
        additionalStatus = AdditionalStatus::BatteryNearEnd;
    else if(additionalStatusStr == "charging")
        additionalStatus = AdditionalStatus::Charging;
    else
        additionalStatus = AdditionalStatus::Nothing;


}


