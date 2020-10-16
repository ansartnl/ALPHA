/****************************************************************************
** Meta object code from reading C++ file 'holdingeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/holdingeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'holdingeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HoldingEditor_t {
    QByteArrayData data[8];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HoldingEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HoldingEditor_t qt_meta_stringdata_HoldingEditor = {
    {
QT_MOC_LITERAL(0, 0, 13), // "HoldingEditor"
QT_MOC_LITERAL(1, 14, 10), // "addHolding"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 13), // "deleteHolding"
QT_MOC_LITERAL(4, 40, 13), // "onOperButtons"
QT_MOC_LITERAL(5, 54, 16), // "QAbstractButton*"
QT_MOC_LITERAL(6, 71, 7), // "pButton"
QT_MOC_LITERAL(7, 79, 17) // "enableOperButtons"

    },
    "HoldingEditor\0addHolding\0\0deleteHolding\0"
    "onOperButtons\0QAbstractButton*\0pButton\0"
    "enableOperButtons"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HoldingEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    1,   36,    2, 0x08 /* Private */,
       7,    0,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,

       0        // eod
};

void HoldingEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HoldingEditor *_t = static_cast<HoldingEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addHolding(); break;
        case 1: _t->deleteHolding(); break;
        case 2: _t->onOperButtons((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 3: _t->enableOperButtons(); break;
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

const QMetaObject HoldingEditor::staticMetaObject = {
    { &BaseEditor::staticMetaObject, qt_meta_stringdata_HoldingEditor.data,
      qt_meta_data_HoldingEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HoldingEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HoldingEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HoldingEditor.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::HoldingEditor"))
        return static_cast< Ui::HoldingEditor*>(this);
    return BaseEditor::qt_metacast(_clname);
}

int HoldingEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
