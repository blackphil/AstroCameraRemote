#include "Fits_File.h"

#include <QFileInfo>
#include <QIODevice>
#include <QStringList>
#include <QDir>

#include "AstroBase_Exception.h"

namespace Fits {

static const int LINE_SIZE = 80;

File File::read(QIODevice *fd)
{
    File f;


    QStringList keyVal;
    int l=0;
    do
    {
        QString line = fd->read(LINE_SIZE);
        keyVal = line.split("=");

        if(keyVal.count() == 0 || keyVal[0].toUpper().startsWith("END"))
            break;

        if(keyVal.count() != 2)
        {
            throw AstroBase::Exception(tr("Invalid line nr. %0 in fits header").arg(l));
        }
        f.header << QPair<QString, QString>(keyVal[0].trimmed(), keyVal[1].trimmed());

        l++;
    }
    while(true);

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
    for(int l=0; l<header.count(); l++)
    {
        const QString& key = header[l].first;
        const QString& value = header[l].second;
        QString line = QString(key.leftJustified(8, ' ') + "=" + value.rightJustified(21, ' ')).leftJustified(LINE_SIZE, ' ');
        fd->write(line.toLocal8Bit());
    }
    fd->write("END");
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
} // namespace Fits
