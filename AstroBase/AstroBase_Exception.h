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
    Exception(const QString& msg) noexcept;
    Exception(const Exception& rhs) noexcept;
    Exception& operator=(const Exception& rhs) noexcept;
    virtual ~Exception() noexcept {}
};

class ASTROBASESHARED_EXPORT FileNotFoundException : public Exception
{
    Q_DECLARE_TR_FUNCTIONS(FileNotFoundException)
public :
    FileNotFoundException(const QString& filePath) : Exception(tr("File not found: %0").arg(filePath)) {}
};

class ASTROBASESHARED_EXPORT DirNotFoundException : public Exception
{
    Q_DECLARE_TR_FUNCTIONS(DirNotFoundException)
public :
    DirNotFoundException(const QString& path) : Exception(tr("Directory now found: %0").arg(path)) {}
};

class ASTROBASESHARED_EXPORT IndexOutOfBoundsException : public Exception
{
    Q_DECLARE_TR_FUNCTIONS(IndexOutOfBoundsException)
public :
    IndexOutOfBoundsException(const QString& context = "")
        : Exception(tr("Index out of bounds%0")
                   .arg(!context.isEmpty() ? QString(" (%0)").arg(context) : QString()))
    {}
};

} // namespace AstroBase

#endif // ASTROBASE_EXCEPTION_H
