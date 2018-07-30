#include "Fits_Repair.h"

#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QByteArray>
#include <QRegularExpression>

#include "Fits_File.h"

namespace Fits {

Repair::Repair()
{

}

QStringList Repair::supportedExtensions() const
{
    static const QStringList ext = { "fit", "fits" };
    return ext;
}

int Repair::processFile(const QString &inputFilePath, const QString &outputFilePath)
{
    File f = File::read(inputFilePath);
    f.write(outputFilePath);
    return 0;
}

} // namespace Fits
