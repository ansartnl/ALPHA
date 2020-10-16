/****************************************************************************
** Meta object code from reading C++ file 'RtoRService.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rtor/src/RtoRService.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RtoRService.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RtoRHelper_t {
    QByteArrayData data[16];
    char stringdata0[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RtoRHelper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RtoRHelper_t qt_meta_stringdata_RtoRHelper = {
    {
QT_MOC_LITERAL(0, 0, 10), // "RtoRHelper"
QT_MOC_LITERAL(1, 11, 12), // "onChangedRWF"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 5), // "idFIR"
QT_MOC_LITERAL(4, 31, 19), // "RunwayWidgetDataVPP"
QT_MOC_LITERAL(5, 51, 3), // "rwf"
QT_MOC_LITERAL(6, 55, 8), // "operType"
QT_MOC_LITERAL(7, 64, 13), // "onChangedRWF1"
QT_MOC_LITERAL(8, 78, 19), // "RunwayWidgetDataFIR"
QT_MOC_LITERAL(9, 98, 13), // "onChangedRWF2"
QT_MOC_LITERAL(10, 112, 13), // "onChangedSFPL"
QT_MOC_LITERAL(11, 126, 21), // "QSharedPointer<CSFPL>"
QT_MOC_LITERAL(12, 148, 4), // "sfpl"
QT_MOC_LITERAL(13, 153, 9), // "_OnTimer1"
QT_MOC_LITERAL(14, 163, 9), // "_OnTimer2"
QT_MOC_LITERAL(15, 173, 9) // "_OnTimer3"

    },
    "RtoRHelper\0onChangedRWF\0\0idFIR\0"
    "RunwayWidgetDataVPP\0rwf\0operType\0"
    "onChangedRWF1\0RunwayWidgetDataFIR\0"
    "onChangedRWF2\0onChangedSFPL\0"
    "QSharedPointer<CSFPL>\0sfpl\0_OnTimer1\0"
    "_OnTimer2\0_OnTimer3"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RtoRHelper[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x0a /* Public */,
       7,    3,   56,    2, 0x0a /* Public */,
       9,    3,   63,    2, 0x0a /* Public */,
      10,    2,   70,    2, 0x0a /* Public */,
      13,    0,   75,    2, 0x0a /* Public */,
      14,    0,   76,    2, 0x0a /* Public */,
      15,    0,   77,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4, QMetaType::QString,    3,    5,    6,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8, QMetaType::QString,    3,    5,    6,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8, QMetaType::QString,    3,    5,    6,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QString,   12,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RtoRHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RtoRHelper *_t = static_cast<RtoRHelper *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onChangedRWF((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< RunwayWidgetDataVPP(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->onChangedRWF1((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< RunwayWidgetDataFIR(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->onChangedRWF2((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< RunwayWidgetDataFIR(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: _t->onChangedSFPL((*reinterpret_cast< QSharedPointer<CSFPL>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->_OnTimer1(); break;
        case 5: _t->_OnTimer2(); break;
        case 6: _t->_OnTimer3(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< RunwayWidgetDataVPP >(); break;
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
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSharedPointer<CSFPL> >(); break;
            }
            break;
        }
    }
}

const QMetaObject RtoRHelper::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RtoRHelper.data,
      qt_meta_data_RtoRHelper,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RtoRHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RtoRHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RtoRHelper.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RtoRHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
