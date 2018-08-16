#include "BatchProcess_Manager.h"

#include "AstroBase_Exception.h"
#include "AstroBase.h"
#include "AstroBase_PersistentDirInfo.h"

#include "BatchProcess_NumericValueDialog.h"

#include <QFileDialog>

namespace BatchProcess {

QSharedPointer<Manager> Manager::instance(Q_NULLPTR);

Manager::Manager(QWidget *parentWidget)
    : parentWidget(parentWidget)
{
}

Manager::~Manager()
{
    AB_DBG("VISIT");
}

QWidget *Manager::getParentWidget() const
{
    return parentWidget;
}

Manager *Manager::createInstance(QWidget *parentWidget)
{
    if(instance)
        throw AstroBase::Exception(tr("Batch process manager already created"));

    instance.reset(new Manager(parentWidget));
    return instance.data();
}

Manager *Manager::get()
{
    return instance.data();
}

QStringList Manager::getImageFilesGui()
{
    AstroBase::PersistentDirInfo dir("BatchProcess?LastDir");
    QStringList files = QFileDialog::getOpenFileNames(
                parentWidget
                , tr("Open files")
                , dir
                , "FITS (*.fit *.fts *.fits)"
                  ";;JPEG (*.jpg *.jpeg)"
                  ";;TIFF (*.tif)"
                  ";;All files(*.*)");

    if(!files.isEmpty())
        dir = QFileInfo(files[0]).absolutePath();

    return files;

}

double Manager::getNumericValueGui(const double &defaultVal)
{
    NumericValueDialog dlg(parentWidget);
    dlg.setValue(defaultVal);

    if(dlg.exec())
        return dlg.getValue();

    return std::numeric_limits<double>::signaling_NaN();
}


void Manager::addTask(TaskPtr task)
{
    int nextId = 1;
    while(tasks.contains(nextId))
        nextId++;

    task->setId(nextId);
    tasks[nextId] = task;
    taskDeps[nextId] = 0;
}

void Manager::removeTask(int id)
{
    tasks.remove(id);
}

void Manager::setTaskDependency(int taskId, int dependsOn)
{
    taskDeps[taskId] = dependsOn;
}

void Manager::execute()
{

    QMap<int, int> tasksToExecute = taskDeps;
    QMap<int, int>::Iterator it = tasksToExecute.begin();
    while(!tasksToExecute.isEmpty())
    {
        int key = it.key();
        int val = it.value();

        if(!tasksToExecute.contains(val))
        {
            TaskPtr t = tasks[key];
            execute(t);
            it = tasksToExecute.erase(it);
        }
        else
            it++;
    }

}

void Manager::execute(TaskPtr task)
{
    AB_DBG("would execute task" << task->getTitle() << "(" << task->getId() << ") now");
}

void Manager::registerTaskPrototype(TaskPtr prototype, const QString &path)
{
    taskPrototypes[path+"?"+prototype->getTitle()] = prototype;
}

bool Manager::isRegistered(TaskPtr task) const
{
    return taskPrototypes.contains(task->getDictionaryPath() + "?" + task->getTitle());
}

} // namespace BatchProcess
