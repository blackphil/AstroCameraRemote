#include "BatchProcess_Task.h"

#include <AstroBase/AstroBase>

#include <QThreadPool>
#include <QRunnable>

namespace BatchProcess {


Task::ExecutionType Task::getExecutionType() const
{
    return executionType;
}

bool Task::edit()
{
    return true;
}

Task::Task(const Task & rhs)
    : QObject(rhs.parent())
    , error(rhs.error)
    , title(rhs.title)
    , id(-1)
    , threadPool(new QThreadPool(this))
    , executionType(rhs.executionType)
{

}

Task::Task(const QString& title, ExecutionType execType, QObject *parent)
    : QObject(parent)
    , error(NoError)
    , title(title)
    , id(-1)
    , threadPool(new QThreadPool(this))
    , executionType(execType)
{

}

Task::~Task()
{
    AB_DBG("VISIT (" << title << "/" << id << ")");
}

int Task::preExecute()
{
    Q_EMIT statusChanged(Status_Execution);
    return NoError;
}

namespace helper
{
class Executer : public QRunnable
{
    Task* task;
    int pixelIndex;

    int& result;

public :
    Executer(Task* task, int pixelIndex, int& result)
        : task(task)
        , pixelIndex(pixelIndex)
        , result(result)
    {}

    void run()
    {
        for(int i=0; i<task->numImages(); i++)
        {
            try {
                //                AB_DBG("for img(" << i << "), pixel(" << pixelIndex << ")");
                int r = task->execute(i, pixelIndex);
                if(r != Task::NoError)
                {
                    result = r;
                    break;
                }
            } catch (std::exception& e) {
                AB_ERR(e.what());
            }
            catch(...)
            {
                AB_ERR("unknown error");
            }

        }
    }
};
}
int Task::execute()
{
    int result = NoError;


    switch(executionType)
    {
    case Execute_PerImage :
        threadPool->start(new helper::Executer(this, -1, result));
        break;
    case Execute_PerImageAndPixel :
        for(int i=0; i<numPixelsPerImage(); i++)
        {
            threadPool->start(new helper::Executer(this, i, result));
        }
        break;
    }

    threadPool->waitForDone();


    return result;
}

int Task::postExecute()
{
    Q_EMIT statusChanged(Status_Idle);
    return NoError;
}

int Task::getId() const
{
    return id;
}

void Task::setId(int id)
{
    this->id = id;
}

QString Task::getTitle() const
{
    return title;
}

void Task::setTitle(const QString &value)
{
    title = value;
}



} // namespace BatchProcess
