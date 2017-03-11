#include "codesophia.h"
#include "ui_codesophia.h"
#include "msgtipsautoshut.h"


#define SETTITLE(TITLE) \
    current_lan_num = (TITLE);\
    current_lan = getKeyClass(TITLE); \
    showtitle = title_org + splitsign + current_lan + splitsign + current_subtype;\
    setWindowTitle(showtitle);\
    FillComBoxKeyTips();


#define SETSUBTITLE(TITLE) \
    current_subtype_num = TITLE; \
    current_subtype = getSubType(TITLE); \
    showtitle = title_org + splitsign + current_lan + splitsign + current_subtype;\
    setWindowTitle(showtitle);\
    FillComBoxKeyTips();

#define SETOPTITLE(TITLE) \
    current_optype_num = TITLE; \
    current_optype = getOPType(TITLE); \
    current_subtype_num = 0;\
    current_subtype.clear();\
    showtitle = title_org + splitsign + current_lan + splitsign + current_optype;\
    setWindowTitle(showtitle);\
    FillComBoxKeyTips();


#define SETTITLE_STARTUP \
    showtitle = title_org + splitsign + current_lan + splitsign + current_subtype;\
    setWindowTitle(showtitle);\
    FillComBoxKeyTips();





CodeSophia::CodeSophia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CodeSophia)
{
    ui->setupUi(this);
    title_org = this->windowTitle();
    splitsign = " -- ";
    current_subtype = "";

    current_lan.clear(); //当前语言
    current_lan_num = 0;
    current_subtype.clear(); //sub class
    current_subtype_num = 0;
    current_optype.clear(); //op class
    current_optype_num = 0;
    showtitle.clear(); //sub class

    enter = "\n";
    semisign = ";";
    underlinesign = "_";
    equalsign = " = ";
    spacesign = " ";
    tabsign = "    ";


    FillStringList();

    ActiveSets();
    ButtonSets();
    ComboBoxSets();
    TextEditSets();
    ReadHistorySettings();
    SETTITLE_STARTUP;

}

CodeSophia::~CodeSophia()
{
    delete ui;
}

void CodeSophia::ActiveSets()
{
    connect(ui->actionC_2, SIGNAL(triggered()), this, SLOT(CMsg()));
    connect(ui->actionCPP, SIGNAL(triggered()), this, SLOT(CPPMsg()));
    connect(ui->actionJAVA, SIGNAL(triggered()), this, SLOT(JavaMsg()));
    connect(ui->actionSHELL, SIGNAL(triggered()), this, SLOT(ShellMsg()));
    connect(ui->action_declare, SIGNAL(triggered()), this, SLOT(DeclareMsg()));
    connect(ui->action_define, SIGNAL(triggered()), this, SLOT(DefineMsg()));
    connect(ui->action_fucntion, SIGNAL(triggered()), this, SLOT(FuncMsg()));
    connect(ui->action_struct,  SIGNAL(triggered()), this, SLOT(StructMsg()));
    connect(ui->action_structprint, SIGNAL(triggered()), this, SLOT(StructPrintMsg()));
    connect(ui->action_header, SIGNAL(triggered()), this, SLOT(HeaderMsg()));
    connect(ui->action_ifcondition, SIGNAL(triggered()), this, SLOT(IfConditionMsg()));
    connect(ui->action_loop, SIGNAL(triggered()), this, SLOT(LoopMsg()));
    connect(ui->action_note, SIGNAL(triggered()), this, SLOT(NoteMsg()));
    connect(ui->action_save, SIGNAL(triggered()), this, SLOT(SaveMsg()));

}


void CodeSophia::ButtonSets()
{
    connect(ui->pushButton_gen,SIGNAL(clicked()), this, SLOT(on_pushButton_gen_clicked()));
}

