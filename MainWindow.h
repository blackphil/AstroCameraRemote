#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTimer>

#include "SonyAlphaRemote_Sender.h"
#include "SonyAlphaRemote_StatusPoller.h"
#include "SonyAlphaRemote_BulbShootSequencer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    SonyAlphaRemote::Json::StartRecMode* startRecMode;
    SonyAlphaRemote::Json::StopRecMode* stopRecMode;
    SonyAlphaRemote::Json::GetAvailableShutterSpeeds* getAvailableShutterSpeeds;
    SonyAlphaRemote::Json::SetShutterSpeed* setShutterSpeed;
    SonyAlphaRemote::Json::ActTakePicture* actTakePicture;
    SonyAlphaRemote::Json::StartBulbShooting* startBulbShooting;
    SonyAlphaRemote::Json::StopBulbShooting* stopBulbShooting;

    SonyAlphaRemote::Sender* sender;
    SonyAlphaRemote::StatusPoller* statusPoller;
    SonyAlphaRemote::Sequencer::BulbShootSequencer* bulbShootSequencer;

    bool aboutToClose;


    void closeEvent(QCloseEvent *event);


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void on_toggleRecordModeBtn_clicked();

    void toggleRecordModeBtnStopped();
    void toggleRecordModeBtnStarted();

    void hello();
    void helloError(QString msg);

    void error(QString);

    void availableShutterSpeedsChanged();

    void updateAvailableShutterSpeeds();

    void on_shutterSpeed_activated(const QString &speed);

    void on_takeShotBtn_clicked();

    void onPostView(const QString &url);
    void onPostView(const QString &url, int i, int numShots);

    void updatePostViewImage(QByteArray data);

    void on_startBulbSequence_clicked();

    void applyBulbSettings();

    void appendOutputMessage(QString msg);

    void on_simCamReadyBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
