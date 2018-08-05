#include "BatchProcess_Processor.h"
#include "BatchProcess_TaskInterface.h"

namespace BatchProcess {

Processor::Processor(TaskInterfacePtr task)
    : task(task)
{
}

void Processor::run()
{
    if(task)
        task->process();
    Q_EMIT finished();
}

} // namespace BatchProcess
