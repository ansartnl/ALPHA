#include <QtCore/QtGlobal>


#if defined(UTILITIES_STATIC)
#	define UTILITIES_EXPORT 
#else
#	if defined(UTILITIES_LIBRARY)
#	  define UTILITIES_EXPORT Q_DECL_EXPORT
#	else
#	  define UTILITIES_EXPORT Q_DECL_IMPORT
#	endif
#endif