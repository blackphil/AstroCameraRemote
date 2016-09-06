#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SonyAlphaRemote_Helper.h"
#include "SonyAlphaRemote_Sender.h"

#include <QMessageBox>
#include <QRegExp>

namespace helper
{
int shutterSpeedStrToMilliseconds(QString shutterSpeedStr)
{
    QRegExp rx1("1/(\\d+)");
    QRegExp rx2("(\\d+)\"");
    int milliSeconds = 0;
    if(rx1.exactMatch(shutterSpeedStr))
        milliSeconds = (int)(1000. / rx1.cap(1).toDouble());
    else if(rx2.exactMatch(shutterSpeedStr))
        milliSeconds = 1000 * rx2.cap(1).toDouble();

    return milliSeconds;
}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    settings->save();

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
    , setShutterSpeed(new SonyAlphaRemote::Json::SetShutterSpeed(this))
    , setIsoSpeedRate(new SonyAlphaRemote::Json::SetIsoSpeedRate(this))
    , actTakePicture(new SonyAlphaRemote::Json::ActTakePicture(this))
    , startBulbShooting(new SonyAlphaRemote::Json::StartBulbShooting(this))
    , stopBulbShooting(new SonyAlphaRemote::Json::StopBulbShooting(this))
    , sender(new SonyAlphaRemote::Sender(this))
    , statusPoller(new SonyAlphaRemote::StatusPoller(sender, this))
    , bulbShootSequencer(new SonyAlphaRemote::Sequencer::BulbShootSequencer(statusPoller, sender, this))
    , normalShootSequencer(new SonyAlphaRemote::Sequencer::NormalShootSequencer(statusPoller, sender, this))
    , settings(new SonyAlphaRemote::Settings(this))
    , sequencerSettingsManager(new SonyAlphaRemote::Sequencer::SettingsManager(settings))
    , connectionState(State_NotConnected)
    , aboutToClose(false)
    , currentTimeDisplayTimer(new QTimer(this))

    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings->add(sequencerSettingsManager);

    connect(ui->shutterSpeed, SIGNAL(currentTextChanged(QString)), this, SLOT(shutterSpeedChanged(QString)));

    ui->startDelayTuBtn->connectToSpinbox(ui->startDelay);
    connect(ui->startDelayTuBtn, SIGNAL(valueChanged(int)), sequencerSettingsManager, SLOT(setStartDelay(int)));
    connect(ui->startDelayTuBtn, SIGNAL(unitChanged(int)), sequencerSettingsManager, SLOT(setStartDelayUnit(int)));

    ui->shutterSpeedTuBtn->connectToSpinbox(ui->shutterSpeedBulb);
    connect(ui->shutterSpeedTuBtn, SIGNAL(valueChanged(int)), sequencerSettingsManager, SLOT(setShutterSpeedBulb(int)));
    connect(ui->shutterSpeedTuBtn, SIGNAL(unitChanged(int)), sequencerSettingsManager, SLOT(setShutterSpeedBulbUnit(int)));

    ui->pauseTuBtn->connectToSpinbox(ui->pause);
    connect(ui->pauseTuBtn, SIGNAL(valueChanged(int)), sequencerSettingsManager, SLOT(setPause(int)));
    connect(ui->pauseTuBtn, SIGNAL(unitChanged(int)), sequencerSettingsManager, SLOT(setPauseUnit(int)));

    connect(ui->isoSpeedRate, SIGNAL(currentTextChanged(QString)), sequencerSettingsManager, SLOT(setIso(QString)));
    connect(ui->shutterSpeed, SIGNAL(currentTextChanged(QString)), sequencerSettingsManager, SLOT(setShutterSpeed(QString)));
    connect(ui->numShots, SIGNAL(valueChanged(int)), sequencerSettingsManager, SLOT(setNumShots(int)));

    connect(currentTimeDisplayTimer, SIGNAL(timeout()), this, SLOT(updateCurrentTimeDisplay()));
    currentTimeDisplayTimer->start(10);

