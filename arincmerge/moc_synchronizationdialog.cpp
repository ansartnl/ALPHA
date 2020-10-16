/****************************************************************************
** Meta object code from reading C++ file 'synchronizationdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/dialogs/synchronizationdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'synchronizationdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SynchronizationDialog_t {
    QByteArrayData data[12];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SynchronizationDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SynchronizationDialog_t qt_meta_stringdata_SynchronizationDialog = {
    {
QT_MOC_LITERAL(0, 0, 21), // "SynchronizationDialog"
QT_MOC_LITERAL(1, 22, 10), // "pushServer"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 10), // "ServerInfo"
QT_MOC_LITERAL(4, 45, 4), // "info"
QT_MOC_LITERAL(5, 50, 11), // "synchronize"
QT_MOC_LITERAL(6, 62, 2), // "id"
QT_MOC_LITERAL(7, 65, 19), // "onSynchronizeButton"
QT_MOC_LITERAL(8, 85, 27), // "onChangeLastSynchronization"
QT_MOC_LITERAL(9, 113, 19), // "lastSynchronization"
QT_MOC_LITERAL(10, 133, 14), // "onChangeStatus"
QT_MOC_LITERAL(11, 148, 6) // "status"

    },
    "SynchronizationDialog\0pushServer\0\0"
    "ServerInfo\0info\0synchronize\0id\0"
    "onSynchronizeButton\0onChangeLastSynchronization\0"
    "lastSynchronization\0onChangeStatus\0"
    "status"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SynchronizationDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   45,    2, 0x0a /* Public */,
       8,    2,   46,    2, 0x08 /* Private */,
      10,    2,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,   11,

       0        // eod
};

void SynchronizationDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SynchronizationDialog *_t = static_cast<SynchronizationDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pushServer((*reinterpret_cast< const ServerInfo(*)>(_a[1]))); break;
        case 1: _t->synchronize((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->onSynchronizeButton(); break;
        case 3: _t->onChangeLastSynchronization((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->onChangeStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SynchronizationDialog::*_t)(const ServerInfo & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SynchronizationDialog::pushServer)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SynchronizationDialog::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SynchronizationDialog::synchronize)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject SynchronizationDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SynchronizationDialog.data,
      qt_meta_data_SynchronizationDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SynchronizationDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SynchronizationDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SynchronizationDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SynchronizationDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SynchronizationDialog::pushServer(const ServerInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SynchronizationDialog::synchronize(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
