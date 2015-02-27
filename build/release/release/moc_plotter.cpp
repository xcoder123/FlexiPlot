/****************************************************************************
** Meta object code from reading C++ file 'plotter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../plotter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plotter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Plotter_t {
    QByteArrayData data[24];
    char stringdata[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Plotter_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Plotter_t qt_meta_stringdata_Plotter = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 4),
QT_MOC_LITERAL(2, 13, 0),
QT_MOC_LITERAL(3, 14, 13),
QT_MOC_LITERAL(4, 28, 2),
QT_MOC_LITERAL(5, 31, 9),
QT_MOC_LITERAL(6, 41, 12),
QT_MOC_LITERAL(7, 54, 4),
QT_MOC_LITERAL(8, 59, 5),
QT_MOC_LITERAL(9, 65, 3),
QT_MOC_LITERAL(10, 69, 8),
QT_MOC_LITERAL(11, 78, 9),
QT_MOC_LITERAL(12, 88, 5),
QT_MOC_LITERAL(13, 94, 14),
QT_MOC_LITERAL(14, 109, 2),
QT_MOC_LITERAL(15, 112, 13),
QT_MOC_LITERAL(16, 126, 6),
QT_MOC_LITERAL(17, 133, 13),
QT_MOC_LITERAL(18, 147, 13),
QT_MOC_LITERAL(19, 161, 11),
QT_MOC_LITERAL(20, 173, 4),
QT_MOC_LITERAL(21, 178, 17),
QT_MOC_LITERAL(22, 196, 15),
QT_MOC_LITERAL(23, 212, 15)
    },
    "Plotter\0plot\0\0legendChecked\0on\0showCurve\0"
    "QwtPlotItem*\0item\0setId\0str\0setTitle\0"
    "setBuffer\0value\0setRefreshRate\0ms\0"
    "setDateFormat\0format\0setXAxisLabel\0"
    "setYAxisLabel\0typeChanged\0type\0"
    "startStopPlotting\0settingsChanged\0"
    "openColorDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Plotter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    2,   85,    2, 0x0a /* Public */,
       5,    2,   90,    2, 0x0a /* Public */,
       8,    1,   95,    2, 0x0a /* Public */,
      10,    1,   98,    2, 0x0a /* Public */,
      11,    1,  101,    2, 0x0a /* Public */,
      13,    1,  104,    2, 0x0a /* Public */,
      15,    1,  107,    2, 0x0a /* Public */,
      17,    1,  110,    2, 0x0a /* Public */,
      18,    1,  113,    2, 0x0a /* Public */,
      19,    1,  116,    2, 0x0a /* Public */,
      21,    0,  119,    2, 0x0a /* Public */,
      22,    0,  120,    2, 0x0a /* Public */,
      23,    0,  121,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant, QMetaType::Bool,    2,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Bool,    7,    4,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Plotter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Plotter *_t = static_cast<Plotter *>(_o);
        switch (_id) {
        case 0: _t->plot(); break;
        case 1: _t->legendChecked((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->showCurve((*reinterpret_cast< QwtPlotItem*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->setId((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setTitle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->setBuffer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setRefreshRate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setDateFormat((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->setXAxisLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->setYAxisLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->typeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->startStopPlotting(); break;
        case 12: _t->settingsChanged(); break;
        case 13: _t->openColorDialog(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QwtPlotItem* >(); break;
            }
            break;
        }
    }
}

const QMetaObject Plotter::staticMetaObject = {
    { &AbstractWidget::staticMetaObject, qt_meta_stringdata_Plotter.data,
      qt_meta_data_Plotter,  qt_static_metacall, 0, 0}
};


const QMetaObject *Plotter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Plotter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Plotter.stringdata))
        return static_cast<void*>(const_cast< Plotter*>(this));
    return AbstractWidget::qt_metacast(_clname);
}

int Plotter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
