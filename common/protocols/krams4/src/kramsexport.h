#ifndef KRAMS_EXPORT_H
#define KRAMS_EXPORT_H

#include <QtCore/qglobal.h>

#if defined(KRAMS_LIBRARY)
#  define KRAMS_EXPORT Q_DECL_EXPORT
#else
#  define KRAMS_EXPORT Q_DECL_IMPORT
#endif //defined(KRAMS_LIBRARY)

#endif // KRAMS_EXPORT_H
