#ifndef BATCHPROCESS_TASK_H
#define BATCHPROCESS_TASK_H

#include "batchprocess_global.h"

#include <QObject>

#include "BatchProcess_Signal.h"

class QThreadPool;

namespace BatchProcess {

class BATCHPROCESSSHARED_EXPORT Task : public QObject
{
    Q_OBJECT

public :
    enum ErrorCode
    {
        Err_DivisionByZero = -1
        , NoError = 0
    };

    enum ExecutionType
    {
        Execute_PerImageAndPixel
        , Execute_PerImage
    };

    enum Status
    {
        Status_Idle
        , Status_Execution
        , Status_Error
    };

private :
    int error;
    QString title;
    int id;

    QThreadPool* threadPool;

    Task& operator=(const Task&) { return *this; }

protected :
    Task(const Task& rhs);
    void setError(int err) { error = err; }
    int getError() const { return error; }

    ExecutionType executionType;

Q_SIGNALS :
    void statusChanged(Task::Status);

public:
    Task(const QString& title = "unknown", ExecutionType execType = Execute_PerImageAndPixel, QObject* parent = Q_NULLPTR);
    virtual ~Task();

    virtual Task* clone() const = 0;

    virtual void getInputs(QList<SignalPtr>& inputs) const = 0;
    virtual void getOutputs(QList<SignalPtr>& outputs) const = 0;

    virtual int preExecute();
    virtual int execute();
    virtual int execute(int imageIndex, int pixelIndex) = 0;
    virtual int postExecute();
    QString getTitle() const;
    void setTitle(const QString &value);
    int getId() const;
    void setId(int id);

    virtual int numPixelsPerImage() const = 0;
    virtual int numImages() const = 0;

    virtual QString getDictionaryPath() const = 0;
    ExecutionType getExecutionType() const;

    virtual bool edit();
};

typedef QPointer<Task> TaskPtr;

} // namespace BatchProcess

#endif // BATCHPROCESS_TASK_H
