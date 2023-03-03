#include "Sequencer_ControlWidget.h"
#include "ui_Sequencer_ControlWidget.h"

#include <AstroBase/AstroBase>

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
        disconnect(Sender::get(), nullptr, currentProtocol, nullptr);

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

        AB_DBG("protocol to sequencer: " << currentProtocol->getObjectName());


        connect(sequencer, SIGNAL(started()), currentProtocol, SLOT(start()));
        connect(sequencer, SIGNAL(havePostViewUrl(QString, int, int)), currentProtocol, SLOT(havePostViewUrl(QString, int, int)));
        connect(sequencer, SIGNAL(stopped()), currentProtocol, SLOT(stop()));
        connect(Sender::get(), SIGNAL(loadedPostViewImage(QByteArray)), currentProtocol, SLOT(havePostViewImage(QByteArray)));

//        sequencer->setStartIndex(currentProtocol->getNumShotsFinished());
        sequencer->setStartIndex(0);
        sequencer->setNumShots(currentProtocol->getProperties().numShots);
        sequencer->setShutterSpeed(currentProtocol->getProperties().getShutterSpeedInMilliseconds());
        sequencer->setPauseDelay(currentProtocol->getProperties().pause);
        sequencer->setStartDelay(currentProtocol->getProperties().startDelay);
        //        duration = sequencer->calculateSequenceDuration();

        protocolModel->addProtocol(currentProtocol);

        currentProtocol->setReferenceMarkers(referenceMarkers);
        currentProtocol->save();

    }
    else
        ui->objectLineEdit->setText("");
}

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent)
  ,  ui(new Ui::ControlWidget)
  , msgPoster(new MessagePoster(this))
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

    for(PhotoShot::Type t : PhotoShot::AllTypes)
    {
        ui->type->addItem(PhotoShot::typeToString(t));
    }

    ui->tabWidget->setCurrentWidget(ui->mainTab);

    ui->stashedShootings->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->shutterSpeedTuBtn->setCurrentUnit(TimeUnit::Unit::Seconds);
    ui->startDelayTuBtn->setCurrentUnit(TimeUnit::Unit::Seconds);
    ui->pauseTuBtn->setCurrentUnit(TimeUnit::Unit::Seconds);

    connect(StatusPoller::get(), SIGNAL(statusChanged(QString)), this, SLOT(handleCameraStatus(QString)));

    ui->stashedShootings->setModel(protocolModel);
    connect(ui->stashedShootings, SIGNAL(activateSelectedProtocol()), this, SLOT(on_loadBtn_clicked()));


    connect(setShutterSpeed, SIGNAL(error(QString)), msgPoster, SIGNAL(error(QString)));
    connect(setIsoSpeedRate, SIGNAL(error(QString)), msgPoster, SIGNAL(error(QString)));

    connect(actTakePicture, SIGNAL(havePostViewUrl(QString)), this, SLOT(onPostView(QString)));
    connect(actTakePicture, SIGNAL(error(QString)), msgPoster, SIGNAL(error(QString)));

    connect(startBulbShooting, SIGNAL(error(QString)), msgPoster, SIGNAL(error(QString)));
    connect(stopBulbShooting, SIGNAL(error(QString)), msgPoster, SIGNAL(error(QString)));

    connect(bulbShootSequencer, SIGNAL(statusMessage(QString)), msgPoster, SIGNAL(info(QString)));
    connect(bulbShootSequencer, SIGNAL(havePostViewUrl(QString, int, int)), this, SLOT(onPostView(QString, int, int)));
    connect(bulbShootSequencer, SIGNAL(started()), this, SLOT(shootSequencerStarted()));
    connect(bulbShootSequencer, SIGNAL(stopped()), this, SLOT(shootSequencerStopped()));
    connect(bulbShootSequencer, SIGNAL(updateStatus(QString)), ui->status, SLOT(setText(QString)));

    connect(normalShootSequencer, SIGNAL(statusMessage(QString)), msgPoster, SIGNAL(info(QString)));
    connect(normalShootSequencer, SIGNAL(havePostViewUrl(QString,int,int)), this, SLOT(onPostView(QString,int,int)));
    connect(normalShootSequencer, SIGNAL(started()), this, SLOT(shootSequencerStarted()));
    connect(normalShootSequencer, SIGNAL(stopped()), this, SLOT(shootSequencerStopped()));
    connect(normalShootSequencer, SIGNAL(updateStatus(QString)), ui->status, SLOT(setText(QString)));



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
    newInfo.setSubject(ui->objectLineEdit->text());
    newInfo.setShutterSpeed(ui->shutterSpeed->currentText());
    newInfo.setShutterSpeedBulbMs(ui->shutterSpeedTuBtn->getValueInMilliseconds());
    newInfo.setIso(ui->isoSpeedRate->currentText().toInt(&ok));
    QDateTime ts = QDateTime::currentDateTime();
    newInfo.setTimestamp(ts);
    newInfo.setUrl(url);

    Q_EMIT newPostViewInfo(newInfo);

    Q_EMIT msgPoster->info(tr("have new image: %0").arg(url));
    //    ui->imageSubTitle->setText(ts.toString("yyyy-MM-ddTHH:mm:ss:zzz"));
    Sender::get()->loadPostViewImage(url);
}

