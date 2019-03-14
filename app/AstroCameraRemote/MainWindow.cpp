#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AstroBase.h"

#include "Helper.h"
#include "Sender.h"
#include "SettingsDialog.h"
#include "LiveView_Widget.h"
#include "hfd/Hfd_Calculator.h"
#include "StarTrack_LenseGraphcisScene.h"
#include "BatchProcess_FitsRepair.h"
#include "BatchProcess_SelectFilesDialog.h"
#include "StarTrack_StarInfo.h"
#include "MessagePoster.h"

#include <QMessageBox>
#include <QRegExp>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

class TextEditMessageHandler : public MessageHandler
{
    QTextEdit* target;

public :
    TextEditMessageHandler(QTextEdit* target)
        : target(target)
    {

    }

    void infoMessage(QString msg)
    {
        target->append(msg);
    }

    void warningMessage(QString msg)
    {
        target->append(msg);
    }

    void errorMessage(QString msg)
    {
        target->append(msg);
    }
};

void MainWindow::closeEvent(QCloseEvent *event)
{
    settings->save();


    if(ui->actionToggleRecordMode->isChecked())
    {
        Sender::get()->send(stopRecMode);

        QTimer* forceCloseTimer = new QTimer(this);

        aboutToClose = true;
        event->ignore();

        forceCloseTimer->singleShot(
                    1000
                    , [this]()
        {
            this->ui->actionToggleRecordMode->toggle();
            this->close();
        });

    }
    else
    {
        if(fullScreenStarTrackView)
        {
            fullScreenStarTrackView->close();
            delete fullScreenStarTrackView;
            fullScreenStarTrackView = nullptr;
        }
        StatusPoller::get()->stop();
        event->accept();
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , settings(new Settings(this))
    , startRecMode(new Json::StartRecMode(this))
    , stopRecMode(new Json::StopRecMode(this))
    , fullScreenStarTrackView(nullptr)
    , connectionState(State_NotConnected)
    , aboutToClose(false)
    , currentTimeDisplayTimer(new QTimer(this))
    , ui(new Ui::MainWindow)
{
    AB_INF("start ...");

    ui->setupUi(this);

    messageHandler = new TextEditMessageHandler(ui->output);

    ui->sequencerControl->setMsgHandler(messageHandler);


    connect(currentTimeDisplayTimer, SIGNAL(timeout()), this, SLOT(updateCurrentTimeDisplay()));
    currentTimeDisplayTimer->start(200);

    connect(Sender::get(), SIGNAL(loadedPostViewImage(QByteArray)), ui->postViewWidget, SLOT(updatePostViewImage(QByteArray)));
    connect(ui->sequencerControl, SIGNAL(newPostViewInfo(PostView::Info)), ui->postViewWidget, SLOT(newInfo(PostView::Info)));

    connect(
                ui->liveViewWidget->getStarTrackScene()
                , SIGNAL(newReferenceMarkers(const QList<QRectF>&))
                , ui->sequencerControl
                , SLOT(handleNewReferenceMarkers(const QList<QRectF>&)));

    connect(
                ui->sequencerControl
                , SIGNAL(newReferenceMarkers(const QList<QRectF>&))
                , ui->liveViewWidget->getStarTrackScene()
                , SLOT(applyReferenceMarkers(const QList<QRectF>&)));

    connect(
                ui->postViewWidget->getStarTrackScene()
                , SIGNAL(newReferenceMarkers(const QList<QRectF>&))
                , ui->sequencerControl
                , SLOT(handleNewReferenceMarkers(const QList<QRectF>&)));

    connect(
                ui->sequencerControl
                , SIGNAL(newReferenceMarkers(const QList<QRectF>&))
                , ui->postViewWidget->getStarTrackScene()
                , SLOT(applyReferenceMarkers(const QList<QRectF>&)));

    connect(StatusPoller::get(), SIGNAL(statusChanged(QString)), this, SLOT(handleCameraStatus(QString)));
    connect(StatusPoller::get(), SIGNAL(isoSpeedRatesChanged(QStringList,QString))
            , this, SLOT(isoSpeedRatesChanged(QStringList,QString)));
    connect(StatusPoller::get(), SIGNAL(shutterSpeedsChanged(QStringList,QString))
            , this, SLOT(shutterSpeedsChanged(QStringList,QString)));
    connect(StatusPoller::get(), SIGNAL(batteryStatusChanged()), this, SLOT(updateBatteryStatus()));

    connect(ui->actionToggleRecordMode, SIGNAL(toggled(bool)), this, SLOT(toggleRecordModeBtn(bool)));
    connect(startRecMode, SIGNAL(confirmed()), this, SLOT(toggleRecordModeBtnStarted()));
    connect(startRecMode, SIGNAL(confirmed()), this, SLOT(hello()));
    connect(startRecMode, SIGNAL(error(QString)), this, SLOT(helloError(QString)));

    connect(stopRecMode, SIGNAL(confirmed()), this, SLOT(toggleRecordModeBtnStopped()));
    connect(stopRecMode, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(this, SIGNAL(connectedToCamera()), ui->liveViewWidget, SLOT(start()));
    connect(this, SIGNAL(disconnectedFromCamera()), ui->liveViewWidget, SLOT(stop()));


    setupStarTrackView(ui->starTrackView);

    on_viewsTabWidget_currentChanged(ui->viewsTabWidget->currentIndex());


    Sender::get()->send(startRecMode);

}

MainWindow::~MainWindow()
{
    if(fullScreenStarTrackView)
        delete fullScreenStarTrackView;

    if(messageHandler)
        delete messageHandler;

    delete ui;
}

void MainWindow::isoSpeedRatesChanged(const QStringList &candidates, const QString& current)
{
    if(connectionState & State_IsoSpeeds)
        return;

    ui->sequencerControl->isoSpeedRatesChanged(candidates, current);


    connectionState |= State_IsoSpeeds;
    connectionStateChanged();
}

void MainWindow::shutterSpeedsChanged(const QStringList &candidates, const QString& current)
{
    if(connectionState & State_ShutterSpeeds)
        return;

    ui->sequencerControl->shutterSpeedsChanged(candidates, current);

    connectionState |= State_ShutterSpeeds;
    connectionStateChanged();
}

void MainWindow::toggleStarTrackViewFullScreen(bool yes)
{
    if(yes)
    {
        if(!fullScreenStarTrackView)
        {
            fullScreenStarTrackView = new StarTrack::StarTrackView(nullptr, true);
            setupStarTrackView(fullScreenStarTrackView);
        }

        fullScreenStarTrackView->showFullScreen();
        fullScreenStarTrackView->applyStatusFrom(ui->starTrackView);
    }
    else
    {
        if(fullScreenStarTrackView)
        {
            ui->starTrackView->applyStatusFrom(fullScreenStarTrackView);
            fullScreenStarTrackView->close();
            fullScreenStarTrackView->deleteLater();
            fullScreenStarTrackView = nullptr;
        }
    }


    ui->starTrackView->fullScreenToggled(yes);

}

void MainWindow::toggleRecordModeBtn(bool on)
{
    if(on)
    {
        ui->output->append(tr("turning on record mode"));
        Sender::get()->send(startRecMode);
    }
    else
    {
        ui->output->append(tr("turning off record mode"));
        Sender::get()->send(stopRecMode);
    }
}



void MainWindow::toggleRecordModeBtnStopped()
{
    ui->actionToggleRecordMode->setChecked(false);
    ui->output->append(tr("record mode turned off"));
    if(aboutToClose)
        close();
}

void MainWindow::toggleRecordModeBtnStarted()
{
    ui->actionToggleRecordMode->setChecked(true);
    ui->output->append(tr("record mode turned on"));
}

void MainWindow::connectionStateChanged()
{
    if(connectionState == State_Completed)
    {
        settings->load();
        connectionState |= State_SettingsLoaded;
    }

    if(connectionState == State_NotConnected)
        Q_EMIT disconnectedFromCamera();
    else
        Q_EMIT connectedToCamera();
}

void MainWindow::setupStarTrackView(StarTrack::StarTrackView *v)
{
    connect(ui->postViewWidget->getStarTrackScene(), SIGNAL(starCentered(QImage)), v, SLOT(updateStar(QImage)));
    connect(ui->postViewWidget->getStarTrackScene(), SIGNAL(newHfdValue(StarInfoPtr)), v, SLOT(updateHfdValue(StarInfoPtr)));
    connect(ui->liveViewWidget->getStarTrackScene(), SIGNAL(starCentered(QImage)), v, SLOT(updateStar(QImage)));
    connect(ui->liveViewWidget->getStarTrackScene(), SIGNAL(newHfdValue(StarInfoPtr)), v, SLOT(updateHfdValue(StarInfoPtr)));
    connect(v, SIGNAL(trackingEnabledStatusToggled(bool)), ui->postViewWidget->getStarTrackScene(), SIGNAL(starTrackingEnabled(bool)));
    connect(v, SIGNAL(trackingEnabledStatusToggled(bool)), ui->liveViewWidget->getStarTrackScene(), SIGNAL(starTrackingEnabled(bool)));
    connect(v, SIGNAL(updateMarker()), ui->liveViewWidget->getStarTrackScene(), SLOT(updateMarker()));
    connect(v, SIGNAL(updateMarker()), ui->postViewWidget->getStarTrackScene(), SLOT(updateMarker()));
    connect(v, SIGNAL(toggleFullScreen(bool)), this, SLOT(toggleStarTrackViewFullScreen(bool)));
}


void MainWindow::hello()
{
    ui->centralWidget->setEnabled(true);

    if(!StatusPoller::get()->isActive())
    {
        StatusPoller::get()->start(1);
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
            Sender::get()->send(startRecMode);
        }
    }
    while(QMessageBox::Yes == btn && State_NotConnected != connectionState);

    connectionState &= ~State_Hello;
    connectionStateChanged();
}

void MainWindow::error(QString msg)
{
    ui->output->append(msg);
}







void MainWindow::appendOutputMessage(QString msg)
{
    ui->output->append(tr("%0: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")).arg(msg));
}

void MainWindow::handleCameraStatus(QString status)
{
    ui->cameraStatus->setText(status);
}


void MainWindow::updateCurrentTimeDisplay()
{
    QDateTime now = QDateTime::currentDateTime();

    ui->currentDate->setText(now.toString("dd. MM. yyyy"));
    ui->currentTime->setText(now.toString("HH:mm:ss:zzz"));
}

void MainWindow::updateBatteryStatus()
{
    BatteryInfo info = StatusPoller::get()->getBatteryInfo();
    ui->batteryStatus->setMinimum(0);
    ui->batteryStatus->setMaximum(info.getLevelDenom());
    ui->batteryStatus->setValue(info.getLevelNumber());
    double load = static_cast<double>(info.getLevelNumber()) / info.getLevelDenom();

    if(0.001 > load || info.getAdditionalStatus() == BatteryInfo::AdditionalStatus_batteryNearEnd)
        ui->batteryStatus->setStyleSheet(BatteryInfo::getStyleCritical());
    else if(0.3 > load)
        ui->batteryStatus->setStyleSheet(BatteryInfo::getStyleLow());
    else
        ui->batteryStatus->setStyleSheet(BatteryInfo::getStyleNormal());


}



void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog dlg(settings, this);
    dlg.exec();

}

void MainWindow::on_lenrCheckbox_clicked(bool checked)
{
    GeneralSettings::setLenrEnabled(checked);
}


void MainWindow::on_viewsTabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    if(ui->viewsTabWidget->currentWidget() == ui->liveViewWidget)
    {
        ui->postViewWidget->setEnabled(false);
        ui->liveViewWidget->start();
    }
    else if(ui->viewsTabWidget->currentWidget() == ui->postViewWidget)
    {
        ui->postViewWidget->setEnabled(true);
        ui->liveViewWidget->stop();
    }
}

void MainWindow::on_scaledImageCheckbox_toggled(bool checked)
{
    StarTrack::Settings::setPublishScaledImage(checked);
    ui->postViewWidget->getStarTrackScene()->setPublishScaledImage(checked);
    ui->liveViewWidget->getStarTrackScene()->setPublishScaledImage(checked);
}

void MainWindow::on_actionFits_repair_triggered()
{
    BatchProcess::SelectFilesDialog dlg(this, "FITS (*.fits *.fts *.fit)");
    BatchProcess::FitsRepair repairProcessor;
    dlg.setTaskPrototype(&repairProcessor);
    dlg.exec();
}
