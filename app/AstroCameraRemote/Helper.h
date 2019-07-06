#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QWidget>
#include <QCoreApplication>

//#include "Exception.h"



class Helper
{
    Q_DECLARE_TR_FUNCTIONS(Helper)

public:

    class Log
    {
        static FILE *logfile;
        QString message;
    public :
        enum class Level
        {
            Info = 0
            , Warning
            , Error
        };

        static const QString* modelItemTypeStrings;

        Log(Level level, const QString& file, const QString& func, int line);
        ~Log();

        void apply(const QString& text);
        void apply(const char* text);

        static void close();

        Helper::Log& operator<<(const char* text);
        Helper::Log& operator<<(const QString& text);
        Helper::Log& operator<<(int val);
        Helper::Log& operator<<(const double& val);
        Helper::Log& operator<<(const qint64& val);
        Helper::Log& operator<<(const quint64& val);


    };

};

//Helper::Log& operator<<(Helper::Log& log, const char* text);
//Helper::Log& operator<<(Helper::Log& log, const QString& text);
//Helper::Log& operator<<(Helper::Log& log, int val);
//Helper::Log& operator<<(Helper::Log& log, const double& val);
//Helper::Log& operator<<(Helper::Log& log, const qint64& val);


#ifdef QT_DEBUG
#define SAR_INF(X) ::Helper::Log(Helper::Log::Info, __FILE__, __FUNCTION__, __LINE__) << X
#define SAR_WRN(X) ::Helper::Log(Helper::Log::Warning, __FILE__, __FUNCTION__,  __LINE__) << X
#define SAR_ERR(X) ::Helper::Log(Helper::Log::Error, __FILE__, __FUNCTION__, __LINE__) << X
#define SAR_NOT_IMPLEMENTED ::Helper::Log(Helper::Log::Warning, __FILE__, __FUNCTION__, __LINE__) << " not implemented yet!"
#else
#define SAR_INF(X) qt_noop()
#define SAR_WRN(X) qt_noop()
#define SAR_ERR(X) qt_noop()
#define SAR_NOT_IMPLEMENTED qt_noop()
#endif

#endif // HELPER_H
