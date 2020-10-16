/****************************************************************************
** Meta object code from reading C++ file 'synchronizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/database/synchronizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'synchronizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Synchronizer_t {
    QByteArrayData data[15];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Synchronizer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Synchronizer_t qt_meta_stringdata_Synchronizer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Synchronizer"
QT_MOC_LITERAL(1, 13, 25), // "changeLastSynchronization"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 2), // "id"
QT_MOC_LITERAL(4, 43, 19), // "lastSynchronization"
QT_MOC_LITERAL(5, 63, 12), // "changeStatus"
QT_MOC_LITERAL(6, 76, 6), // "status"
QT_MOC_LITERAL(7, 83, 9), // "addServer"
QT_MOC_LITERAL(8, 93, 10), // "ServerInfo"
QT_MOC_LITERAL(9, 104, 4), // "info"
QT_MOC_LITERAL(10, 109, 11), // "synchronize"
QT_MOC_LITERAL(11, 121, 7), // "onError"
QT_MOC_LITERAL(12, 129, 9), // "QSqlQuery"
QT_MOC_LITERAL(13, 139, 5), // "query"
QT_MOC_LITERAL(14, 145, 4) // "text"

    },
    "Synchronizer\0changeLastSynchronization\0"
    "\0id\0lastSynchronization\0changeStatus\0"
    "status\0addServer\0ServerInfo\0info\0"
    "synchronize\0onError\0QSqlQuery\0query\0"
    "text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Synchronizer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       5,    2,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   54,    2, 0x0a /* Public */,
      10,    1,   57,    2, 0x0a /* Public */,
      11,    1,   60,    2, 0x08 /* Private */,
      11,    1,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::QString,   14,

       0        // eod
};

void Synchronizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Synchronizer *_t = static_cast<Synchronizer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeLastSynchronization((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->changeStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->addServer((*reinterpret_cast< const ServerInfo(*)>(_a[1]))); break;
        case 3: _t->synchronize((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->onError((*reinterpret_cast< const QSqlQuery(*)>(_a[1]))); break;
        case 5: _t->onError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ServerInfo >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Synchronizer::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Synchronizer::changeLastSynchronization)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Synchronizer::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Synchronizer::changeStatus)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Synchronizer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Synchronizer.data,
      qt_meta_data_Synchronizer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Synchronizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Synchronizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Synchronizer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Synchronizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Synchronizer::changeLastSynchronization(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Synchronizer::changeStatus(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
