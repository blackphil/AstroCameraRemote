#ifndef SEQUENCER_SEQUENCER_CONTROLWIDGET_H
#define SEQUENCER_SEQUENCER_CONTROLWIDGET_H

#include <QWidget>

#include "Json_Command.h"
#include "StatusPoller.h"
#include "MessagePoster.h"
#include "PostView_Info.h"
#include "Sequencer_Protocol.h"
#include "Sequencer_ProtocolModel.h"

class Settings;

namespace Sequencer {

class BulbShootSequencer;
class NormalShootSequencer;
class Base;

//class SettingsManager;

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget, public MessagePoster
{
    Q_OBJECT

private:
    Ui::ControlWidget *ui;
//    SettingsManager* sequencerSettingsManager;

    Json::SetShutterSpeed* setShutterSpeed;
    Json::SetIsoSpeedRate* setIsoSpeedRate;
    Json::ActTakePicture* actTakePicture;
    Json::StartBulbShooting* startBulbShooting;
    Json::StopBulbShooting* stopBulbShooting;

    StatusPoller* statusPoller;
    BulbShootSequencer* bulbShootSequencer;
    NormalShootSequencer* normalShootSequencer;

    QPointer<Protocol> currentProtocol;

    ProtocolModel* protocolModel;

    int connectionState;

    Sequencer::Base* getRunningSequencer() const;

public:
    explicit ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget();

Q_SIGNALS :

    void newPostViewInfo(PostView::Info);

public Q_SLOTS :
    void isoSpeedRatesChanged(const QStringList &candidates, const QString& current);
    void shutterSpeedsChanged(const QStringList &candidates, const QString &current);

    void error(QString msg);
    void appendOutputMessage(QString msg);

    void loadProtocols();

    void setupProtocol(Protocol* p) const;

private Q_SLOTS :

    void on_isoSpeedRate_activated(const QString &isoSpeedRate);

    void onPostView(const QString &url);

    void onPostView(const QString& url, int i, int numShots);

//    void addCurrentSequencerSettings();
//    void applySequencerSettings(const QString& name, const QStringList& availableSettings);
    void applySequencerSettingsFromCurrentProtocol();
//    void removeSequencerSettings(const QString& name);

    bool stopRunningSequence();

    void shootSequencerStarted();
    void shootSequencerStopped();
    void recalcSequenceDuration();

    void on_startBulbSequence_clicked();

    void startSequence();

    void shutterSpeedChanged(const QString& value);

    void on_shutterSpeed_activated(const QString &speed);

    void on_takeShotBtn_clicked();

    void on_cotinueBtn_clicked();
};


} // namespace Sequencer
#endif // SEQUENCER_SEQUENCER_CONTROLWIDGET_H
