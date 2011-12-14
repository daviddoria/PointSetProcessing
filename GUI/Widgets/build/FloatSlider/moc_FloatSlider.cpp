/****************************************************************************
** Meta object code from reading C++ file 'FloatSlider.h'
**
** Created: Wed Dec 14 14:58:03 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../FloatSlider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FloatSlider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FloatSlider[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       2,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   33,   12,   12, 0x0a,
      84,   79,   12,   12, 0x0a,
     115,   79,   12,   12, 0x0a,

 // properties: name, type, flags
     153,  146, 0x06085103,
     166,  146, 0x06085103,

       0        // eod
};

static const char qt_meta_stringdata_FloatSlider[] = {
    "FloatSlider\0\0valueChanged(float)\0value\0"
    "slot_horizontalSlider_valueChanged(int)\0"
    "text\0on_txtMin_textChanged(QString)\0"
    "on_txtMax_textChanged(QString)\0double\0"
    "minValueText\0maxValueText\0"
};

const QMetaObject FloatSlider::staticMetaObject = {
    { &LabeledSlider::staticMetaObject, qt_meta_stringdata_FloatSlider,
      qt_meta_data_FloatSlider, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FloatSlider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FloatSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FloatSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FloatSlider))
        return static_cast<void*>(const_cast< FloatSlider*>(this));
    return LabeledSlider::qt_metacast(_clname);
}

int FloatSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = LabeledSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
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
        case 0: *reinterpret_cast< double*>(_v) = minValueText(); break;
        case 1: *reinterpret_cast< double*>(_v) = maxValueText(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setMinValueText(*reinterpret_cast< double*>(_v)); break;
        case 1: setMaxValueText(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void FloatSlider::valueChanged(float _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
