#include <DataFormat/Fits/File>

#include <AstroBase/AstroBase>

#include <QFileInfo>
#include <QIODevice>
#include <QStringList>
#include <QDir>
#include <QBuffer>
#include <QPainter>
#include <QRegularExpression>
#include <QSettings>

#include <AstroBase/Exception>

namespace Fits {


File::PixelFormat File::getPixelFormat() const
{
    return pixelFormat;
}

File::ColorFormat File::getColorFormat() const
{
    return colorFormat;
}

QString File::getFilePath() const
{
    return filePath;
}

void File::setFilePath(const QString &value)
{
    filePath = value;
}

void File::init()
{


    pixelFormat = PixelFormat_16Bit_Int;
    colorFormat = ColorFormat_Grayscale;

    for(int i=0; i<3; i++)
    {
        minVal[i] = std::numeric_limits<double>::max();
        maxVal[i] = std::numeric_limits<double>::min();
    }

    loaded = false;

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

File::File()
{
    init();
}

File::File(int w, int h, File::PixelFormat pf, File::ColorFormat cf)
{
    init();

    header.attr["SIMPLE"] = "T";
    header.attr["NAXIS1"] = QString::number(w);
    header.attr["NAXIS2"] = QString::number(h);

    switch(cf)
    {
    case ColorFormat_BayerRGGB :
    case ColorFormat_Grayscale :
        header.attr["NAXIS"] = "2";
        break;
    case ColorFormat_RGB :
        header.attr["NAXIS3"] = "3";
        header.attr["NAXIS"] = "3";
        break;
    }

    header.attr["BITPIX"] = QString::number(int(pf));

    data.resize(numPixels() * bitPix() / 8);

}

File::File(const QString &filePath, bool loadNow)
    : filePath(filePath)
{
    init();

    if(loadNow)
        load();
}

bool File::load()
{
    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly))
        return false;

    return load(&f);
}

bool File::load(QIODevice* fd)
{
    if(!loadHeader(fd))
        return false;

    data = fd->readAll();

    AB_DBG("have" << data.count() << "bytes of pixel data");
    return true;

}

bool File::loadHeader()
{

    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly))
        return false;

    return loadHeader(&f);
}

bool File::loadHeader(QIODevice *fd)
{
    for(int l=0; l<HeaderLineCount; l++)
    {
        QString line = fd->read(HeaderLineSize);
        header.lines << line;
        QStringList keyVal = line.split("=");
        if(keyVal.count() == 2)
        {
            header.attr[keyVal[0].trimmed()] = keyVal[1].remove(QRegularExpression("/.*$")).trimmed();
        }
    }

    if(!header.attr.contains("BITPIX"))
        throw AstroBase::Exception(tr("cannot determine pixel format"));

    pixelFormat = static_cast<PixelFormat>(header.getIntAttr("BITPIX"));

    if(!header.attr.contains("NAXIS"))
        throw AstroBase::Exception(tr("cannot determine color format"));

    if(header.getIntAttr("NAXIS") == 2)
    {
        if(header.attr.contains("BAYER") && header.attr["BAYER"] == "RGGB")
            colorFormat = ColorFormat_BayerRGGB;
        else
            colorFormat = ColorFormat_Grayscale;
    }
    else if(header.getIntAttr("NAXIS") == 3)
    {
        colorFormat = ColorFormat_RGB;
    }
    else
        throw AstroBase::Exception(tr("cannot determine color format"));

    return true;
}

bool File::isHeaderLoaded() const
{
    return header.attr.contains("SIMPLE") && header.attr["SIMPLE"] == "T";
}

FilePtr File::fromByteArray(const QByteArray &data)
{
    QBuffer buffer;
    buffer.setData(data);
    buffer.open(QBuffer::ReadOnly);
    return fromDevice(&buffer);
}

FilePtr File::fromDevice(QIODevice *fd)
{
    FilePtr f(new File());
    f->load(fd);
    return f;
}

FilePtr File::fromFile(const QString &filePath)
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

void File::save()
{
    save(filePath);
}

void File::save(QIODevice *fd) const
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

void File::save(QByteArray &ba)
{
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    save(&buffer);
}

void File::save(const QString &filePath)
{
    saveAs(filePath);
    this->filePath = filePath;
}

void File::saveAs(const QString &filePath) const
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

    save(&f);

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
