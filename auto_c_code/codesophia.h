#ifndef CODESOPHIA_H
#define CODESOPHIA_H

#include <QMainWindow>
//#include "codesophia.h"

namespace Ui {
    class CodeSophia;
} // namespace Ui


enum KeyClass
{
    KEY_INVALID,
    KEY_C,
    KEY_CPP,
    KEY_JAVA,
    KEY_SHELL,
    KEY_OTHER,
};

enum SubType
{
    SUB_INVALID,
    SUB_DECLARE,
    SUB_FUNCTION,
    SUB_STRUCT,
    SUB_STRUCTPRINT,
    SUB_DEFINE,
    SUB_HEADER,
    SUB_IFCONDITION,
    SUB_LOOP,
    SUB_OTHER,
};

enum OPTYPE
{
    OP_INVALID,
    OP_SAVE,
    OP_OTHER,
};




class CodeSophia : public QMainWindow
{
    Q_OBJECT
public:
    explicit CodeSophia(QWidget *parent = 0);
    
signals:
    
public slots:
    void CMsg();
    void CPPMsg();
    void JavaMsg();
    void ShellMsg();
    void DeclareMsg();
    void DefineMsg();
    void FuncMsg();
    void StructMsg();
    void StructPrintMsg();
    void HeaderMsg();
    void IfConditionMsg();
    void LoopMsg();

    void SaveMsg();

    void on_pushButton_gen_clicked();
    void on_comboBox_keytips_clicked();


protected:
    void ActiveSets();
    void ButtonSets();
    void ComboBoxSets();

    void ShowTipsInfo(QString s);
    void FillComBoxKeyTips();

    QString getSubType(SubType type);
    QString getKeyClass(KeyClass cls);
    QString getOPType(OPTYPE type);

private:
    Ui::CodeSophia *ui;

private:
    //变量

    QString title_org;
    QString splitsign;
    QString current_lan; //当前语言
    quint32 current_lan_num;
    QString current_subtype; //sub class
    quint32 current_subtype_num;
    QString current_optype; //op class
    quint32 current_optype_num;
    QString showtitle; //sub class
    
};

#endif // CODESOPHIA_H
