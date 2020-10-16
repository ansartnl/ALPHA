#include "utils.h"

#if defined(Q_OS_WIN)
#include <windows.h>
#if _MSC_VER >= 1800
#include <VersionHelpers.h>
#endif
#elif defined(Q_OS_DARWIN)
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include "Darwin/AppHandler.h"
#endif


#include <algorithm>

#include <QObject>
#include <QString>
#include <QMap>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QByteArray>
#include <QUrl>
#include <QThread>
#include <QDesktopServices>
#ifndef Q_OS_MAC
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#endif
#include <QMetaProperty>
#include <QDebug>
#include <QCoreApplication>

// for serializing
Q_DECLARE_METATYPE(QObjectList)
Q_DECLARE_METATYPE(QObjectMap)


namespace utilities {


const char kValidIPAddressRegex[] =
    "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$";
const char kValidHostnameRegex[] = // modified to have at least one '.'
    "^(([a-zA-Z]|[a-zA-Z][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)+([A-Za-z]|[A-Za-z][A-Za-z0-9\\-]*[A-Za-z0-9])$";
const char kClassNameAttribute[] = "className";
const char kKeyValueAttribute[] = "keyValue";
const char kObjectNameProperty[] = "objectName";
const char* kURLPrefixes[] = {"http://", "https://", "vidxden://1", "ftp://"};
const int kOffsetPastSeparator[] = {2, 2, 3, 2};


// self cleaning containers
class MapStringToObjectList : public QMap<QString, QObjectList> {
public:
    ~MapStringToObjectList() {
        std::for_each<iterator, void (*)(const QObjectList&)>(begin(), end(), qDeleteAll<QObjectList>);
    }
};

class MapStringToObjectMap : public QMap<QString, QObjectMap> {
public:
    ~MapStringToObjectMap() {
        std::for_each<iterator, void (*)(const QObjectMap&)>(begin(), end(), qDeleteAll<QObjectMap>);
    }
};


template <typename T, size_t N>
char(&ArraySizeHelper(T(&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))


bool DeserializeObjectInternal(QXmlStreamReader* stream, QObject* object) {
    if (0 == object) {
        return false;
    }

    const int qObjectListId = qMetaTypeId<QObjectList>();
    const int qStringListId = qMetaTypeId<QStringList>();
    const int qObjectMapId = qMetaTypeId<QObjectMap>();
    const int qObjectPtrId = qMetaTypeId<QObject*>();

    Q_ASSERT(qObjectListId != 0);
    Q_ASSERT(qStringListId != 0);
    Q_ASSERT(qObjectMapId != 0);
    Q_ASSERT(qObjectPtrId != 0);

    MapStringToObjectList children;
    MapStringToObjectMap mapChildren;

    const QMetaObject* metaObject = object->metaObject();

    // save attributes before proceeding further
    QXmlStreamAttributes attributes = stream->attributes();

    while (stream->readNextStartElement()) {
        QString name = stream->name().toString();
        QString className = stream->attributes().value(kClassNameAttribute).toString();
        QString keyValue = stream->attributes().value(kKeyValueAttribute).toString();
        const int classId = QMetaType::type(className.toLatin1().data());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QObject* instance = (QObject*) QMetaType::create(classId, 0);
#else
        QObject* instance = (QObject*) QMetaType::construct(classId, 0);
#endif
        if (!DeserializeObjectInternal(stream, instance)) {
            qDebug()
                    << __FUNCTION__ << ": cannot serialize object with name: " << name
                    << ", className: " << className;
            return false;
        }
        if (keyValue.isEmpty()) {
            children[name].push_back(instance);
        } else {
            mapChildren[name][keyValue] = instance;
        }
    }

    if (stream->hasError()) {
        qDebug()
                << __FUNCTION__ << ": serialization error: " << stream->errorString()
                << " at line: " << stream->lineNumber() << ", column: " << stream->columnNumber();
        return false;
    }

    // Do stuffing after validation
    // so that serialized object would not be touched if error happens

    const int propCount = metaObject->propertyCount();
    for (int i = 0; i < propCount; ++i) {
        QMetaProperty prop = metaObject->property(i);
        QString propName = prop.name();
        if (kObjectNameProperty == propName) {
            continue;
        }

        const int userType = prop.userType();
        if (qObjectMapId == userType) {
            MapStringToObjectMap::iterator it = mapChildren.find(propName);
            if (mapChildren.end() != it) {
                void* pData = (void*)&*it;
                QVariant value(userType, pData);
                prop.write(object, value);
                // we transfer ownership here
                mapChildren.erase(it);
            }
        } else if (qObjectListId == userType || qObjectPtrId == userType) {
            MapStringToObjectList::iterator it = children.find(propName);
            if (children.end() != it) {
                void* pData = (qObjectListId == userType) ? (void*)&*it : &(*it)[0];
                QVariant value(userType, pData);
                prop.write(object, value);
                // we transfer ownership here
                children.erase(it);
            }
        } else if (qStringListId == userType) {
            QString separatedList = attributes.value(propName).toString();
            QStringList list = separatedList.split('|');
            QVariant value(list);
            prop.write(object, value);
        } else if (attributes.hasAttribute(propName)) {
            if (prop.isEnumType()) {
                QString strValue = attributes.value(propName).toString().trimmed();
                if (!strValue.isEmpty()) {
                    bool ok = false;
                    int iValue = strValue.toInt(&ok);
                    QVariant value;
                    if (ok) {
                        value = iValue;
                    } else {
                        QMetaEnum qme = prop.enumerator();
                        value = qme.keyToValue(strValue.toLatin1().data());
                    }
                    prop.write(object, value);
                }
            } else {
                QVariant value(attributes.value(propName).toString());
                if (value.convert(prop.type())) {
                    prop.write(object, value);
                } else {
                    qDebug()
                            << __FUNCTION__ << ": could not convert property with name: " << propName
                            << " and value: " << attributes.value(propName) << " to type: " << prop.type();
                }
            }
        } else {
            // remove if we omit serialization of empty properties
            qDebug() << __FUNCTION__ << ": missing attribute with name: " << propName;
        }
    }

    Q_ASSERT(children.empty());
    Q_ASSERT(mapChildren.empty());

    return true;
}

QString GetOSName() {
#if defined(Q_OS_LINUX)
    return "linux";
#elif defined(Q_OS_MAC)
    return "mac";
#elif defined(Q_OS_WIN)

#if defined(_WIN64)
    return "win64";  // 64-bit programs run only on Win64
#elif defined(_WIN32) && defined(_MSC_VER)
    // 32-bit programs run on both 32-bit and 64-bit Windows
    // so must sniff
    BOOL f64 = FALSE;
    return (IsWow64Process(GetCurrentProcess(), &f64) && f64 ? "win64" : "win32");
#else
    return "win32"; // Win64 does not support Win16
#endif

#else
    return "unknown";
#endif
}

QString GetOSVersion() {
#if defined(Q_OS_LINUX)
    return "linux";
#elif defined(Q_OS_MAC)
    return Darwin::getOsVersion();
#elif defined(Q_OS_WIN)
#if _MSC_VER < 1800
    OSVERSIONINFO osvi;
    //  BOOL bIsWindowsXPorLater;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    return QString("%1.%2").arg(osvi.dwMajorVersion).arg(osvi.dwMinorVersion);
#else
    for (int major = 9; major > 4; --major)
        for (int minor = 5; minor > -1; --minor)
            for (int sp = 5; sp > -1; --sp)
                if (IsWindowsVersionOrGreater(major, minor, sp))
                    return QString("%1.%2.%3").arg(major).arg(minor).arg(sp);
#endif
#endif
    return "Undefined";
}

bool CopyFolder(const QString& source, const QString& destination) {
    QDir source_dir(source);
    if (!source_dir.exists()) return false;

    QDir dest_dir(destination);
    if (!dest_dir.exists()) dest_dir.mkpath(destination);

    QStringList files = source_dir.entryList(QDir::Files);
    for (int i = 0; i < files.count(); ++i)
        QFile::copy(source_dir.absoluteFilePath(files[i]), dest_dir.absoluteFilePath(files[i]));

    files.clear();
    files = source_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < files.count(); ++i)
        CopyFolder(source_dir.absoluteFilePath(files[i]), dest_dir.absoluteFilePath(files[i]));

    return true;
}

bool RemoveDir(const QString& dir_name) {
    bool res = true;
    QDir dir(dir_name);
    if (dir.exists(dir_name)) {
        QFileInfoList info_list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
        for (auto it = info_list.begin(); it != info_list.end(); ++it) {
            if (it->isDir())
                res = RemoveDir(it->absoluteFilePath());
            else
                res = QFile::remove(it->absoluteFilePath());
            if (!res) return res;
        }
        res = dir.rmdir(dir_name);
    }
    return res;
}

bool IsFolderName(const QString& folder, const QString& name) {
    QString _folder = QDir::fromNativeSeparators(folder.toLower());
    QString _name = QDir::fromNativeSeparators(name.toLower());

    //QString test=QString("^%1[/\\\\]?$").arg(_name);// check 'test', 'test\' and 'test\\'
    return QRegExp(QString("^%1[/\\\\]?$").arg(_name)).exactMatch(_folder);
}

QList<QUrl> ParseUrls(const QString& data) {
    QStringList list = data.split(QRegExp("[\\s\\\"\\n]+"), QString::SkipEmptyParts);
    QList<QUrl> res;
    for (auto it = list.begin(); it != list.end(); ++it) {
        QString t = it->trimmed();
        if (t.isEmpty())
            continue;
        QUrl url;
        int offset = t.indexOf("//");
        if (offset > 0) {
            // fix for urls that have few starting symbols lost
            static_assert(arraysize(kURLPrefixes) == arraysize(kOffsetPastSeparator),
                          "Sizes of kURLPrefixes and kOffsetPastSeparator should be the same");
            for (size_t i = 0; i < arraysize(kURLPrefixes); ++i) {
                int start = offset + kOffsetPastSeparator[i];
                QString protocol_prefix = t.left(start);
                QString prefix(kURLPrefixes[i]);
                if (prefix.endsWith(protocol_prefix, Qt::CaseInsensitive)) {
                    url.setUrl(prefix + t.mid(start).remove('"'));
                    break;
                }
            }
        } else {
            // Assume that http:// is default
            if (0 == offset)
                t = t.mid(2);
            else if (t.startsWith('/'))
                t = t.mid(1);

            url.setUrl("http://" + t);
            QString host = url.host();
            if (!host.contains(QRegExp(kValidIPAddressRegex)) &&
                    !host.contains(QRegExp(kValidHostnameRegex)))
                continue;
            if (url.path().isEmpty())
                continue;
        }
        if (url.isValid())
            res << url;
    }
    return res;
}

void InitializeProjectDescription() {
    static struct Initializer {
        Initializer() {
            QCoreApplication::setApplicationVersion(BRAND_VERSION);
            QCoreApplication::setApplicationName(PROJECT_NAME);
            QCoreApplication::setOrganizationName(PROJECT_NAME_LOWERCASE);
            QCoreApplication::setOrganizationDomain(PROJECT_DOMAIN);
        }
    } initializer;
}

QString PrepareCacheFolder(const QString& subdir) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QString path = QStandardPaths::standardLocations(QStandardPaths::DataLocation)[0];
#else
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
    if (!subdir.isEmpty())
        path += QDir::separator() + subdir;
    Q_ASSERT(!path.isEmpty());

