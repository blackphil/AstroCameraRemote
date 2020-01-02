#ifndef BATCHPROCESS_SAVEIMAGESTASK_H
#define BATCHPROCESS_SAVEIMAGESTASK_H

#include "BatchProcess_Task.h"

#include "BatchProcess_ImageSignal.h"

namespace BatchProcess {

class SaveImagesTask : public Task
{

    Q_OBJECT

    ImageSignalPtr inImages;

    QString targetPath;
    QString targetFilePattern;
    bool usePattern;

public:
    SaveImagesTask(QObject* parent = Q_NULLPTR);

    Task* clone() const;
    void getInputs(QList<SignalPtr>& inputs) const;
    void getOutputs(QList<SignalPtr>& outputs) const;
    int execute(int imageIndex, int pixelIndex);
    int numPixelsPerImage() const;
    int numImages() const;
    QString getDictionaryPath() const;

    bool edit();

    QString getTargetPath() const;
    void setTargetPath(const QString &value);
    QString getTargetFilePattern() const;
    void setTargetFilePattern(const QString &value);
    bool getUsePattern() const;
    void setUsePattern(bool value);
};

} // namespace BatchProcess

#endif // BATCHPROCESS_SAVEIMAGESTASK_H
