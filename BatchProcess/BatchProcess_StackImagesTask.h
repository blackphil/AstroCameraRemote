#ifndef BATCHPROCESS_STACKIMAGESTASK_H
#define BATCHPROCESS_STACKIMAGESTASK_H

#include <QObject>

#include "batchprocess_global.h"
#include "BatchProcess_Task.h"
#include "BatchProcess_ImageSignal.h"
#include "BatchProcess_Visual_Task.h"

namespace BatchProcess {

class BATCHPROCESSSHARED_EXPORT StackImagesTask : public QObject, public BatchProcess::Task
{
    Q_OBJECT

    ImageSignalPtr in, out;
    Visual::Task* visual;

public:
    StackImagesTask();

    void getInputs(QList<ImageSignalPtr>& inputs) const;
    void getOutputs(QList<ImageSignalPtr>& outputs) const;
    Visual::Task* getVisual();

    int execute(int imageIndex, int pixelIndex);

};

} // namespace BatchProcess

#endif // BATCHPROCESS_STACKIMAGESTASK_H
