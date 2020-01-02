#ifndef BATCHPROCESS_STACKIMAGESTASK_H
#define BATCHPROCESS_STACKIMAGESTASK_H

#include <QObject>

#include "BatchProcess_Task.h"
#include "BatchProcess_ImageSignal.h"

namespace BatchProcess {

#if 0

class StackImagesTask : public Task
{
    Q_OBJECT

    ImageSignalPtr in, out;

public:
    StackImagesTask();

    void getInputs(QList<SignalPtr>& inputs) const;
    void getOutputs(QList<SignalPtr>& outputs) const;
    int execute(int imageIndex, int pixelIndex);

};

#endif

} // namespace BatchProcess

#endif // BATCHPROCESS_STACKIMAGESTASK_H
