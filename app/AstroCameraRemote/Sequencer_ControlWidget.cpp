#include "Sequencer_ControlWidget.h"
#include "ui_Sequencer_ControlWidget.h"

#include "AstroBase.h"

#include "Settings.h"
#include "Settings_General.h"
#include "Sender.h"
#include "Sequencer_BulbShootSequencer.h"
#include "Sequencer_NormalShootSequencer.h"
#include "Sequencer_CreateNewDialog.h"

#include <QMessageBox>

namespace Sequencer {



Base *ControlWidget::getRunningSequencer() const
{
    if(bulbShootSequencer->isRunning())
        return bulbShootSequencer;
    else if(normalShootSequencer->isRunning())
        return normalShootSequencer;

    return nullptr;
}

Base *ControlWidget::getActiveSequencer() const
{
    if(currentProtocol)
    {
        if(currentProtocol->getProperties().isBulb())
            return bulbShootSequencer;
        else
            return normalShootSequencer;

    }

    return nullptr;
}

void ControlWidget::setCurrentProtocol(Protocol *p)
{
    if(currentProtocol)
    {
        disconnect(bulbShootSequencer, nullptr, currentProtocol, nullptr);
        disconnect(normalShootSequencer, nullptr, currentProtocol, nullptr);
    }

    currentProtocol = p;

    if(currentProtocol)
    {
        Sequencer::Base* sequencer = nullptr;

        if(currentProtocol->getProperties().isBulb())
        {
            sequencer = bulbShootSequencer;
        }
        else
        {
            sequencer = normalShootSequencer;
        }

        applySequencerSettingsFromCurrentProtocol();
        connect(sequencer, SIGNAL(started()), currentProtocol, SLOT(start()));
        connect(sequencer, SIGNAL(havePostViewUrl(QString, int, int)), currentProtocol, SLOT(shotFinished(QString, int, int)));
        connect(sequencer, SIGNAL(stopped()), currentProtocol, SLOT(stop()));

        sequencer->setStartIndex(currentProtocol->getNumShotsFinished());
        sequencer->setNumShots(currentProtocol->getProperties().numShots);
        sequencer->setShutterSpeed(currentProtocol->getProperties().getShutterSpeedInMilliseconds());
        sequencer->setPauseDelay(currentProtocol->getProperties().pause);
        sequencer->setStartDelay(currentProtocol->getProperties().startDelay);
//        duration = sequencer->calculateSequenceDuration();
    }
}

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent)
  ,  ui(new Ui::ControlWidget)
  , setShutterSpeed(new Json::SetShutterSpeed(this))
  , setIsoSpeedRate(new Json::SetIsoSpeedRate(this))
  , actTakePicture(new Json::ActTakePicture(this))
  , startBulbShooting(new Json::StartBulbShooting(this))
  , stopBulbShooting(new Json::StopBulbShooting(this))
  , bulbShootSequencer(new BulbShootSequencer(this))
  , normalShootSequencer(new NormalShootSequencer(this))
  , currentProtocol(nullptr)
  , protocolModel(new ProtocolModel(this))
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentWidget(ui->mainTab);

    ui->stashedShootings->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->shutterSpeedTuBtn->setCurrentUnit(TimeUnitButton::Unit_Seconds);
    ui->startDelayTuBtn->setCurrentUnit(TimeUnitButton::Unit_Seconds);
    ui->pauseTuBtn->setCurrentUnit(TimeUnitButton::Unit_Seconds);

    connect(StatusPoller::get(), SIGNAL(statusChanged(QString)), this, SLOT(handleCameraStatus(QString)));

    ui->stashedShootings->setModel(protocolModel);


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
    ui->shutterSpeedTuBtn->connectToSpinbox(ui->shutterSpeedBulb);
    ui->pauseTuBtn->connectToSpinbox(ui->pause);

    connect(ui->startDelay, SIGNAL(valueChanged(double)), this, SLOT(recalcSequenceDuration()));
    connect(ui->shutterSpeedBulb, SIGNAL(valueChanged(double)), this, SLOT(recalcSequenceDuration()));
    connect(ui->pause, SIGNAL(valueChanged(double)), this, SLOT(recalcSequenceDuration()));
    connect(ui->numShots, SIGNAL(valueChanged(int)), this, SLOT(recalcSequenceDuration()));

    recalcSequenceDuration();

    loadProtocols();
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
    newInfo.setSubject(ui->subjectLineEdit->text());
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
    newInfo.setSubject(ui->subjectLineEdit->text());
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


