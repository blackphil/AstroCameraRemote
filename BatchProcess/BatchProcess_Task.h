#ifndef BATCHPROCESS_TASK_H
#define BATCHPROCESS_TASK_H

#include "BatchProcess_ImageSignal.h"
#include "BatchProcess_Visual_Task.h"

namespace BatchProcess {

class Task
{
public:
    Task() {}

    virtual ~Task() {}
    virtual void getInputs(QList<ImageSignalPtr>& inputs) const = 0;
    virtual void getOutputs(QList<ImageSignalPtr>& outputs) const = 0;
    virtual Visual::Task* getVisual() = 0;

    virtual int execute(int imageIndex, int pixelIndex) = 0;
};

} // namespace BatchProcess

#endif // BATCHPROCESS_TASK_H
