#ifndef FITS_GLOBAL_H
#define FITS_GLOBAL_H

#include "fits.h"
#include <QtCore/qglobal.h>

#if defined(FITS_LIBRARY)
#  define FITSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FITSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FITS_GLOBAL_H
