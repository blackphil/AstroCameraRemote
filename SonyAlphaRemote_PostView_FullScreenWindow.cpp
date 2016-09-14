#include "SonyAlphaRemote_PostView_FullScreenWindow.h"
#include "ui_SonyAlphaRemote_PostView_FullScreenWindow.h"

namespace SonyAlphaRemote {
namespace PostView {

FullScreenWindow::FullScreenWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullScreenWindow)
{
    ui->setupUi(this);
}

FullScreenWindow::~FullScreenWindow()
{
    delete ui;
}

} // namespace PostView
} // namespace SonyAlphaRemote
