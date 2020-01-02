#ifndef BATCHPROCESS_MANAGER_H
#define BATCHPROCESS_MANAGER_H


#include "BatchProcess_Task.h"
#include <QObject>
#include <QPointer>
#include <QMap>
#include <QMultiMap>

class QWidget;


namespace BatchProcess {

class Manager : public QObject
{
    Q_OBJECT

    QWidget* parentWidget;
    static QSharedPointer<Manager> instance;

    Manager(QWidget* parentWidget);

    QMap<int, TaskPtr> tasks;
    QMap<int, int> taskDeps;

    QMap<QString, TaskPtr> taskPrototypes;

public:
    ~Manager();
    QWidget *getParentWidget() const;
    static Manager* createInstance(QWidget* parentWidget);
    static Manager* get();

    QStringList getImageFilesGui();
    double getNumericValueGui(const double& defaultVal = 0);

    void addTask(TaskPtr task);
    void removeTask(int id);
    void setTaskDependency(int taskId, int dependsOn);

    void execute();
    void execute(TaskPtr task);

    void registerTaskPrototype(TaskPtr prototype, const QString& path);
    bool isRegistered(TaskPtr task) const;


};

} // namespace BatchProcess

#endif // BATCHPROCESS_MANAGER_H