void ControlWidget::isoSpeedRatesChanged(const QStringList &candidates, const QString &current)
{

    ui->isoSpeedRate->blockSignals(true);
    ui->isoSpeedRate->clear();
    foreach(QString speed, candidates)
    {
        ui->isoSpeedRate->insertItem(0, speed);
    }
    ui->isoSpeedRate->setCurrentText(current);
    ui->isoSpeedRate->blockSignals(false);

}

void ControlWidget::shutterSpeedsChanged(const QStringList &candidates, const QString& current)
{

    ui->shutterSpeed->blockSignals(true);
    ui->shutterSpeed->clear();
    foreach(QString speed, candidates)
    {
        ui->shutterSpeed->insertItem(0, speed);
    }
    ui->shutterSpeed->setCurrentText(current);
    ui->shutterSpeed->blockSignals(false);

}

void ControlWidget::loadProtocols()
{
    QDir dir(Protocol::getProtocolPath());
    if(!dir.exists())
        return;

    QFileInfoList protocolFilePaths = dir.entryInfoList(QStringList() << "*.xml", QDir::Files, QDir::Name);
    for(const QFileInfo& fi : protocolFilePaths)
    {
        Protocol* p = new Protocol(this);
        QFile f(fi.absoluteFilePath());
        f.open(QIODevice::ReadOnly | QIODevice::Text);
        QXmlStreamReader reader(&f);
        p->deSerializeXml(reader);
        protocolModel->addProtocol(p);
    }
}

void ControlWidget::setupProtocol() const
{
    if(currentProtocol)
    {
        Properties properties;
        properties.shutterSpeed = ui->shutterSpeed->currentText();
        properties.shutterSpeedBulb = ui->shutterSpeedTuBtn->getValueInMilliseconds();
        properties.shutterSpeedBulbUnit = ui->shutterSpeedTuBtn->currentUnit();
        properties.iso = ui->isoSpeedRate->currentText();
        properties.startDelay = ui->startDelayTuBtn->getValueInMilliseconds();
        properties.startDelayUnit = ui->startDelayTuBtn->currentUnit();
        properties.pause = ui->pauseTuBtn->getValueInMilliseconds();
        properties.pauseUnit = ui->pauseTuBtn->currentUnit();
        properties.numShots = ui->numShots->value();

        currentProtocol->setProperties(properties);

        if(currentProtocol->getType() != Protocol::Type_Focusing)
            protocolModel->addProtocol(currentProtocol);

        currentProtocol->setReferenceMarkers(referenceMarkers);
        currentProtocol->save();

        ui->subjectLineEdit->setText(currentProtocol->getSubject());
    }
    else
    {
        ui->subjectLineEdit->setText("");
    }


}

void ControlWidget::handleNewReferenceMarkers(const QList<QRectF> &markers)
{

    referenceMarkers = markers;

    if(currentProtocol)
    {
        currentProtocol->cleanUpMarkers();
        currentProtocol->setReferenceMarkers(markers);
    }


}

void ControlWidget::applySequencerSettingsFromCurrentProtocol()
{
    Q_ASSERT(currentProtocol);
    if(!currentProtocol)
    {
        errorMessage(tr("Cannot apply settings: no data available"));
        AB_WRN("currentProtocol missing");
        return;
    }

    ui->subjectLineEdit->setText(currentProtocol->getSubject());

    const Properties& p = currentProtocol->getProperties();
    ui->shutterSpeed->setCurrentText(p.shutterSpeed);
    ui->isoSpeedRate->setCurrentText(p.iso);

    ui->shutterSpeedTuBtn->setCurrentUnit(p.shutterSpeedBulbUnit);
    ui->shutterSpeedTuBtn->setValueInMilliseconds(p.shutterSpeedBulb);

    ui->startDelayTuBtn->setCurrentUnit(p.startDelayUnit);
    ui->startDelayTuBtn->setValueInMilliseconds(p.startDelay);

    ui->pauseTuBtn->setCurrentUnit(p.pauseUnit);
    ui->pauseTuBtn->setValueInMilliseconds(p.pause);

    ui->numShots->setValue(p.numShots);

    referenceMarkers = currentProtocol->getReferenceMarkers();

    setShutterSpeed->setShutterSpeed(ui->shutterSpeed->currentText());
    Sender::get()->send(setShutterSpeed);
    setIsoSpeedRate->setIsoSpeedRate(ui->isoSpeedRate->currentText());
    Sender::get()->send(setIsoSpeedRate);
}

