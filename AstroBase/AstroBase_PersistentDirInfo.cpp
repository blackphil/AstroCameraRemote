#include "AstroBase_PersistentDirInfo.h"

#include <QSettings>

#include "AstroBase_Exception.h"

namespace AstroBase {

PersistentDirInfo::PersistentDirInfo(const QString& context, const QString& defaultDir)
    : defaultDir(defaultDir.isEmpty() ? qApp->applicationDirPath() : defaultDir)
{
    QStringList groupsAndName = context.split("?");
    Q_ASSERT(2 == groupsAndName.count());
    if(2 != groupsAndName.count())
        throw Exception(tr("invalid conext for persistent dir info: %0").arg(context));

    groups = groupsAndName[0];
    name = groupsAndName[1];

}

PersistentDirInfo::operator QString() const
{
    QSettings s;
    s.beginGroup(groups);
    QString storedDir = s.value(name).toString();
    if(storedDir.isEmpty())
        return defaultDir;

    return storedDir;
}

void PersistentDirInfo::setDir(const QString &value)
{
    if(value == defaultDir)
        return;

    QSettings s;
    s.beginGroup(groups);
    s.setValue(name, value);
    s.endGroup();
}

} // namespace AstroBase
