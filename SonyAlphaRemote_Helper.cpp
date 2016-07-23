#include "SonyAlphaRemote_Helper.h"

#include <QDateTime>
#include <QTextStream>

//#include "SonyAlphaRemote_Exception.h"

namespace SonyAlphaRemote {

FILE* Helper::Log::logfile(0);

Helper::Log::Log(Level level, const QString& func, int line)
{
    if(!logfile)
    {
        logfile = fopen("log.txt", "w");
    }

    static const QString levelStr[] = { "INF", "WRN", "ERR" };
    QString message = QString("%0 [%1 %2:%3]\t")
                  .arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz"))
                  .arg(levelStr[qBound((int)Info, (int)level, (int)Error)])
                .arg(func)
                .arg(line);
    fprintf_s(logfile, message.toStdString().c_str());
    if (level < Warning)
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
    fprintf(logfile, text);
    if (!message.isEmpty())
        message.append(text);
}


void Helper::Log::close()
{
    if(logfile)
    {
        fclose(logfile);
        logfile = 0;
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



#if 0


Helper::Log& operator<<(Helper::Log& log, const char* text)
{
    return log.operator<<(text);
}

Helper::Log& operator<<(Helper::Log& log, const QString& text)
{
    return log.operator<<(text);
}

Helper::Log& operator<<(Helper::Log& log, int val)
{
    return log.operator<<(val);
}

Helper::Log& operator<<(Helper::Log& log, const double& val)
{
    return log.operator<<(val);
}

Helper::Log& operator<<(Helper::Log& log, const qint64& val)
{
    return log.operator <<(val);
}

#endif

} // namespace Creo