void CodeSophia::ComboBoxSets()
{
    connect(ui->comboBox_keytips,SIGNAL(clicked()), this, SLOT(on_comboBox_keytips_clicked()));
    connect(ui->comboBox_keytips,SIGNAL(currentIndexChanged(int)), this, SLOT(on_pushButton_gen_clicked()));

}
void CodeSophia::TextEditSets()
{
    connect(ui->textEdit_key,SIGNAL(textChanged()), this, SLOT(on_pushButton_gen_clicked()));


}

void CodeSophia::ShowTipsInfo(QString s)
{
    MsgTipsAutoShut *tipsinfo = new MsgTipsAutoShut(NULL,1000);
    tipsinfo->SetTipsInfo(s);
    tipsinfo->show();
}


void CodeSophia::on_pushButton_gen_clicked()
{
    if(current_lan.isEmpty())
    {
        ShowTipsInfo("laguage empty!!!");
        return;
    }
    if(current_subtype.isEmpty())
    {
        ShowTipsInfo("subtype empty!!!");
        return;
    }
//    if(ui->textEdit_key->toPlainText().isEmpty())
//    {
//        ui->textEdit_result->clear();
//        //        ShowTipsInfo("Key TextEdit empty,please input something!!!");
//        return;
//    }

    int n;
    int Number= ui->textEdit_key->document()->lineCount();
    QString orgText = ui->textEdit_key->toPlainText();
    QStringList middlestrList;
    qDebug() << "Number :" << Number;
    qDebug() << "key    :" << orgText << endl;

    for(n=0;n<Number;n++)
    {
        QString str=ui->textEdit_key->toPlainText().section('\n',n-Number ,n-Number,QString::SectionSkipEmpty); //取得每行（以换行符进行换行）搜索
        QStringList list=str.split("\n");
        //        ......... //list.at(n) 随便获得各行中以空格分开的各个字符串
        foreach (QString string, list) {
            if(string.isEmpty())
                continue;
            middlestrList << string;
            qDebug() << "middle str :" << string;
        }

    }
    qDebug() << "middle result size:" << middlestrList.size();


    switch(current_lan_num)
    {
    case KEY_C:
    {
        switch(current_subtype_num)
        {
        case SUB_HEADER:
            Proc_C_Header(middlestrList);
            break;

        case SUB_NOTE:
            Proc_C_Note(middlestrList);
            break;
        case SUB_FUNCTION:
            Proc_C_Function(middlestrList);
            break;

        default:
            break;
        }
    }
        break;
    default:
        break;
    }




}

void CodeSophia::on_comboBox_keytips_clicked()
{


}


void CodeSophia::CMsg()
{
    SETTITLE((KEY_C));

}

void CodeSophia::CPPMsg()
{
    SETTITLE((KEY_CPP));


}


void CodeSophia::JavaMsg()
{
    SETTITLE((KEY_JAVA));

}

void CodeSophia::ShellMsg()
{
    SETTITLE((KEY_SHELL));

}

void CodeSophia::DeclareMsg()
{
    SETSUBTITLE((SUB_DECLARE));
}
void CodeSophia::DefineMsg()
{
    SETSUBTITLE((SUB_DEFINE));

}
void CodeSophia::FuncMsg()
{
    SETSUBTITLE((SUB_FUNCTION));

}
void CodeSophia::StructMsg()
{
    SETSUBTITLE((SUB_STRUCT));

}
void CodeSophia::StructPrintMsg()
{
    SETSUBTITLE((SUB_STRUCTPRINT));

}

void CodeSophia::HeaderMsg()
{
    SETSUBTITLE((SUB_HEADER));

}
void CodeSophia::IfConditionMsg()
{
    SETSUBTITLE((SUB_IFCONDITION));

}

void CodeSophia::LoopMsg()
{
    SETSUBTITLE((SUB_LOOP));

}
void CodeSophia::NoteMsg()
{
    SETSUBTITLE((SUB_NOTE));

}


void CodeSophia::SaveMsg()
{
    SETOPTITLE((OP_SAVE));

}

