/****************************************************************************
** Meta object code from reading C++ file 'databaseappender.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../common/log4qt/src/databaseappender.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'databaseappender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Log4Qt__DatabaseAppender_t {
    QByteArrayData data[3];
    char stringdata0[42];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Log4Qt__DatabaseAppender_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Log4Qt__DatabaseAppender_t qt_meta_stringdata_Log4Qt__DatabaseAppender = {
    {
QT_MOC_LITERAL(0, 0, 24), // "Log4Qt::DatabaseAppender"
QT_MOC_LITERAL(1, 25, 10), // "connection"
QT_MOC_LITERAL(2, 36, 5) // "table"

    },
    "Log4Qt::DatabaseAppender\0connection\0"
    "table"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Log4Qt__DatabaseAppender[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QString, 0x00095103,
       2, QMetaType::QString, 0x00095103,

       0        // eod
};

void Log4Qt::DatabaseAppender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        DatabaseAppender *_t = static_cast<DatabaseAppender *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->connection(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->table(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        DatabaseAppender *_t = static_cast<DatabaseAppender *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setConnection(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setTable(*reinterpret_cast< QString*>(_v)); break;
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

const QMetaObject Log4Qt::DatabaseAppender::staticMetaObject = {
    { &AppenderSkeleton::staticMetaObject, qt_meta_stringdata_Log4Qt__DatabaseAppender.data,
      qt_meta_data_Log4Qt__DatabaseAppender,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Log4Qt::DatabaseAppender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Log4Qt::DatabaseAppender::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Log4Qt__DatabaseAppender.stringdata0))
        return static_cast<void*>(this);
    return AppenderSkeleton::qt_metacast(_clname);
}

int Log4Qt::DatabaseAppender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppenderSkeleton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
