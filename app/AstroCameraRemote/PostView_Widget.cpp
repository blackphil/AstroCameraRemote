#include "PostView_Widget.h"
#include "ui_PostView_Widget.h"
#include "SonyAlphaRemote_Helper.h"
#include "StarTrack_GraphicsScene.h"

#include "hfd/Hfd_Calculator.h"
#include "Fits_File.h"
#include "AstroBase_PersistentDirInfo.h"
#include "AstroBase_Exception.h"
#include "Fits_ImageIOPlugin.h"
#include "Fits_ImageIOHandler.h"
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
    , starTrackScene(new StarTrack::GraphicsScene(this))
    , ui(new Ui::Widget)
{
    SAR_INF("ctor");
    ui->setupUi(this);
    ui->graphicsView->setScene(starTrackScene);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updatePostViewImage(const QByteArray &data)
{
    QImage img = QImage::fromData(data);
    updatePostViewImage(QPixmap::fromImage(img));
}

void Widget::updatePostViewImage(const QPixmap &pixmap)
{
    //    Q_ASSERT(!imageStack.isEmpty());
    if(imageStack.isEmpty())
    {
        SAR_ERR(tr("post view image stack is empty!!!"));
        starTrackScene->updateBackground(pixmap);
        return;
    }


    imageStack.back().setImage(pixmap);
    cursor = imageStack.count()-1;
    updatePostView();

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
    imageStack.push_back(info);
    if(!info.getImage().isNull())
        updatePostViewImage(info.getImage());
}

void Widget::on_postViewFwd_clicked()
{
    int nextPos = qMin(cursor+1, imageStack.count()-1);
    if(cursor != nextPos)
    {
        cursor = nextPos;
        updatePostView();
    }
}

void Widget::on_postViewBwd_clicked()
{
    int nextPos = qMax(cursor-1, 0);
    if(cursor != nextPos)
    {
        cursor = nextPos;
        updatePostView();
    }
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
            newInfo(dummyInfo);
            //        SAR_INF(fn << " (" << dummyInfo.getTimestamp().toString("HH:mm:ss") << ")");

            if(!fileInfo.exists())
            {
                SAR_ERR("file " << fn << " doesn't exist!");
                continue;
            }

            QString filePath = fileInfo.absoluteFilePath();

            QFile f(filePath);
            if(!f.open(QIODevice::ReadOnly))
            {
                SAR_ERR("could not open file " << fn);
                continue;
            }
            QByteArray data = f.readAll();
            if(data.size() == 0)
            {
                SAR_ERR("no data read from file " << fn);
                continue;
            }

            updatePostViewImage(data);
        }
        catch(std::exception& e)
        {
            errorMsgBox->showAgain(e.what());
        }
        progress.setValue(index);
    }

    cursor = 0;
    updatePostView();
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

} // namespace PostView


void PostView::Widget::on_clearBtn_clicked()
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
