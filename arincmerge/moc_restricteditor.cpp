/****************************************************************************
** Meta object code from reading C++ file 'restricteditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/restricteditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'restricteditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RestrictEditor_t {
    QByteArrayData data[11];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RestrictEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RestrictEditor_t qt_meta_stringdata_RestrictEditor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "RestrictEditor"
QT_MOC_LITERAL(1, 15, 14), // "addRestriction"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 17), // "deleteRestriction"
QT_MOC_LITERAL(4, 49, 18), // "restrictionChanged"
QT_MOC_LITERAL(5, 68, 6), // "ModInd"
QT_MOC_LITERAL(6, 75, 8), // "addPoint"
QT_MOC_LITERAL(7, 84, 11), // "deletePoint"
QT_MOC_LITERAL(8, 96, 12), // "onOperButton"
QT_MOC_LITERAL(9, 109, 16), // "QAbstractButton*"
QT_MOC_LITERAL(10, 126, 19) // "activateOperButtons"

    },
    "RestrictEditor\0addRestriction\0\0"
    "deleteRestriction\0restrictionChanged\0"
    "ModInd\0addPoint\0deletePoint\0onOperButton\0"
    "QAbstractButton*\0activateOperButtons"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RestrictEditor[] = {

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
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    1,   51,    2, 0x08 /* Private */,
       6,    0,   54,    2, 0x08 /* Private */,
       7,    0,   55,    2, 0x08 /* Private */,
       8,    1,   56,    2, 0x08 /* Private */,
      10,    0,   59,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void,

       0        // eod
};

void RestrictEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RestrictEditor *_t = static_cast<RestrictEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addRestriction(); break;
        case 1: _t->deleteRestriction(); break;
        case 2: _t->restrictionChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->addPoint(); break;
        case 4: _t->deletePoint(); break;
        case 5: _t->onOperButton((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 6: _t->activateOperButtons(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        }
    }
}

const QMetaObject RestrictEditor::staticMetaObject = {
    { &BaseEditor::staticMetaObject, qt_meta_stringdata_RestrictEditor.data,
      qt_meta_data_RestrictEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RestrictEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RestrictEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RestrictEditor.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::RestrictEditor"))
        return static_cast< Ui::RestrictEditor*>(this);
    return BaseEditor::qt_metacast(_clname);
}

int RestrictEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseEditor::qt_metacall(_c, _id, _a);
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
