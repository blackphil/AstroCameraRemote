#include "Fits_File.h"

#include <QFileInfo>
#include <QIODevice>
#include <QStringList>

namespace Fits {

static const int LINE_SIZE = 80;
static const int LINE_COUNT = 36;

File File::read(QIODevice *fd)
{
    File f;


    for(int l=0; l<LINE_COUNT; l++)
    {
        QString line = fd->read(LINE_SIZE);
        QStringList keyVal = line.split("=");
        if(keyVal.count() != 2)
        {
            throw; //ToDo: Exception!
        }
        f.header << QPair<QString, QString>(keyVal[0].trimmed(), keyVal[1].trimmed());
    }

    f.data = fd->readAll();

    return f;
}

File File::read(const QString &filePath)
{
    QFileInfo info(filePath);
    if(!info.isReadable())
    {
        throw; //ToDo: Exception
    }

    QFile in(filePath);
    if(!in.open(QIODevice::ReadOnly))
    {
        throw; //ToDo: Exception!
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
    fd->write(data);
}

void File::write(const QString &filePath)
{
    QFileInfo info(filePath);
    if(!info.isWritable())
    {
        throw; //ToDo: Exception!
    }

    QFile f(filePath);
    if(!f.open(QIODevice::WriteOnly))
    {
        throw; //ToDo; Exception!
    }

    write(&f);
}
} // namespace Fits
