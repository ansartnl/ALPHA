#ifndef REPLICACLIENT_GLOBAL_H
#define REPLICACLIENT_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(XTPROTO_LIBRARY)
#  define XTPROTO_EXPORT Q_DECL_EXPORT
#else
#  define XTPROTO_EXPORT Q_DECL_IMPORT
#endif

#endif // REPLICACLIENT_GLOBAL_H
