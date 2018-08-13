#include "BatchProcess_ImageSignal.h"

namespace BatchProcess {

ImageSignal::ImageSignal(const QString &name)
    : name(name)
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

QString ImageSignal::getName() const
{
    return name;
}

void ImageSignal::setName(const QString &value)
{
    name = value;
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