void ControlWidget::onPostView(const QString& url, int i, int numShots)
{
    bool ok = false;

    PostView::Info newInfo;
    newInfo.setSubject(ui->objectLineEdit->text());
    newInfo.setShutterSpeed(ui->shutterSpeed->currentText());
    newInfo.setShutterSpeedBulbMs(ui->shutterSpeedTuBtn->getValueInMilliseconds());
    newInfo.setIso(ui->isoSpeedRate->currentText().toInt(&ok));
    QDateTime ts = QDateTime::currentDateTime();
    newInfo.setTimestamp(ts);
    newInfo.setUrl(url);
    newInfo.setSeqNr(i);
    newInfo.setNumShots(numShots);

    Q_EMIT newPostViewInfo(newInfo);


    Q_EMIT msgPoster->info(tr("have new image: %0 (%1/%2)").arg(url).arg(i).arg(numShots));
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
    if(QDir dir(Protocol::getProtocolPath()); dir.exists())
    {
        QFileInfoList protocolFilePaths { dir.entryInfoList(QStringList() << "*.xml", QDir::Files, QDir::Name) };
        for(const QFileInfo& fi : protocolFilePaths)
        {
            Protocol* p { new Protocol(fi.absoluteFilePath(), this) };
            QFile f(fi.absoluteFilePath());
            f.open(QIODevice::ReadOnly | QIODevice::Text);
            p->deSerializeXml(f.readAll());
            protocolModel->addProtocol(p);
        }
    }
}

