/****************************************************************************
** Meta object code from reading C++ file 'redisdb.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rtor/src/redisdb.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'redisdb.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RedisConnectionsWorker_t {
    QByteArrayData data[21];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RedisConnectionsWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RedisConnectionsWorker_t qt_meta_stringdata_RedisConnectionsWorker = {
    {
QT_MOC_LITERAL(0, 0, 22), // "RedisConnectionsWorker"
QT_MOC_LITERAL(1, 23, 9), // "connected"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 14), // "connectedParam"
QT_MOC_LITERAL(4, 49, 2), // "ip"
QT_MOC_LITERAL(5, 52, 4), // "port"
QT_MOC_LITERAL(6, 57, 13), // "connectFailed"
QT_MOC_LITERAL(7, 71, 11), // "RedisParams"
QT_MOC_LITERAL(8, 83, 1), // "p"
QT_MOC_LITERAL(9, 85, 12), // "disconnected"
QT_MOC_LITERAL(10, 98, 5), // "error"
QT_MOC_LITERAL(11, 104, 3), // "err"
QT_MOC_LITERAL(12, 108, 8), // "finished"
QT_MOC_LITERAL(13, 117, 20), // "chekConnectionStatus"
QT_MOC_LITERAL(14, 138, 6), // "status"
QT_MOC_LITERAL(15, 145, 18), // "changeCurrentIndex"
QT_MOC_LITERAL(16, 164, 5), // "index"
QT_MOC_LITERAL(17, 170, 5), // "start"
QT_MOC_LITERAL(18, 176, 4), // "stop"
QT_MOC_LITERAL(19, 181, 7), // "onTimer"
QT_MOC_LITERAL(20, 189, 11) // "onConnected"

    },
    "RedisConnectionsWorker\0connected\0\0"
    "connectedParam\0ip\0port\0connectFailed\0"
    "RedisParams\0p\0disconnected\0error\0err\0"
    "finished\0chekConnectionStatus\0status\0"
    "changeCurrentIndex\0index\0start\0stop\0"
    "onTimer\0onConnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RedisConnectionsWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    2,   75,    2, 0x06 /* Public */,
       6,    1,   80,    2, 0x06 /* Public */,
       9,    0,   83,    2, 0x06 /* Public */,
      10,    1,   84,    2, 0x06 /* Public */,
      12,    0,   87,    2, 0x06 /* Public */,
      13,    2,   88,    2, 0x06 /* Public */,
      15,    1,   93,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    0,   96,    2, 0x0a /* Public */,
      18,    0,   97,    2, 0x0a /* Public */,
      19,    0,   98,    2, 0x09 /* Protected */,
      20,    0,   99,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    4,   14,
    QMetaType::Void, QMetaType::Int,   16,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RedisConnectionsWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RedisConnectionsWorker *_t = static_cast<RedisConnectionsWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->connectedParam((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->connectFailed((*reinterpret_cast< const RedisParams(*)>(_a[1]))); break;
        case 3: _t->disconnected(); break;
        case 4: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->finished(); break;
        case 6: _t->chekConnectionStatus((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->changeCurrentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->start(); break;
        case 9: _t->stop(); break;
        case 10: _t->onTimer(); break;
        case 11: _t->onConnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RedisConnectionsWorker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisConnectionsWorker::connected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RedisConnectionsWorker::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisConnectionsWorker::connectedParam)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RedisConnectionsWorker::*_t)(const RedisParams & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisConnectionsWorker::connectFailed)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RedisConnectionsWorker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisConnectionsWorker::disconnected)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RedisConnectionsWorker::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisConnectionsWorker::error)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (RedisConnectionsWorker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisConnectionsWorker::finished)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (RedisConnectionsWorker::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisConnectionsWorker::chekConnectionStatus)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (RedisConnectionsWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisConnectionsWorker::changeCurrentIndex)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject RedisConnectionsWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RedisConnectionsWorker.data,
      qt_meta_data_RedisConnectionsWorker,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RedisConnectionsWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RedisConnectionsWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RedisConnectionsWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RedisConnectionsWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void RedisConnectionsWorker::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RedisConnectionsWorker::connectedParam(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RedisConnectionsWorker::connectFailed(const RedisParams & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RedisConnectionsWorker::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void RedisConnectionsWorker::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RedisConnectionsWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void RedisConnectionsWorker::chekConnectionStatus(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void RedisConnectionsWorker::changeCurrentIndex(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
struct qt_meta_stringdata_RedisDB_t {
    QByteArrayData data[30];
    char stringdata0[335];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RedisDB_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RedisDB_t qt_meta_stringdata_RedisDB = {
    {
QT_MOC_LITERAL(0, 0, 7), // "RedisDB"
QT_MOC_LITERAL(1, 8, 17), // "changedConnection"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 2), // "ip"
QT_MOC_LITERAL(4, 30, 9), // "connected"
QT_MOC_LITERAL(5, 40, 22), // "changedConnectionState"
QT_MOC_LITERAL(6, 63, 13), // "connectFailed"
QT_MOC_LITERAL(7, 77, 11), // "RedisParams"
QT_MOC_LITERAL(8, 89, 1), // "p"
QT_MOC_LITERAL(9, 91, 12), // "disconnected"
QT_MOC_LITERAL(10, 104, 5), // "error"
QT_MOC_LITERAL(11, 110, 3), // "err"
QT_MOC_LITERAL(12, 114, 11), // "onRedisData"
QT_MOC_LITERAL(13, 126, 14), // "QtRedis::Reply"
QT_MOC_LITERAL(14, 141, 4), // "data"
QT_MOC_LITERAL(15, 146, 28), // "onRedisSubscribeDisconnected"
QT_MOC_LITERAL(16, 175, 26), // "onRedisPublishDisconnected"
QT_MOC_LITERAL(17, 202, 16), // "onPublishToOther"
QT_MOC_LITERAL(18, 219, 3), // "sID"
QT_MOC_LITERAL(19, 223, 6), // "dtType"
QT_MOC_LITERAL(20, 230, 6), // "otType"
QT_MOC_LITERAL(21, 237, 7), // "onTimer"
QT_MOC_LITERAL(22, 245, 9), // "heartbeat"
QT_MOC_LITERAL(23, 255, 5), // "start"
QT_MOC_LITERAL(24, 261, 11), // "onConnected"
QT_MOC_LITERAL(25, 273, 4), // "port"
QT_MOC_LITERAL(26, 278, 22), // "onChekConnectionStatus"
QT_MOC_LITERAL(27, 301, 6), // "status"
QT_MOC_LITERAL(28, 308, 20), // "onChangeCurrentIndex"
QT_MOC_LITERAL(29, 329, 5) // "index"

    },
    "RedisDB\0changedConnection\0\0ip\0connected\0"
    "changedConnectionState\0connectFailed\0"
    "RedisParams\0p\0disconnected\0error\0err\0"
    "onRedisData\0QtRedis::Reply\0data\0"
    "onRedisSubscribeDisconnected\0"
    "onRedisPublishDisconnected\0onPublishToOther\0"
    "sID\0dtType\0otType\0onTimer\0heartbeat\0"
    "start\0onConnected\0port\0onChekConnectionStatus\0"
    "status\0onChangeCurrentIndex\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RedisDB[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   99,    2, 0x06 /* Public */,
       5,    1,  104,    2, 0x06 /* Public */,
       6,    1,  107,    2, 0x06 /* Public */,
       9,    0,  110,    2, 0x06 /* Public */,
      10,    1,  111,    2, 0x06 /* Public */,
       4,    0,  114,    2, 0x06 /* Public */,
       4,    1,  115,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,  118,    2, 0x09 /* Protected */,
      15,    0,  121,    2, 0x09 /* Protected */,
      16,    0,  122,    2, 0x09 /* Protected */,
      17,    3,  123,    2, 0x09 /* Protected */,
      21,    0,  130,    2, 0x09 /* Protected */,
      22,    0,  131,    2, 0x09 /* Protected */,
      23,    0,  132,    2, 0x0a /* Public */,
      24,    2,  133,    2, 0x0a /* Public */,
      26,    2,  138,    2, 0x0a /* Public */,
      28,    1,  143,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    3,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   18,   19,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,   25,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,   27,
    QMetaType::Void, QMetaType::Int,   29,

       0        // eod
};

void RedisDB::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RedisDB *_t = static_cast<RedisDB *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changedConnection((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->changedConnectionState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->connectFailed((*reinterpret_cast< const RedisParams(*)>(_a[1]))); break;
        case 3: _t->disconnected(); break;
        case 4: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->connected(); break;
        case 6: _t->connected((*reinterpret_cast< const RedisParams(*)>(_a[1]))); break;
        case 7: _t->onRedisData((*reinterpret_cast< QtRedis::Reply(*)>(_a[1]))); break;
        case 8: _t->onRedisSubscribeDisconnected(); break;
        case 9: _t->onRedisPublishDisconnected(); break;
        case 10: _t->onPublishToOther((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 11: _t->onTimer(); break;
        case 12: _t->heartbeat(); break;
        case 13: _t->start(); break;
        case 14: _t->onConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->onChekConnectionStatus((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->onChangeCurrentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RedisDB::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDB::changedConnection)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RedisDB::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDB::changedConnectionState)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RedisDB::*_t)(const RedisParams & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDB::connectFailed)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RedisDB::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDB::disconnected)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RedisDB::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDB::error)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (RedisDB::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDB::connected)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (RedisDB::*_t)(const RedisParams & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RedisDB::connected)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject RedisDB::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RedisDB.data,
      qt_meta_data_RedisDB,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RedisDB::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RedisDB::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RedisDB.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RedisDB::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void RedisDB::changedConnection(const QString & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RedisDB::changedConnectionState(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RedisDB::connectFailed(const RedisParams & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RedisDB::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void RedisDB::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RedisDB::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void RedisDB::connected(const RedisParams & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
