/****************************************************************************
** Meta object code from reading C++ file 'LabeledSlider.h'
**
** Created: Wed Dec 14 14:58:04 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LabeledSlider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LabeledSlider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LabeledSlider[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       3,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      33,   14,   14,   14, 0x0a,
      78,   73,   14,   14, 0x0a,
     109,   73,   14,   14, 0x0a,

 // properties: name, type, flags
     148,  140, 0x0a085103,
     157,  153, 0x02085103,
     170,  153, 0x02085103,

       0        // eod
};

static const char qt_meta_stringdata_LabeledSlider[] = {
    "LabeledSlider\0\0valueChanged(int)\0"
    "slot_horizontalSlider_valueChanged(int)\0"
    "text\0on_txtMin_textChanged(QString)\0"
    "on_txtMax_textChanged(QString)\0QString\0"
    "name\0int\0minValueText\0maxValueText\0"
};

const QMetaObject LabeledSlider::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LabeledSlider,
      qt_meta_data_LabeledSlider, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LabeledSlider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LabeledSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LabeledSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LabeledSlider))
        return static_cast<void*>(const_cast< LabeledSlider*>(this));
    if (!strcmp(_clname, "Ui::LabeledSlider"))
        return static_cast< Ui::LabeledSlider*>(const_cast< LabeledSlider*>(this));
    return QWidget::qt_metacast(_clname);
}

int LabeledSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slot_horizontalSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: on_txtMin_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: on_txtMax_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = name(); break;
        case 1: *reinterpret_cast< int*>(_v) = minValueText(); break;
        case 2: *reinterpret_cast< int*>(_v) = maxValueText(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setName(*reinterpret_cast< QString*>(_v)); break;
        case 1: setMinValueText(*reinterpret_cast< int*>(_v)); break;
        case 2: setMaxValueText(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void LabeledSlider::valueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