void ControlWidget::shootSequencerStarted()
{
    ui->startBulbSequence->setText(tr("Stop sequence"));
}

void ControlWidget::shootSequencerStopped()
{
    Q_ASSERT(currentProtocol);
    if(!currentProtocol)
    {
        errorMessage(tr("Cannot handle \"sequencer stopped\" event: missing protocol data."));
        AB_WRN("Failed to handle sequencer stop event: no currentProtocol available");
        return;
    }

    currentProtocol->stop();

    Sequencer::Base* sequencer;
    if(currentProtocol->getProperties().isBulb())
    {
        sequencer = bulbShootSequencer;
    }
    else
    {
        sequencer = normalShootSequencer;
    }

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

        int milliSeconds = 0;//helper::shutterSpeedStrToMilliseconds(ui->shutterSpeed->currentText());
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
    Sequencer::Base* sequencer = getRunningSequencer();
    if(nullptr == sequencer)
    {
        return false;
    }

    if(sequencer->isRunning())
        sequencer->stop();
    return true;

}

void ControlWidget::on_startBulbSequence_clicked()
{
    if(stopRunningSequence())
        return;

    startSequence();
}

void ControlWidget::startSequence()
{
//    Q_ASSERT(currentProtocol);
    if(!currentProtocol)
    {
        errorMessage("No active protocol ... cannot start any sequence");
        AB_WRN("No active protocol ... cannot start any sequence");
        return;
    }

    setupProtocol();


    infoMessage("-------------------------------------------------------------");
    infoMessage(tr("start sequence at %0").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")));

    int duration = 0;


    QTime dt = QTime(0,0,0,0).addMSecs(duration);
    infoMessage(
                tr("estimated duration: %0h %1min %2sec %3msec")
                .arg(dt.hour()).arg(dt.minute()).arg(dt.second()).arg(dt.msec()));

    infoMessage(
                tr("estimated finish time: %0")
                .arg(QDateTime::currentDateTime()
                     .addMSecs(duration).toString("yyyy-MM-ddTHH:mm:ss:zzz")));
    infoMessage("-------------------------------------------------------------");

    Base* sequencer = getActiveSequencer();
    Q_ASSERT(sequencer);
    sequencer->setStartIndex(currentProtocol->getNumShotsFinished());
    sequencer->start();
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



void ControlWidget::on_loadBtn_clicked()
{

    Protocol* selProtocol = ui->stashedShootings->getSelectedProtocol();
    if(!selProtocol)
        return;

    QList<QRectF> markers = selProtocol->getReferenceMarkers();
    if(!markers.isEmpty())
        Q_EMIT newReferenceMarkers(markers);

    setCurrentProtocol(selProtocol);

    applySequencerSettingsFromCurrentProtocol();

    ui->tabWidget->setCurrentWidget(ui->mainTab);

}

void ControlWidget::on_newSequenceBtn_clicked()
{
    Q_ASSERT(currentProtocol == nullptr || !currentProtocol->getRecording());
    if(currentProtocol != nullptr)
    {
        if(currentProtocol->getRecording())
        {
            errorMessage(tr("Cannot create sequence: another seuqence is running"));
            AB_WRN("already have a running sequence");
            return;
        }

        currentProtocol.clear();
    }

    CreateNewDialog dlg(this);
    if(dlg.exec() == QDialog::Accepted)
    {
        Protocol* newProtocol = new Protocol(this);
        newProtocol->setSubject(dlg.getSubject());
        newProtocol->setType(dlg.getType());
        setCurrentProtocol(newProtocol);
        setupProtocol();


        QMessageBox::information(this, tr("New sequence"), tr("New sequence \"%0\" created.").arg(currentProtocol->getSubject()));
    }
}

void ControlWidget::handleCameraStatus(const QString & status)
{
    if(status == "IDLE")
    {
        ui->settingsGroup->setEnabled(true);
    }
    else
    {
        ui->settingsGroup->setEnabled(false);
    }
}

} // namespace Sequencer






