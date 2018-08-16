#include "BatchProcess_PixelMathTask.h"
#include "BatchProcess_Manager.h"

namespace BatchProcess {

static const QString OperatorString[4] = { "+", "-", "*", "/" };

PixelMathTask PixelMathTask::plus(Op_Plus);

PixelMathTask::PixelMathTask(const PixelMathTask &rhs)
    : Task(rhs)
    , op(rhs.op)
    , out(new ImageSignal("out"))
{

}

PixelMathTask::PixelMathTask(Operator op, QObject* parent)
    : Task(OperatorString[op], parent)
    , op(op)
    , out(new ImageSignal("out"))
{

}

void PixelMathTask::setLhs(SignalPtr s)
{
    lhs = s;
}

void PixelMathTask::setRhs(SignalPtr s)
{
    rhs = s;
}

void PixelMathTask::getInputs(QList<SignalPtr>& inputs) const
{
    inputs << lhs << rhs;
}

void PixelMathTask::getOutputs(QList<SignalPtr> &outputs) const
{
    outputs << out;
}

int PixelMathTask::execute(int imageIndex, int pixelIndex)
{
    double left = lhs->getPixel(imageIndex, pixelIndex);
    double right = rhs->getPixel(imageIndex, pixelIndex);

    double result = 0;
    switch(op)
    {
    case Op_Plus :
        result = left + right;
        break;
    case Op_Minus :
        result = left - right;
        break;
    case Op_Multiply :
        result = left * right;
        break;
    case Op_Divide :
        if(qFloatDistance(right, 0.) == 0)
            setError(Task::Err_DivisionByZero);
        else
            result = left / right;
        break;
    }
    out->setPixel(imageIndex, pixelIndex, result);

    return getError();
}

Task *PixelMathTask::clone() const
{
    return new PixelMathTask(*this);
}

QString PixelMathTask::getDictionaryPath() const
{
    return "PixelMath";
}



} // namespace BatchProcess
