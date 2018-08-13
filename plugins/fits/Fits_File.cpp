#include "Fits_File.h"

#include "AstroBase.h"

#include <QFileInfo>
#include <QIODevice>
#include <QStringList>
#include <QDir>
#include <QBuffer>
#include <QPainter>
#include <QRegularExpression>
#include <QSettings>

#include "AstroBase_Exception.h"

namespace Fits {


File::PixelFormat File::getPixelFormat() const
{
    return pixelFormat;
}

File::ColorFormat File::getColorFormat() const
{
    return colorFormat;
}

File::File()
{
    QSettings s;
    s.beginGroup("Plugins/Fits");
    QStringList bayerMaskStr = s.value("bayerMask", "r g g b").toString().split(" ");

    if(bayerMaskStr.length() == 4)
    {
        for(auto i=0; i<4; i++)
        {
            char val = bayerMaskStr[i].toLatin1()[0];
            switch(val)
            {
            case 'r' : bayerMask[i] = 0; break;
            case 'g' : bayerMask[i] = 1; break;
            case 'b' : bayerMask[i] = 2; break;
            }
        }
    }
    else
    {
        static const char defaultBayerMask[4] = { 0, 1, 1, 2 };
        memcpy(bayerMask, defaultBayerMask, 4);

        s.setValue("bayerMask", "r g g b");
    }
}

File File::fromByteArray(const QByteArray &data)
{
    QBuffer buffer;
    buffer.setData(data);
    buffer.open(QBuffer::ReadOnly);
    return fromDevice(&buffer);
}

File File::fromDevice(QIODevice *fd)
{
    File f;


    for(int l=0; l<HeaderLineCount; l++)
    {
        QString line = fd->read(HeaderLineSize);
        f.header.lines << line;
        QStringList keyVal = line.split("=");
        if(keyVal.count() == 2)
        {
            f.header.attr[keyVal[0].trimmed()] = keyVal[1].remove(QRegularExpression("/.*$")).trimmed();
        }
    }

    if(!f.header.attr.contains("BITPIX"))
        throw AstroBase::Exception(tr("cannot determine pixel format"));

    f.pixelFormat = static_cast<PixelFormat>(f.header.getIntAttr("BITPIX"));

    if(!f.header.attr.contains("NAXIS"))
        throw AstroBase::Exception(tr("cannot determine color format"));

    if(f.header.getIntAttr("NAXIS") == 2)
    {
        if(f.header.attr.contains("BAYER") && f.header.attr["BAYER"] == "RGGB")
            f.colorFormat = ColorFormat_BayerRGGB;
        else
            f.colorFormat = ColorFormat_Grayscale;
    }
    else if(f.header.getIntAttr("NAXIS") == 3)
    {
        f.colorFormat = ColorFormat_RGB;
    }
    else
        throw AstroBase::Exception(tr("cannot determine color format"));


    f.data = fd->readAll();

    AB_DBG("have" << f.data.count() << "bytes of pixel data");
    return f;
}

File File::fromFile(const QString &filePath)
{
    QFileInfo info(filePath);
    if(!info.isReadable())
    {
        throw AstroBase::Exception(tr("Cannot read file: %0").arg(filePath));
    }

    QFile in(filePath);
    if(!in.open(QIODevice::ReadOnly))
    {
        throw AstroBase::Exception(tr("Cannot open file for reading: %0").arg(filePath));
    }

    return fromDevice(&in);
}

void File::write(QIODevice *fd)
{
    for(int l=0; l<HeaderLineCount; l++)
    {
        QString line = header.lines[l];
        QStringList keyVal = line.split("=");
        if(keyVal.count() == 2)
        {
            QString key = keyVal[0].trimmed();
            QString val = header.attr.contains(key) ? header.attr[key] : keyVal[1].trimmed();
            line = QString(key.leftJustified(8, ' ') + "=" + val.rightJustified(21, ' ')).leftJustified(HeaderLineSize, ' ');
        }
        fd->write(line.toLocal8Bit());
    }
    int restSize = 2880 - static_cast<int>(fd->size());
    fd->write(QByteArray(restSize, ' '));
    fd->write(data);
}

void File::write(QByteArray &ba)
{
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    write(&buffer);
}

void File::write(const QString &filePath)
{
    QFileInfo info(filePath);
    if(info.exists() && !info.isWritable())
    {
        throw AstroBase::Exception(tr("Cannot write on file: %0").arg(filePath));
    }

    QFile f(filePath);
    if(!f.open(QIODevice::WriteOnly))
    {
        throw AstroBase::Exception(tr("Cannot open file for writing: %0").arg(filePath));
    }

    write(&f);
}

int File::HeaderData::getIntAttr(const QString& key) const
{
    HeaderData::Attributes::ConstIterator it = attr.find(key);
    if(attr.end() == it)
    {
        throw AstroBase::Exception(tr("Cannot get %0 from fits header data").arg(key));
    }

    return it->toInt();

}

int File::width() const
{
    return header.getIntAttr("NAXIS1");
}

int File::height() const
{
    return header.getIntAttr("NAXIS2");
}

int File::bitPix() const
{
    return header.getIntAttr("BITPIX");
}

int File::numPixels() const
{
    int numPixels = width() * height();
    if(colorFormat == ColorFormat_RGB)
        numPixels *= 3;
    return numPixels;
}

namespace helper
{
template<typename T> T getPixel(const QByteArray& data, int pixelIndex)
{
    T result = 0;
    const int size = sizeof(T);
    char* bytes = reinterpret_cast<char*>(&result);
    for(int b=0; b<size; b++)
    {
        bytes[size-b-1] = data[int(pixelIndex*size+b)];
    }
    return result;
}
}
double File::getPixel(int pixelIndex) const
{
    double result = 0;

    switch(pixelFormat)
    {
    case PixelFormat_8Bit         :
        result = double(helper::getPixel<qint8>(data, pixelIndex));
        break;
    case PixelFormat_16Bit_Int    :
        result = double(helper::getPixel<qint16>(data, pixelIndex));
        break;
    case PixelFormat_32Bit_Int    :
        result = double(helper::getPixel<qint32>(data, pixelIndex));
        break;
    case PixelFormat_64Bit_Int    :
        result = double(helper::getPixel<qint64>(data, pixelIndex));
        break;
    case PixelFormat_32Bit_Single :
        result = double(helper::getPixel<float>(data, pixelIndex));
        break;
    case PixelFormat_64Bit_Double :
        result = double(helper::getPixel<double>(data, pixelIndex));
        break;
    }

    return result;
}

void File::setPixel(int pixelIndex, const double &value)
{
    int baIndex = pixelIndex * bitPix() / 8;
    if(baIndex >= data.count())
        throw AstroBase::IndexOutOfBoundsException(tr("pixel index(%0)").arg(pixelIndex));

    switch(pixelFormat)
    {
    case PixelFormat_8Bit         :
    case PixelFormat_16Bit_Int    :
    case PixelFormat_32Bit_Int    :
    case PixelFormat_64Bit_Int    :
    {
        qint64 intVal = qint64(value); //assume, thate the incoming double value is - in fact - an integer
        int numBytes = pixelFormat / 8;
        char* bytes = reinterpret_cast<char*>(&intVal);
        for(int b=0; b<numBytes; b++)
        {
            data[baIndex+b] = bytes[b];
        }
        break;
    }
    case PixelFormat_32Bit_Single :
    case PixelFormat_64Bit_Double :
    {
        int numBytes = pixelFormat / 8;
        const char* bytes = reinterpret_cast<const char*>(&value);
        for(int b=0; b<numBytes; b++)
        {
            data[baIndex+b] = bytes[numBytes-b-1];
        }
        break;
    }
    }

}

const QByteArray &File::getData() const
{
    return data;
}

bool File::isValid(QIODevice *fd)
{
    if(!fd || !fd->isReadable())
        return false;

    QByteArray firstLine = fd->peek(HeaderLineSize);
    QStringList keyValue = QString(firstLine).split("=");
    if(2 != keyValue.count())
        return false;

    if(keyValue[0].trimmed() != "SIMPLE" || keyValue[1].remove(QRegularExpression("/.*$")).trimmed() != "T")
        return false;

    return true;

}

} // namespace Fits