    QDir().mkpath(path);
    return path + QDir::separator();
}

QFont GetAdaptedFont(int size, int additional_amount) {
#ifdef Q_OS_DARWIN
    QFont f("Lucida Grande");
    f.setPixelSize(size + additional_amount);
    return f;
#else
    Q_UNUSED(additional_amount);
    const float koef = 4 / 3.f;
    QFont f("Segoe UI");
    f.setPixelSize(size * koef);
    return f;
#endif
}

bool DeserializeObject(QXmlStreamReader* stream, QObject* object) {
    return stream->readNextStartElement() && DeserializeObjectInternal(stream, object);
}

void SerializeObject(QXmlStreamWriter* stream, QObject* object, const QString& name, const QString* keyValue /*= 0*/) {
    if (0 == object) {
        return;
    }

    const QMetaObject* metaObject = object->metaObject();

    stream->writeStartElement(name);
    if (keyValue != 0) {
        stream->writeAttribute(kKeyValueAttribute, *keyValue);
    }
    stream->writeAttribute(kClassNameAttribute, metaObject->className());

    const int propCount = metaObject->propertyCount();
    for (int i = 0; i < propCount; ++i) {
        QMetaProperty prop = metaObject->property(i);
        QString propName = prop.name();
        if (kObjectNameProperty == propName) {
            continue;
        }

        QVariant value = prop.read(object);
        if (value.canConvert<QObjectMap>()) {
            QObjectMap qObjectList = value.value<QObjectMap>();
            for (QObjectMap::iterator it(qObjectList.begin()), itEnd(qObjectList.end()); it != itEnd; ++it) {
                const QString key = it.key();
                SerializeObject(stream, *it, propName, &key);
            }
        } else if (value.canConvert<QObjectList>()) {
            QObjectList qObjectList = value.value<QObjectList>();
            Q_FOREACH(QObject * item, qObjectList) {
                SerializeObject(stream, item, propName);
            }
        } else if (value.canConvert<QStringList>()) {
            // this branch works for QString values as well
            // since QString can be converted to QStringList with one element in it
            // but it is OK for now
            QStringList qStringList = value.value<QStringList>();
            QString separatedList = qStringList.join("|");
            stream->writeAttribute(propName, separatedList);
        } else if (value.canConvert<QObject*>()) {
            QObject* qObject = value.value<QObject*>();
            SerializeObject(stream, qObject, propName);
        } else {
            if (prop.isEnumType()) {
                QMetaEnum qme = prop.enumerator();
                const char* key = qme.valueToKey(value.toInt());
                if (key != 0) {
                    value = key;
                }
            }
            stream->writeAttribute(propName, value.toString());
        }
    }

    stream->writeEndElement();
}

