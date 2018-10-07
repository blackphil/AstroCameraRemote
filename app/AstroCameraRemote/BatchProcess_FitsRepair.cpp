#include "BatchProcess_FitsRepair.h"

#include <QDir>
#include <QFileInfo>

#include "Fits_File.h"

namespace BatchProcess {

FitsRepair::FitsRepair()
{

}

void FitsRepair::setInputFile(const QString &file)
{
    input = file;
}

void FitsRepair::setOutputFile(const QString& file)
{
    output = file;
}

bool FitsRepair::process()
{
    QFileInfo inFile(input);
    if(!inFile.isReadable())
        return false;

    Fits::FilePtr f = Fits::File::fromFile(input);

    QFileInfo outFile(output);
    f->save(output);

    return true;

}

TaskInterface *FitsRepair::clone() const
{
    return new FitsRepair(*this);
}

} // namespace BatchProcess