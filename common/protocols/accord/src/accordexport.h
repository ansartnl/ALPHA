#ifndef ACCORDEXPORT_H
#define ACCORDEXPORT_H

#include <QtCore/qglobal.h>

#if defined(ACCORD_LIBRARY)
#  define ACCORD_EXPORT Q_DECL_EXPORT
#else
#  define ACCORD_EXPORT Q_DECL_IMPORT
#endif //defined(ACCORD_LIBRARY)

#endif // ACCORDEXPORT_H
