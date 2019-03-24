#include "BatchProcess_RawImageGrabber.h"

#include "AstroBase.h"
#include "AstroBase_Exception.h"

#include <QPair>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>

namespace BatchProcess {



RawImageGrabber::RawImageGrabber(QObject *parent)
    : QObject(parent)
    , sourceDir("E:/DCIM/100MSDCF")
{

}

namespace helper
{
struct TimeWindowSelection
{
    typedef QPair<QDateTime, QDateTime> TimeWindow;
    QString timeWindowToString(const TimeWindow& tw) const
    {
        return QString("%0 - %1").arg(tw.first.toString()).arg(tw.second.toString());
    }
    QList<TimeWindow> data;

    TimeWindowSelection(const EasyExif::EXIFInfoList& input)
    {
        for(auto exif : input)
        {
            QDateTime from = QDateTime::fromString(QString::fromStdString(exif->DateTimeOriginal), "yyyy:MM:dd hh:mm:ss").addSecs(-60*60);
            QDateTime to = QDateTime::fromString(QString::fromStdString(exif->DateTimeOriginal), "yyyy:MM:dd hh:mm:ss").addSecs(60*60);

            bool merged = false;
            for(auto sel : data)
            {
                if((from > sel.first && from < sel.second) || (to > sel.first && to < sel.second))
                {
                    sel.first = qMin(sel.first, from);
                    sel.second = qMax(sel.second, to);
                    AB_DBG("changed timewindow: " << timeWindowToString(sel));
                    merged = true;
                    break;
                }
            }

            if(!merged)
            {
                TimeWindow tw(from, to);
                data.append(tw);
                AB_DBG("NEW timewindow: " << timeWindowToString(tw));
            }
        }
    }

    bool intersect(const QDateTime& dt) const
    {
        for(auto tw : data)
        {
            if(tw.first < dt && tw.second > dt)
                return true;
        }

        return false;
    }
};
}

void RawImageGrabber::setInput(const EasyExif::EXIFInfoList &input)
{
    this->input = input;
}

void RawImageGrabber::setTargetDir(const QString &path)
{
    targetDir = path;
}

void RawImageGrabber::setRawFileSuffix(const QString &suffix)
{
    fileSuffix = suffix;
}

void RawImageGrabber::process()
{
    if(!sourceDir.exists())
        throw AstroBase::DirNotFoundException(sourceDir.absolutePath());

    if(!targetDir.exists())
        throw AstroBase::DirNotFoundException(targetDir.absolutePath());

    helper::TimeWindowSelection selection(input);
    QFileInfoList jpegFiles = sourceDir.entryInfoList(QStringList() << "*.jpeg" << "*.jpg", QDir::Files, QDir::Name);
    for(auto info : jpegFiles)
    {
        QDateTime dt = info.birthTime();
        if(selection.intersect(dt))
        {
            EasyExif::EXIFInfo exif;
            QFile f(info.absoluteFilePath());
            f.open(QIODevice::ReadOnly);
            QByteArray data = f.readAll();
            const unsigned char* dataP = reinterpret_cast<const unsigned char*>(data.data());
            const unsigned int size = static_cast<unsigned int>(data.size());
            exif.parseFrom(dataP, size);

            for(auto inputExif : input)
            {
                QDateTime inputDt = QDateTime::fromString(QString::fromStdString(inputExif->DateTimeOriginal), "yyyy:MM:dd hh:mm:ss");
                QDateTime inputFrom = inputDt.addSecs(-1);
                QDateTime inputTo = inputDt.addSecs(1);
                if(dt >= inputFrom && dt <= inputTo)
                {
                    AB_DBG("Found raw file:" << info.fileName() << ", creation:" << dt.toString("yyyy:MM:dd hh:mm:ss"));
                    QFileInfo sourceFileInfo(sourceDir, QString("%0.%1").arg(info.baseName()).arg(fileSuffix));
                    QFileInfo targetFileInfo(targetDir, QString("%0.%1").arg(info.baseName()).arg(fileSuffix));

                    if(QFile::copy(sourceFileInfo.absoluteFilePath(), targetFileInfo.absoluteFilePath()))
                    {
                        Q_EMIT imageGrabbed(targetFileInfo.absoluteFilePath(), inputExif);
                    }
                }
            }
        }
    }
}

} // namespace BatchProcess
