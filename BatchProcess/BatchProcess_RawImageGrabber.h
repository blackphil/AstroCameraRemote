#ifndef BATCHPROCESS_RAWIMAGEGRABBER_H
#define BATCHPROCESS_RAWIMAGEGRABBER_H

#include <QObject>
#include <QDir>

#include "BatchProcess.h"
#include "EasyExif_Exif.h"

namespace BatchProcess {

class RawImageGrabber : public QObject
{
    Q_OBJECT

    EasyExif::EXIFInfoList input;
    QDir sourceDir;
    QDir targetDir;
    QString fileSuffix;

    bool wasCanceled;

Q_SIGNALS :

    void progress(int done, int maxCount);

public Q_SLOTS :
    void cancel();

public:
    explicit RawImageGrabber(QObject *parent = nullptr);

    void setInput(const EasyExif::EXIFInfoList& input);
    void setSourceDir(const QDir& dir);
    void setTargetDir(const QDir &dir);
    void setRawFileSuffix(const QString& suffix);

    void process();



};

} // namespace BatchProcess

#endif // BATCHPROCESS_RAWIMAGEGRABBER_H
