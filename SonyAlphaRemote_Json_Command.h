#ifndef SONYALPHAREMOTE_JSON_COMMAND_H
#define SONYALPHAREMOTE_JSON_COMMAND_H

#include <QObject>
#include <QJsonDocument>
#include <QtNetwork>
#include <QStringList>

namespace SonyAlphaRemote {

class Sender;


namespace Json {

class Command : public QObject
{
    Q_OBJECT


protected :
    QNetworkReply* reply;
    virtual QJsonObject getBase() const;

    virtual void handleReply(const QJsonDocument& replyJson);
    void handleError(const QJsonDocument& replyJson, const QString& msg);

Q_SIGNALS :
    void confirmed();
    void declined();
    void error(QString);


protected Q_SLOTS :
    virtual void handleReply();

public:
    Command(QObject* parent = 0) : QObject(parent) {}
    virtual ~Command() {}
    virtual QJsonDocument getJson() const = 0;
    void setReply(QNetworkReply *value);
};

class SetShutterSpeed : public Command
{
    Q_OBJECT

    QString shutterSpeed;

public :
    SetShutterSpeed(QObject* parent = 0);
    QJsonDocument getJson() const;

    void setShutterSpeed(const QString &value);
    QString getShutterSpeed() const;
};

class SetIsoSpeedRate : public Command
{
    Q_OBJECT

    QString isoSpeedRate;

public :
    SetIsoSpeedRate(QObject* parent = 0);
    QJsonDocument getJson() const;

    void setIsoSpeedRate(const QString &value);
    QString getIsoSpeedRate() const;
};

class GetShutterSpeed : public Command
{
    Q_OBJECT

    QString shutterSpeed;

public :
    GetShutterSpeed(QObject* parent = 0);
    QJsonDocument getJson() const;
//    void handleReply(const QJsonDocument &replyJson);
};

class StartRecMode : public Command
{
    Q_OBJECT

public :
    StartRecMode(QObject* parent = 0);
    QJsonDocument getJson() const;
};

class StopRecMode : public Command
{
    Q_OBJECT

public :
    StopRecMode(QObject* parent = 0);
    QJsonDocument getJson() const;
};


class SetContShootingMode : public Command
{
    Q_OBJECT
public :
    enum Mode
    {
        Single, Continuous, SpdPriorityCont, Burst, MotionShot, InvalidMode
    };
private :
    Mode mode;

public :
    SetContShootingMode(QObject* parent = 0);
    QJsonDocument getJson() const;
    Mode getMode() const;
    void setMode(const Mode &value);

    static QString mode2String(Mode mode);
    static Mode string2Mode(const QString& mode);
};

class GetAvailableContShootingModes : public Command
{
    Q_OBJECT
    QStringList contShootingModes;
    QString currentContShootingMode;

public:
    GetAvailableContShootingModes(QObject *parent = 0);
    QJsonDocument getJson() const;
    void handleReply(const QJsonDocument &replyJson);

    QStringList getContShootingModes() const;
    QString getCurrentContShootingMode() const;
};

class StartBulbShooting : public Command
{
    Q_OBJECT
public :
    StartBulbShooting(QObject* parent = NULL);
    QJsonDocument getJson() const;

};

class PostViewProviderCommand : public Command
{
    Q_OBJECT

    QString postViewUrl;
    void handleReply(const QJsonDocument &replyJson);

Q_SIGNALS :
    void havePostViewUrl(QString);

public :
    PostViewProviderCommand(QObject* parent = 0);
    QString getPostViewUrl() const;
};

class ActTakePicture : public PostViewProviderCommand
{
    Q_OBJECT

public :
    ActTakePicture(QObject* parent = 0);
    QJsonDocument getJson() const;

};


class StopBulbShooting : public Command
{
    Q_OBJECT

public :
    StopBulbShooting(QObject* parent = NULL);
    QJsonDocument getJson() const;


};

class AwaitTakePicture : public PostViewProviderCommand
{
    Q_OBJECT

public :
    AwaitTakePicture(QObject* parent = NULL);
    QJsonDocument getJson() const;
};


} // namespace Json
} // namespace SonyAlphaRemote


#endif // SONYALPHAREMOTE_JSON_COMMAND_H
