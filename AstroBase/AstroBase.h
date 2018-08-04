#ifndef ASTROBASE_H
#define ASTROBASE_H

#include "astrobase_global.h"

#include <QDebug>

#ifdef QT_DEBUG
#define AB_DBG(X) qDebug()    << "INF(" << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ << ")" << X
#else
#define AB_DBG(X) qt_noop()
#endif

#define AB_INF(X) qInfo()     << "INF(" << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ << ")" << X
#define AB_WRN(X) qWarning()  << "WRN(" << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ << ")" << X
#define AB_ERR(X) qCritical() << "ERR(" << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ << ")" << X
#define AB_NOT_IMPLEMENTED AB_WRN("not implemented yet!")



#endif // ASTROBASE_H
