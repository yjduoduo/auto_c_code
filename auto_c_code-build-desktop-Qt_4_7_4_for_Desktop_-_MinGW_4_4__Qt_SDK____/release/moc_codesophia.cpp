/****************************************************************************
** Meta object code from reading C++ file 'codesophia.h'
**
** Created: Thu Sep 20 21:49:02 2018
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../auto_c_code/codesophia.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'codesophia.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CodeSophia[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      19,   11,   11,   11, 0x0a,
      28,   11,   11,   11, 0x0a,
      38,   11,   11,   11, 0x0a,
      49,   11,   11,   11, 0x0a,
      62,   11,   11,   11, 0x0a,
      74,   11,   11,   11, 0x0a,
      84,   11,   11,   11, 0x0a,
      96,   11,   11,   11, 0x0a,
     113,   11,   11,   11, 0x0a,
     125,   11,   11,   11, 0x0a,
     142,   11,   11,   11, 0x0a,
     152,   11,   11,   11, 0x0a,
     162,   11,   11,   11, 0x0a,
     177,   11,   11,   11, 0x0a,
     187,   11,   11,   11, 0x0a,
     204,   11,   11,   11, 0x0a,
     232,   11,   11,   11, 0x0a,
     262,   11,   11,   11, 0x08,
     296,   11,   11,   11, 0x08,
     336,  331,   11,   11, 0x08,
     375,  331,   11,   11, 0x08,
     418,   11,   11,   11, 0x08,
     452,  447,   11,   11, 0x08,
     483,  447,   11,   11, 0x08,
     534,  526,   11,   11, 0x08,
     569,   11,   11,   11, 0x08,
     580,   11,   11,   11, 0x08,
     616,   11,   11,   11, 0x08,
     651,  526,   11,   11, 0x08,
     689,  331,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CodeSophia[] = {
    "CodeSophia\0\0CMsg()\0CPPMsg()\0JavaMsg()\0"
    "ShellMsg()\0DeclareMsg()\0DefineMsg()\0"
    "FuncMsg()\0StructMsg()\0StructPrintMsg()\0"
    "HeaderMsg()\0IfConditionMsg()\0LoopMsg()\0"
    "NoteMsg()\0Common_Print()\0SaveMsg()\0"
    "FillStringList()\0on_pushButton_gen_clicked()\0"
    "on_comboBox_keytips_clicked()\0"
    "on_pushButton_leftclear_clicked()\0"
    "on_pushButton_rightclear_clicked()\0"
    "arg1\0on_lineEdit_print_textChanged(QString)\0"
    "on_lineEdit_dataprint_textChanged(QString)\0"
    "on_pushButton_load_clicked()\0text\0"
    "get_funcname_qregexp(QString&)\0"
    "get_funcname_and_declare_qregexp(QString&)\0"
    "checked\0on_checkBox_showFunc_toggled(bool)\0"
    "writemsg()\0on_pushButton_paster2left_clicked()\0"
    "on_pushButton_fetchright_clicked()\0"
    "on_checkBox_showAllText_toggled(bool)\0"
    "on_comboBox_print_editTextChanged(QString)\0"
};

const QMetaObject CodeSophia::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CodeSophia,
      qt_meta_data_CodeSophia, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CodeSophia::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CodeSophia::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CodeSophia::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CodeSophia))
        return static_cast<void*>(const_cast< CodeSophia*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CodeSophia::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: CMsg(); break;
        case 1: CPPMsg(); break;
        case 2: JavaMsg(); break;
        case 3: ShellMsg(); break;
        case 4: DeclareMsg(); break;
        case 5: DefineMsg(); break;
        case 6: FuncMsg(); break;
        case 7: StructMsg(); break;
        case 8: StructPrintMsg(); break;
        case 9: HeaderMsg(); break;
        case 10: IfConditionMsg(); break;
        case 11: LoopMsg(); break;
        case 12: NoteMsg(); break;
        case 13: Common_Print(); break;
        case 14: SaveMsg(); break;
        case 15: FillStringList(); break;
        case 16: on_pushButton_gen_clicked(); break;
        case 17: on_comboBox_keytips_clicked(); break;
        case 18: on_pushButton_leftclear_clicked(); break;
        case 19: on_pushButton_rightclear_clicked(); break;
        case 20: on_lineEdit_print_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: on_lineEdit_dataprint_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: on_pushButton_load_clicked(); break;
        case 23: get_funcname_qregexp((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: get_funcname_and_declare_qregexp((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: on_checkBox_showFunc_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: writemsg(); break;
        case 27: on_pushButton_paster2left_clicked(); break;
        case 28: on_pushButton_fetchright_clicked(); break;
        case 29: on_checkBox_showAllText_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: on_comboBox_print_editTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 31;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
