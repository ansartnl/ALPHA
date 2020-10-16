#include <QtCore/QtGlobal>

#if defined(SETTINGS_STATIC)
#   define SETTINGS_EXPORT
#else
#   if defined(SETTINGS_LIBRARY)
#     define SETTINGS_EXPORT Q_DECL_EXPORT
#   else
#     define SETTINGS_EXPORT Q_DECL_IMPORT
#   endif
#endif
