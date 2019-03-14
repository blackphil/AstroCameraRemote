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
#include <QTransform>
#include <QSettings>

namespace PostView {

StarTrack::GraphicsScene *Widget::getStarTrackScene() const
{
    return starTrackScene;
}

bool Widget::getEnabled() const
{
    return enabled;
}

void Widget::setEnabled(bool value)
{
    enabled = value;
    starTrackScene->setEnabled(enabled && !imageStack.isEmpty());
}

void Widget::resizeEvent(QResizeEvent *re)
{
    if(ui->zoomControl->fitInWindowEnabled())
    {
        ui->graphicsView->fitToWindow(true);
    }
    QWidget::resizeEvent(re);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , cursor(-1)
    , refImageIndex(-1)
    , starTrackScene(new StarTrack::GraphicsScene(this))
    , ui(new Ui::Widget)
{
    AB_INF("ctor");
    ui->setupUi(this);
    ui->graphicsView->setScene(starTrackScene);

    ui->zoomControl->setup(ui->graphicsView);

    reset();
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
        AB_ERR(tr("post view image stack is empty!!!"));
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
        starTrackScene->setEnabled(enabled);
        const Info& info = imageStack[cursor];
        const QPixmap& image = info.getImage();
        ui->postViewMetaInfo->setText(info.toHtml());
        if(!image.isNull())
        {
            starTrackScene->updateBackground(image);
        }

        ui->refImg->setEnabled(true);
        ui->setAsRefImg->setEnabled(true);
    }
    else
    {
        starTrackScene->setEnabled(false);
        ui->refImg->setEnabled(false);
        ui->setAsRefImg->setEnabled(false);
    }

    if(cursor >= imageStack.count()-1)
    {
        ui->postViewFwd->setEnabled(false);
        ui->latestImg->setEnabled(false);
    }
    else
    {
        ui->postViewFwd->setEnabled(true);
        ui->latestImg->setEnabled(true);
    }

    if(cursor <= 0)
    {
        ui->postViewBwd->setEnabled(false);
        ui->firstImg->setEnabled(false);
    }
    else
    {
        ui->postViewBwd->setEnabled(true);
        ui->firstImg->setEnabled(true);
    }

}

void Widget::newInfo(const Info &info)
{
    imageStack.push_back(info);

    if(info.getImage().isNull())
        return;

    if(ui->autoFwd->isChecked())
        on_latestImg_clicked();
}

void Widget::reset()
{
    imageStack.clear();
    cursor = -1;
    refImageIndex = -1;
    starTrackScene->cleanUpMarkers();

    QFile defaultImage(":/images/LiveView_NoImage.jpg");
    defaultImage.open(QIODevice::ReadOnly);
    QByteArray imageData = defaultImage.readAll();
    starTrackScene->updateBackground(QPixmap::fromImage(QImage::fromData(imageData)));
    updatePostView();
}

void Widget::newHfdValue(StarTrack::StarInfoPtr starInfo)
{
    if(0 <= cursor && cursor < imageStack.count())
    {
        imageStack[cursor].setStarInfo(StarTrack::StarInfoPtr(new StarTrack::StarInfo(*starInfo)));
    }
}

void Widget::setImage(int index)
{
    Q_ASSERT(0 <= index && index < imageStack.count());
    if(0 > index || imageStack.count() <= index)
        return;

//    if(cursor == index)
//        return;

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

    QDir dir = QDir(":/hfd/sequence");
    QStringList entries = dir.entryList(QDir::Files, QDir::Time);
    for(int i=0; i<entries.count(); i++)
    {
        entries[i].prepend("/").prepend(dir.absolutePath());
    }
    this->loadFiles(entries);
}

void Widget::loadFiles(const QStringList &files)
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

            QFileInfo fileInfo(fn);
            Info dummyInfo;
            dummyInfo.setSubject(tr("test image"));
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

            QImage image = QImage::fromData(data);
            if(image.height() > image.width())
            {
                QTransform t;
                t.rotate(90, Qt::ZAxis);
                image = image.transformed(t);
            }

            dummyInfo.setImage(QPixmap::fromImage(image));
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

    QStringList filters;
    filters << "FITS (*.fit *.fts *.fits)"
            << "JPEG (*.jpg *.jpeg)"
            << "TIFF (*.tif *.tiff)"
            << "All files(*.*)";



    QString selectedFilter = QSettings().value("PostView/OpenFilesFilter", filters[0]).toString();


    QStringList files = QFileDialog::getOpenFileNames(
                this
                , tr("Open files")
                , dir
                , filters.join(";;")
                , &selectedFilter);

    if(files.isEmpty())
        return;

    QSettings().setValue("PostView/OpenFilesFilter", selectedFilter);

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
        reset();
    }
}

void Widget::on_setAsRefImg_clicked()
{
    refImageIndex = cursor;
    starTrackScene->setReference();
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
