#ifndef ASTERIXEXPORT_H
#define ASTERIXEXPORT_H

#include <QtCore/qglobal.h>

#if defined(ASTERIX_LIBRARY)
#  define ASTERIX_EXPORT Q_DECL_EXPORT
#else
#  define ASTERIX_EXPORT Q_DECL_IMPORT
#endif //defined(ASTERIX_LIBRARY)

#endif // ASTERIXEXPORT_H
