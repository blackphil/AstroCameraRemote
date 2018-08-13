#include "BatchProcess_StackImagesTask.h"

#include "AstroBase_Exception.h"

namespace BatchProcess {

StackImagesTask::StackImagesTask()
    : in(new ImageSignal("images"))
    , out(new ImageSignal("result"))
//    , visual(new Visual::Task())
{

}

void StackImagesTask::getInputs(QList<ImageSignalPtr>& inputs) const
{
    inputs << in;
}

void StackImagesTask::getOutputs(QList<ImageSignalPtr>& outputs) const
{
    outputs << out;
}

Visual::Task *StackImagesTask::getVisual()
{
    return visual;
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

} // namespace BatchProcess
