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
    SUB_NOTE,
    SUB_OTHER,
};

enum OPTYPE
{
    OP_INVALID,
    OP_SAVE = 0x20,
    OP_OTHER,
};




class CodeSophia : public QMainWindow
{
    Q_OBJECT
public:
    explicit CodeSophia(QWidget *parent = 0);
    ~CodeSophia();
    
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
    void NoteMsg();

    void SaveMsg();
    void FillStringList();

    void on_pushButton_gen_clicked();
    void on_comboBox_keytips_clicked();


protected:
    void ActiveSets();
    void ButtonSets();
    void ComboBoxSets();
    void TextEditSets();

    void ShowTipsInfo(QString s);
    void FillComBoxKeyTips();

    QString getSubType(SubType type);
    QString getKeyClass(KeyClass cls);
    QString getOPType(OPTYPE type);

    void ReadHistorySettings();
    void WriteCurrentSettings();



private slots:
    void on_pushButton_leftclear_clicked();

    void on_pushButton_rightclear_clicked();

private:
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dropEvent(QDropEvent *event);
//    void readTextFile(const QString &fileName);
//    void readTextFileAppend(const QString &fileName);
    void closeEvent(QCloseEvent *event);
    void SetTextEditResult(QString &str);


    void Proc_C_Header(QStringList &lst);
    void Proc_C_Note(QStringList &lst);
    void Proc_C_Function(QStringList &lst);
    QString Proc_C_Function_SetGet(QStringList &lst, bool Local);
    QString Proc_C_Function_GenFunc(QStringList &lst, bool Local);



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

    QString enter;
    QString semisign;
    QString spacesign;
    QString underlinesign;
    QString tabsign;
    QString equalsign;

    //定义显示列表
    QStringList StrLst_KEYC_HEADER;
    QStringList StrLst_KEYC_DECLARE;
    QStringList StrLst_KEYC_DEFINE;
    QStringList StrLst_KEYC_FUNCTION;
    QStringList StrLst_KEYC_STRUCT;
    QStringList StrLst_KEYC_STRUCTPRINT;
    QStringList StrLst_KEYC_IFCONDITION;
    QStringList StrLst_KEYC_LOOP;
    QStringList StrLst_KEYC_NOTE;

    
};

#endif // CODESOPHIA_H
