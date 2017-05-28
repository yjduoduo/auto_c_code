#ifndef CODESOPHIA_H
#define CODESOPHIA_H

#include <QMainWindow>
#include <QFile>
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
    SUB_COMMONPRINT,
    SUB_OTHER,
};

enum OPTYPE
{
    OP_INVALID,
    OP_SAVE = 0x20,
    OP_OTHER,
};



typedef struct T_DataFormat
{
    QString string;
    QString stringright; //替换掉[.*]里面的数据
    QString format;
    bool haveyihao;
}T_DataFormat;

typedef struct T_StructFormat
{
    QString string;
    QString note;
}T_StructFormat;

typedef struct T_DefineInfo
{
    QString defname;
    QString defvalue;
    QString note; //zhushi
}T_DefineInfo;






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
    void Common_Print();

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

    void StructPrintMsgUIHide();
    void StructPrintMsgUIShow();


private slots:
    void on_pushButton_leftclear_clicked();

    void on_pushButton_rightclear_clicked();

    void on_lineEdit_print_textChanged(const QString &arg1);

    void on_lineEdit_dataprint_textChanged(const QString &arg1);

    void on_pushButton_load_clicked();

    void get_funcname_qregexp(QString &text);
    void get_funcname_and_declare_qregexp(QString &text);

    void on_checkBox_showFunc_toggled(bool checked);

private:
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dropEvent(QDropEvent *event);
//    void readTextFile(const QString &fileName);
//    void readTextFileAppend(const QString &fileName);
    void closeEvent(QCloseEvent *event);
    void SetTextEditResult(QString &str);


    void Proc_C_Header(QStringList &lst);
    void Proc_C_Declare(QStringList &lst);
    void Proc_C_Note(QStringList &lst);
    void Proc_C_Function(QStringList &lst);
    void Proc_C_StructPrint(QStringList &lst);
    void Proc_C_Struct(QStringList &lst);
    void Proc_C_Define(QStringList &lst);
    void Proc_C_IFCONDITION(QStringList &lst);
    void Proc_C_LOOP(QStringList &lst);
    void Proc_C_COMMONPRINT(QStringList &lst);
    void Proc_C_COMMONPRINT_LOOPS(QStringList &lst);
    void FormatLst(QStringList &inlst, QString &header, bool &havedouhao, QString &result, QString fmt);
    QString Proc_C_Function_SetGet(QStringList &lst, bool Local);
    QString Proc_C_Function_GenFunc(QStringList &lst, bool Local);
    void needqudizhi(T_DataFormat &single);
    bool Proc_C_Note_IsSpecialSign(QString &str);

    //get function name
    QString Proc_Note_GetFuncName(QString string);
    QString Proc_Note_GetFuncPara(QString string);
    quint32 Proc_Note_GetFuncNameSize(QString string);



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
    QString entersign;
    QString semisign;
    QString douhaosign;
    QString yinhaomsign;
    QString leftkuohaosin;
    QString rightkuohaosign;
    QString leftbigkuohaosin;
    QString rightbigkuohaosign;
    QString spacesign;
    QString maohaosign;
    QString underlinesign;
    QString tabsign;
    QString equalsign;
    QString qudizhisign;
    QString xinghao;
    QString doublexinghao;

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
    QStringList StrLst_COMMONPRINT;


private:
    void LogInitLog();
    void LogWriteFile(QString str);
    QString LogReadFile(QString filename);
    void LogDeleteFile(QString filename);
    void writepythonexecfuncfilename(QString filename);
    void writepythonexecfuncfilename_funcdeclare(QString filename);
private:
    /*************[保存log]**********************/
    QFile *plogFile;
    QString logfilename;
    QString logreadfilename;
    const char * pythonexecfilename;
    const char * pythonexecfilename_funcdeclare;
    const char * pythonexecdir;




    
};

#endif // CODESOPHIA_H
