#include "BatchProcess_ImageSignal.h"

#include "AstroBase_Exception.h"
#include "AstroBase_PersistentDirInfo.h"
#include "BatchProcess_Manager.h"

#include <QFileDialog>

namespace BatchProcess {

ImageSignal::ImageSignal(const QString &name)
    : Signal(name)
{

}

ImageSignal::ImageSignal(const ImageSignal &rhs, bool deep)
{
    if(deep)
    {
        foreach(Fits::FilePtr file, rhs.images)
        {
            images << Fits::FilePtr(new Fits::File(*file.data()));
        }
    }
    else
    {
        images.reserve(rhs.images.count());
        std::copy(rhs.images.begin(), rhs.images.end(), images.begin());
    }
}

ImageSignal::~ImageSignal()
{

}

ImageSignalPtr ImageSignal::clone() const
{
    return ImageSignalPtr(new ImageSignal(*this));
}

int ImageSignal::numImages() const
{
    return images.count();
}

bool ImageSignal::operator=(const ImageSignal &rhs)
{
    Q_UNUSED(rhs);
    return false;
}

double ImageSignal::getPixel(int imageIndex, int pixelIndex) const
{
    if(0 > imageIndex || images.count() <= imageIndex)
        throw AstroBase::IndexOutOfBoundsException(tr("image index (%0)").arg(imageIndex));

    return images[imageIndex]->getPixel(pixelIndex);
}

void ImageSignal::setPixel(int imageIndex, int pixelIndex, const double& value)
{
    if(0 > imageIndex || images.count() <= imageIndex)
        throw AstroBase::IndexOutOfBoundsException(tr("image index (%0)").arg(imageIndex));

    images[imageIndex]->setPixel(pixelIndex, value);
}

QString ImageSignal::getTitle() const
{
    return tr("%0 (%1 files)").arg(getName()).arg(files.count());
}

bool ImageSignal::edit()
{
    QStringList files = Manager::get()->getImageFilesGui();
    if(files.isEmpty())
        return false;

    this->files = files;
    return true;
}

const QList<Fits::FilePtr> &ImageSignal::getImages() const
{
    return images;
}

void ImageSignal::setImages(const QList<Fits::FilePtr> &value)
{
    images = value;
}

} // namespace BatchProcess
