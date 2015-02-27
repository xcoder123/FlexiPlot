/****************************************************************************
** Meta object code from reading C++ file 'gmapwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../gmapwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gmapwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GMapWidget_t {
    QByteArrayData data[17];
    char stringdata[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GMapWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GMapWidget_t qt_meta_stringdata_GMapWidget = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 9),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 3),
QT_MOC_LITERAL(4, 26, 3),
QT_MOC_LITERAL(5, 30, 4),
QT_MOC_LITERAL(6, 35, 13),
QT_MOC_LITERAL(7, 49, 8),
QT_MOC_LITERAL(8, 58, 3),
QT_MOC_LITERAL(9, 62, 11),
QT_MOC_LITERAL(10, 74, 9),
QT_MOC_LITERAL(11, 84, 7),
QT_MOC_LITERAL(12, 92, 16),
QT_MOC_LITERAL(13, 109, 14),
QT_MOC_LITERAL(14, 124, 5),
QT_MOC_LITERAL(15, 130, 16),
QT_MOC_LITERAL(16, 147, 2)
    },
    "GMapWidget\0addMarker\0\0lat\0lng\0name\0"
    "setGMapCenter\0location\0pos\0setGMapZoom\0"
    "zoomLevel\0loadMap\0slotNetworkReply\0"
    "QNetworkReply*\0reply\0slotLoadFinished\0"
    "ok"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GMapWidget[] = {

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
       1,    3,   54,    2, 0x0a /* Public */,
       6,    1,   61,    2, 0x0a /* Public */,
       6,    1,   64,    2, 0x0a /* Public */,
       6,    2,   67,    2, 0x0a /* Public */,
       9,    1,   72,    2, 0x0a /* Public */,
      11,    0,   75,    2, 0x0a /* Public */,
      12,    1,   76,    2, 0x08 /* Private */,
      15,    1,   79,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::QString,    3,    4,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QPointF,    8,
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal,    3,    4,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::Bool,   16,

       0        // eod
};

void GMapWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GMapWidget *_t = static_cast<GMapWidget *>(_o);
        switch (_id) {
        case 0: _t->addMarker((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->setGMapCenter((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->setGMapCenter((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 3: _t->setGMapCenter((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 4: _t->setGMapZoom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->loadMap(); break;
        case 6: _t->slotNetworkReply((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 7: _t->slotLoadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    }
}

const QMetaObject GMapWidget::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_GMapWidget.data,
      qt_meta_data_GMapWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *GMapWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GMapWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GMapWidget.stringdata))
        return static_cast<void*>(const_cast< GMapWidget*>(this));
    return QWebView::qt_metacast(_clname);
}

int GMapWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
