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
    Q_OBJECT

public :

    enum Operator
    {
        Op_Plus, Op_Minus, Op_Multiply, Op_Divide
    };

    enum SummandType
    {
        Summand_Image, Summand_Numeric
    };

private :
    Operator op;
    SignalPtr lhs, rhs, out;

    SummandType l, r, o;


    PixelMathTask(const PixelMathTask& rhs);

    void setupOutput();

private Q_SLOTS :
    void imageInputSignalChanged();

public:
    PixelMathTask(Operator op, SummandType leftSummand, SummandType rightSummand, QObject* parent = Q_NULLPTR);

    void getInputs(QList<SignalPtr>& inputs) const;
    void getOutputs(QList<SignalPtr>& outputs) const;

    int preExecute();
    int execute(int imageIndex, int pixelIndex);

    Task* clone() const;
    QString getDictionaryPath() const;

    int numPixelsPerImage() const;
    int numImages() const;

};

} // namespace BatchProcess

#endif // BATCHPROCESS_PIXELMATHTASK_H