    connect(sender, SIGNAL(loadedPostViewImage(QByteArray)), this, SLOT(updatePostViewImage(QByteArray)));
    connect(statusPoller, SIGNAL(statusChanged(QString)), this, SLOT(handleCameraStatus(QString)));
    connect(statusPoller, SIGNAL(isoSpeedRatesChanged(QStringList,QString))
            , this, SLOT(isoSpeedRatesChanged(QStringList,QString)));
    connect(statusPoller, SIGNAL(shutterSpeedsChanged(QStringList,QString))
            , this, SLOT(shutterSpeedsChanged(QStringList,QString)));
    connect(statusPoller, SIGNAL(batteryStatusChanged()), this, SLOT(updateBatteryStatus()));


    connect(startRecMode, SIGNAL(confirmed()), this, SLOT(toggleRecordModeBtnStarted()));
    connect(startRecMode, SIGNAL(confirmed()), this, SLOT(hello()));
    connect(startRecMode, SIGNAL(error(QString)), this, SLOT(helloError(QString)));

    connect(stopRecMode, SIGNAL(confirmed()), this, SLOT(toggleRecordModeBtnStopped()));
    connect(stopRecMode, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(setShutterSpeed, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(setIsoSpeedRate, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(actTakePicture, SIGNAL(havePostViewUrl(QString)), this, SLOT(onPostView(QString)));
    connect(actTakePicture, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(startBulbShooting, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(stopBulbShooting, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(bulbShootSequencer, SIGNAL(statusMessage(QString)), this, SLOT(appendOutputMessage(QString)));
    connect(bulbShootSequencer, SIGNAL(updateStatus(QString)), this, SLOT(updateSequencerStatus(QString)));
    connect(bulbShootSequencer, SIGNAL(havePostViewUrl(QString, int, int)), this, SLOT(onPostView(QString, int, int)));
    connect(bulbShootSequencer, SIGNAL(started()), this, SLOT(shootSequencerStarted()));
    connect(bulbShootSequencer, SIGNAL(stopped()), this, SLOT(shootSequencerStopped()));

    connect(normalShootSequencer, SIGNAL(statusMessage(QString)), this, SLOT(appendOutputMessage(QString)));
    connect(normalShootSequencer, SIGNAL(updateStatus(QString)), this, SLOT(updateSequencerStatus(QString)));
    connect(normalShootSequencer, SIGNAL(havePostViewUrl(QString,int,int)), this, SLOT(onPostView(QString,int,int)));
    connect(normalShootSequencer, SIGNAL(started()), this, SLOT(shootSequencerStarted()));
    connect(normalShootSequencer, SIGNAL(stopped()), this, SLOT(shootSequencerStopped()));

    connect(ui->startDelay, SIGNAL(valueChanged(double)), this, SLOT(recalcSequenceDuration()));
    connect(ui->shutterSpeedBulb, SIGNAL(valueChanged(double)), this, SLOT(recalcSequenceDuration()));
    connect(ui->pause, SIGNAL(valueChanged(double)), this, SLOT(recalcSequenceDuration()));
    connect(ui->numShots, SIGNAL(valueChanged(int)), this, SLOT(recalcSequenceDuration()));

//    connect(ui->settingsNameCBox, SIGNAL(editTextChanged(QString)), sequencerSettingsManager, SLOT(renameCurrent(QString)));
    connect(ui->addSettingsBtn, SIGNAL(clicked()), this, SLOT(addCurrentSequencerSettings()));
    connect(ui->removeSettingsBtn, SIGNAL(clicked()), sequencerSettingsManager, SLOT(removeCurrent()));
    connect(ui->settingsNameCBox, SIGNAL(currentIndexChanged(QString)), sequencerSettingsManager, SLOT(setCurrent(QString)));
    connect(sequencerSettingsManager, SIGNAL(removed(QString)), this, SLOT(removeSequencerSettings(QString)));

    connect(
                sequencerSettingsManager, SIGNAL(currentChanged(QString,QStringList))
                , this, SLOT(applySequencerSettings(QString,QStringList)));

    ui->settingsNameCBox->addItems(sequencerSettingsManager->getSettingsNames());
//    applySequencerSettings(sequencerSettingsManager->getCurrent());

    recalcSequenceDuration();

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
}

void MainWindow::connectionStateChanged()
{
    if(connectionState == State_Completed)
    {
        settings->load();
        connectionState |= State_SettingsLoaded;
    }
}

void MainWindow::hello()
{
    ui->centralWidget->setEnabled(true);

    if(!statusPoller->isActive())
    {
        statusPoller->start(1);
    }

    connectionState |= State_Hello;
    connectionStateChanged();
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

    connectionState &= ~State_Hello;
    connectionStateChanged();
}

void MainWindow::error(QString msg)
{
    ui->output->append(msg);
}

void MainWindow::isoSpeedRatesChanged(const QStringList &candidates, const QString &)
{
    if(connectionState & State_IsoSpeeds)
        return;

    ui->isoSpeedRate->blockSignals(true);
    ui->isoSpeedRate->clear();
    foreach(QString speed, candidates)
    {
        ui->isoSpeedRate->insertItem(0, speed);
    }
    ui->isoSpeedRate->blockSignals(false);


    connectionState |= State_IsoSpeeds;
    connectionStateChanged();
}

void MainWindow::shutterSpeedsChanged(const QStringList &candidates, const QString &)
{
    if(connectionState & State_ShutterSpeeds)
        return;

    ui->shutterSpeed->blockSignals(true);
    ui->shutterSpeed->clear();
    foreach(QString speed, candidates)
    {
        ui->shutterSpeed->insertItem(0, speed);
    }
    ui->shutterSpeed->blockSignals(false);

    connectionState |= State_ShutterSpeeds;
    connectionStateChanged();
}

void MainWindow::shutterSpeedChanged(const QString &value)
{
    if(value == "BULB")
    {
        ui->shutterSpeedBulb->setEnabled(true);
        ui->shutterSpeedTuBtn->setEnabled(true);
    }
    else
    {
        ui->shutterSpeedBulb->setEnabled(false);
        ui->shutterSpeedTuBtn->setEnabled(false);
    }

}


void MainWindow::on_shutterSpeed_activated(const QString &speed)
{
    setShutterSpeed->setShutterSpeed(speed);
    sender->send(setShutterSpeed);
}

void MainWindow::on_takeShotBtn_clicked()
{
    sender->send(actTakePicture);
}

void MainWindow::onPostView(const QString &url)
{
    bool ok = false;

    SonyAlphaRemote::PostView::Info newInfo;
    newInfo.setShutterSpeedMs(ui->shutterSpeed->currentText().toInt(&ok));
    newInfo.setIso(ui->isoSpeedRate->currentText().toInt(&ok));
    QDateTime ts = QDateTime::currentDateTime();
    newInfo.setTimestamp(ts);
    newInfo.setUrl(url);

    postViewImageStack.push_back(newInfo);

    ui->output->append(tr("have new image: %0").arg(url));
    ui->imageSubTitle->setText(ts.toString("yyyy-MM-ddTHH:mm:ss:zzz"));
    sender->loadPostViewImage(url);
}

void MainWindow::onPostView(const QString& url, int i, int numShots)
{
    bool ok = false;

    SonyAlphaRemote::PostView::Info newInfo;
    newInfo.setShutterSpeedMs(ui->shutterSpeed->currentText().toInt(&ok));
    newInfo.setIso(ui->isoSpeedRate->currentText().toInt(&ok));
    QDateTime ts = QDateTime::currentDateTime();
    newInfo.setTimestamp(ts);
    newInfo.setUrl(url);
    newInfo.setSeqNr(i);
    newInfo.setNumShots(numShots);

    postViewImageStack.push_back(newInfo);


    ui->output->append(tr("have new image: %0 (%1/%2)").arg(url).arg(i).arg(numShots));
    ui->imageSubTitle->setText(tr("image %0/%1").arg(i).arg(numShots));
    sender->loadPostViewImage(url);
}

void MainWindow::updatePostViewImage(QByteArray data)
{

    QPixmap pixmap = QPixmap::fromImage(QImage::fromData(data, "JPG"));
    Q_ASSERT(!postViewImageStack.isEmpty());
    if(postViewImageStack.isEmpty())
    {
        SAR_ERR(tr("post view image stack is empty!!!"));
        ui->postViewImage->setPixmap(pixmap);
        return;
    }

    postViewImageStack.back().setImage(pixmap);
    ui->postViewImage->setPixmap(postViewImageStack.back().getImage());

}

bool MainWindow::stopRunningSequence()
{
    if(bulbShootSequencer->isRunning() || normalShootSequencer->isRunning())
    {
        if(QMessageBox::Yes == QMessageBox::question(
                    this, tr("Stop shoot sequence"), tr("Do you want to stop current shoot sequence?")))
        {
            if(bulbShootSequencer->isRunning())
                bulbShootSequencer->stop();
            else if(normalShootSequencer->isRunning())
                normalShootSequencer->stop();
            return true;
        }
    }

    return false;
}

void MainWindow::updateSequencerStatus(const QString &status)
{
    ui->sequencerStatus->setText(status);
}

void MainWindow::on_startBulbSequence_clicked()
{
    if(stopRunningSequence())
        return;

    ui->output->append("-------------------------------------------------------------");
    ui->output->append(tr("start sequence at %0").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")));

    int duration = 0;

    if(ui->shutterSpeed->currentText() == "BULB")
    {
        bulbShootSequencer->setNumShots(ui->numShots->value());
        bulbShootSequencer->setShutterSpeed(ui->shutterSpeedTuBtn->getValueInMilliseconds());
        bulbShootSequencer->setPauseDelay(ui->pauseTuBtn->getValueInMilliseconds());
        bulbShootSequencer->setStartDelay(ui->startDelayTuBtn->getValueInMilliseconds());
        duration = bulbShootSequencer->calculateSequenceDuration();
    }
    else
    {
        int milliSeconds = helper::shutterSpeedStrToMilliseconds(ui->shutterSpeed->currentText());
        normalShootSequencer->setNumShots(ui->numShots->value());
        normalShootSequencer->setShutterSpeed(milliSeconds);
        normalShootSequencer->setPauseDelay(ui->pauseTuBtn->getValueInMilliseconds());
        normalShootSequencer->setStartDelay(ui->startDelayTuBtn->getValueInMilliseconds());
        duration = normalShootSequencer->calculateSequenceDuration();
    }


    QTime dt = QTime(0,0,0,0).addMSecs(duration);
    ui->output->append(
                tr("estimated duration: %0h %1min %2sec %3msec")
                .arg(dt.hour()).arg(dt.minute()).arg(dt.second()).arg(dt.msec()));

    ui->output->append(
                tr("estimated finish time: %0")
                .arg(QDateTime::currentDateTime()
                     .addMSecs(duration).toString("yyyy-MM-ddTHH:mm:ss:zzz")));
    ui->output->append("-------------------------------------------------------------");

    if(ui->shutterSpeed->currentText() == "BULB")
        bulbShootSequencer->start();
    else
        normalShootSequencer->start();
}


void MainWindow::appendOutputMessage(QString msg)
{
    ui->output->append(tr("%0: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")).arg(msg));
}

void MainWindow::on_simCamReadyBtn_clicked()
{
    statusPoller->simCamReady();
}

void MainWindow::handleCameraStatus(QString status)
{
    ui->cameraStatus->setText(status);
}

void MainWindow::on_isoSpeedRate_activated(const QString &isoSpeedRate)
{
    setIsoSpeedRate->setIsoSpeedRate(isoSpeedRate);
    sender->send(setIsoSpeedRate);
}

void MainWindow::updateCurrentTimeDisplay()
{
    QDateTime now = QDateTime::currentDateTime();

    ui->currentDate->setText(now.toString("dd. MM. yyyy"));
    ui->currentTime->setText(now.toString("HH:mm:ss:zzz"));
}

void MainWindow::updateBatteryStatus()
{
    SonyAlphaRemote::BatteryInfo info = statusPoller->getBatteryInfo();
    ui->batteryStatus->setMinimum(0);
    ui->batteryStatus->setMaximum(info.getLevelDenom());
    ui->batteryStatus->setValue(info.getLevelNumber());
    double load = (double)info.getLevelNumber() / (double)info.getLevelDenom();

    if(0.001 > load || info.getAdditionalStatus() == SonyAlphaRemote::BatteryInfo::AdditionalStatus_batteryNearEnd)
        ui->batteryStatus->setStyleSheet(SonyAlphaRemote::BatteryInfo::getStyleCritical());
    else if(0.3 > load)
        ui->batteryStatus->setStyleSheet(SonyAlphaRemote::BatteryInfo::getStyleLow());
    else
        ui->batteryStatus->setStyleSheet(SonyAlphaRemote::BatteryInfo::getStyleNormal());


}

void MainWindow::shootSequencerStarted()
{
    ui->startBulbSequence->setText(tr("Stop sequence"));
}

void MainWindow::shootSequencerStopped()
{
    ui->output->append(tr("shoot sequence stopped."));
    ui->startBulbSequence->setText(tr("Start sequence"));
}

void MainWindow::recalcSequenceDuration()
{
    QTime dt;
    if(ui->shutterSpeed->currentText() == "BULB")
    {
        dt = QTime(0,0,0,0).addMSecs(
                    SonyAlphaRemote::Sequencer::Base::calculateSequenceDuration(
                        ui->startDelayTuBtn->getValueInMilliseconds()
                        , ui->shutterSpeedTuBtn->getValueInMilliseconds()
                        , ui->pauseTuBtn->getValueInMilliseconds()
                        , ui->numShots->value()));
    }
    else
    {

        int milliSeconds = helper::shutterSpeedStrToMilliseconds(ui->shutterSpeed->currentText());
        dt = QTime(0,0,0,0).addMSecs(
                    SonyAlphaRemote::Sequencer::Base::calculateSequenceDuration(
                        ui->startDelayTuBtn->getValueInMilliseconds()
                        , milliSeconds
                        , ui->pauseTuBtn->getValueInMilliseconds()
                        , ui->numShots->value()));
    }

    ui->estimatedDuration->setText(
                tr("%0h %1min %2sec %3msec")
                .arg(dt.hour()).arg(dt.minute()).arg(dt.second()).arg(dt.msec()));




}

void MainWindow::addCurrentSequencerSettings()
{
    if(!sequencerSettingsManager->getSettingsNames().contains(ui->settingsNameCBox->currentText()))
    {
        SonyAlphaRemote::Sequencer::Settings* s(new SonyAlphaRemote::Sequencer::Settings(sequencerSettingsManager));
        s->setObjectName(ui->settingsNameCBox->currentText());
        s->setShutterSpeed(ui->shutterSpeed->currentText());
        s->setIso(ui->isoSpeedRate->currentText());
        s->setShutterSpeedBulb(ui->shutterSpeedTuBtn->getValueInMilliseconds());
        s->setShutterspeedBulbUnit(ui->shutterSpeedTuBtn->currentUnit());
        s->setStartDelay(ui->startDelayTuBtn->getValueInMilliseconds());
        s->setStartDelayUnit(ui->startDelayTuBtn->currentUnit());
        s->setPause(ui->pauseTuBtn->getValueInMilliseconds());
        s->setPauseUnit(ui->pauseTuBtn->currentUnit());
        s->setNumShots(ui->numShots->value());

        sequencerSettingsManager->add(s);
        sequencerSettingsManager->setCurrent(s->objectName());

//        ui->settingsNameCBox->blockSignals(true);
//        ui->settingsNameCBox->addItem(s->objectName());
//        ui->settingsNameCBox->setCurrentText(s->objectName());
//        ui->settingsNameCBox->blockSignals(false);

    }
}

void MainWindow::applySequencerSettings(const QString &name, const QStringList &availableSettings)
{
    ui->settingsNameCBox->blockSignals(true);
    ui->settingsNameCBox->clear();
    ui->settingsNameCBox->addItems(availableSettings);
    ui->settingsNameCBox->setCurrentText(name);
    ui->settingsNameCBox->blockSignals(false);

    SonyAlphaRemote::Sequencer::Settings* s = sequencerSettingsManager->getCurrent();
    ui->shutterSpeed->setCurrentText(s->getShutterSpeed());
    ui->isoSpeedRate->setCurrentText(s->getIso());

    ui->shutterSpeedTuBtn->setCurrentUnit(s->getShutterspeedBulbUnit());
    ui->shutterSpeedTuBtn->setValueInMilliseconds(s->getShutterSpeedBulb());

    ui->startDelayTuBtn->setCurrentUnit(s->getStartDelayUnit());
    ui->startDelayTuBtn->setValueInMilliseconds(s->getStartDelay());

    ui->pauseTuBtn->setCurrentUnit(s->getPauseUnit());
    ui->pauseTuBtn->setValueInMilliseconds(s->getPause());

    ui->numShots->setValue(s->getNumShots());

    setShutterSpeed->setShutterSpeed(ui->shutterSpeed->currentText());
    sender->send(setShutterSpeed);
    setIsoSpeedRate->setIsoSpeedRate(ui->isoSpeedRate->currentText());
    sender->send(setIsoSpeedRate);
}

void MainWindow::removeSequencerSettings(const QString &name)
{
    ui->settingsNameCBox->removeItem(ui->settingsNameCBox->findText(name));
}
