#pragma once

#include <QList>
#include <QMap>
#include <QRegExp>
#include <QFile>
#ifdef Q_OS_MAC
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#endif

#include "utilities_global.h"


class QMutexLocker;


#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);               \
    TypeName& operator=(const TypeName&)

#define MUTEX_LOCKER(arg) \
    QMutexLocker objMutexLocker(arg); \
    (void)objMutexLocker;

#define STRINGIZE(str) #str
#define STRINGIZEGIZE(x) STRINGIZE(x)

#ifdef QT_NO_DEBUG
#define VERIFY(x) (x)
#else //QT_NO_DEBUG
#define VERIFY(x) Q_ASSERT(x)
#endif //QT_NO_DEBUG

#ifndef PROJECT_NAME
#define PROJECT_NAME "ABSTRACT_PROJECT"
#endif//PROJECT_NAME

#ifndef PROJECT_NAME_LOWERCASE
#define PROJECT_NAME_LOWERCASE "abstract_project"
#endif//PROJECT_NAME_LOWERCASE

#ifndef PROJECT_DOMAIN
#define PROJECT_DOMAIN "abstract-project.com"
#endif//PROJECT_DOMAIN

#ifndef BRAND_VERSION
#define BRAND_VERSION "1.0"
#endif//BRAND_VERSION


class QString;
class QFont;
class QUrl;
class QFile;
class QObject;
#ifndef Q_OS_MAC
class QXmlStreamReader;
class QXmlStreamWriter;
#endif
class QVariant;


class UTILITIES_EXPORT QObjectMap : public QMap<QString, QObject*> {};


#if defined(_MSC_VER) && _MSC_VER < 1800
namespace std {inline int round(double d) {return static_cast<int>(d + 0.5);} }
#endif


namespace utilities {


UTILITIES_EXPORT QString GetOSName();
UTILITIES_EXPORT QString GetOSVersion();
UTILITIES_EXPORT bool CopyFolder(const QString& source, const QString& destination);
UTILITIES_EXPORT bool RemoveDir(const QString& dir_name);
UTILITIES_EXPORT bool IsFolderName(const QString& folder, const QString& name);
UTILITIES_EXPORT void InitializeProjectDescription();
UTILITIES_EXPORT QString PrepareCacheFolder(const QString& subdir = QString());// Parameter: const QString & subdir - subdir to create;  Returns:   QT_NAMESPACE::QString - directory name with ending slash
UTILITIES_EXPORT QFont GetAdaptedFont(int size, int additional_amount = 2);
UTILITIES_EXPORT QList<QUrl> ParseUrls(const QString& data);
UTILITIES_EXPORT bool DeserializeObject(QXmlStreamReader* stream, QObject* object);
UTILITIES_EXPORT void SerializeObject(QXmlStreamWriter* stream, QObject* object, const QString& name, const QString* keyValue = 0);
UTILITIES_EXPORT QString GetFileName(const QString& full_path);
UTILITIES_EXPORT QString SizeToString(quint64 size, int precision = -1, int fieldWidth = 0);
UTILITIES_EXPORT bool IsAsyncUrl(const QString& path);
UTILITIES_EXPORT bool DeleteFileWithWaiting(const QString& file);


// returns true only if (min <= val < max)
template <class Ty>
inline bool IsInBounds(const Ty& min, const Ty& value, const Ty& max) {
    return (min <= value) && (value < max);
}


// correct QString args handling
struct UTILITIES_EXPORT QStringConversionHelper : public QString {
    using QString::number;

    template<typename T>
    static QString number(const T& value) {
        return value.toString();
    }
};


template<typename T>
inline QString asString(const T& value) {
    return QStringConversionHelper::number(value);
}

inline QString asString(const char* value) {
    return QString(value);
}

inline QString asString(const QString& value) {
    return value;
}


#define UTILS_ARGS_MACRO_TEMPL(v) typename T##v
#define UTILS_ARGS_MACRO_DECL(v) const T##v &param##v
#define UTILS_ARGS_MACRO_IMPL(v) asString( param##v )


#define UTILS_INVOKE_PARAMS_MACRO(macro)                                            \
    template<macro(UTILS_ARGS_MACRO_TEMPL)>                                         \
    inline QString args(const char* format, macro(UTILS_ARGS_MACRO_DECL))           \
    {                                                                               \
        return QString(format).arg( macro(UTILS_ARGS_MACRO_IMPL) );                 \
    }


#include "multiparametric.h"


#undef UTILS_ARGS_MACRO_TEMPL
#undef UTILS_ARGS_MACRO_DECL
#undef UTILS_ARGS_MACRO_IMPL


UTILITIES_EXPORT QString multiArg(const QString& str, int numArgs, const QString* args);


template<typename Class, typename FunctionType>
class MemFnAdaptor {
public:
    MemFnAdaptor(Class* c, FunctionType f)
        : m_c(c), m_f(f) {}

    void operator()() { (m_c->*m_f)(); }

#define UTILS_ARGS_MACRO_TEMPL(v) typename T##v
#define UTILS_ARGS_MACRO_DECL(v) const T##v &param##v
#define UTILS_ARGS_MACRO_IMPL(v) param##v

#define UTILS_INVOKE_PARAMS_MACRO(macro) \
    template<macro(UTILS_ARGS_MACRO_TEMPL)> \
    void operator()(macro(UTILS_ARGS_MACRO_DECL)) { (m_c->*m_f)(macro(UTILS_ARGS_MACRO_IMPL)); }

#include "multiparametric.h"

#undef UTILS_ARGS_MACRO_TEMPL
#undef UTILS_ARGS_MACRO_DECL
#undef UTILS_ARGS_MACRO_IMPL

private:
    Class* const m_c;
    FunctionType const m_f;
};


template<typename Class, typename FunctionType>
inline MemFnAdaptor<Class, FunctionType> getMemFnAdaptor(Class* c, FunctionType f) {
    return MemFnAdaptor<Class, FunctionType>(c, f);
}


} // namespace utilities