QString CodeSophia::getSubType(SubType type)
{
    switch(type)
    {
    case SUB_DECLARE:
        return "Declare";
    case SUB_DEFINE:
        return "Define";
    case SUB_FUNCTION:
        return "Function";
    case SUB_STRUCT:
        return "Struct";
    case SUB_STRUCTPRINT:
        return "StructPrint";
    case SUB_HEADER:
        return "Header";
    case SUB_IFCONDITION:
        return "IFCONDITION";
    case SUB_LOOP:
        return "LOOP";
    case SUB_NOTE:
        return "NOTE";
    default:
        break;
    }
    return "";
}
QString CodeSophia::getOPType(OPTYPE type)
{
    switch(type)
    {
    case OP_SAVE:
        return "SAVE";
    default:
        break;
    }
    return "";
}

QString CodeSophia::getKeyClass(KeyClass cls)
{
    switch(cls)
    {
    case KEY_C:
        return "C";
    case KEY_CPP:
        return "CPP";
    case KEY_JAVA:
        return "JAVA";
    case KEY_SHELL:
        return "SHELL";
    default:
        break;
    }
    return "";
}









void CodeSophia::FillComBoxKeyTips()
{
//    if(current_lan.isEmpty() || current_subtype.isEmpty())
//        return;
    qDebug() << "current   lan:" << current_lan ;
    qDebug() << "  subtype lan:" << current_subtype ;
    qDebug() << "  optype  lan:" << current_optype;
    QStringList list;
    list.clear();

    switch((current_lan_num))
    {
    case KEY_C:
    {
        switch(( current_subtype_num))
        {
        case SUB_HEADER:
            list = StrLst_KEYC_HEADER;
            break;
        case SUB_DECLARE:
            list  = StrLst_KEYC_DECLARE;
            break;
        case SUB_DEFINE:
            list  = StrLst_KEYC_DEFINE;
            break;
        case SUB_FUNCTION:
            list  = StrLst_KEYC_FUNCTION;
            break;
        case SUB_STRUCT:
            list  = StrLst_KEYC_STRUCT;
            break;
        case SUB_STRUCTPRINT:
            list  = StrLst_KEYC_STRUCTPRINT;
            break;
        case SUB_LOOP:
            list  = StrLst_KEYC_LOOP;
            break;
        case SUB_IFCONDITION:
            list  = StrLst_KEYC_IFCONDITION;
            break;
        case SUB_NOTE:
            list  = StrLst_KEYC_NOTE;
            break;
        default:
            break;
        }

    }
        break;




    default:
        break;
    }

    foreach(QString str, list)
    {
        qDebug() << "str:" << str;
    }

    ui->comboBox_keytips->clear();
    ui->comboBox_keytips->addItems(list);

}


void CodeSophia::ReadHistorySettings()
{
    QSettings m_settings("codesophia.com.cn", "codesophia");
    //    qDebug() << "read setting filename:" << m_settings.fileName();

    current_lan_num =  m_settings.value("current_lan_num").toInt();
    current_subtype_num =  m_settings.value("current_subtype_num").toInt();
    current_optype_num =  m_settings.value("current_optype_num").toInt();
    current_lan =  m_settings.value("current_lan").toString();
    current_subtype =  m_settings.value("current_subtype").toString();
    current_optype =  m_settings.value("current_optype").toString();



    this->restoreGeometry(m_settings.value("AutoCCode_Geometry").toByteArray());
}

void CodeSophia::WriteCurrentSettings()
{
    QSettings m_settings("codesophia.com.cn", "codesophia");
    m_settings.setValue("current_lan_num", current_lan_num);
    m_settings.setValue("current_subtype_num", current_subtype_num);
    m_settings.setValue("current_optype_num", current_optype_num);
    m_settings.setValue("current_lan", current_lan);
    m_settings.setValue("current_subtype", current_subtype);
    m_settings.setValue("current_optype", current_optype);

    m_settings.setValue("AutoCCode_Geometry", this->saveGeometry());

    //    qDebug() << "setting filename:" << m_settings.fileName();

}



