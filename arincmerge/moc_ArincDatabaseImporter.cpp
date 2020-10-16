/****************************************************************************
** Meta object code from reading C++ file 'ArincDatabaseImporter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/database/ArincDatabaseImporter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ArincDatabaseImporter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ArincDatabaseImporter_t {
    QByteArrayData data[9];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArincDatabaseImporter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArincDatabaseImporter_t qt_meta_stringdata_ArincDatabaseImporter = {
    {
QT_MOC_LITERAL(0, 0, 21), // "ArincDatabaseImporter"
QT_MOC_LITERAL(1, 22, 14), // "rangeEvaluated"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 3), // "max"
QT_MOC_LITERAL(4, 42, 10), // "posChanged"
QT_MOC_LITERAL(5, 53, 5), // "value"
QT_MOC_LITERAL(6, 59, 13), // "statusChanged"
QT_MOC_LITERAL(7, 73, 6), // "status"
QT_MOC_LITERAL(8, 80, 6) // "cancel"

    },
    "ArincDatabaseImporter\0rangeEvaluated\0"
    "\0max\0posChanged\0value\0statusChanged\0"
    "status\0cancel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArincDatabaseImporter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,
       6,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ArincDatabaseImporter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ArincDatabaseImporter *_t = static_cast<ArincDatabaseImporter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->rangeEvaluated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->posChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->statusChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->cancel(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ArincDatabaseImporter::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArincDatabaseImporter::rangeEvaluated)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ArincDatabaseImporter::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArincDatabaseImporter::posChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ArincDatabaseImporter::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArincDatabaseImporter::statusChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject ArincDatabaseImporter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ArincDatabaseImporter.data,
      qt_meta_data_ArincDatabaseImporter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ArincDatabaseImporter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArincDatabaseImporter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ArincDatabaseImporter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ArincDatabaseImporter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ArincDatabaseImporter::rangeEvaluated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ArincDatabaseImporter::posChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ArincDatabaseImporter::statusChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
