/****************************************************************************
** Meta object code from reading C++ file 'pointeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/pointeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pointeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PointEditor_t {
    QByteArrayData data[15];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PointEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PointEditor_t qt_meta_stringdata_PointEditor = {
    {
QT_MOC_LITERAL(0, 0, 11), // "PointEditor"
QT_MOC_LITERAL(1, 12, 17), // "pointsDataChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 20), // "firPointsDataChanged"
QT_MOC_LITERAL(4, 52, 7), // "topLeft"
QT_MOC_LITERAL(5, 60, 11), // "bottomRight"
QT_MOC_LITERAL(6, 72, 13), // "onOperButtons"
QT_MOC_LITERAL(7, 86, 16), // "QAbstractButton*"
QT_MOC_LITERAL(8, 103, 7), // "pButton"
QT_MOC_LITERAL(9, 111, 8), // "addPoint"
QT_MOC_LITERAL(10, 120, 11), // "deletePoint"
QT_MOC_LITERAL(11, 132, 13), // "pointSelected"
QT_MOC_LITERAL(12, 146, 6), // "qIndex"
QT_MOC_LITERAL(13, 153, 11), // "addFirPoint"
QT_MOC_LITERAL(14, 165, 14) // "deleteFirPoint"

    },
    "PointEditor\0pointsDataChanged\0\0"
    "firPointsDataChanged\0topLeft\0bottomRight\0"
    "onOperButtons\0QAbstractButton*\0pButton\0"
    "addPoint\0deletePoint\0pointSelected\0"
    "qIndex\0addFirPoint\0deleteFirPoint"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PointEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    2,   55,    2, 0x08 /* Private */,
       6,    1,   60,    2, 0x08 /* Private */,
       9,    0,   63,    2, 0x08 /* Private */,
      10,    0,   64,    2, 0x08 /* Private */,
      11,    1,   65,    2, 0x08 /* Private */,
      13,    0,   68,    2, 0x08 /* Private */,
      14,    0,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::QModelIndex,    4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PointEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PointEditor *_t = static_cast<PointEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pointsDataChanged(); break;
        case 1: _t->firPointsDataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 2: _t->onOperButtons((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 3: _t->addPoint(); break;
        case 4: _t->deletePoint(); break;
        case 5: _t->pointSelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->addFirPoint(); break;
        case 7: _t->deleteFirPoint(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        }
    }
}

const QMetaObject PointEditor::staticMetaObject = {
    { &BaseEditor::staticMetaObject, qt_meta_stringdata_PointEditor.data,
      qt_meta_data_PointEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PointEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PointEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PointEditor.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::PointEditor"))
        return static_cast< Ui::PointEditor*>(this);
    return BaseEditor::qt_metacast(_clname);
}

int PointEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
