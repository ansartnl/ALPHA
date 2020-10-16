/****************************************************************************
** Meta object code from reading C++ file 'viewdelegate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/viewdelegate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'viewdelegate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ModifyDelegate_t {
    QByteArrayData data[3];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModifyDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModifyDelegate_t qt_meta_stringdata_ModifyDelegate = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ModifyDelegate"
QT_MOC_LITERAL(1, 15, 6), // "change"
QT_MOC_LITERAL(2, 22, 0) // ""

    },
    "ModifyDelegate\0change\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModifyDelegate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void ModifyDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ModifyDelegate *_t = static_cast<ModifyDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->change(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ModifyDelegate::*_t)() const;
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModifyDelegate::change)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ModifyDelegate::staticMetaObject = {
    { &QItemDelegate::staticMetaObject, qt_meta_stringdata_ModifyDelegate.data,
      qt_meta_data_ModifyDelegate,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ModifyDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModifyDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModifyDelegate.stringdata0))
        return static_cast<void*>(this);
    return QItemDelegate::qt_metacast(_clname);
}

int ModifyDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QItemDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ModifyDelegate::change()const
{
    QMetaObject::activate(const_cast< ModifyDelegate *>(this), &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_RelModifyDelegate_t {
    QByteArrayData data[1];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RelModifyDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RelModifyDelegate_t qt_meta_stringdata_RelModifyDelegate = {
    {
QT_MOC_LITERAL(0, 0, 17) // "RelModifyDelegate"

    },
    "RelModifyDelegate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RelModifyDelegate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void RelModifyDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject RelModifyDelegate::staticMetaObject = {
    { &QSqlRelationalDelegate::staticMetaObject, qt_meta_stringdata_RelModifyDelegate.data,
      qt_meta_data_RelModifyDelegate,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RelModifyDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RelModifyDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RelModifyDelegate.stringdata0))
        return static_cast<void*>(this);
    return QSqlRelationalDelegate::qt_metacast(_clname);
}

int RelModifyDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSqlRelationalDelegate::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_OctetDelegate_t {
    QByteArrayData data[3];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OctetDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OctetDelegate_t qt_meta_stringdata_OctetDelegate = {
    {
QT_MOC_LITERAL(0, 0, 13), // "OctetDelegate"
QT_MOC_LITERAL(1, 14, 20), // "commitAndCloseEditor"
QT_MOC_LITERAL(2, 35, 0) // ""

    },
    "OctetDelegate\0commitAndCloseEditor\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OctetDelegate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void OctetDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OctetDelegate *_t = static_cast<OctetDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->commitAndCloseEditor(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject OctetDelegate::staticMetaObject = {
    { &RelModifyDelegate::staticMetaObject, qt_meta_stringdata_OctetDelegate.data,
      qt_meta_data_OctetDelegate,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *OctetDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OctetDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OctetDelegate.stringdata0))
        return static_cast<void*>(this);
    return RelModifyDelegate::qt_metacast(_clname);
}

int OctetDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RelModifyDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_UpperCaseDelegate_t {
    QByteArrayData data[5];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UpperCaseDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UpperCaseDelegate_t qt_meta_stringdata_UpperCaseDelegate = {
    {
QT_MOC_LITERAL(0, 0, 17), // "UpperCaseDelegate"
QT_MOC_LITERAL(1, 18, 6), // "onEdit"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "text"
QT_MOC_LITERAL(4, 31, 20) // "commitAndCloseEditor"

    },
    "UpperCaseDelegate\0onEdit\0\0text\0"
    "commitAndCloseEditor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UpperCaseDelegate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x08 /* Private */,
       4,    0,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void UpperCaseDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UpperCaseDelegate *_t = static_cast<UpperCaseDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->commitAndCloseEditor(); break;
        default: ;
        }
    }
}

const QMetaObject UpperCaseDelegate::staticMetaObject = {
    { &RelModifyDelegate::staticMetaObject, qt_meta_stringdata_UpperCaseDelegate.data,
      qt_meta_data_UpperCaseDelegate,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *UpperCaseDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UpperCaseDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UpperCaseDelegate.stringdata0))
        return static_cast<void*>(this);
    return RelModifyDelegate::qt_metacast(_clname);
}

int UpperCaseDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RelModifyDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_YesNoDelegate_t {
    QByteArrayData data[3];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_YesNoDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_YesNoDelegate_t qt_meta_stringdata_YesNoDelegate = {
    {
QT_MOC_LITERAL(0, 0, 13), // "YesNoDelegate"
QT_MOC_LITERAL(1, 14, 20), // "commitAndCloseEditor"
QT_MOC_LITERAL(2, 35, 0) // ""

    },
    "YesNoDelegate\0commitAndCloseEditor\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_YesNoDelegate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void YesNoDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        YesNoDelegate *_t = static_cast<YesNoDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->commitAndCloseEditor(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject YesNoDelegate::staticMetaObject = {
    { &RelModifyDelegate::staticMetaObject, qt_meta_stringdata_YesNoDelegate.data,
      qt_meta_data_YesNoDelegate,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *YesNoDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *YesNoDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_YesNoDelegate.stringdata0))
        return static_cast<void*>(this);
    return RelModifyDelegate::qt_metacast(_clname);
}

int YesNoDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RelModifyDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
