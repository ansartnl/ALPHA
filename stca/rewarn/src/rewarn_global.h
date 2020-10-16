#ifndef _REWARN_GLOBAL_H__
#define _REWARN_GLOBAL_H__

#include <QtCore/qglobal.h>

#if defined(REWARN_LIBRARY)
#  define REWARN_EXPORT Q_DECL_EXPORT
#else
#  define REWARN_EXPORT Q_DECL_IMPORT
#endif

#endif // _REWARN_GLOBAL_H__
