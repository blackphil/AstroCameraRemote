#ifndef SONYALPHAREMOTE_SEQUENCER_STATEBASE_H
#define SONYALPHAREMOTE_SEQUENCER_STATEBASE_H

#include <QObject>
#include <QState>
#include <QTimer>
#include <QPointer>


namespace SonyAlphaRemote {
namespace Sequencer {

class StateBase : public QState
{
    Q_OBJECT

protected :
    QString entryMessage;
    QString exitMessage;
    QPointer<QTimer> t;

    QTimer* countDownTimer;
    int countDown;

    QString countdownMsg;
    QString normalMsg;

public :

    StateBase(const QString& entryMessage, const QString& exitMessage, QTimer* t);
    virtual ~StateBase();

    void onEntry(QEvent *);
    void onExit(QEvent *);

    QTimer* getTimer() const { return t; }

private Q_SLOTS:
    void triggerCountDown();


Q_SIGNALS :
    void message(QString);
    void updateStatus(QString);
};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SEQUENCER_STATEBASE_H
