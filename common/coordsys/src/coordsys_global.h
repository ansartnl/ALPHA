#ifndef COORDSYS_GLOBAL_H
#define COORDSYS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COORDSYS_LIBRARY)
#  define COORDSYS_EXPORT Q_DECL_EXPORT
#else
#  define COORDSYS_EXPORT Q_DECL_IMPORT
#endif

#endif // COORDSYS_GLOBAL_H
