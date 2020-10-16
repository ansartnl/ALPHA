/****************************************************************************
** Meta object code from reading C++ file 'databaselayout.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../common/log4qt/src/databaselayout.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'databaselayout.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Log4Qt__DatabaseLayout_t {
    QByteArrayData data[6];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Log4Qt__DatabaseLayout_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Log4Qt__DatabaseLayout_t qt_meta_stringdata_Log4Qt__DatabaseLayout = {
    {
QT_MOC_LITERAL(0, 0, 22), // "Log4Qt::DatabaseLayout"
QT_MOC_LITERAL(1, 23, 15), // "timeStampColumn"
QT_MOC_LITERAL(2, 39, 16), // "loggerNameColumn"
QT_MOC_LITERAL(3, 56, 16), // "threadNameColumn"
QT_MOC_LITERAL(4, 73, 11), // "levelColumn"
QT_MOC_LITERAL(5, 85, 13) // "messageColumn"

    },
    "Log4Qt::DatabaseLayout\0timeStampColumn\0"
    "loggerNameColumn\0threadNameColumn\0"
    "levelColumn\0messageColumn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Log4Qt__DatabaseLayout[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QString, 0x00095103,
       2, QMetaType::QString, 0x00095103,
       3, QMetaType::QString, 0x00095103,
       4, QMetaType::QString, 0x00095103,
       5, QMetaType::QString, 0x00095103,

       0        // eod
};

void Log4Qt::DatabaseLayout::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        DatabaseLayout *_t = static_cast<DatabaseLayout *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->timeStampColumn(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->loggerNameColumn(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->threadNameColumn(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->levelColumn(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->messageColumn(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        DatabaseLayout *_t = static_cast<DatabaseLayout *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTimeStampColumn(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setLoggerNameColumn(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setThreadNameColumn(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setLevelColumn(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setMessageColumn(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Log4Qt::DatabaseLayout::staticMetaObject = {
    { &Layout::staticMetaObject, qt_meta_stringdata_Log4Qt__DatabaseLayout.data,
      qt_meta_data_Log4Qt__DatabaseLayout,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Log4Qt::DatabaseLayout::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Log4Qt::DatabaseLayout::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__DatabaseLayout.stringdata0))
        return static_cast<void*>(this);
    return Layout::qt_metacast(_clname);
}

int Log4Qt::DatabaseLayout::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Layout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
