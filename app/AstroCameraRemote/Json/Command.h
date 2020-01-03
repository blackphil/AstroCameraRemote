#ifndef JSON_COMMAND_H
#define JSON_COMMAND_H

#include <QObject>
#include <QJsonDocument>
#include <QtNetwork>
#include <QStringList>

class Sender;


namespace Json {

class Command : public QObject
{
    Q_OBJECT


protected :

    QJsonObject json;
    QNetworkReply* reply;

    virtual void handleReply(const QJsonDocument& replyJson);
    void handleError(const QJsonDocument& replyJson, const QString& msg);

Q_SIGNALS :
    void confirmed();
    void declined();
    void error(QString);


protected Q_SLOTS :
    virtual void handleReply();

public:
    Command(QObject* parent = nullptr);
    virtual ~Command() {}
    QByteArray getJson() const;
    void setReply(QNetworkReply *value);
};

class SetShutterSpeed : public Command
{
    Q_OBJECT


public :
    SetShutterSpeed(QObject* parent = nullptr);

    void setShutterSpeed(const QString &value);
    QString getShutterSpeed() const;
};

class SetIsoSpeedRate : public Command
{
    Q_OBJECT

    QString isoSpeedRate;

public :
    SetIsoSpeedRate(QObject* parent = nullptr);
    void setIsoSpeedRate(const QString &value);
    QString getIsoSpeedRate() const;
};

class GetShutterSpeed : public Command
{
    Q_OBJECT

    QString shutterSpeed;

public :
    GetShutterSpeed(QObject* parent = nullptr);
};

class StartRecMode : public Command
{
    Q_OBJECT

public :
    StartRecMode(QObject* parent = nullptr);
};

class StopRecMode : public Command
{
    Q_OBJECT

public :
    StopRecMode(QObject* parent = nullptr);
};


class SetContShootingMode : public Command
{
    Q_OBJECT
public :
    enum class Mode
    {
        Single, Continuous, SpdPriorityCont, Burst, MotionShot, InvalidMode
    };

    SetContShootingMode(QObject* parent = nullptr);
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
    GetAvailableContShootingModes(QObject *parent = nullptr);
    void handleReply(const QJsonDocument &replyJson) override;

    const QStringList& getContShootingModes() const;
    const QString& getCurrentContShootingMode() const;
};

class StartBulbShooting : public Command
{
    Q_OBJECT
public :
    StartBulbShooting(QObject* parent = nullptr);

};

class PostViewProviderCommand : public Command
{
    Q_OBJECT

    QString postViewUrl;
    void handleReply(const QJsonDocument &replyJson);

Q_SIGNALS :
    void havePostViewUrl(QString);

public :
    PostViewProviderCommand(QObject* parent = nullptr);
    QString getPostViewUrl() const;
};

class ActTakePicture : public PostViewProviderCommand
{
    Q_OBJECT
public :
    ActTakePicture(QObject* parent = nullptr);
};


class StopBulbShooting : public Command
{
    Q_OBJECT
public :
    StopBulbShooting(QObject* parent = nullptr);
    const QJsonDocument& getJson() const;
};

class AwaitTakePicture : public PostViewProviderCommand
{
    Q_OBJECT
public :
    AwaitTakePicture(QObject* parent = nullptr);
};


} // namespace Json


#endif // JSON_COMMAND_H
