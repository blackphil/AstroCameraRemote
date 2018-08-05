#ifndef BATCHPROCESS_PROCESSOR_H
#define BATCHPROCESS_PROCESSOR_H

#include <QRunnable>
#include <QObject>
#include <QSharedPointer>

namespace BatchProcess {

class TaskInterface;
typedef QSharedPointer<TaskInterface> TaskInterfacePtr;

class Processor : public QObject, public QRunnable
{
    Q_OBJECT

    TaskInterfacePtr task;

public:
    Processor(TaskInterfacePtr task);

    void run();

Q_SIGNALS :
    void finished();
};

} // namespace BatchProcess

#endif // BATCHPROCESS_PROCESSOR_H
