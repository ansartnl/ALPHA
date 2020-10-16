/****************************************************************************
** Meta object code from reading C++ file 'baseeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/baseeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'baseeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BaseEditor_t {
    QByteArrayData data[8];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseEditor_t qt_meta_stringdata_BaseEditor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "BaseEditor"
QT_MOC_LITERAL(1, 11, 12), // "onFirChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 2), // "id"
QT_MOC_LITERAL(4, 28, 4), // "name"
QT_MOC_LITERAL(5, 33, 6), // "onFind"
QT_MOC_LITERAL(6, 40, 3), // "obj"
QT_MOC_LITERAL(7, 44, 10) // "onFindNext"

    },
    "BaseEditor\0onFirChanged\0\0id\0name\0"
    "onFind\0obj\0onFindNext"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x0a /* Public */,
       5,    1,   34,    2, 0x09 /* Protected */,
       7,    1,   37,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QObjectStar,    6,
    QMetaType::Void, QMetaType::QObjectStar,    6,

       0        // eod
};

void BaseEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaseEditor *_t = static_cast<BaseEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onFirChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->onFind((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 2: _t->onFindNext((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject BaseEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_BaseEditor.data,
      qt_meta_data_BaseEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BaseEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BaseEditor.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "RefreshInterface"))
        return static_cast< RefreshInterface*>(this);
    return QWidget::qt_metacast(_clname);
}

int BaseEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
