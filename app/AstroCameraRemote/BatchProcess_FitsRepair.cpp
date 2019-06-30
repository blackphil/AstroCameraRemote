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
    if(QFileInfo inFile { input }; !inFile.isReadable())
        return false;

    if(auto f { Fits::File::fromFile(input) })
        f->save(output);

    return true;

}

TaskInterface *FitsRepair::clone() const
{
    return new FitsRepair(*this);
}

} // namespace BatchProcess
