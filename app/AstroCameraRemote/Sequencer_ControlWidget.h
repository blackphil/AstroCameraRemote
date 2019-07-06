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

class ControlWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::ControlWidget *ui;
//    SettingsManager* sequencerSettingsManager;

    MessagePoster* msgPoster;

    Json::SetShutterSpeed* setShutterSpeed;
    Json::SetIsoSpeedRate* setIsoSpeedRate;
    Json::ActTakePicture* actTakePicture;
    Json::StartBulbShooting* startBulbShooting;
    Json::StopBulbShooting* stopBulbShooting;

    StatusPoller* statusPoller;
    BulbShootSequencer* bulbShootSequencer;
    NormalShootSequencer* normalShootSequencer;

    QPointer<Protocol> currentProtocol;

    QList<QRectF> referenceMarkers;

    ProtocolModel* protocolModel;

    int connectionState;

    Sequencer::Base* getRunningSequencer() const;
    Sequencer::Base* getActiveSequencer() const;

    void setCurrentProtocol(Protocol* p);

public:
    explicit ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget();

Q_SIGNALS :

    void newPostViewInfo(PostView::Info);
    void newReferenceMarkers(const QList<QRectF>&);

public Q_SLOTS :
    void isoSpeedRatesChanged(const QStringList &candidates, const QString& current);
    void shutterSpeedsChanged(const QStringList &candidates, const QString &current);

    void loadProtocols();

    void uiToProtocol(Protocol *protocol) const;

    void handleNewReferenceMarkers(const QList<QRectF>& markers);

private Q_SLOTS :

    void on_isoSpeedRate_activated(const QString &isoSpeedRate);

    void onPostView(const QString &url);

    void onPostView(const QString& url, int i, int numShots);

//    void addCurrentSequencerSettings();
//    void applySequencerSettings(const QString& name, const QStringList& availableSettings);
    void protocolToUi(Protocol* protocol);
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

    void on_newSequenceBtn_clicked();

    void handleCameraStatus(const QString&status);
    void on_loadBtn_clicked();
};


} // namespace Sequencer
#endif // SEQUENCER_SEQUENCER_CONTROLWIDGET_H
