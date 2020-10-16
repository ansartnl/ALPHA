/****************************************************************************
** Meta object code from reading C++ file 'ssaeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/ssaeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ssaeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SsaEditor_t {
    QByteArrayData data[29];
    char stringdata0[373];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SsaEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SsaEditor_t qt_meta_stringdata_SsaEditor = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SsaEditor"
QT_MOC_LITERAL(1, 10, 14), // "airportChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 6), // "iIndex"
QT_MOC_LITERAL(4, 33, 13), // "schemeChanged"
QT_MOC_LITERAL(5, 47, 9), // "addRunway"
QT_MOC_LITERAL(6, 57, 12), // "deleteRunway"
QT_MOC_LITERAL(7, 70, 17), // "OnRunBeforeInsert"
QT_MOC_LITERAL(8, 88, 11), // "QSqlRecord&"
QT_MOC_LITERAL(9, 100, 7), // "qRecord"
QT_MOC_LITERAL(10, 108, 11), // "OnRunSelect"
QT_MOC_LITERAL(11, 120, 6), // "qIndex"
QT_MOC_LITERAL(12, 127, 6), // "addSsa"
QT_MOC_LITERAL(13, 134, 9), // "deleteSsa"
QT_MOC_LITERAL(14, 144, 17), // "OnSSABeforeInsert"
QT_MOC_LITERAL(15, 162, 11), // "OnSSASelect"
QT_MOC_LITERAL(16, 174, 11), // "addSsaPoint"
QT_MOC_LITERAL(17, 186, 14), // "deleteSsaPoint"
QT_MOC_LITERAL(18, 201, 22), // "OnSSAPointBeforeInsert"
QT_MOC_LITERAL(19, 224, 13), // "onOperButtons"
QT_MOC_LITERAL(20, 238, 16), // "QAbstractButton*"
QT_MOC_LITERAL(21, 255, 16), // "OnRunDataChanged"
QT_MOC_LITERAL(22, 272, 16), // "OnSSADataChanged"
QT_MOC_LITERAL(23, 289, 21), // "OnSSAPointDataChanged"
QT_MOC_LITERAL(24, 311, 18), // "OnChangedRunwaySSA"
QT_MOC_LITERAL(25, 330, 11), // "newRunwayID"
QT_MOC_LITERAL(26, 342, 11), // "oldRunwayID"
QT_MOC_LITERAL(27, 354, 10), // "QList<int>"
QT_MOC_LITERAL(28, 365, 7) // "listSSA"

    },
    "SsaEditor\0airportChanged\0\0iIndex\0"
    "schemeChanged\0addRunway\0deleteRunway\0"
    "OnRunBeforeInsert\0QSqlRecord&\0qRecord\0"
    "OnRunSelect\0qIndex\0addSsa\0deleteSsa\0"
    "OnSSABeforeInsert\0OnSSASelect\0addSsaPoint\0"
    "deleteSsaPoint\0OnSSAPointBeforeInsert\0"
    "onOperButtons\0QAbstractButton*\0"
    "OnRunDataChanged\0OnSSADataChanged\0"
    "OnSSAPointDataChanged\0OnChangedRunwaySSA\0"
    "newRunwayID\0oldRunwayID\0QList<int>\0"
    "listSSA"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SsaEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x08 /* Private */,
       4,    1,  107,    2, 0x08 /* Private */,
       5,    0,  110,    2, 0x08 /* Private */,
       6,    0,  111,    2, 0x08 /* Private */,
       7,    1,  112,    2, 0x08 /* Private */,
      10,    1,  115,    2, 0x08 /* Private */,
      12,    0,  118,    2, 0x08 /* Private */,
      13,    0,  119,    2, 0x08 /* Private */,
      14,    1,  120,    2, 0x08 /* Private */,
      15,    1,  123,    2, 0x08 /* Private */,
      16,    0,  126,    2, 0x08 /* Private */,
      17,    0,  127,    2, 0x08 /* Private */,
      18,    1,  128,    2, 0x08 /* Private */,
      19,    1,  131,    2, 0x08 /* Private */,
      21,    0,  134,    2, 0x08 /* Private */,
      22,    0,  135,    2, 0x08 /* Private */,
      23,    0,  136,    2, 0x08 /* Private */,
      24,    3,  137,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QModelIndex,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QModelIndex,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 20,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 27,   25,   26,   28,

       0        // eod
};

void SsaEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SsaEditor *_t = static_cast<SsaEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->airportChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->schemeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->addRunway(); break;
        case 3: _t->deleteRunway(); break;
        case 4: _t->OnRunBeforeInsert((*reinterpret_cast< QSqlRecord(*)>(_a[1]))); break;
        case 5: _t->OnRunSelect((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->addSsa(); break;
        case 7: _t->deleteSsa(); break;
        case 8: _t->OnSSABeforeInsert((*reinterpret_cast< QSqlRecord(*)>(_a[1]))); break;
        case 9: _t->OnSSASelect((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 10: _t->addSsaPoint(); break;
        case 11: _t->deleteSsaPoint(); break;
        case 12: _t->OnSSAPointBeforeInsert((*reinterpret_cast< QSqlRecord(*)>(_a[1]))); break;
        case 13: _t->onOperButtons((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 14: _t->OnRunDataChanged(); break;
        case 15: _t->OnSSADataChanged(); break;
        case 16: _t->OnSSAPointDataChanged(); break;
        case 17: _t->OnChangedRunwaySSA((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QList<int>(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        }
    }
}

const QMetaObject SsaEditor::staticMetaObject = {
    { &BaseEditor::staticMetaObject, qt_meta_stringdata_SsaEditor.data,
      qt_meta_data_SsaEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SsaEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SsaEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SsaEditor.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::SsaEditor"))
        return static_cast< Ui::SsaEditor*>(this);
    return BaseEditor::qt_metacast(_clname);
}

int SsaEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
