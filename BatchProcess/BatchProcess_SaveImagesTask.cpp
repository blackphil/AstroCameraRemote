#include "BatchProcess_SaveImagesTask.h"
#include "BatchProcess_SaveImagesEditDlg.h"
#include "BatchProcess_Manager.h"

#include <AstroBase/AstroBase>

#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>

namespace BatchProcess {

QString SaveImagesTask::getTargetPath() const
{
    return targetPath;
}

void SaveImagesTask::setTargetPath(const QString &value)
{
    targetPath = value;
}

QString SaveImagesTask::getTargetFilePattern() const
{
    return targetFilePattern;
}

void SaveImagesTask::setTargetFilePattern(const QString &value)
{
    targetFilePattern = value;
}

bool SaveImagesTask::getUsePattern() const
{
    return usePattern;
}

void SaveImagesTask::setUsePattern(bool value)
{
    usePattern = value;
}


SaveImagesTask::SaveImagesTask(QObject* parent)
    : Task(tr("Save images"), Task::Execute_PerImage, parent)
    , inImages(new ImageSignal(Signal::Direction_In, "images", this))
    , usePattern(false)
{

}

Task* SaveImagesTask::clone() const
{
    return new SaveImagesTask(*this);
}

void SaveImagesTask::getInputs(QList<SignalPtr>& inputs) const
{
    inputs << SignalPtr(inImages);
}

void SaveImagesTask::getOutputs(QList<SignalPtr>& outputs) const
{
    Q_UNUSED(outputs)
}

int SaveImagesTask::execute(int imageIndex, int pixelIndex)
{
    Q_UNUSED(pixelIndex)

    Fits::FilePtr image = inImages->getImages()[imageIndex];

    QString fileName;
    if(usePattern)
    {
        QRegularExpression rxIndex("%i(\\d*)");
        QRegularExpressionMatch indexMatch = rxIndex.match(targetFilePattern);
        int numDigits = 0;
        if(indexMatch.hasPartialMatch())
        {
            numDigits = indexMatch.captured(1).toInt();
            fileName = targetFilePattern.replace(rxIndex, QString::number(imageIndex).rightJustified(numDigits, '0'));
        }
    }
    else
        fileName = QFileInfo(image->getFilePath()).fileName();

    QDir targetDir(targetPath);
    if(!targetDir.exists())
    {
        AB_ERR("target dir \"" << targetPath << "\" doesn't exist!");
        Q_EMIT errorUpdate(tr("target dir \"%0\" doesn't exist!").arg(targetPath));
        return -1;
    }

    QRegularExpression rxFileName(".+\\.fits");
    if(!rxFileName.match(fileName).hasMatch())
    {
        AB_ERR("target file name invalid: \"" << fileName << "\"");
        Q_EMIT errorUpdate(tr("target file name invalid: \"%0\"").arg(fileName));
        return -2;
    }

    QFileInfo targetFilePath(targetDir, fileName);

    image->saveAs(targetFilePath.absoluteFilePath());
    Q_EMIT infoUpdate(tr("Saved \"%0\"").arg(fileName));

    return true;
}

int SaveImagesTask::numPixelsPerImage() const
{
    if(0 < inImages->numImages())
        return inImages->getImages()[0]->numPixels();

    return 0;
}

int SaveImagesTask::numImages() const
{
    return inImages->numImages();
}

QString SaveImagesTask::getDictionaryPath() const
{
    return "SaveImages";
}

bool SaveImagesTask::edit()
{
    SaveImagesEditDlg dlg(this, Manager::get()->getParentWidget());
    if(dlg.exec())
    {
        QString text = tr("path: %0<br/>file name: %1").arg(getTargetPath()).arg(getTargetFilePattern());
        AB_DBG(text);
        Q_EMIT infoUpdate(text);
        return Task::edit();
    }
    return false;
}

} // namespace BatchProcess
