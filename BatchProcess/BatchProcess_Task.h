#ifndef BATCHPROCESS_TASK_H
#define BATCHPROCESS_TASK_H

#include "batchprocess_global.h"

#include <QObject>

#include "BatchProcess_Signal.h"
#include "BatchProcess_Visual_TaskBox.h"

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

private :
    int error;
    QString title;
    int id;

    Task& operator=(const Task&) { return *this; }

protected :
    Task(const Task& rhs);
    void setError(int err) { error = err; }
    int getError() const { return error; }

public:
    Task(const QString& title = "unknown", QObject* parent = Q_NULLPTR);
    virtual ~Task();

    virtual Task* clone() const = 0;

    virtual void getInputs(QList<SignalPtr>& inputs) const = 0;
    virtual void getOutputs(QList<SignalPtr>& outputs) const = 0;

    virtual int preExecute() { return NoError; }
    virtual int execute(int imageIndex, int pixelIndex) = 0;
    virtual int postExecute() { return NoError; }
    QString getTitle() const;
    void setTitle(const QString &value);
    int getId() const;
    void setId(int id);

    virtual QString getDictionaryPath() const = 0;
};

typedef QPointer<Task> TaskPtr;

} // namespace BatchProcess

#endif // BATCHPROCESS_TASK_H
