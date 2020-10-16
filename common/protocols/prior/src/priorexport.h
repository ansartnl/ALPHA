#ifndef PRIOR_EXPORT_H
#define PRIOR_EXPORT_H

#include <QtCore/qglobal.h>

#if defined(PRIOR_LIBRARY)
#  define PRIOR_EXPORT Q_DECL_EXPORT
#else
#  define PRIOR_EXPORT Q_DECL_IMPORT
#endif //defined(PRIOR_LIBRARY)

#endif // PRIOR_EXPORT_H