QString GetFileName(const QString& full_path) {
    QString file_name = QFileInfo(full_path).fileName();
    return (file_name.isEmpty()) ? full_path : file_name;
}

QString SizeToString(quint64 size, int precision, int fieldWidth) {
    QString result;
    double sizef = 0.0f;
    const unsigned int Kbytes_limit = 1 << 10; //1 Kb
    const unsigned int Mbytes_limit = 1 << 20; //1 Mb
    const unsigned int Gbytes_limit = 1 << 30; //1 Gb

    if (size < Kbytes_limit) {
        result = QString("%1 B").arg(size);
    } else if (size < Mbytes_limit) {
        sizef = size / static_cast<double>(Kbytes_limit);
        result = QString("%1 kB").arg(sizef, fieldWidth, 'f', precision);
    } else if (size < Gbytes_limit) {
        sizef = size / static_cast<double>(Mbytes_limit);
        result = QString("%1 MB").arg(sizef, fieldWidth, 'f', precision);
    } else {
        sizef = size / static_cast<double>(Gbytes_limit);
        result = QString("%1 GB").arg(sizef, fieldWidth, 'f', precision);
    }
    return result;
}

bool IsAsyncUrl(const QString& path) {
    QString s(path.toLower().trimmed());
    return s.contains(QRegExp("^https?://")) || s.startsWith("qrc:/");
}

