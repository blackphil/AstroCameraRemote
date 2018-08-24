#include "BatchProcess_ImageSignal.h"

#include "AstroBase.h"
#include "AstroBase_Exception.h"
#include "AstroBase_PersistentDirInfo.h"
#include "BatchProcess_Manager.h"

#include <QFileDialog>

namespace BatchProcess {

void ImageSignal::connectedSignalChanged()
{
    ImageSignalPtr con = qobject_cast<ImageSignal*>(getConnectedTo());
    if(con)
    {
        setImages(con->getImages());
    }
}

ImageSignal::ImageSignal(Direction direction, const QString &name, QObject* parent)
    : Signal(direction, name, parent)
{

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

int ImageSignal::numPixels() const
{
    if(0 < images.count())
    {
        Fits::FilePtr image = images[0];
        if(image)
            return image->numPixels();
    }

    return 0;
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
    return tr("%0 (%1 files)").arg(getName()).arg(images.count());
}

void ImageSignal::connectToSignal(Signal *other)
{
    ImageSignalPtr otherImageSignal = qobject_cast<ImageSignal*>(other);
    if(!otherImageSignal)
        throw AstroBase::Exception(tr("Cannot connect signal %0 to %1: invalid type").arg(getName()).arg(other->getName()));

    otherImageSignal->setImages(images);
    connect(otherImageSignal, SIGNAL(signalChanged()), this, SLOT(connectedSignalChanged()));
    setConnectedTo(other);
}

bool ImageSignal::edit()
{
    QStringList files = Manager::get()->getImageFilesGui();
    if(files.isEmpty())
        return false;

    foreach(QString fileName, files)
        images << Fits::FilePtr(new Fits::File(fileName, false));

    AB_DBG("signal \"" << getName() << "\" has" << numImages() << "images");
    Signal::edit();
    return true;
}

const QList<Fits::FilePtr> &ImageSignal::getImages() const
{
    return images;
}

void ImageSignal::setImages(const QList<Fits::FilePtr> &value)
{
    images = value;
    Q_EMIT signalChanged();
}

} // namespace BatchProcess
