#ifndef SONYALPHAREMOTE_BULBSHOOTSEQUENCER_H
#define SONYALPHAREMOTE_BULBSHOOTSEQUENCER_H

#include <QObject>
#include <QTimer>
#include <QStateMachine>
#include <QState>
#include <QByteArray>

#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_StatusPoller.h"

namespace SonyAlphaRemote {
namespace Sequencer {



class BulbShootSequencer : public QObject
{
    Q_OBJECT


private :

    QPointer<StatusPoller> statusPoller;
    QPointer<Sender> sender;
    QStateMachine* stateMachine;

    int count;

    QTimer* shutterSpeedTm;
    QTimer* pauseDelayTm;
    QTimer* startDelayTm;
    int numShots;


public:


    explicit BulbShootSequencer(StatusPoller* statusPoller, Sender* sender, QObject *parent = 0);



    int getNumShots() const;
    void setNumShots(int value);

    int getStartDelay() const;
    void setStartDelay(int value);

    int getShutterSpeed() const;
    void setShutterSpeed(int value);

    int getPauseDelay() const;
    void setPauseDelay(int value);

    bool isRunning() const;

    int calculateSequenceDuration() const;


public Q_SLOTS :
    void start();
    void handleCameraStatus(QString status);

Q_SIGNALS :
    void statusMessage(QString);
    void countChanged(int);
    void finished();
    void cameraReady();
    void havePostViewUrl(QString, int, int);

};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_BULBSHOOTSEQUENCER_H