void ControlWidget::uiToProtocol(Protocol* protocol) const
{
    Q_ASSERT(protocol);
    if(!protocol)
        return;

    AB_DBG("UI to protocol: " << protocol->getObjectName());

    protocol->setCurrentPhotoShotType(PhotoShot::typeFromString(ui->type->currentText()));

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

    protocol->setProperties(properties);




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

void ControlWidget::protocolToUi(Protocol *protocol)
{
    Q_ASSERT(protocol);
    if(!protocol)
    {
        Q_EMIT msgPoster->error(tr("Cannot apply settings: no data available"));
        AB_WRN("Protocol missing");
        return;
    }

    AB_DBG("protocol to UI: " << protocol->getObjectName());


    ui->objectLineEdit->setText(protocol->getObjectName());

    ui->type->setCurrentText(PhotoShot::typeToString(protocol->getCurrentPhotoShotType()));

    const Properties& p = protocol->getProperties();
    ui->shutterSpeed->setCurrentText(p.shutterSpeed);
    ui->isoSpeedRate->setCurrentText(p.iso);

    ui->shutterSpeedTuBtn->setCurrentUnit(p.shutterSpeedBulbUnit);
    ui->shutterSpeedTuBtn->setValueInMilliseconds(p.shutterSpeedBulb);

    ui->startDelayTuBtn->setCurrentUnit(p.startDelayUnit);
    ui->startDelayTuBtn->setValueInMilliseconds(p.startDelay);

    ui->pauseTuBtn->setCurrentUnit(p.pauseUnit);
    ui->pauseTuBtn->setValueInMilliseconds(p.pause);

    ui->numShots->setValue(p.numShots);

    referenceMarkers = protocol->getReferenceMarkers();

    setShutterSpeed->setShutterSpeed(ui->shutterSpeed->currentText());
    Sender::get()->send(setShutterSpeed);
    setIsoSpeedRate->setIsoSpeedRate(ui->isoSpeedRate->currentText());
    Sender::get()->send(setIsoSpeedRate);
}

void ControlWidget::shootSequencerStarted()
{
    ui->startBulbSequence->setText(tr("Stop sequence"));
//    ui->type->setEnabled(false);
}

void ControlWidget::shootSequencerStopped()
{
    Q_ASSERT(currentProtocol);
    if(!currentProtocol)
    {
        Q_EMIT msgPoster->error(tr("Cannot handle \"sequencer stopped\" event: missing protocol data."));
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

    Q_EMIT msgPoster->info(tr("shoot sequence stopped."));
    ui->startBulbSequence->setText(tr("Start sequence"));
    ui->type->setEnabled(true);
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
        Q_EMIT msgPoster->error("No active protocol ... cannot start any sequence");
        AB_WRN("No active protocol ... cannot start any sequence");
        return;
    }

    uiToProtocol(currentProtocol);
    protocolToUi(currentProtocol);
    setCurrentProtocol(currentProtocol);


    Q_EMIT msgPoster->info("-------------------------------------------------------------");
    Q_EMIT msgPoster->info(tr("start sequence at %0").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")));

    int duration = 0;


    QTime dt = QTime(0,0,0,0).addMSecs(duration);
    Q_EMIT msgPoster->info(
                tr("estimated duration: %0h %1min %2sec %3msec")
                .arg(dt.hour()).arg(dt.minute()).arg(dt.second()).arg(dt.msec()));

    Q_EMIT msgPoster->info(
                tr("estimated finish time: %0")
                .arg(QDateTime::currentDateTime()
                     .addMSecs(duration).toString("yyyy-MM-ddTHH:mm:ss:zzz")));
    Q_EMIT msgPoster->info("-------------------------------------------------------------");

    Base* sequencer = getActiveSequencer();
    Q_ASSERT(sequencer);
    if(!sequencer)
    {
        Q_EMIT msgPoster->error(tr("No sequencer available"));
        AB_WRN("no sequencer");
        return;
    }

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
       // ui->shutterSpeedBulb->setEnabled(false);
      //  ui->shutterSpeedTuBtn->setEnabled(false);
    }

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

    protocolToUi(selProtocol);
    setCurrentProtocol(selProtocol);


    ui->tabWidget->setCurrentWidget(ui->mainTab);

}


void ControlWidget::handleCameraStatus(const QString & status)
{
    if(status == "IDLE")
    {
        ui->settingsGroup->setEnabled(true);
    }
    else
    {
     //   ui->settingsGroup->setEnabled(false);
    }
}

void ControlWidget::on_newBtn_clicked()
{

    Q_ASSERT(currentProtocol == nullptr || !currentProtocol->isRecording());
    if(currentProtocol != nullptr)
    {
        if(currentProtocol->isRecording())
        {
            Q_EMIT msgPoster->error(tr("Cannot create sequence: another seuqence is running"));
            AB_WRN("already have a running sequence");
            return;
        }

        currentProtocol.clear();
    }

    CreateNewDialog dlg(this);
    if(dlg.exec() == QDialog::Accepted)
    {
        Protocol* newProtocol = new Protocol(dlg.getObject(), this);
        uiToProtocol(newProtocol);
        setCurrentProtocol(newProtocol);

        ui->objectLineEdit->setText(newProtocol->getObjectName());


        QMessageBox::information(this, tr("New sequence"), tr("New sequence \"%0\" created.").arg(currentProtocol->getObjectName()));
    }
}


void ControlWidget::on_shutterSpeed_textActivated(const QString &speed)
{
  AB_DBG(__PRETTY_FUNCTION__);
  setShutterSpeed->setShutterSpeed(speed);
  Sender::get()->send(setShutterSpeed);
}

} // namespace Sequencer








