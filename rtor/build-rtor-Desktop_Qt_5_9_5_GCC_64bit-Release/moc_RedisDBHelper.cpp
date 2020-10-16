/****************************************************************************
** Meta object code from reading C++ file 'RedisDBHelper.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rtor/src/RedisDBHelper.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RedisDBHelper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RedisDBHelper_t {
    QByteArrayData data[20];
    char stringdata0[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RedisDBHelper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RedisDBHelper_t qt_meta_stringdata_RedisDBHelper = {
    {
QT_MOC_LITERAL(0, 0, 13), // "RedisDBHelper"
QT_MOC_LITERAL(1, 14, 11), // "changedSFPL"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 21), // "QSharedPointer<CSFPL>"
QT_MOC_LITERAL(4, 49, 4), // "sfpl"
QT_MOC_LITERAL(5, 54, 8), // "operType"
QT_MOC_LITERAL(6, 63, 11), // "changedRWF1"
QT_MOC_LITERAL(7, 75, 5), // "idFIR"
QT_MOC_LITERAL(8, 81, 19), // "RunwayWidgetDataFIR"
QT_MOC_LITERAL(9, 101, 3), // "rwf"
QT_MOC_LITERAL(10, 105, 11), // "changedRWF2"
QT_MOC_LITERAL(11, 117, 13), // "changedRWFVPP"
QT_MOC_LITERAL(12, 131, 19), // "RunwayWidgetDataVPP"
QT_MOC_LITERAL(13, 151, 18), // "changedRestriction"
QT_MOC_LITERAL(14, 170, 3), // "uid"
QT_MOC_LITERAL(15, 174, 9), // "operation"
QT_MOC_LITERAL(16, 184, 11), // "onRedisData"
QT_MOC_LITERAL(17, 196, 14), // "QtRedis::Reply"
QT_MOC_LITERAL(18, 211, 4), // "data"
QT_MOC_LITERAL(19, 216, 12) // "updateExpire"

    },
    "RedisDBHelper\0changedSFPL\0\0"
    "QSharedPointer<CSFPL>\0sfpl\0operType\0"
    "changedRWF1\0idFIR\0RunwayWidgetDataFIR\0"
    "rwf\0changedRWF2\0changedRWFVPP\0"
    "RunwayWidgetDataVPP\0changedRestriction\0"
    "uid\0operation\0onRedisData\0QtRedis::Reply\0"
    "data\0updateExpire"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RedisDBHelper[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       6,    3,   54,    2, 0x06 /* Public */,
      10,    3,   61,    2, 0x06 /* Public */,
      11,    3,   68,    2, 0x06 /* Public */,
      13,    2,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    1,   80,    2, 0x09 /* Protected */,
      19,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8, QMetaType::QString,    7,    9,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8, QMetaType::QString,    7,    9,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 12, QMetaType::QString,    7,    9,    5,
    QMetaType::Void, QMetaType::QUuid, QMetaType::QString,   14,   15,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Bool,

       0        // eod
};

void RedisDBHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RedisDBHelper *_t = static_cast<RedisDBHelper *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changedSFPL((*reinterpret_cast< QSharedPointer<CSFPL>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->changedRWF1((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< RunwayWidgetDataFIR(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->changedRWF2((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< RunwayWidgetDataFIR(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: _t->changedRWFVPP((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< RunwayWidgetDataVPP(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 4: _t->changedRestriction((*reinterpret_cast< const QUuid(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->onRedisData((*reinterpret_cast< QtRedis::Reply(*)>(_a[1]))); break;
        case 6: { bool _r = _t->updateExpire();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSharedPointer<CSFPL> >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< RunwayWidgetDataFIR >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< RunwayWidgetDataFIR >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< RunwayWidgetDataVPP >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RedisDBHelper::*_t)(QSharedPointer<CSFPL> , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDBHelper::changedSFPL)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RedisDBHelper::*_t)(QString , RunwayWidgetDataFIR , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDBHelper::changedRWF1)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RedisDBHelper::*_t)(QString , RunwayWidgetDataFIR , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDBHelper::changedRWF2)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RedisDBHelper::*_t)(QString , RunwayWidgetDataVPP , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDBHelper::changedRWFVPP)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RedisDBHelper::*_t)(const QUuid & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDBHelper::changedRestriction)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject RedisDBHelper::staticMetaObject = {
    { &RedisDB::staticMetaObject, qt_meta_stringdata_RedisDBHelper.data,
      qt_meta_data_RedisDBHelper,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RedisDBHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RedisDBHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RedisDBHelper.stringdata0))
        return static_cast<void*>(this);
    return RedisDB::qt_metacast(_clname);
}

int RedisDBHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RedisDB::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void RedisDBHelper::changedSFPL(QSharedPointer<CSFPL> _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RedisDBHelper::changedRWF1(QString _t1, RunwayWidgetDataFIR _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RedisDBHelper::changedRWF2(QString _t1, RunwayWidgetDataFIR _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RedisDBHelper::changedRWFVPP(QString _t1, RunwayWidgetDataVPP _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RedisDBHelper::changedRestriction(const QUuid & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
