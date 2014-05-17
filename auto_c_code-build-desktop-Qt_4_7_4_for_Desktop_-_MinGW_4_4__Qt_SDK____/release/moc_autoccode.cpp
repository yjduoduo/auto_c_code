/****************************************************************************
** Meta object code from reading C++ file 'autoccode.h'
**
** Created: Sat May 17 17:17:08 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../auto_c_code/autoccode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'autoccode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_autoCCode[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      38,   33,   10,   10, 0x08,
      72,   10,   10,   10, 0x08,
     101,   10,   10,   10, 0x08,
     126,   10,   10,   10, 0x08,
     148,   10,   10,   10, 0x08,
     171,   10,   10,   10, 0x0a,
     195,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_autoCCode[] = {
    "autoCCode\0\0on_save_btn_clicked()\0arg1\0"
    "on_db_comboBox_activated(QString)\0"
    "on_choseCodeDB_btn_clicked()\0"
    "on_gencode_btn_clicked()\0on_indb_btn_clicked()\0"
    "on_outdb_btn_clicked()\0on_ok_btn_dia_clicked()\0"
    "on_cancel_btn_dia_clicked()\0"
};

const QMetaObject autoCCode::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_autoCCode,
      qt_meta_data_autoCCode, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &autoCCode::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *autoCCode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *autoCCode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_autoCCode))
        return static_cast<void*>(const_cast< autoCCode*>(this));
    return QWidget::qt_metacast(_clname);
}

int autoCCode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_save_btn_clicked(); break;
        case 1: on_db_comboBox_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: on_choseCodeDB_btn_clicked(); break;
        case 3: on_gencode_btn_clicked(); break;
        case 4: on_indb_btn_clicked(); break;
        case 5: on_outdb_btn_clicked(); break;
        case 6: on_ok_btn_dia_clicked(); break;
        case 7: on_cancel_btn_dia_clicked(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
