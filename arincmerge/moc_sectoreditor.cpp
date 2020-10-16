/****************************************************************************
** Meta object code from reading C++ file 'sectoreditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/sectoreditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sectoreditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SectorEditor_t {
    QByteArrayData data[17];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SectorEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SectorEditor_t qt_meta_stringdata_SectorEditor = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SectorEditor"
QT_MOC_LITERAL(1, 13, 16), // "beforeEdgeInsert"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 11), // "QSqlRecord&"
QT_MOC_LITERAL(4, 43, 7), // "qRecord"
QT_MOC_LITERAL(5, 51, 15), // "onSectorChanged"
QT_MOC_LITERAL(6, 67, 6), // "modInd"
QT_MOC_LITERAL(7, 74, 20), // "onCurrentEdgeChanged"
QT_MOC_LITERAL(8, 95, 6), // "curInd"
QT_MOC_LITERAL(9, 102, 9), // "addSector"
QT_MOC_LITERAL(10, 112, 12), // "deleteSector"
QT_MOC_LITERAL(11, 125, 12), // "addEdgePoint"
QT_MOC_LITERAL(12, 138, 15), // "deleteEdgePoint"
QT_MOC_LITERAL(13, 154, 13), // "onOperButtons"
QT_MOC_LITERAL(14, 168, 16), // "QAbstractButton*"
QT_MOC_LITERAL(15, 185, 19), // "onSectorDataChanged"
QT_MOC_LITERAL(16, 205, 17) // "onEdgeDataChanged"

    },
    "SectorEditor\0beforeEdgeInsert\0\0"
    "QSqlRecord&\0qRecord\0onSectorChanged\0"
    "modInd\0onCurrentEdgeChanged\0curInd\0"
    "addSector\0deleteSector\0addEdgePoint\0"
    "deleteEdgePoint\0onOperButtons\0"
    "QAbstractButton*\0onSectorDataChanged\0"
    "onEdgeDataChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SectorEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x08 /* Private */,
       5,    1,   67,    2, 0x08 /* Private */,
       7,    1,   70,    2, 0x08 /* Private */,
       9,    0,   73,    2, 0x08 /* Private */,
      10,    0,   74,    2, 0x08 /* Private */,
      11,    0,   75,    2, 0x08 /* Private */,
      12,    0,   76,    2, 0x08 /* Private */,
      13,    1,   77,    2, 0x08 /* Private */,
      15,    0,   80,    2, 0x08 /* Private */,
      16,    0,   81,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QModelIndex,    6,
    QMetaType::Void, QMetaType::QModelIndex,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SectorEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SectorEditor *_t = static_cast<SectorEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->beforeEdgeInsert((*reinterpret_cast< QSqlRecord(*)>(_a[1]))); break;
        case 1: _t->onSectorChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->onCurrentEdgeChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->addSector(); break;
        case 4: _t->deleteSector(); break;
        case 5: _t->addEdgePoint(); break;
        case 6: _t->deleteEdgePoint(); break;
        case 7: _t->onOperButtons((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 8: _t->onSectorDataChanged(); break;
        case 9: _t->onEdgeDataChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        }
    }
}

const QMetaObject SectorEditor::staticMetaObject = {
    { &BaseEditor::staticMetaObject, qt_meta_stringdata_SectorEditor.data,
      qt_meta_data_SectorEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SectorEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SectorEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SectorEditor.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::SectorEditor"))
        return static_cast< Ui::SectorEditor*>(this);
    return BaseEditor::qt_metacast(_clname);
}

int SectorEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
