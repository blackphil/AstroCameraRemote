#include <AstroBase/AstroBase>

#include <QDir>
#include <QFileInfo>
#include <QTime>
#include <QThread>
#include <QMutexLocker>

namespace AstroBase
{

QSharedPointer<Logging> Logging::instance(new Logging());

void Logging::initLogging(QString applicationName)
{
    instance->logFile.setFileName(QFileInfo(QDir::temp(), QString("%0.log").arg(applicationName)).absoluteFilePath());
    instance->logFile.open(QIODevice::WriteOnly | QIODevice::Text);
    instance->stream.setDevice(&instance->logFile);

    qInstallMessageHandler(messageOutput);
}

static QMutex m;
void Logging::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker l(&m);
    Q_UNUSED(l);

    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        instance->stream << "DBG|";
        break;
    case QtInfoMsg:
        instance->stream << "INF|";
        break;
    case QtWarningMsg:
        instance->stream << "WRN|";
        break;
    case QtCriticalMsg:
        instance->stream << "CRT|";
        break;
    case QtFatalMsg:
        instance->stream << "ERR|";
    }

    instance->stream
            << QTime::currentTime().toString("HH:mm:ss:zzz")
            << "[" << QThread::currentThread()->objectName() << "(" << QThread::currentThreadId() << ")]"
            << "|" << context.file
            << ":" << context.line
            << "(" << context.function << ")"
            << "\t" << localMsg
            << "\n";

    instance->stream.flush();
//    if(QtFatalMsg == type)
//        abort();
}

} //namespace AstroBase
