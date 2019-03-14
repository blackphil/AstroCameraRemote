#include "ImageViewZoomControl.h"
#include "ui_ImageViewZoomControl.h"

#include "ImageView.h"

ImageViewZoomControl::ImageViewZoomControl(QWidget *parent)
    : QWidget(parent)
    , iv(nullptr)
    , ui(new Ui::ImageViewZoomControl)
{
    ui->setupUi(this);
}

ImageViewZoomControl::~ImageViewZoomControl()
{
    delete ui;
}

void ImageViewZoomControl::setup(ImageView *iv)
{
    if(this->iv)
    {
        disconnect(iv, nullptr, this, nullptr);
        disconnect(this, nullptr, iv, nullptr);
    }

    this->iv = iv;

    if(iv)
    {
        connect(ui->zoomIn, SIGNAL(clicked()), iv, SLOT(zoomIn()));
        connect(ui->zoomOut, SIGNAL(clicked()), iv, SLOT(zoomOut()));
        connect(ui->zoom1to1, SIGNAL(clicked()), iv, SLOT(zoom1to1()));
        connect(ui->zoomFit, SIGNAL(toggled(bool)), iv, SLOT(fitToWindow(bool)));

        connect(
                    iv
                    , &ImageView::zoomed
                    , [this](bool yes)
        {
            ui->zoomFit->setChecked(!yes);
        });
    }

}

bool ImageViewZoomControl::fitInWindowEnabled() const
{
    return ui->zoomFit->isChecked();
}
