#ifndef ASTROBASE_EXCEPTION_H
#define ASTROBASE_EXCEPTION_H

#include "astrobase_global.h"
#include <stdexcept>

#include <QCoreApplication>
#include <QString>

namespace AstroBase {

class ASTROBASESHARED_EXPORT Exception : public std::runtime_error
{
public:
    Exception(const QString& msg);
    virtual ~Exception() {}
};

class ASTROBASESHARED_EXPORT FileNotFoundException : public Exception
{
    Q_DECLARE_TR_FUNCTIONS(FileNotFoundException)
public :
    FileNotFoundException(const QString& filePath) : Exception(tr("File not found: %0").arg(filePath)) {}
};

} // namespace AstroBase

#endif // ASTROBASE_EXCEPTION_H
