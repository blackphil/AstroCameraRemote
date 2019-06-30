#include "Helper.h"

#include <QDateTime>
#include <QTextStream>

//#include "Exception.h"



FILE* Helper::Log::logfile { nullptr };

Helper::Log::Log(Level level, const QString& file, const QString& func, int line)
{

    if(!logfile)
    {
        if(auto err = fopen_s(&logfile, "log.txt", "w"); err != 0)
        {
            return;
        }
    }

    constexpr const char* levelStr[] = { "INF", "WRN", "ERR" };
    QString message
    {
        QString("%0 [%1 %2(%3):%4]\t")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz"))
            .arg(levelStr[qBound(static_cast<int>(Level::Info), static_cast<int>(level), static_cast<int>(Level::Error))])
            .arg(file)
            .arg(func)
            .arg(line)
    };

    fprintf_s(logfile, message.toStdString().c_str());
    if (level < Level::Warning)
        message.clear();
}

Helper::Log::~Log()
{
    fprintf(logfile, "\n");
    fflush(logfile);
}

void Helper::Log::apply(const QString& text)
{
    apply(text.toStdString().c_str());
}

void Helper::Log::apply(const char* text)
{
    fprintf_s(logfile, text);
    if (!message.isEmpty())
        message.append(text);
}


void Helper::Log::close()
{
    if(logfile)
    {
        fclose(logfile);
        logfile = nullptr;
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




