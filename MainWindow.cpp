#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SonyAlphaRemote_Helper.h"
#include "SonyAlphaRemote_Sender.h"

#include <QMessageBox>

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(ui->toggleRecordModeBtn->isChecked())
    {
        sender->send(stopRecMode);
        aboutToClose = true;
        event->ignore();
    }
    else
    {
        statusPoller->stop();
        event->accept();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , startRecMode(new SonyAlphaRemote::Json::StartRecMode(this))
    , stopRecMode(new SonyAlphaRemote::Json::StopRecMode(this))
    , getAvailableShutterSpeeds(new SonyAlphaRemote::Json::GetAvailableShutterSpeeds(this))
    , setShutterSpeed(new SonyAlphaRemote::Json::SetShutterSpeed(this))
    , actTakePicture(new SonyAlphaRemote::Json::ActTakePicture(this))
    , startBulbShooting(new SonyAlphaRemote::Json::StartBulbShooting(this))
    , stopBulbShooting(new SonyAlphaRemote::Json::StopBulbShooting(this))
    , sender(new SonyAlphaRemote::Sender(this))
    , statusPoller(new SonyAlphaRemote::StatusPoller(sender, this))
    , bulbShootSequencer(new SonyAlphaRemote::BulbShootSequencer(statusPoller, sender, this))
    , aboutToClose(false)

    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(sender, SIGNAL(loadedPostViewImage(QByteArray)), this, SLOT(updatePostViewImage(QByteArray)));

    connect(statusPoller, SIGNAL(statusChanged(QString)), ui->cameraStatus, SLOT(setText(QString)));
    connect(statusPoller, SIGNAL(haveNewPostViewPictureUrl(QString)), this, SLOT(onPostView(QString)));


    connect(startRecMode, SIGNAL(confirmed()), this, SLOT(toggleRecordModeBtnStarted()));
    connect(startRecMode, SIGNAL(confirmed()), this, SLOT(hello()));
    connect(startRecMode, SIGNAL(error(QString)), this, SLOT(helloError(QString)));

    connect(stopRecMode, SIGNAL(confirmed()), this, SLOT(toggleRecordModeBtnStopped()));
    connect(stopRecMode, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(getAvailableShutterSpeeds, SIGNAL(confirmed()), this, SLOT(availableShutterSpeedsChanged()));
    connect(getAvailableShutterSpeeds, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(setShutterSpeed, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(actTakePicture, SIGNAL(havePostViewUrl(QString)), this, SLOT(onPostView(QString)));
    connect(actTakePicture, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(startBulbShooting, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(stopBulbShooting, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(bulbShootSequencer, SIGNAL(statusMessage(QString)), this, SLOT(appendOutputMessage(QString)));
    connect(bulbShootSequencer, SIGNAL(havePostViewUrl(QString, int, int)), this, SLOT(onPostView(QString)));


//    ui->centralWidget->setEnabled(false);
    sender->send(startRecMode);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toggleRecordModeBtn_clicked()
{
    if(!ui->toggleRecordModeBtn->isChecked())
    {
        sender->send(stopRecMode);
    }
    else
    {
        sender->send(startRecMode);
    }
}

void MainWindow::toggleRecordModeBtnStopped()
{
    ui->toggleRecordModeBtn->setText(tr("Start record mode"));
    ui->toggleRecordModeBtn->setChecked(false);
    if(aboutToClose)
        close();
}

void MainWindow::toggleRecordModeBtnStarted()
{
    ui->toggleRecordModeBtn->setText(tr("Stop record mode"));
    ui->toggleRecordModeBtn->setChecked(true);

    QTimer::singleShot(4000, this, SLOT(updateAvailableShutterSpeeds()));
}

void MainWindow::hello()
{
    ui->centralWidget->setEnabled(true);

    if(!statusPoller->isActive())
    {
        statusPoller->start(1);
    }
}

void MainWindow::helloError(QString msg)
{
    error(msg);

//    ui->centralWidget->setEnabled(false);
    QMessageBox::StandardButtons btn;
    do
    {
        btn = QMessageBox::question(
                    this, tr("No connection")
                    , tr("no connection to camera!\rRetry to connect?"));

        if(QMessageBox::Yes == btn)
        {
            sender->send(startRecMode);
        }
    }
    while(QMessageBox::Yes == btn);
}

void MainWindow::error(QString msg)
{
    ui->output->append(msg);
}

void MainWindow::availableShutterSpeedsChanged()
{
    ui->shutterSpeed->clear();
    QStringList speeds = getAvailableShutterSpeeds->getShutterSpeeds();
    foreach(QString speed, speeds)
    {
        ui->shutterSpeed->insertItem(0, speed);
    }
    ui->shutterSpeed->setCurrentText(getAvailableShutterSpeeds->getCurrentShutterSpeed());

}

void MainWindow::updateAvailableShutterSpeeds()
{
    sender->send(getAvailableShutterSpeeds);
}

void MainWindow::on_shutterSpeed_activated(const QString &speed)
{
    setShutterSpeed->setShutterSpeed(speed);
    sender->send(setShutterSpeed);
}

void MainWindow::on_takeShotBtn_clicked()
{
    statusPoller->setPollPostViewPictureUrlEnabled(false);

    if(ui->shutterSpeed->currentText() == "BULB")
    {
        QString cameraStatus = statusPoller->getCameraStatus();
        if(cameraStatus == "IDLE")
        {
            SAR_INF("start BULB shooting now ...");
            sender->send(startBulbShooting);
        }
        else if(cameraStatus == "StillCapturing")
        {
            SAR_INF("stop continous shooting now");
            sender->send(stopBulbShooting);
            statusPoller->setPollPostViewPictureUrlEnabled(true);
        }
    }
    else
    {
        sender->send(actTakePicture);
    }
}

void MainWindow::onPostView(const QString& url, int i, int numShots)
{
    ui->output->append(tr("have new image: %0 (%1/%2").arg(url).arg(i).arg(numShots));
    ui->imageSubTitle->setText(tr("image %0/%1").arg(i).arg(numShots));
    sender->loadPostViewImage(url);
}

void MainWindow::updatePostViewImage(QByteArray data)
{
    QPixmap pixmap = QPixmap::fromImage(QImage::fromData(data, "JPG"));
    ui->postViewImage->setPixmap(pixmap);
}

void MainWindow::on_startBulbSequence_clicked()
{
    if(bulbShootSequencer->isRunning())
    {
        ui->output->append(tr("cannot start BULB sequence until running sequence has been finished!"));
        return;
    }

    ui->output->append("-------------------------------------------------------------");
    ui->output->append(tr("start BULB sequence at %0").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")));

    applyBulbSettings();
    int duration = bulbShootSequencer->calculateSequenceDuration();

    ui->output->append(tr("calculated duration: %0").arg(QTime().addMSecs(duration).toString("HH:mm:ss:zzz")));
    ui->output->append(
                tr("calculated finish time: %0")
                .arg(QDateTime::currentDateTime()
                     .addMSecs(duration).toString("yyyy-MM-ddTHH:mm:ss:zzz")));
    ui->output->append("-------------------------------------------------------------");

    bulbShootSequencer->start();
}

void MainWindow::applyBulbSettings()
{
    bulbShootSequencer->setNumShots(ui->numShots->value());
    bulbShootSequencer->setShutterSpeed(ui->shutterSpeedBulb->value());
    bulbShootSequencer->setPauseDelay(ui->pause->value());
    bulbShootSequencer->setStartDelay(ui->startDelay->value());
}

void MainWindow::appendOutputMessage(QString msg)
{
    ui->output->append(tr("%0: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")).arg(msg));
}

void MainWindow::on_simCamReadyBtn_clicked()
{
    statusPoller->simCamReady();
}
