#include "BatchProcess_StackImagesTask.h"

#include "AstroBase_Exception.h"

namespace BatchProcess {
#if 0

StackImagesTask::StackImagesTask()
    : in(new ImageSignal("images"))
    , out(new ImageSignal("result"))
{

}

void StackImagesTask::getInputs(QList<SignalPtr>& inputs) const
{
    inputs << in;
}

void StackImagesTask::getOutputs(QList<SignalPtr>& outputs) const
{
    outputs << out;
}

int StackImagesTask::execute(int imageIndex, int pixelIndex)
{

    if(0 > imageIndex || in->numImages() <= imageIndex)
    {
        throw AstroBase::IndexOutOfBoundsException(tr("image-index is %0").arg(imageIndex));
    }

    Fits::FilePtr file = in->getImages()[imageIndex];

    if(0 > pixelIndex || file->numPixels() <= pixelIndex)
    {
        throw AstroBase::IndexOutOfBoundsException(tr("pixel-index is %0)").arg(pixelIndex));
    }

    Fits::FilePtr result = out->getImages()[0];
    result->setPixel(pixelIndex, result->getPixel(pixelIndex)+file->getPixel(pixelIndex));



    return 0;
}
#endif

} // namespace BatchProcess
