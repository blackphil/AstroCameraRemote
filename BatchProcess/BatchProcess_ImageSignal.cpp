#include "BatchProcess_ImageSignal.h"

#include "AstroBase_Exception.h"
#include "AstroBase_PersistentDirInfo.h"
#include "BatchProcess_Manager.h"

#include <QFileDialog>

namespace BatchProcess {

ImageSignal::ImageSignal(Direction direction, const QString &name)
    : Signal(direction, name)
{

}

ImageSignal::ImageSignal(const ImageSignal &rhs, bool deep)
    : Signal(rhs)
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

void ImageSignal::connectToSignal(SignalPtr other)
{
    ImageSignalPtr otherImageSignal = other.dynamicCast<ImageSignal>();
    if(!otherImageSignal)
        throw AstroBase::Exception(tr("Cannot connect signal %0 to %1: invalid type").arg(getName()).arg(other->getName()));

    otherImageSignal->setImages(images);
    setConnectedTo(otherImageSignal);
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
