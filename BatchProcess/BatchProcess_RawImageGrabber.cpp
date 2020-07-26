#include "BatchProcess_RawImageGrabber.h"

#include "AstroBase.h"
#include "AstroBase_Exception.h"

#include <QPair>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QThread>
namespace BatchProcess {



void RawImageGrabber::cancel()
{
    wasCanceled = true;
}

RawImageGrabber::RawImageGrabber(QObject *parent)
    : QObject(parent)
    , wasCanceled(false)
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

void RawImageGrabber::setSourceDir(const QDir &dir)
{
    sourceDir = dir;
}

void RawImageGrabber::setTargetDir(const QDir &dir)
{
    targetDir = dir;
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

    int count = 0;
    Q_EMIT progress(count, input.count());

    helper::TimeWindowSelection selection(input);
    QFileInfoList jpegFiles = sourceDir.entryInfoList(QStringList() << "*.jpeg" << "*.jpg", QDir::Files, QDir::Name);
    for(auto info : jpegFiles)
    {
        if(wasCanceled)
            return;

        QDateTime dt = info.birthTime();
        if(selection.intersect(dt))
        {
            QThread::msleep(100);

            EasyExif::EXIFInfo exif;
            QFile f(info.absoluteFilePath());
            f.open(QIODevice::ReadOnly);
            QByteArray data = f.readAll();
            const unsigned char* dataP = reinterpret_cast<const unsigned char*>(data.data());
            const unsigned int size = static_cast<unsigned int>(data.size());
            auto parseResult = exif.parseFrom(dataP, size);

            switch(parseResult)
            {
            case PARSE_EXIF_SUCCESS                    : AB_DBG("PARSE_EXIF_SUCCESS                "); break;
            case PARSE_EXIF_ERROR_NO_JPEG              : AB_WRN("PARSE_EXIF_ERROR_NO_JPEG          "); break;
            case PARSE_EXIF_ERROR_NO_EXIF              : AB_WRN("PARSE_EXIF_ERROR_NO_EXIF          "); break;
            case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN    : AB_WRN("PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN"); break;
            case PARSE_EXIF_ERROR_CORRUPT              : AB_WRN("PARSE_EXIF_ERROR_CORRUPT          "); break;
            }

            for(auto inputExif : input)
            {
                if(wasCanceled)
                    return;

                QDateTime inputDt = QDateTime::fromString(QString::fromStdString(inputExif->DateTimeOriginal), "yyyy:MM:dd hh:mm:ss");
                QDateTime inputFrom = inputDt.addSecs(-2);
                QDateTime inputTo = inputDt.addSecs(2);

//                QString dbgDt = dt.toString("yyyy:MM:dd hh:mm:ss");
//                QString dbgInDt = inputDt.toString("yyyy:MM:dd hh:mm:ss");
//                QString dbgInFrom = inputFrom.toString("yyyy:MM:dd hh:mm:ss");
//                QString dbgInTo = inputTo.toString("yyyy:MM:dd hh:mm:ss");


                if(dt >= inputFrom && dt <= inputTo)
                {
                    AB_DBG("Found raw file:" << info.fileName() << ", creation:" << dt.toString("yyyy:MM:dd hh:mm:ss"));
                    QFileInfo sourceFileInfo(sourceDir, QString("%0.%1").arg(info.baseName()).arg(fileSuffix));
                    QFileInfo targetFileInfo(targetDir, QString("%0.%1").arg(info.baseName()).arg(fileSuffix));

                    QFile::copy(sourceFileInfo.absoluteFilePath(), targetFileInfo.absoluteFilePath());

                    count++;
                    Q_EMIT progress(count, input.count());

                    break;

                }
            }
        }
    }
}

} // namespace BatchProcess
