#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTimer>
#include <QList>

#include "SonyAlphaRemote_Sender.h"
#include "SonyAlphaRemote_StatusPoller.h"
#include "SonyAlphaRemote_BatteryInfo.h"
#include "SonyAlphaRemote_Settings.h"
#include "SonyAlphaRemote_Sequencer_SettingsManager.h"
#include "PostView_Info.h"
#include "Settings_General.h"
#include "LiveView_Settings.h"
#include "StarTrack_Settings.h"

namespace StarTrack
{
class StarTrackView;
}


namespace Ui {
class MainWindow;
}

class TextEditMessageHandler;

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

    TextEditMessageHandler* messageHandler;

    Settings* settings;

    Json::StartRecMode* startRecMode;
    Json::StopRecMode* stopRecMode;

    StarTrack::StarTrackView* fullScreenStarTrackView;


    int connectionState;
    bool aboutToClose;

    QTimer* currentTimeDisplayTimer;



    void closeEvent(QCloseEvent *event);

    void connectionStateChanged();

    void setupStarTrackView(StarTrack::StarTrackView* v);


Q_SIGNALS :
    void connectedToCamera();
    void disconnectedFromCamera();

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private Q_SLOTS:

    void isoSpeedRatesChanged(const QStringList& candidates, const QString&);
    void shutterSpeedsChanged(const QStringList& candidates, const QString&);

    void toggleStarTrackViewFullScreen(bool yes);

    void toggleRecordModeBtn(bool on);

    void toggleRecordModeBtnStopped();
    void toggleRecordModeBtnStarted();

    void hello();
    void helloError(QString msg);

    void error(QString);

    void appendOutputMessage(QString msg);

    void handleCameraStatus(QString status);

    void updateCurrentTimeDisplay();

    void updateBatteryStatus();

    void on_actionSettings_triggered();

    void viewsTabChanged(int index);

    void on_testHfdBtn_clicked();

    void on_lenrCheckbox_clicked(bool checked);

    void on_viewsTabWidget_currentChanged(int index);

    void on_scaledImageCheckbox_toggled(bool checked);

    void on_actionFits_repair_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
