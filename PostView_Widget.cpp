#include "PostView_Widget.h"
#include "ui_PostView_Widget.h"
#include "SonyAlphaRemote_Helper.h"

namespace PostView {

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , cursor(0)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updatePostViewImage(const QByteArray &data)
{

    QPixmap pixmap = QPixmap::fromImage(QImage::fromData(data, "JPG"));
    updatePostViewImage(pixmap);
}

void Widget::updatePostViewImage(const QPixmap &pixmap)
{
    Q_ASSERT(!imageStack.isEmpty());
    if(imageStack.isEmpty())
    {
        SAR_ERR(tr("post view image stack is empty!!!"));
        ui->postViewImage->setPixmap(pixmap);
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
            ui->postViewImage->setPixmap(
                        image.scaled(ui->postViewImage->width()
                                      , ui->postViewImage->height()
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

void Widget::newInfo(const PostView::Info &info)
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

} // namespace PostView
