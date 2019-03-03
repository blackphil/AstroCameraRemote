#include "PostView_Widget.h"
#include "ui_PostView_Widget.h"
#include "AstroBase.h"
#include "StarTrack_GraphicsScene.h"

#include "hfd/Hfd_Calculator.h"
#include "Fits_File.h"
#include "AstroBase_PersistentDirInfo.h"
#include "AstroBase_Exception.h"
#include "AstroBase_MessageBox.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QProgressDialog>
#include <QFileDialog>
#include <QPluginLoader>
#include <QBuffer>

namespace PostView {

StarTrack::GraphicsScene *Widget::getStarTrackScene() const
{
    return starTrackScene;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , cursor(0)
    , refImageIndex(0)
    , starTrackScene(new StarTrack::GraphicsScene(this))
    , ui(new Ui::Widget)
{
    AB_INF("ctor");
    ui->setupUi(this);
    ui->graphicsView->setScene(starTrackScene);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updatePostView()
{
    if(0 <= cursor && imageStack.count() > cursor)
    {
        const Info& info = imageStack[cursor];
        const QPixmap& image = info.getImage();
        ui->postViewMetaInfo->setText(info.toHtml());
        if(!image.isNull())
        {
            starTrackScene->updateBackground(
                        image.scaled(starTrackScene->sceneRect().toRect().size()
                                     , Qt::KeepAspectRatio
                                     , Qt::SmoothTransformation));

        }

    }

    if(cursor >= imageStack.count()-1)
        ui->postViewFwd->setEnabled(false);
    else
        ui->postViewFwd->setEnabled(true);

    if(cursor <= 0)
        ui->postViewBwd->setEnabled(false);
    else
        ui->postViewBwd->setEnabled(true);

}

void Widget::newInfo(const Info &info)
{
    if(info.getImage().isNull())
        return;

    imageStack.push_back(info);

    if(ui->autoFwd->isChecked())
        on_latestImg_clicked();
}

void Widget::setImage(int index)
{
    Q_ASSERT(0 <= index && index < imageStack.count());
    if(0 > index || imageStack.count() <= index)
        return;

    if(cursor == index)
        return;

    cursor = index;
    updatePostView();
}

void Widget::on_postViewFwd_clicked()
{
    setImage(qMin(cursor+1, imageStack.count()-1));
}

void Widget::on_postViewBwd_clicked()
{
    setImage(qMax(cursor-1, 0));
}

void Widget::on_loadTestDataBtn_clicked()
{

    QDir sequence(":/hfd/sequence");
    QStringList entries = sequence.entryList(QDir::Files, QDir::Time);


    this->loadFiles(entries, sequence);
}

void Widget::loadFiles(const QStringList &files, const QDir& mainDir)
{

    QProgressDialog progress(tr("Loading test data ..."), tr("Cancel"), 0, files.count(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    int index = 0;

    QSharedPointer<AstroBase::MessageBox> errorMsgBox(AstroBase::MessageBox::createCritical(&progress, tr("Error on loading files")));

    foreach(QString fn, files)
    {
        try
        {

            if(progress.wasCanceled())
                break;

            QImage img;

            QFileInfo fileInfo(mainDir, fn);
            Info dummyInfo;
            dummyInfo.setShutterSpeed(fileInfo.baseName());
            dummyInfo.setTimestamp(fileInfo.created());
            dummyInfo.setSeqNr(index++);
            //        AB_INF(fn << " (" << dummyInfo.getTimestamp().toString("HH:mm:ss") << ")");

            if(!fileInfo.exists())
            {
                AB_ERR("file " << fn << " doesn't exist!");
                continue;
            }

            QString filePath = fileInfo.absoluteFilePath();

            QFile f(filePath);
            if(!f.open(QIODevice::ReadOnly))
            {
                AB_ERR("could not open file " << fn);
                continue;
            }
            QByteArray data = f.readAll();
            if(data.size() == 0)
            {
                AB_ERR("no data read from file " << fn);
                continue;
            }

            dummyInfo.setImage(QPixmap::fromImage(QImage::fromData(data)));
            newInfo(dummyInfo);
        }
        catch(std::exception& e)
        {
            errorMsgBox->showAgain(e.what());
        }
        progress.setValue(index);
    }
}


void Widget::on_openFilesBtn_clicked()
{
    AstroBase::PersistentDirInfo dir("PostView?LastDir");
    QStringList files = QFileDialog::getOpenFileNames(
                this
                , tr("Open files")
                , dir
                , "FITS (*.fit *.fts *.fits)"
                  ";;JPEG (*.jpg *.jpeg)"
                  ";;TIFF (*.tif)"
                  ";;All files(*.*)");

    if(files.isEmpty())
        return;

    dir = QFileInfo(files[0]).absolutePath();
    loadFiles(files);
}



void Widget::on_clearBtn_clicked()
{
    if(QMessageBox::Yes ==
            QMessageBox::question(
                this
                , tr("Clear post view"), tr("Do you really want to clear the post view?")))
    {
        imageStack.clear();

        QFile defaultImage(":/images/LiveView_NoImage.jpg");
        defaultImage.open(QIODevice::ReadOnly);
        QByteArray imageData = defaultImage.readAll();
        QRect defaultRect(0, 0, 808, 540);

        starTrackScene->updateBackground(QPixmap::fromImage(QImage::fromData(imageData, "JPG").scaled(defaultRect.size())));
    }
}

void Widget::on_setAsRefImg_clicked()
{
    refImageIndex = cursor;
}


void Widget::on_refImg_clicked()
{
    ui->autoFwd->setChecked(false);
    setImage(refImageIndex);
}

void Widget::on_firstImg_clicked()
{
    setImage(0);
}

void Widget::on_latestImg_clicked()
{
    setImage(imageStack.count()-1);
    ui->autoFwd->setChecked(true);
}

} // namespace PostView
