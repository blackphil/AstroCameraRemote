#include <AstroBase/AstroBase>

#include <QDir>
#include <QFileInfo>
#include <QTime>
#include <QThread>
#include <QMutexLocker>

namespace AstroBase
{

QSharedPointer<Logging> Logging::instance(new Logging());



void Logging::initLogging(QString applicationName, bool logToFile)
{
    instance->m_logToFile = logToFile;
    if(logToFile)
    {
        instance->logFile.setFileName(QFileInfo(QDir::temp(), QString("%0.log").arg(applicationName)).absoluteFilePath());
        instance->logFile.open(QIODevice::WriteOnly | QIODevice::Text);
        instance->m_stream = std::make_unique<QTextStream>();
        instance->m_stream->setDevice(&instance->logFile);
    }
    qInstallMessageHandler(messageOutput);
}

void Logging::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(instance->m_logToFile)
    {
        instance->_messageOutput<QTextStream>(type, context, msg);
    }
    else
    {
        instance->_messageOutput<std::ostream>(type, context, msg);
    }
}


template<> std::ostream& Logging::stream<std::ostream>() const
{
    return std::cout;
}

template<> QTextStream& Logging::stream<QTextStream>() const
{
    return *m_stream.get();
}

} //namespace AstroBase
