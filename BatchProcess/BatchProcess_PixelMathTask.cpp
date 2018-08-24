#include "BatchProcess_PixelMathTask.h"
#include "BatchProcess_Manager.h"

#include "AstroBase.h"
#include "AstroBase_Exception.h"

namespace BatchProcess {

static const QString OperatorString[4] = { "+", "-", "*", "/" };


PixelMathTask::PixelMathTask(const PixelMathTask &rhs)
    : Task(rhs)
    , op(rhs.op)
    , out(new ImageSignal(Signal::Direction_Out, "out", this))
{

}

void PixelMathTask::imageInputSignalChanged()
{
    AB_DBG("VISIT");
    int numImages = 0;
    ImageSignalPtr imageSignal;
    if(Summand_Image == l)
    {
        imageSignal = qobject_cast<ImageSignal*>(lhs);
        AB_DBG("signal \"" << imageSignal->getName() << "\" has" << imageSignal->numImages() << "images");
        numImages = imageSignal->numImages();
    }

    if(Summand_Image == r)
    {
        imageSignal = qobject_cast<ImageSignal*>(rhs);
        AB_DBG("signal \"" << imageSignal->getName() << "\" has" << imageSignal->numImages() << "images");
        numImages = qMax(numImages, imageSignal->numImages());
    }

    AB_DBG("have" << numImages << "images");

    if(0 >= numImages)
        return;

    Fits::FilePtr inFile = imageSignal->getImages()[0];
    if(!inFile->isHeaderLoaded())
        inFile->loadHeader();

    if(!inFile->isHeaderLoaded())
        throw AstroBase::Exception(tr("Cannot load header info from first file (%0)").arg(inFile->getFilePath()));

    QList<Fits::FilePtr> outImages;
    for(int i=0; i<numImages; i++)
    {
        Fits::FilePtr f(new Fits::File(inFile->width(), inFile->height(), inFile->getPixelFormat(), inFile->getColorFormat()));
        outImages << f;
    }

    imageSignal = qobject_cast<ImageSignal*>(out);
    imageSignal->setImages(outImages);

    AB_DBG("out has" << imageSignal->numImages() << "images");

}

PixelMathTask::PixelMathTask(Operator op, SummandType leftSummand, SummandType rightSummand, QObject* parent)
    : Task(OperatorString[op], Task::Execute_PerImageAndPixel, parent)
    , op(op)
    , l(leftSummand)
    , r(rightSummand)
{
    o = Summand_Numeric;
    if(leftSummand == Summand_Image)
    {
        o = Summand_Image;
        lhs = new ImageSignal(Signal::Direction_In, "lhs", this);
    }
    else
        lhs = new NumericSignal(Signal::Direction_In, "lhs", 0, this);

    connect(lhs, SIGNAL(signalChanged()), this, SLOT(imageInputSignalChanged()));


    if(rightSummand == Summand_Image)
    {
        o = Summand_Image;
        rhs = new ImageSignal(Signal::Direction_In, "rhs", this);
    }
    else
        rhs = new NumericSignal(Signal::Direction_In, "rhs", 0, this);

    connect(rhs, SIGNAL(signalChanged()), this, SLOT(imageInputSignalChanged()));

    if(o == Summand_Image)
        out = new ImageSignal(Signal::Direction_Out, "out", this);
    else
        out = new NumericSignal(Signal::Direction_Out, "out", 0, this);


}


void PixelMathTask::getInputs(QList<SignalPtr>& inputs) const
{
    inputs << lhs << rhs;
}

void PixelMathTask::getOutputs(QList<SignalPtr> &outputs) const
{
    outputs << out;
}

int PixelMathTask::preExecute()
{
    int result = Task::preExecute();
    if(result != Task::NoError)
        return result;

    QList<Fits::FilePtr> _images;
    ImageSignalPtr imgSig = qobject_cast<ImageSignal*>(lhs);
    if(imgSig)
        _images << imgSig->getImages();

    imgSig = qobject_cast<ImageSignal*>(rhs);
    if(imgSig)
        _images << imgSig->getImages();

    foreach(Fits::FilePtr f, _images)
    {
        if(f->isLoaded())
            continue;

        if(!f->load())
            return -10;
    }

    return NoError;
}

int PixelMathTask::execute(int imageIndex, int pixelIndex)
{
    //    AB_DBG("VISIT");
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

//    AB_DBG("img(" << imageIndex << "), pixel(" << pixelIndex << "):" << left << OperatorString[op] << right << "=" << result);

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

int PixelMathTask::numPixelsPerImage() const
{
    if(Summand_Image == o)
    {
//        AB_DBG("output is an image signal");
        ImageSignalPtr outSig = qobject_cast<ImageSignal*>(out);
        if(!outSig)
            throw AstroBase::Exception(tr("invalid output signal type (not an image)"));

//        AB_DBG("have output image signal object");
        return outSig->numPixels();
    }
    else
    {
        AB_DBG("output is NOT an image signal");
    }
    return 0;
}

int PixelMathTask::numImages() const
{
    if(Summand_Image == o)
    {
        ImageSignalPtr outSig = qobject_cast<ImageSignal*>(out);
        if(!outSig)
            throw AstroBase::Exception(tr("invalid output signal type (not an image)"));

        return outSig->numImages();
    }

    return 0;
}



} // namespace BatchProcess
