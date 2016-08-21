#ifndef SONYALPHAREMOTE_SEQUENCER_SETTINGSMANAGER_H
#define SONYALPHAREMOTE_SEQUENCER_SETTINGSMANAGER_H


#include <QObject>
#include <QSettings>


#include "SonyAlphaRemote_Sequencer_Settings.h"

namespace SonyAlphaRemote {
namespace Sequencer {

class SettingsManager : public QObject
{
    Q_OBJECT

    mutable QSettings qSettings;

    QString currentName;

public:
    explicit SettingsManager(QObject *parent = 0);

    QStringList getSettingsNames() const;
    void handleCurrentChanged();

Q_SIGNALS :
    void currentChanged(QString);
    void removed(QString);

public Q_SLOTS :
    bool rename(const QString& oldName, const QString& newName);
    bool renameCurrent(const QString& newName);
    bool remove(QString name);
    bool removeCurrent();
    void setCurrent(const QString& name);

};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SEQUENCER_SETTINGSMANAGER_H
