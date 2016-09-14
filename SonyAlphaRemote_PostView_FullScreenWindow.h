#ifndef SONYALPHAREMOTE_POSTVIEW_SONYALPHAREMOTE_POSTVIEW_FULLSCREENWINDOW_H
#define SONYALPHAREMOTE_POSTVIEW_SONYALPHAREMOTE_POSTVIEW_FULLSCREENWINDOW_H

#include <QWidget>

namespace SonyAlphaRemote {
namespace PostView {

namespace Ui {
class FullScreenWindow;
}

class FullScreenWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FullScreenWindow(QWidget *parent = 0);
    ~FullScreenWindow();

private:
    Ui::FullScreenWindow *ui;
};


} // namespace PostView
} // namespace SonyAlphaRemote
#endif // SONYALPHAREMOTE_POSTVIEW_SONYALPHAREMOTE_POSTVIEW_FULLSCREENWINDOW_H