void CodeSophia::closeEvent(QCloseEvent *event)
{
    qDebug() << "CodeSophia closeEvent";
    WriteCurrentSettings();
    event->accept();
}

void CodeSophia::SetTextEditResult(QString &str)
{
    ui->textEdit_result->setText(str);
    QTextCursor cursor = ui->textEdit_result->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit_result->setTextCursor(cursor);
}



void CodeSophia::Proc_C_Header(QStringList &lst)
{
    QString result;
    result.clear();

    QString leftsign;
    QString rightsign;
    QString header = "#include ";

    QString combotext = ui->comboBox_keytips->currentText();
    if(combotext.contains("<"))
    {
        leftsign = "<";
        rightsign = ">";
    }
    else if (combotext.contains("\""))
    {
        leftsign = "\"";
        rightsign = "\"";
    }
    else
    {
        return;
    }

    foreach (QString string, lst) {
        if(string.isEmpty())
            continue;
        result += header + leftsign + string + rightsign +"\n";
        qDebug() << "result str :" << result;
    }






    SetTextEditResult(result);

}

void CodeSophia::Proc_C_Note(QStringList &lst)
{
    QString result;
    result.clear();

    QString leftsign;
    QString rightsign;
    QString header;
    bool hassplit = true;
    bool ignorelst = false;

    quint32 index = ui->comboBox_keytips->currentIndex();
    switch(index)
    {
    case 0:
        result = "\n"
                "\/*\n"
                 "* See the file LICENSE for redistribution information.\n"
                 "*\n"
                 "* Copyright (c) 2012, 2017 WeilaiDb and/or its affiliates.  All rights reserved.\n"
                 "*\n"
                 "* $Id$\n"
                 "*\/\n";
        ignorelst = true;
        break;
    case 1:
        break;
    case 2:
        hassplit =false;
        header = "";
        leftsign = "/*  \n\n";
        rightsign = "\n\n*/";
        foreach (QString string, lst) {
            if(string.isEmpty())
                continue;
            if(string.contains("//"))
            {
                ShowTipsInfo("Invalid Text, having //");
                break;
            }
            if(string.contains("/*"))
            {
                ShowTipsInfo("Invalid Text, having /*");
                break;
            }
            if(string.contains("*/"))
            {
                ShowTipsInfo("Invalid Text, having */");
                break;
            }
        }
        break;
    case 3:
        header = "";
        leftsign = "//  ";
        rightsign = "";
        break;
    default:
        return;
        break;
    }

    if(!hassplit)
        result +=  header + leftsign;

    foreach (QString string, lst) {
        if(ignorelst)
            break;
        if(string.isEmpty())
            continue;
        if(hassplit)
            result += header + leftsign + string + rightsign +"\n";
        else
            result +=  string + "\n";

        qDebug() << "result str :" << result;
    }

    if(!hassplit)
        result +=  rightsign;





    SetTextEditResult(result);

}

QString CodeSophia::Proc_C_Function_SetGet(QStringList &lst, bool Local)
{
    QString result;
    QString type;
    QString name;
    QString _inname;
    QString staticFlag = "";
    if(Local)
        staticFlag = "static ";

    if(lst.size())
        result += "/* variable declare begin */" + enter;
    foreach (QString string, lst) {
        string = string.simplified();
        if(!string.contains(";"))
            result += string + semisign + enter;
        else
            result += string + enter;
    }

    if(lst.size())
        result += "/* variable declare end */" + enter;


    foreach(QString string, lst) {
        string = string.simplified();
        QStringList list=string.split(" ");
        if(list.size() < 2)
            continue;

        if(list.at(1) == "*" || list.at(1) == "&")
        {
            if(list.size() <3)
                continue;
            type = list.at(0) + list.at(1);
            name = list.at(2);
        }
        else
        {
            type = list.at(0);
            name = list.at(1);
            if(name.contains("*"))
            {
                type +="*";
                name.replace("*", "");
            }else if(name.contains("&"))
            {
                type +="&";
                name.replace("&", "");
            }
        }
        name.replace(";","");

        if(name.contains("."))
        {
            QStringList inlist = name.split(".");
            _inname = inlist.at(1);
        }
        else
        {
            _inname = name;
        }


        //note for set and get name
        result += "/*";
        result += enter;
        result += staticFlag + "Set and Get for " + _inname;
        result += enter;
        result += "*/";
        result += enter;


        //set function
        result += staticFlag;
        result += "void" + spacesign;
        result += "Set" + underlinesign + _inname;
        result += "(" + type + spacesign + underlinesign + _inname + ")";
        result += enter;
        result += "{";
        result += enter;
        result += tabsign;
        result += name + equalsign + underlinesign + _inname + semisign;
        result += enter;
        result += "}";
        result += enter;

        result += enter;

        //get function
        result += staticFlag;
        result += type + spacesign;
        result += "Get" + underlinesign + _inname;
        result += "(void)";
        result += enter;
        result += "{";
        result += enter;
        result += tabsign;
        result += "return " +name +semisign;
        result += enter;
        result += "}";
        result += enter;
        result += enter;
    }

    return result;
}

