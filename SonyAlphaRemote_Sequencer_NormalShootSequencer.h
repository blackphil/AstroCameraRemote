#ifndef SONYALPHAREMOTE_NormalShootSequencer_H
#define SONYALPHAREMOTE_NormalShootSequencer_H

#include <QObject>
#include <QTimer>
#include <QStateMachine>
#include <QState>
#include <QByteArray>

#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_StatusPoller.h"

namespace SonyAlphaRemote {
namespace Sequencer {



class NormalShootSequencer : public QObject
{
    Q_OBJECT


private :

    QPointer<StatusPoller> statusPoller;
    QPointer<Sender> sender;
    QStateMachine* stateMachine;


    int count;

    QTimer* pauseDelayTm;
    QTimer* startDelayTm;
    int numShots;
    int shutterSpeed;


public:


    explicit NormalShootSequencer(StatusPoller* statusPoller, Sender* sender, QObject *parent = 0);



    int getNumShots() const;
    void setNumShots(int value);

    int getStartDelay() const;
    void setStartDelay(int value);

    int getPauseDelay() const;
    void setPauseDelay(int value);

    bool isRunning() const;

    int calculateSequenceDuration() const;
    static int calculateSequenceDuration(int startDelay, int shutterSpeed, int pauseDelay, int numShots);

    int getShutterSpeed() const;
    void setShutterSpeed(int value);

private Q_SLOTS:
    void handleStopped();


public Q_SLOTS :
    void start();
    void stop();
    void handleCameraStatus(QString status);

Q_SIGNALS :
    void started();
    void stopped();
    void statusMessage(QString);
    void countChanged(int);
    void cameraReady();
    void havePostViewUrl(QString, int, int);

};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_NormalShootSequencer_H
