/****************************************************************************
** Meta object code from reading C++ file 'routeeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/routeeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'routeeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RouteEditor_t {
    QByteArrayData data[15];
    char stringdata0[197];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RouteEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RouteEditor_t qt_meta_stringdata_RouteEditor = {
    {
QT_MOC_LITERAL(0, 0, 11), // "RouteEditor"
QT_MOC_LITERAL(1, 12, 8), // "addRoute"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 11), // "deleteRoute"
QT_MOC_LITERAL(4, 34, 17), // "BeforeInsertRoute"
QT_MOC_LITERAL(5, 52, 11), // "QSqlRecord&"
QT_MOC_LITERAL(6, 64, 7), // "qRecord"
QT_MOC_LITERAL(7, 72, 13), // "OnRouteSelect"
QT_MOC_LITERAL(8, 86, 6), // "qIndex"
QT_MOC_LITERAL(9, 93, 14), // "addRoutePoints"
QT_MOC_LITERAL(10, 108, 17), // "deleteRoutePoints"
QT_MOC_LITERAL(11, 126, 13), // "onOperButtons"
QT_MOC_LITERAL(12, 140, 16), // "QAbstractButton*"
QT_MOC_LITERAL(13, 157, 16), // "routeDataChanged"
QT_MOC_LITERAL(14, 174, 22) // "routePointsDataChanged"

    },
    "RouteEditor\0addRoute\0\0deleteRoute\0"
    "BeforeInsertRoute\0QSqlRecord&\0qRecord\0"
    "OnRouteSelect\0qIndex\0addRoutePoints\0"
    "deleteRoutePoints\0onOperButtons\0"
    "QAbstractButton*\0routeDataChanged\0"
    "routePointsDataChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RouteEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    1,   61,    2, 0x08 /* Private */,
       7,    1,   64,    2, 0x08 /* Private */,
       9,    0,   67,    2, 0x08 /* Private */,
      10,    0,   68,    2, 0x08 /* Private */,
      11,    1,   69,    2, 0x08 /* Private */,
      13,    0,   72,    2, 0x08 /* Private */,
      14,    0,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QModelIndex,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RouteEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RouteEditor *_t = static_cast<RouteEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addRoute(); break;
        case 1: _t->deleteRoute(); break;
        case 2: _t->BeforeInsertRoute((*reinterpret_cast< QSqlRecord(*)>(_a[1]))); break;
        case 3: _t->OnRouteSelect((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->addRoutePoints(); break;
        case 5: _t->deleteRoutePoints(); break;
        case 6: _t->onOperButtons((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 7: _t->routeDataChanged(); break;
        case 8: _t->routePointsDataChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        }
    }
}

const QMetaObject RouteEditor::staticMetaObject = {
    { &BaseEditor::staticMetaObject, qt_meta_stringdata_RouteEditor.data,
      qt_meta_data_RouteEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RouteEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RouteEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RouteEditor.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::RouteEditor"))
        return static_cast< Ui::RouteEditor*>(this);
    return BaseEditor::qt_metacast(_clname);
}

int RouteEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
