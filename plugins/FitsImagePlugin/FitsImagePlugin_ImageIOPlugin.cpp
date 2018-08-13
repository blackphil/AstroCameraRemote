#include "FitsImagePlugin_ImageIOPlugin.h"

#include <QStringList>
#include "AstroBase_Exception.h"
#include "FitsImagePlugin_ImageIOHandler.h"
#include "Fits_File.h"

namespace Fits {

FitsImagePlugin::FitsImagePlugin()
{

}

QImageIOPlugin::Capabilities FitsImagePlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    static const QStringList supportedFormats = { "fits", "fts", "fit" };

    Capabilities cap;

    if (supportedFormats.contains(QString(format)))
        return Capabilities(CanRead | CanWrite);
    if (!format.isEmpty())
        return cap;
    if (!device->isOpen())
        return cap;

    if (device->isReadable() && Fits::File::isValid(device))
        cap |= CanRead;
    if (device->isWritable())
        cap |= CanWrite;
    return cap;

}

QImageIOHandler *FitsImagePlugin::create(QIODevice *device, const QByteArray &format) const
{
    ImageIOHandler* handler = new ImageIOHandler();
    handler->setDevice(device);
    handler->setFormat(format);

    return handler;
}

} // namespace Fits

