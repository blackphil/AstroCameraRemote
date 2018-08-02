#include "Fits_ImageIOPlugin.h"

#include <QStringList>
#include "AstroBase_Exception.h"
#include "Fits_ImageIOHandler.h"

namespace Fits {

ImageIOPlugin::ImageIOPlugin()
{

}

QImageIOPlugin::Capabilities ImageIOPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if(format != "fits")
        throw AstroBase::Exception(tr("Invalid format (%0)").arg(QString(format)));

    if(device == nullptr)
        return QImageIOPlugin::CanRead;

    QStringList nameAndValue = QString(device->peek(HeaderLineSize)).split("=");
    if(nameAndValue.count() != 2)
        throw AstroBase::Exception(tr("Invalid fits header"));

    QString name = nameAndValue[0].trimmed();
    QString value = nameAndValue[1].trimmed();

    if(!(name.toUpper() == "SIMPLE" && value.toUpper() == "T"))
        throw AstroBase::Exception(tr("Invalid fits header"));

    return CanRead | CanWrite;

}

QImageIOHandler *ImageIOPlugin::create(QIODevice *device, const QByteArray &format) const
{
    ImageIOHandler* handler = new ImageIOHandler();
    handler->setDevice(device);
    handler->setFormat(format);

    return handler;
}

} // namespace Fits

