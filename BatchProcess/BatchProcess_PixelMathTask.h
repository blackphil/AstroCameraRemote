#ifndef BATCHPROCESS_PIXELMATHTASK_H
#define BATCHPROCESS_PIXELMATHTASK_H

#include "batchprocess_global.h"

#include "BatchProcess_Task.h"
#include "BatchProcess_ImageSignal.h"
#include "BatchProcess_NumericSignal.h"
#include <QVector>
#include <QList>

namespace BatchProcess {

class BATCHPROCESSSHARED_EXPORT PixelMathTask : public Task
{
public :

    enum Operator
    {
        Op_Plus, Op_Minus, Op_Multiply, Op_Divide
    };

private :
    Operator op;
    SignalPtr lhs, rhs, out;

    static PixelMathTask plus;

    PixelMathTask(const PixelMathTask& rhs);

public:
    PixelMathTask(Operator op, QObject* parent = Q_NULLPTR);

    void setLhs(SignalPtr s);
    void setRhs(SignalPtr s);

    void getInputs(QList<SignalPtr>& inputs) const;
    void getOutputs(QList<SignalPtr>& outputs) const;

    int execute(int imageIndex, int pixelIndex);

    Task* clone() const;
    QString getDictionaryPath() const;

};

} // namespace BatchProcess

#endif // BATCHPROCESS_PIXELMATHTASK_H