bool DeleteFileWithWaiting(const QString& file) {
    if (!QFile::remove(file)) {
        QThread::yieldCurrentThread();
        if (!QFile::remove(file)) return false;
    }
    return true;
}

// shamelessly stolen from qstring.cpp
int getEscape(const QChar* uc, int* pos, int len, int maxNumber = 999) {
    int i = *pos;
    ++i;
    if (i < len && uc[i] == QLatin1Char('L'))
        ++i;
    if (i < len) {
        int escape = uc[i].unicode() - '0';
        if (uint(escape) >= 10U)
            return -1;
        ++i;
        while (i < len) {
            int digit = uc[i].unicode() - '0';
            if (uint(digit) >= 10U)
                break;
            escape = (escape * 10) + digit;
            ++i;
        }
        if (escape <= maxNumber) {
            *pos = i;
            return escape;
        }
    }
    return -1;
}

QString multiArg(const QString& str, int numArgs, const QString* args) {
    QString result;
    QMap<int, int> numbersUsed;
    const QChar* uc = str.constData();
    const int len = str.size();
    const int end = len - 1;
    int lastNumber = -1;
    int i = 0;

    // populate the numbersUsed map with the %n's that actually occur in the string
    while (i < end) {
        if (uc[i] == QLatin1Char('%')) {
            int number = getEscape(uc, &i, len);
            if (number != -1) {
                numbersUsed.insert(number, -1);
                continue;
            }
        }
        ++i;
    }

    // assign an argument number to each of the %n's
    QMap<int, int>::iterator j = numbersUsed.begin();
    QMap<int, int>::iterator jend = numbersUsed.end();
    int arg = 0;
    while (j != jend && arg < numArgs) {
        *j = arg++;
        lastNumber = j.key();
        ++j;
    }

    // sanity
    if (numArgs > arg) {
        qWarning("QString::arg: %d argument(s) missing in %s", numArgs - arg, str.toLocal8Bit().data());
        numArgs = arg;
    }

    i = 0;
    while (i < len) {
        if (uc[i] == QLatin1Char('%') && i != end) {
            int number = getEscape(uc, &i, len, lastNumber);
            int arg = numbersUsed[number];
            if (number != -1 && arg != -1) {
                result += args[arg];
                continue;
            }
        }
        result += uc[i++];
    }
    return result;
}


} // namespace utilities
