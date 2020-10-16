#ifndef TVAUTILS_GLOBAL_H
#define TVAUTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifndef QL1S
    #define QL1S QLatin1String
#endif
#ifndef QL1C
    #define QL1C QLatin1Char
#endif

#if defined(TVAUTILS_LIBRARY)
#  define TVAUTILSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TVAUTILSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TVAUTILS_GLOBAL_H
