#include "SonyAlphaRemote_Json_Command.h"

#include <QJsonObject>
#include <QJsonArray>
#include "SonyAlphaRemote_Helper.h"
#include "AstroBase.h"


namespace Json {

void Command::setReply(QNetworkReply *value)
{
    if(!value)
        AB_WRN("no reply object");
    reply = value;
    connect(reply, SIGNAL(finished()), this, SLOT(handleReply()));
}

QJsonObject Command::getBase() const
{
    return QJsonObject
    {
        { "id", 1 },
        { "version", "1.0" }
    };
}

void Command::handleReply(const QJsonDocument &replyJson)
{
    if(!replyJson.isObject() || !replyJson.object().contains("result") || !replyJson.object()["result"].isArray())
    {
        handleError(replyJson, tr("invalid reply"));
        return;
    }

    Q_EMIT confirmed();

}

void Command::handleError(const QJsonDocument &replyJson, const QString &msg)
{
    Q_EMIT error(tr("%0 (command: %1, json: %2)").arg(msg).arg(objectName()).arg(QString(replyJson.toJson())));
}

void Command::handleReply()
{
    if(!reply)
    {
        AB_ERR("(" << objectName() << ") reply is NULL");
        return;
    }
    QByteArray replyData = reply->readAll();

    reply->deleteLater();
    reply = NULL;

    AB_INF("(" << objectName() << ") reply: " << QString(replyData));
    QJsonDocument replyJson = QJsonDocument::fromJson(replyData);
    handleReply(replyJson);
}

void SetShutterSpeed::setShutterSpeed(const QString &value)
{
    shutterSpeed = value;
}

QString SetShutterSpeed::getShutterSpeed() const
{
    return shutterSpeed;
}

SetShutterSpeed::SetShutterSpeed(QObject* parent)
    : Command(parent)
    , shutterSpeed("")
{
    setObjectName("SetShutterSpeed");
}

QJsonDocument SetShutterSpeed::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "setShutterSpeed";
    command["params"] = QJsonArray
    {
        { QString("%0").arg(shutterSpeed) }
    };

    return QJsonDocument(command);
}

void SetIsoSpeedRate::setIsoSpeedRate(const QString &value)
{
    isoSpeedRate = value;
}

QString SetIsoSpeedRate::getIsoSpeedRate() const
{
    return isoSpeedRate;
}

SetIsoSpeedRate::SetIsoSpeedRate(QObject* parent)
    : Command(parent)
    , isoSpeedRate("")
{
    setObjectName("SetIsoSpeedRate");
}

QJsonDocument SetIsoSpeedRate::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "setIsoSpeedRate";
    command["params"] = QJsonArray
    {
        { QString("%0").arg(isoSpeedRate) }
    };

    return QJsonDocument(command);
}

GetShutterSpeed::GetShutterSpeed(QObject* parent)
    : Command(parent)
{
    setObjectName("GetShutterSpeed");
}

QJsonDocument GetShutterSpeed::getJson() const
{
    QJsonObject command = getBase();

    command["method"] = "getShutterSpeed";
    command["params"] = QJsonArray {};

    return QJsonDocument(command);
}

StartRecMode::StartRecMode(QObject* parent)
    : Command(parent)
{
    setObjectName("StartRecMode");
}

QJsonDocument StartRecMode::getJson() const
{
    QJsonObject command = getBase();

    command["method"] = "startRecMode";
    command["params"] = QJsonArray {};

    return QJsonDocument(command);
}

StopRecMode::StopRecMode(QObject* parent)
    : Command(parent)
{
    setObjectName("StopRecMode");
}

QJsonDocument StopRecMode::getJson() const
{
    QJsonObject command = getBase();

    command["method"] = "stopRecMode";
    command["params"] = QJsonArray {};

    return QJsonDocument(command);
}

ActTakePicture::ActTakePicture(QObject *parent)
    : PostViewProviderCommand(parent)
{
    setObjectName("ActTakePicture");
}

QJsonDocument ActTakePicture::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "actTakePicture";
    command["params"] = QJsonArray();

    return QJsonDocument(command);
}

SetContShootingMode::SetContShootingMode(QObject *parent)
    : Command(parent)
    , mode(InvalidMode)
{
    setObjectName("SetContShootingMode");
}

QJsonDocument SetContShootingMode::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "setContShootingMode";
    command["params"] = QJsonObject
    {
        { "contShootingMode", mode2String(mode) }
    };

    return QJsonDocument(command);
}

