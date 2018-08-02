#include "Fits_File.h"

#include <QFileInfo>
#include <QIODevice>
#include <QStringList>
#include <QDir>
#include <QBuffer>
#include <QPainter>

#include "AstroBase_Exception.h"

namespace Fits {


File File::read(const QByteArray &data)
{
    QBuffer buffer;
    buffer.setData(data);
    buffer.open(QBuffer::ReadOnly);
    return read(&buffer);
}

File File::read(QIODevice *fd)
{
    File f;


    for(int l=0; l<HeaderLineCount; l++)
    {
        QString line = fd->read(HeaderLineSize);
        f.header.lines << line;
        QStringList keyVal = line.split("=");
        if(keyVal.count() == 2)
        {
            f.header.attr[keyVal[0].trimmed()] = keyVal[1].trimmed();
        }
    }

    f.data = fd->readAll();

    return f;
}

File File::read(const QString &filePath)
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

    return read(&in);
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
    fd->write("END");
    int restSize = 2880 - static_cast<int>(fd->size());
    fd->write(QByteArray(restSize, ' '));
    fd->write(data);
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

const QByteArray &File::getData() const
{
    return data;
}

} // namespace Fits
