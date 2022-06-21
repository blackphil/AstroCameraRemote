#include "Helper.h"

#include <QDateTime>
#include <QFile>
#include <QTextStream>

std::unique_ptr<QFile> Helper::Log::logfile { nullptr };

Helper::Log::Log(Level level, const QString& file, const QString& func, int line)
{

    if(!logfile)
    {
        logfile = std::make_unique<QFile>("log.txt");
        if(!logfile->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }
    }

    constexpr const char* levelStr[] = { "INF", "WRN", "ERR" };
    QTextStream strm(logfile.get());
    strm
            << QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")
            << "[" << levelStr[qBound(static_cast<int>(Level::Info), static_cast<int>(level), static_cast<int>(Level::Error))]
            << " " << file
            <<  "(" << func
             << "):" << line
             << "]\t"
                ;
}

Helper::Log::~Log()
{
    logfile->write("\n");
    logfile->flush();
}

void Helper::Log::apply(const QString& text)
{
    apply(text.toStdString().c_str());
}

void Helper::Log::apply(const char* text)
{
    logfile->write(text);
}


void Helper::Log::close()
{
    if(logfile)
    {
        logfile->close();
        logfile.release();
    }
}

Helper::Log& Helper::Log::operator<<(const char* text)
{
    apply(text);
    return *this;
}

Helper::Log& Helper::Log::operator<<(const QString& text)
{
    apply(text);
    return *this;
}

Helper::Log& Helper::Log::operator<<(int val)
{
    apply(QString::number(val));
    return *this;
}

Helper::Log& Helper::Log::operator<<(const double& val)
{
    apply(QString::number(val, 'g'));
    return *this;
}

Helper::Log& Helper::Log::operator<<(const qint64& val)
{
    apply(QString::number(val));
    return *this;
}

Helper::Log& Helper::Log::operator<<(const quint64& val)
{
    apply(QString::number(val));
    return *this;
}




