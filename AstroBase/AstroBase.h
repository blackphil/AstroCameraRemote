#ifndef ASTROBASE_H
#define ASTROBASE_H

#include "astrobase_global.h"
#include <QFile>
#include <QTextStream>
#include <QSharedPointer>
#include <QDebug>
#include <QMutex>
#include <QTime>
#include <QThread>

#include <iostream>

#ifdef QT_DEBUG
#define AB_DBG(X) qDebug() << ":" << X
#else
#define AB_DBG(X) qt_noop()
#endif

#define AB_INF(X) qInfo()     << X
#define AB_WRN(X) qWarning()  << X
#define AB_ERR(X) qCritical() << X
#define AB_NOT_IMPLEMENTED AB_WRN("not implemented yet!")

#define AB_EPSILON 0.000001
#define AB_EQUAL(lhs, rhs) ((qAbs(lhs-rhs) < AB_EPSILON) ? true : false)


namespace AstroBase
{

class ASTROBASESHARED_EXPORT Logging
{
    static QSharedPointer<Logging> instance;
    QFile logFile;
    std::unique_ptr<QTextStream> m_stream;
    bool m_logToFile {false};

    QMutex m;

    template<typename STRM> STRM&
    stream() const;

    template<typename STRM>
    void _messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

public :
    static void initLogging(QString applicationName, bool logToFile = false);
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};


template<typename STRM>
void Logging::_messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QMutexLocker l(&m);
    Q_UNUSED(l);

    switch (type) {
    case QtDebugMsg:
        stream<STRM>() << "DBG|";
        break;
    case QtInfoMsg:
        stream<STRM>() << "INF|";
        break;
    case QtWarningMsg:
        stream<STRM>() << "WRN|";
        break;
    case QtCriticalMsg:
        stream<STRM>() << "CRT|";
        break;
    case QtFatalMsg:
        stream<STRM>() << "ERR|";
    }

    stream<STRM>()
            << QTime::currentTime().toString("HH:mm:ss:zzz").toStdString().c_str()
            << "[" << QThread::currentThread()->objectName().toStdString().c_str() << "(" << QThread::currentThreadId() << ")]"
            << "|" << context.file
            << ":" << context.line
            << "(" << context.function << ")"
            << "\t" << msg.toStdString().c_str()
            << "\n";

    stream<STRM>().flush();
}



} //namespace AstroBase

#endif // ASTROBASE_H
