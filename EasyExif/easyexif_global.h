#ifndef EASYEXIF_GLOBAL_H
#define EASYEXIF_GLOBAL_H

//#include <QtCore/qglobal.h>

#if defined(EASYEXIF_LIBRARY)
#  define EASYEXIFSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EASYEXIFSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EASYEXIF_GLOBAL_H
