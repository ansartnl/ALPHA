/****************************************************************************
** Meta object code from reading C++ file 'restrictusereditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/editors/restrictusereditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'restrictusereditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RestrictUserEditor_t {
    QByteArrayData data[14];
    char stringdata0[327];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RestrictUserEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RestrictUserEditor_t qt_meta_stringdata_RestrictUserEditor = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RestrictUserEditor"
QT_MOC_LITERAL(1, 19, 31), // "on_firCombo_currentIndexChanged"
QT_MOC_LITERAL(2, 51, 0), // ""
QT_MOC_LITERAL(3, 52, 22), // "on_operButtons_clicked"
QT_MOC_LITERAL(4, 75, 16), // "QAbstractButton*"
QT_MOC_LITERAL(5, 92, 31), // "on_addRestrictionButton_clicked"
QT_MOC_LITERAL(6, 124, 34), // "on_deleteRestrictionButton_cl..."
QT_MOC_LITERAL(7, 159, 25), // "on_addPointButton_clicked"
QT_MOC_LITERAL(8, 185, 28), // "on_deletePointButtun_clicked"
QT_MOC_LITERAL(9, 214, 18), // "restrictionChanged"
QT_MOC_LITERAL(10, 233, 6), // "ModInd"
QT_MOC_LITERAL(11, 240, 19), // "activateOperButtons"
QT_MOC_LITERAL(12, 260, 33), // "on_toolButtonPastePolygon_cli..."
QT_MOC_LITERAL(13, 294, 32) // "on_toolButtonPasteCircle_clicked"

    },
    "RestrictUserEditor\0on_firCombo_currentIndexChanged\0"
    "\0on_operButtons_clicked\0QAbstractButton*\0"
    "on_addRestrictionButton_clicked\0"
    "on_deleteRestrictionButton_clicked\0"
    "on_addPointButton_clicked\0"
    "on_deletePointButtun_clicked\0"
    "restrictionChanged\0ModInd\0activateOperButtons\0"
    "on_toolButtonPastePolygon_clicked\0"
    "on_toolButtonPasteCircle_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RestrictUserEditor[] = {

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
       3,    1,   67,    2, 0x08 /* Private */,
       5,    0,   70,    2, 0x08 /* Private */,
       6,    0,   71,    2, 0x08 /* Private */,
       7,    0,   72,    2, 0x08 /* Private */,
       8,    0,   73,    2, 0x08 /* Private */,
       9,    1,   74,    2, 0x08 /* Private */,
      11,    0,   77,    2, 0x08 /* Private */,
      12,    0,   78,    2, 0x08 /* Private */,
      13,    0,   79,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RestrictUserEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RestrictUserEditor *_t = static_cast<RestrictUserEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_firCombo_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_operButtons_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 2: _t->on_addRestrictionButton_clicked(); break;
        case 3: _t->on_deleteRestrictionButton_clicked(); break;
        case 4: _t->on_addPointButton_clicked(); break;
        case 5: _t->on_deletePointButtun_clicked(); break;
        case 6: _t->restrictionChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 7: _t->activateOperButtons(); break;
        case 8: _t->on_toolButtonPastePolygon_clicked(); break;
        case 9: _t->on_toolButtonPasteCircle_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject RestrictUserEditor::staticMetaObject = {
    { &BaseEditor::staticMetaObject, qt_meta_stringdata_RestrictUserEditor.data,
      qt_meta_data_RestrictUserEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RestrictUserEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RestrictUserEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RestrictUserEditor.stringdata0))
        return static_cast<void*>(this);
    return BaseEditor::qt_metacast(_clname);
}

int RestrictUserEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