SetContShootingMode::Mode SetContShootingMode::getMode() const
{
    return mode;
}

void SetContShootingMode::setMode(const Mode &value)
{
    mode = value;
}

QString SetContShootingMode::mode2String(Mode mode)
{
    switch(mode)
    {
    case Single : return "Single";
    case Continuous : return "Continuous";
    case SpdPriorityCont : return "Spd Priority Cont.";
    case Burst : return "Burst";
    case MotionShot : return "MotionShot";
    case InvalidMode :
    default :
        break;


    }

    return "Invalid mode";
}

SetContShootingMode::Mode SetContShootingMode::string2Mode(const QString &mode)
{
    if(mode == "Single")
        return Single;
    else if(mode == "Continuous")
        return Continuous;
    else if(mode == "Spd Priority Cont.")
        return SpdPriorityCont;
    else if(mode == "Burst")
        return Burst;
    else if(mode == "MotionShot")
        return MotionShot;

    return InvalidMode;
}

QString GetAvailableContShootingModes::getCurrentContShootingMode() const
{
    return currentContShootingMode;
}

QStringList GetAvailableContShootingModes::getContShootingModes() const
{
    return contShootingModes;
}

GetAvailableContShootingModes::GetAvailableContShootingModes(QObject *parent)
    : Command(parent)
{
    setObjectName("GetAvailableContShootingModes");
}

QJsonDocument GetAvailableContShootingModes::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "getAvailableContShootingMode";
    command["params"] = QJsonArray {};

    return QJsonDocument(command);
}

void GetAvailableContShootingModes::handleReply(const QJsonDocument &replyJson)
{
    if(!replyJson.isObject() || !replyJson.object().contains("result") || !replyJson.object()["result"].isArray())
    {
        handleError(replyJson, tr("invalid reply"));
        return;
    }

    QJsonArray result = replyJson.object()["result"].toArray();
    if(result.count() != 2)
    {
        handleError(replyJson, tr("\"result\" is not an array"));
        return;
    }

    currentContShootingMode = result[0].toString();
    if(!result[1].isArray())
    {
        handleError(replyJson, tr("second entry of \"result\" (cont. shooting modes) is not an array"));
        return;
    }

    contShootingModes.clear();
    QJsonArray available = result[1].toArray();
    for(int i=0; i<available.count(); i++)
    {
        contShootingModes << available[i].toString();
    }

    if(!contShootingModes.contains(currentContShootingMode))
    {
        handleError(replyJson, tr("cont. shooting modes don't contain curent cont. shooting mode"));
        return;
    }

    Q_EMIT confirmed();

}

StartBulbShooting::StartBulbShooting(QObject* parent)
    : Command(parent)
{
    setObjectName("StartBulbShooting");
}

QJsonDocument StartBulbShooting::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "startBulbShooting";
    command["params"] = QJsonArray {};

    return QJsonDocument(command);
}

StopBulbShooting::StopBulbShooting(QObject* parent)
    : Command(parent)
{
    setObjectName("StopBulbShooting");
}

QJsonDocument StopBulbShooting::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "stopBulbShooting";
    command["params"] = QJsonArray {};

    return QJsonDocument(command);
}

AwaitTakePicture::AwaitTakePicture(QObject* parent)
    : PostViewProviderCommand(parent)
{
    setObjectName("AwaitTakePicture");
}

QJsonDocument AwaitTakePicture::getJson() const
{
    QJsonObject command = getBase();
    command["method"] = "awaitTakePicture";
    command["params"] = QJsonArray {};

    return QJsonDocument(command);
}


void PostViewProviderCommand::handleReply(const QJsonDocument &replyJson)
{
    if(!replyJson.isObject() || !replyJson.object().contains("result") || !replyJson.object()["result"].isArray())
    {
        handleError(replyJson, tr("invalid reply"));
        return;
    }

    QJsonObject obj = replyJson.object();
    QJsonArray result = obj["result"].toArray();
    if(result.count() < 1 || !result[0].isArray())
    {
        handleError(replyJson, tr("invalid result"));
        return;
    }

    QJsonArray postView = result[0].toArray();
    if(postView.count() < 1)
    {
        handleError(replyJson, tr("no post view url"));
        return;
    }

    postViewUrl = postView[0].toString();

    Q_EMIT havePostViewUrl(postViewUrl);

    Q_EMIT confirmed();

}

PostViewProviderCommand::PostViewProviderCommand(QObject *parent)
    : Command(parent)
{

}

} // namespace Json