void CodeSophia::Proc_C_Function(QStringList &lst)
{
    QString result;
    result.clear();

    QString leftsign;
    QString rightsign;
    QString header;


    quint32 index = ui->comboBox_keytips->currentIndex();
    switch(index)
    {
    case 0:
        header = "extern ";
        foreach(QString string, lst) {
            string = string.simplified();
            if(string.contains(semisign))
                result += header + string + enter;
            else
                result += header + string + semisign + enter;
        }
        break;
    case 1:
        result = Proc_C_Function_SetGet(lst,false);
        break;
    case 2:
        result = Proc_C_Function_SetGet(lst, true);
        break;
    case 3:
        header = "";
        leftsign = "//  ";
        rightsign = "";
        break;
    default:
        return;
        break;
    }



    SetTextEditResult(result);

}

void CodeSophia::on_pushButton_leftclear_clicked()
{
    ui->textEdit_key->clear();
}

void CodeSophia::on_pushButton_rightclear_clicked()
{
    ui->textEdit_result->clear();
}



void CodeSophia::FillStringList()
{
    //    QStringList StrLst_KEYC_HEADER;

    StrLst_KEYC_HEADER
            << "#include <>"
            << "#include \"\""
               ;
    //    QStringList StrLst_KEYC_DECLARE;
    StrLst_KEYC_DECLARE
            << "extern"
            << "extern C"
               ;
    //    QStringList StrLst_KEYC_DEFINE;
    StrLst_KEYC_DEFINE
            << "#ifdef "
            << "#ifndef "
            << "#ifdef  else "
            << "#ifndef  else"
            << "#if 0"
            << "#if 1"
               ;
    //    QStringList StrLst_KEYC_FUNCTION;
    StrLst_KEYC_FUNCTION
            << "extern"
            << "set get"
            << "set get local"
               ;
    //    QStringList StrLst_KEYC_STRUCT;
    StrLst_KEYC_STRUCT
            << "typedef struct "
               ;
    //    QStringList StrLst_KEYC_STRUCTPRINT;
    StrLst_KEYC_STRUCTPRINT
            << "print %u"
            << "print %lu"
            << "print %llu"
            << "print %d"
            << "print %s"
            << "print %-02s"
            << "print %-04s"
            << "print %-08s"
            << "print %#x"
            << "print %x"
            << "print %-2x"
            << "print %-02x"
            << "print %-4x"
            << "print %-04x"
            << "print %-8x"
            << "print %-08x"
               ;
    //    QStringList StrLst_KEYC_LOOP;
    StrLst_KEYC_LOOP
            << "for"
            << "while"
               ;
    StrLst_KEYC_IFCONDITION
            << "if"
            << "if else"
            << "if elseif else"
            << "switch"
               ;
    //    QStringList StrLst_KEYC_NOTE;
    StrLst_KEYC_NOTE
            << "file note"
            << "function note"
            << "/* */"
            << "//"
               ;


}
