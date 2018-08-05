#ifndef BATCHPROCESS_TASKINTERFACE_H
#define BATCHPROCESS_TASKINTERFACE_H

#include <QStringList>
#include <QSharedPointer>

namespace BatchProcess {

class TaskInterface
{
public:
    virtual ~TaskInterface() {}

    virtual void setInputFile(const QString& file) = 0;
    virtual void setOutputFile(const QString& file) = 0;
    virtual bool process() = 0;
    virtual TaskInterface* clone() const = 0;

};

typedef QSharedPointer<TaskInterface> TaskInterfacePtr;

} // namespace BatchProcess

#endif // BATCHPROCESS_TASKINTERFACE_H
