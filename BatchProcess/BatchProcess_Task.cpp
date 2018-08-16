#include "BatchProcess_Task.h"

#include "AstroBase.h"

namespace BatchProcess {


Task::Task(const Task & rhs)
    : QObject(rhs.parent())
    , error(rhs.error)
    , title(rhs.title)
    , id(-1)
{

}

Task::Task(const QString& title, QObject *parent)
    : QObject(parent)
    , error(NoError)
    , title(title)
    , id(-1)
{

}

Task::~Task()
{
    AB_DBG("VISIT (" << title << "/" << id << ")");
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
