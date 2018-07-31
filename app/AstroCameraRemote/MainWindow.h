#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTimer>
#include <QList>

#include "SonyAlphaRemote_Sender.h"
#include "SonyAlphaRemote_StatusPoller.h"
#include "SonyAlphaRemote_Sequencer_BulbShootSequencer.h"
#include "SonyAlphaRemote_Sequencer_NormalShootSequencer.h"
#include "SonyAlphaRemote_BatteryInfo.h"
#include "SonyAlphaRemote_Settings.h"
#include "SonyAlphaRemote_Sequencer_SettingsManager.h"
#include "PostView_Info.h"
#include "Settings_General.h"
#include "LiveView_Settings.h"
#include "StarTrack_Settings.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum CameraConnectionState
    {
        State_NotConnected = 0x00
        , State_Hello = 0x01
        , State_ShutterSpeeds = 0x02
        , State_IsoSpeeds = 0x04
        , State_SettingsLoaded = 0x08
        , State_Completed = 0x07
    };

    SonyAlphaRemote::Json::StartRecMode* startRecMode;
    SonyAlphaRemote::Json::StopRecMode* stopRecMode;
    SonyAlphaRemote::Json::SetShutterSpeed* setShutterSpeed;
    SonyAlphaRemote::Json::SetIsoSpeedRate* setIsoSpeedRate;
    SonyAlphaRemote::Json::ActTakePicture* actTakePicture;
    SonyAlphaRemote::Json::StartBulbShooting* startBulbShooting;
    SonyAlphaRemote::Json::StopBulbShooting* stopBulbShooting;

    SonyAlphaRemote::Sender* sender;
    SonyAlphaRemote::StatusPoller* statusPoller;
    SonyAlphaRemote::Sequencer::BulbShootSequencer* bulbShootSequencer;
    SonyAlphaRemote::Sequencer::NormalShootSequencer* normalShootSequencer;

    SonyAlphaRemote::Settings* settings;
    SonyAlphaRemote::Sequencer::SettingsManager* sequencerSettingsManager;


    int connectionState;
    bool aboutToClose;

    QTimer* currentTimeDisplayTimer;



    void closeEvent(QCloseEvent *event);

    void connectionStateChanged();


Q_SIGNALS :
    void newPostViewInfo(PostView::Info);
    void connectedToCamera();
    void disconnectedFromCamera();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:

    void updateHfdValue(float hfd);

    void toggleRecordModeBtn(bool on);

    void toggleRecordModeBtnStopped();
    void toggleRecordModeBtnStarted();

    void hello();
    void helloError(QString msg);

    void error(QString);

    void isoSpeedRatesChanged(const QStringList& candidates, const QString&);
    void shutterSpeedsChanged(const QStringList& candidates, const QString&);
    void shutterSpeedChanged(const QString& value);

    void on_shutterSpeed_activated(const QString &speed);

    void on_takeShotBtn_clicked();

    void onPostView(const QString &url);
    void onPostView(const QString &url, int i, int numShots);

    void on_startBulbSequence_clicked();

    void appendOutputMessage(QString msg);

    void on_simCamReadyBtn_clicked();

    void handleCameraStatus(QString status);

    void on_isoSpeedRate_activated(const QString &isoSpeedRate);

    void updateCurrentTimeDisplay();

    void updateBatteryStatus();

    void shootSequencerStarted();
    void shootSequencerStopped();
    void recalcSequenceDuration();

    void addCurrentSequencerSettings();
    void applySequencerSettings(const QString& name, const QStringList& availableSettings);
    void removeSequencerSettings(const QString& name);

    bool stopRunningSequence();

    void updateSequencerStatus(const QString& status);

    void on_actionSettings_triggered();

    void on_actionDebug_triggered();

    void viewsTabChanged(int index);

    void on_testHfdBtn_clicked();

    void on_lenrCheckbox_clicked(bool checked);

    void on_markerModusCombobox_activated(int index);

    void on_markerFixedRectSpinbox_editingFinished();

    void on_viewsTabWidget_currentChanged(int index);

    void on_scaledImageCheckbox_toggled(bool checked);

    void on_actionFits_repair_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H