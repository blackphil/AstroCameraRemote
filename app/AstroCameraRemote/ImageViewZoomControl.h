#ifndef IMAGEVIEWZOOMCONTROL_H
#define IMAGEVIEWZOOMCONTROL_H

#include <QWidget>

namespace Ui {
class ImageViewZoomControl;
}

class ImageView;

class ImageViewZoomControl : public QWidget
{
    Q_OBJECT

    ImageView* iv;

public:
    explicit ImageViewZoomControl(QWidget *parent = nullptr);
    ~ImageViewZoomControl();

    void setup(ImageView* iv);

    bool fitInWindowEnabled() const;

private:
    Ui::ImageViewZoomControl *ui;
};

#endif // IMAGEVIEWZOOMCONTROL_H
