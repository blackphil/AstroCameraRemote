#ifndef ASTROBASE_H
#define ASTROBASE_H

#include <QFile>
#include <QTextStream>
#include <QSharedPointer>
#include <QDebug>

#ifdef QT_DEBUG
#define AB_DBG(X) qDebug() << __FUNCTION__ << ":" << X
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

class Logging
{
    static QSharedPointer<Logging> instance;
    QFile logFile;
    QTextStream stream;

public :
    static void initLogging(QString applicationName);
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

} //namespace AstroBase

#endif // ASTROBASE_H
