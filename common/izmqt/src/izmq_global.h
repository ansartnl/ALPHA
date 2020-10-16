#include <QtCore/QtGlobal>


#if defined(IZMQT_STATIC)
#   define IZMQT_EXPORT
#else
#   if defined(IZMQT_LIBRARY)
#     define IZMQT_EXPORT Q_DECL_EXPORT
#   else
#     define IZMQT_EXPORT Q_DECL_IMPORT
#   endif
#endif