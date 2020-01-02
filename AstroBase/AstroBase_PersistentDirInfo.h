#ifndef ASTROBASE_PERSISTENTDIRINFO_H
#define ASTROBASE_PERSISTENTDIRINFO_H

#include <QString>
#include <QDir>
#include <QCoreApplication>

namespace AstroBase {

class PersistentDirInfo
{
    Q_DECLARE_TR_FUNCTIONS(AstroBase::PersistentDirInfo)

    QString defaultDir;
    QString groups, name;

public:
    PersistentDirInfo(const QString& context, const QString& defaultDir = "");

    operator QString() const;
    void operator=(const QString& value);
};

} // namespace AstroBase

#endif // ASTROBASE_PERSISTENTDIRINFO_H
