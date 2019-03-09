#include "Sequencer_ControlWidget.h"
#include "ui_Sequencer_ControlWidget.h"

#include "SonyAlphaRemote_Settings.h"
#include "SonyAlphaRemote_Sequencer_SettingsManager.h"
#include "Settings_General.h"
#include "SonyAlphaRemote_Sender.h"
#include "SonyAlphaRemote_Sequencer_BulbShootSequencer.h"
#include "SonyAlphaRemote_Sequencer_NormalShootSequencer.h"


#include <QMessageBox>

namespace Sequencer {

namespace helper
{
int shutterSpeedStrToMilliseconds(QString shutterSpeedStr)
{
    QRegExp rx1("1/(\\d+)");
    QRegExp rx2("(\\d+)\"");
    int milliSeconds = 0;
    if(rx1.exactMatch(shutterSpeedStr))
        milliSeconds = static_cast<int>(1000. / rx1.cap(1).toDouble());
    else if(rx2.exactMatch(shutterSpeedStr))
        milliSeconds = static_cast<int>(1000. * rx2.cap(1).toDouble());

    return milliSeconds;
}
}

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent)
  ,  ui(new Ui::ControlWidget)
  , settings(new ::Settings(this))
  , sequencerSettingsManager(new Sequencer::SettingsManager(settings))
  , setShutterSpeed(new Json::SetShutterSpeed(this))
  , setIsoSpeedRate(new Json::SetIsoSpeedRate(this))
  , actTakePicture(new Json::ActTakePicture(this))
  , startBulbShooting(new Json::StartBulbShooting(this))
  , stopBulbShooting(new Json::StopBulbShooting(this))
  , statusPoller(new StatusPoller(this))
  , bulbShootSequencer(new BulbShootSequencer(statusPoller, Sender::get(), this))
  , normalShootSequencer(new NormalShootSequencer(statusPoller, Sender::get(), this))
{
    ui->setupUi(this);

    settings->add(sequencerSettingsManager);

    connect(setShutterSpeed, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(setIsoSpeedRate, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(actTakePicture, SIGNAL(havePostViewUrl(QString)), this, SLOT(onPostView(QString)));
    connect(actTakePicture, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(startBulbShooting, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(stopBulbShooting, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(bulbShootSequencer, SIGNAL(statusMessage(QString)), this, SLOT(appendOutputMessage(QString)));
    connect(bulbShootSequencer, SIGNAL(havePostViewUrl(QString, int, int)), this, SLOT(onPostView(QString, int, int)));
    connect(bulbShootSequencer, SIGNAL(started()), this, SLOT(shootSequencerStarted()));
    connect(bulbShootSequencer, SIGNAL(stopped()), this, SLOT(shootSequencerStopped()));

    connect(normalShootSequencer, SIGNAL(statusMessage(QString)), this, SLOT(appendOutputMessage(QString)));
    connect(normalShootSequencer, SIGNAL(havePostViewUrl(QString,int,int)), this, SLOT(onPostView(QString,int,int)));
    connect(normalShootSequencer, SIGNAL(started()), this, SLOT(shootSequencerStarted()));
    connect(normalShootSequencer, SIGNAL(stopped()), this, SLOT(shootSequencerStopped()));




    connect(ui->lenrCheckbox, SIGNAL(toggled(bool)), this, SLOT(recalcSequenceDuration()));
    ui->lenrCheckbox->setChecked(GeneralSettings::getLenrEnabled());

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

    connect(ui->startDelay, SIGNAL(valueChanged(double)), this, SLOT(recalcSequenceDuration()));
    connect(ui->shutterSpeedBulb, SIGNAL(valueChanged(double)), this, SLOT(recalcSequenceDuration()));
    connect(ui->pause, SIGNAL(valueChanged(double)), this, SLOT(recalcSequenceDuration()));
    connect(ui->numShots, SIGNAL(valueChanged(int)), this, SLOT(recalcSequenceDuration()));

    connect(ui->addSettingsBtn, SIGNAL(clicked()), this, SLOT(addCurrentSequencerSettings()));
    connect(ui->removeSettingsBtn, SIGNAL(clicked()), sequencerSettingsManager, SLOT(removeCurrent()));
    connect(ui->settingsNameCBox, SIGNAL(currentIndexChanged(QString)), sequencerSettingsManager, SLOT(setCurrent(QString)));
    connect(sequencerSettingsManager, SIGNAL(removed(QString)), this, SLOT(removeSequencerSettings(QString)));

    connect(
                sequencerSettingsManager, SIGNAL(currentChanged(QString,QStringList))
                , this, SLOT(applySequencerSettings(QString,QStringList)));
    ui->settingsNameCBox->addItems(sequencerSettingsManager->getSettingsNames());

    recalcSequenceDuration();


}

ControlWidget::~ControlWidget()
{
    delete ui;
}


void ControlWidget::on_isoSpeedRate_activated(const QString &isoSpeedRate)
{
    setIsoSpeedRate->setIsoSpeedRate(isoSpeedRate);
    Sender::get()->send(setIsoSpeedRate);
}

void ControlWidget::onPostView(const QString &url)
{
    bool ok = false;

    PostView::Info newInfo;
    newInfo.setShutterSpeed(ui->shutterSpeed->currentText());
    newInfo.setShutterSpeedBulbMs(ui->shutterSpeedTuBtn->getValueInMilliseconds());
    newInfo.setIso(ui->isoSpeedRate->currentText().toInt(&ok));
    QDateTime ts = QDateTime::currentDateTime();
    newInfo.setTimestamp(ts);
    newInfo.setUrl(url);

    Q_EMIT newPostViewInfo(newInfo);

    infoMessage(tr("have new image: %0").arg(url));
//    ui->imageSubTitle->setText(ts.toString("yyyy-MM-ddTHH:mm:ss:zzz"));
    Sender::get()->loadPostViewImage(url);
}

void ControlWidget::onPostView(const QString& url, int i, int numShots)
{
    bool ok = false;

    PostView::Info newInfo;
    newInfo.setShutterSpeed(ui->shutterSpeed->currentText());
    newInfo.setShutterSpeedBulbMs(ui->shutterSpeedTuBtn->getValueInMilliseconds());
    newInfo.setIso(ui->isoSpeedRate->currentText().toInt(&ok));
    QDateTime ts = QDateTime::currentDateTime();
    newInfo.setTimestamp(ts);
    newInfo.setUrl(url);
    newInfo.setSeqNr(i);
    newInfo.setNumShots(numShots);

    Q_EMIT newPostViewInfo(newInfo);


    infoMessage(tr("have new image: %0 (%1/%2)").arg(url).arg(i).arg(numShots));
//    ui->imageSubTitle->setText(tr("image %0/%1").arg(i).arg(numShots));
    Sender::get()->loadPostViewImage(url);
}


void ControlWidget::isoSpeedRatesChanged(const QStringList &candidates)
{

    ui->isoSpeedRate->blockSignals(true);
    ui->isoSpeedRate->clear();
    foreach(QString speed, candidates)
    {
        ui->isoSpeedRate->insertItem(0, speed);
    }
    ui->isoSpeedRate->blockSignals(false);

}

void ControlWidget::shutterSpeedsChanged(const QStringList &candidates)
{

    ui->shutterSpeed->blockSignals(true);
    ui->shutterSpeed->clear();
    foreach(QString speed, candidates)
    {
        ui->shutterSpeed->insertItem(0, speed);
    }
    ui->shutterSpeed->blockSignals(false);

}

void ControlWidget::addCurrentSequencerSettings()
{
    if(!sequencerSettingsManager->getSettingsNames().contains(ui->settingsNameCBox->currentText()))
    {
        Sequencer::Settings* s(new Sequencer::Settings(sequencerSettingsManager));
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
    }
}

void ControlWidget::applySequencerSettings(const QString &name, const QStringList &availableSettings)
{
    ui->settingsNameCBox->blockSignals(true);
    ui->settingsNameCBox->clear();
    ui->settingsNameCBox->addItems(availableSettings);
    ui->settingsNameCBox->setCurrentText(name);
    ui->settingsNameCBox->blockSignals(false);

    Sequencer::Settings* s = sequencerSettingsManager->getCurrent();
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
    Sender::get()->send(setShutterSpeed);
    setIsoSpeedRate->setIsoSpeedRate(ui->isoSpeedRate->currentText());
    Sender::get()->send(setIsoSpeedRate);
}

void ControlWidget::removeSequencerSettings(const QString &name)
{
    ui->settingsNameCBox->removeItem(ui->settingsNameCBox->findText(name));
}

void ControlWidget::shootSequencerStarted()
{
    ui->startBulbSequence->setText(tr("Stop sequence"));
}

void ControlWidget::shootSequencerStopped()
{
    infoMessage(tr("shoot sequence stopped."));
    ui->startBulbSequence->setText(tr("Start sequence"));
}

void ControlWidget::recalcSequenceDuration()
{
    QTime dt;
    if(ui->shutterSpeed->currentText() == "BULB")
    {
        dt = QTime(0,0,0,0).addMSecs(
                    Sequencer::Base::calculateSequenceDuration(
                        ui->startDelayTuBtn->getValueInMilliseconds()
                        , ui->shutterSpeedTuBtn->getValueInMilliseconds()
                        , ui->pauseTuBtn->getValueInMilliseconds()
                        , ui->numShots->value()));
    }
    else
    {

        int milliSeconds = helper::shutterSpeedStrToMilliseconds(ui->shutterSpeed->currentText());
        dt = QTime(0,0,0,0).addMSecs(
                    Sequencer::Base::calculateSequenceDuration(
                        ui->startDelayTuBtn->getValueInMilliseconds()
                        , milliSeconds
                        , ui->pauseTuBtn->getValueInMilliseconds()
                        , ui->numShots->value()));
    }

    ui->estimatedDuration->setText(
                tr("%0h %1min %2sec %3msec")
                .arg(dt.hour()).arg(dt.minute()).arg(dt.second()).arg(dt.msec()));




}

bool ControlWidget::stopRunningSequence()
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

void ControlWidget::on_startBulbSequence_clicked()
{
    if(stopRunningSequence())
        return;

    infoMessage("-------------------------------------------------------------");
    infoMessage(tr("start sequence at %0").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")));

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
    infoMessage(
                tr("estimated duration: %0h %1min %2sec %3msec")
                .arg(dt.hour()).arg(dt.minute()).arg(dt.second()).arg(dt.msec()));

    infoMessage(
                tr("estimated finish time: %0")
                .arg(QDateTime::currentDateTime()
                     .addMSecs(duration).toString("yyyy-MM-ddTHH:mm:ss:zzz")));
    infoMessage("-------------------------------------------------------------");

    if(ui->shutterSpeed->currentText() == "BULB")
        bulbShootSequencer->start();
    else
        normalShootSequencer->start();
}



void ControlWidget::shutterSpeedChanged(const QString &value)
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


void ControlWidget::on_shutterSpeed_activated(const QString &speed)
{
    setShutterSpeed->setShutterSpeed(speed);
    Sender::get()->send(setShutterSpeed);
}

void ControlWidget::on_takeShotBtn_clicked()
{
    Sender::get()->send(actTakePicture);
}

} // namespace Sequencer
