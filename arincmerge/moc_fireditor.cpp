/****************************************************************************
** Meta object code from reading C++ file 'fireditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/fireditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fireditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FirEditor_t {
    QByteArrayData data[16];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FirEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FirEditor_t qt_meta_stringdata_FirEditor = {
    {
QT_MOC_LITERAL(0, 0, 9), // "FirEditor"
QT_MOC_LITERAL(1, 10, 15), // "beforeFIRInsert"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 11), // "QSqlRecord&"
QT_MOC_LITERAL(4, 39, 7), // "qRecord"
QT_MOC_LITERAL(5, 47, 16), // "beforeEdgeInsert"
QT_MOC_LITERAL(6, 64, 15), // "onFirChangedOld"
QT_MOC_LITERAL(7, 80, 6), // "ModInd"
QT_MOC_LITERAL(8, 87, 6), // "addFir"
QT_MOC_LITERAL(9, 94, 9), // "deleteFir"
QT_MOC_LITERAL(10, 104, 12), // "addEdgePoint"
QT_MOC_LITERAL(11, 117, 15), // "deleteEdgePoint"
QT_MOC_LITERAL(12, 133, 13), // "onOperButtons"
QT_MOC_LITERAL(13, 147, 16), // "QAbstractButton*"
QT_MOC_LITERAL(14, 164, 16), // "onFirDataChanged"
QT_MOC_LITERAL(15, 181, 17) // "onEdgeDataChanged"

    },
    "FirEditor\0beforeFIRInsert\0\0QSqlRecord&\0"
    "qRecord\0beforeEdgeInsert\0onFirChangedOld\0"
    "ModInd\0addFir\0deleteFir\0addEdgePoint\0"
    "deleteEdgePoint\0onOperButtons\0"
    "QAbstractButton*\0onFirDataChanged\0"
    "onEdgeDataChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FirEditor[] = {

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
       6,    1,   70,    2, 0x08 /* Private */,
       8,    0,   73,    2, 0x08 /* Private */,
       9,    0,   74,    2, 0x08 /* Private */,
      10,    0,   75,    2, 0x08 /* Private */,
      11,    0,   76,    2, 0x08 /* Private */,
      12,    1,   77,    2, 0x08 /* Private */,
      14,    0,   80,    2, 0x08 /* Private */,
      15,    0,   81,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QModelIndex,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FirEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FirEditor *_t = static_cast<FirEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->beforeFIRInsert((*reinterpret_cast< QSqlRecord(*)>(_a[1]))); break;
        case 1: _t->beforeEdgeInsert((*reinterpret_cast< QSqlRecord(*)>(_a[1]))); break;
        case 2: _t->onFirChangedOld((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->addFir(); break;
        case 4: _t->deleteFir(); break;
        case 5: _t->addEdgePoint(); break;
        case 6: _t->deleteEdgePoint(); break;
        case 7: _t->onOperButtons((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 8: _t->onFirDataChanged(); break;
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

const QMetaObject FirEditor::staticMetaObject = {
    { &BaseEditor::staticMetaObject, qt_meta_stringdata_FirEditor.data,
      qt_meta_data_FirEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FirEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FirEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FirEditor.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::FirEditor"))
        return static_cast< Ui::FirEditor*>(this);
    return BaseEditor::qt_metacast(_clname);
}

int FirEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
