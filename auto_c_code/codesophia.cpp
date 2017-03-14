#include "codesophia.h"
#include "ui_codesophia.h"
#include "msgtipsautoshut.h"

#define STRUCTPRINTUIOP \
    if(current_lan_num == KEY_C && current_subtype_num == SUB_STRUCTPRINT)\
        StructPrintMsgUIShow();\
    else\
        StructPrintMsgUIHide();



#define SETTITLE(TITLE) \
    current_lan_num = (TITLE);\
    current_lan = getKeyClass(TITLE); \
    showtitle = title_org + splitsign + current_lan + splitsign + current_subtype;\
    setWindowTitle(showtitle);\
    STRUCTPRINTUIOP\
    FillComBoxKeyTips();


#define SETSUBTITLE(TITLE) \
    current_subtype_num = TITLE; \
    current_subtype = getSubType(TITLE); \
    showtitle = title_org + splitsign + current_lan + splitsign + current_subtype;\
    setWindowTitle(showtitle);\
    STRUCTPRINTUIOP\
    FillComBoxKeyTips();

#define SETOPTITLE(TITLE) \
    current_optype_num = TITLE; \
    current_optype = getOPType(TITLE); \
    current_subtype_num = 0;\
    current_subtype.clear();\
    showtitle = title_org + splitsign + current_lan + splitsign + current_optype;\
    setWindowTitle(showtitle);\
    STRUCTPRINTUIOP\
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
    leftkuohaosin = "(";
    rightkuohaosign = ")";
    yinhaomsign = "\"";
    douhaosign = ",";
    entersign = "\\n";
    maohaosign = ":";


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
        case SUB_STRUCTPRINT:
            Proc_C_StructPrint(middlestrList);
            break;
        case SUB_DEFINE:
            Proc_C_Define(middlestrList);
            break;
        case SUB_IFCONDITION:
            Proc_C_IFCONDITION(middlestrList);
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

QString CodeSophia::Proc_C_Function_GenFunc(QStringList &lst, bool Local)
{
    QStringList resultlst;
    QString result;
    QString type;
    QString name;
    QString funcname;
    QString parainfo;
    QString _inname;
    QString staticFlag;
    quint32 loop1 = 0;
    quint32 loop2 = 0;
    quint8 invalid = false;
    if(Local)
        staticFlag = "static ";

    if(lst.size())
        result += "/* function declare begin */" + enter;
    foreach (QString string, lst) {
        string = string.simplified();
        string = string.replace(";", "");
        string = string.replace("(", " ");
        string = string.replace(")", " ");
        parainfo.clear();
        QStringList splitinfo = string.split(",");
        foreach (QString t2, splitinfo) {
            if(t2.split(" ").size() < 2)
            {
                invalid = true;
                break;
            }
        }
        if(invalid)
            continue;

        foreach (QString str, splitinfo) {
            QStringList tmp = str.split(" ");
            type = tmp.at(0);
            funcname = tmp.at(1);
            break;
        }
        parainfo += "(";
        loop1 = 0;
        loop2 = 0;
        foreach (QString str, splitinfo) {
            QStringList tmp = str.split(" ");
            loop1++;
            loop2 = 0;
            foreach (QString str2, tmp) {
                loop2++;
                if(loop1 == 1 && loop2 < 3)
                    continue;
                parainfo +=  spacesign + str2.simplified();
            }
            if(loop1 != splitinfo.size())
                parainfo += ",";
        }

        parainfo += ")";

        result += staticFlag + type + spacesign +funcname + parainfo + semisign + enter;
        resultlst <<  staticFlag + type + spacesign +funcname + parainfo  + enter;

    }

    if(lst.size())
        result += "/* function declare end */" + enter;

    result += enter;
    result += enter;
    if(lst.size())
        result += "/* function implement */" + enter;

    foreach (QString resl, resultlst) {
        result += resl;
        result += "{";
        result += enter;
        result += enter;
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
        result = Proc_C_Function_GenFunc(lst, false);
        break;
    case 4:
        result = Proc_C_Function_GenFunc(lst, true);
        break;
    default:
        return;
        break;
    }



    SetTextEditResult(result);

}


void CodeSophia::Proc_C_StructPrint(QStringList &lst)
{
    QString result;
    result.clear();

    QString format;
    QList<T_DataFormat> m_nameLst;
    QList<T_DataFormat> after_nameLst;
    T_DataFormat single;
    QString m_name;
    QString header;
    QString structname;
    QString structinfo;
    QString var_name;




    if(!ui->lineEdit_print->text().trimmed().isEmpty())
        header = ui->lineEdit_print->text().trimmed();
    else
        header = "printf";

    if(!ui->lineEdit_dataprint->text().trimmed().isEmpty())
        var_name = ui->lineEdit_dataprint->text().trimmed();
    else
        var_name = "";


    QString format_p = "%p   ";


    quint32 index = ui->comboBox_keytips->currentIndex();
    switch(index)
    {
    case 0:

        format = "%u   ";
        break;
    case 1:
        format = "%lu  ";
        break;
    case 2:
        format = "%llu ";
        break;
    case 3:
        format = "%d   ";
        break;
    case 4:
        format = "%s   ";
        break;
    case 5:
        format = "%-02s";
        //    << "print %-02s"
        break;
    case 6:
        format = "%-04s";
        //    << "print %-04s"
        break;
    case 7:
        format = "%-08s";
        //    << "print %-08s"
        break;
    case 8:
        format = "%#x  ";
        //    << "print %#x"
        break;
    case 9:
        format = "%x   ";
        //    << "print %x"
        break;
    case 10:
        format = "%-2x ";
        //    << "print %-2x"
        break;
    case 11:
        format = "%-02x";
        //    << "print %-02x"
        break;
    case 12:
        format = "%-4x ";
        //    << "print %-4x"
        break;
    case 13:
        format = "%-04x";
        //    << "print %-04x"
        break;
    case 14:
        format = "%-8x ";
        //    << "print %-8x"
        break;
    case 15:
        format = "%-08x";
        //    << "print %-08x"
        break;
        break;
    default:
        return;
        break;
    }
    single.format = format;

//    << "print %u"
//    << "print %lu"
//    << "print %llu"
//    << "print %d"
//    << "print %s"
//    << "print %-02s"
//    << "print %-04s"
//    << "print %-08s"
//    << "print %#x"
//    << "print %x"
//    << "print %-2x"
//    << "print %-02x"
//    << "print %-4x"
//    << "print %-04x"
//    << "print %-8x"
//    << "print %-08x"


    if(header.isEmpty())
        return;


    foreach (QString string, lst) {
        string = string.simplified();
        if (string.contains("struct") && string.contains("{"))
        {
            structname = string.split("struct").at(1).split("{").at(0).trimmed();
        }
        if (string.contains("}") && structname.isEmpty())
        {
            structname = string.split("}").at(1).split(";").at(0).trimmed();
            qDebug() << "string split name:" << string.split("}").at(0);
            qDebug() << "string split name:" << string.split("}").at(1);
        }
        if(!structname.isEmpty())
        {
            qDebug() << "strucgt name:" << structname;
            break;
        }
    }

    if(structname.isEmpty())
        return;

    structinfo += header + leftkuohaosin;
    structinfo += yinhaomsign;
    structinfo += "Struct" + spacesign;
    structinfo += structname;
    structinfo += " info following ===== ";
    structinfo += "[" + var_name + "]";
    structinfo += entersign;
    structinfo += rightkuohaosign;
    structinfo += semisign;
    structinfo += enter;


    foreach(QString string, lst) {
        string = string.simplified();
        if(!string.contains(";"))
            continue;
        if(string.left(1) == "*")
            continue;
        if(string.contains("{") || string.contains("}"))
            continue;
        QStringList tmps = string.split(";");
//        qDebug() << "; split size :" << tmps.size();
        string.clear();
        //分隔;号
        foreach (QString s, tmps) {
            if(s != tmps.last())
                string += s;
        }
//        qDebug() << "string filter :" << string;
        if(string.isEmpty())
            continue;
        if(string.contains("*"))
        {
            single.format = format_p;
        }
        else
        {
            single.format = format;
        }
        string.replace("*", "");
        tmps = string.split(",");
        if(tmps.size() > 1)
        {
            qDebug() << ", size :" << tmps.size();
            string.clear();
            //分隔,号
            foreach (QString s, tmps) {
                if(s == tmps.first())
                {
                    m_name = s.split(" ").last();
                    qDebug() << ", m_name split name:" <<m_name;
                    single.string = m_name;
                    m_nameLst << single;
                }
                else
                {
                    qDebug() << ", m_name split name:" <<s;
                    single.string = s.simplified();
                    m_nameLst << single;
                }
            }
        }
        else
        {

            m_name = string.split(" ").last();
            single.string = m_name.simplified();
            m_nameLst << single;
        }
    }

    //寻找最大长度
    quint32 maxlen = 0;
    foreach (T_DataFormat ele, m_nameLst) {
        if(ele.string.length() > maxlen)
            maxlen = ele.string.length();
    }

    foreach (T_DataFormat el, m_nameLst) {
        quint32 tmplen = el.string.length();
        if(tmplen < maxlen)
        {
            el.string = QString("%1%2").arg(el.string).arg(" ", maxlen - tmplen);
        }
        after_nameLst << el;
    }


    result += structinfo ;

    foreach (T_DataFormat el, after_nameLst) {
        result += header ;
        result += leftkuohaosin ;
        result += yinhaomsign ;
        result += el.string ;
        result += spacesign ;
        result += maohaosign ;
        result += tabsign ;
        result += el.format ;
        result += entersign ;
        result += yinhaomsign ;
        result += douhaosign ;
        result += spacesign ;
        result += var_name ;
        result += el.string ;
        result += rightkuohaosign ;
        result += semisign ;
        result += enter ;
    }


    SetTextEditResult(result);

}

void CodeSophia::Proc_C_Define(QStringList &lst)
{
    QString result;
    result.clear();

    QString first;
    QString middle;
    QString end;
    QString m_lsign;
    QString m_rsign;
    QList<T_DefineInfo> dealafter;
    QList<T_DefineInfo> after_nameLst;
    T_DefineInfo tDefInfo;


//    << ""
//    << "#define ()"
//    << "#ifdef "
//    << "#ifndef "
//    << "#ifdef  else endif"
//    << "#ifndef  else endif"
//    << "#if 0"
//    << "#if 1"


    quint32 index = ui->comboBox_keytips->currentIndex();
    switch(index)
    {
    case 0:

        first = "#define";
        middle = "";
        end = "";
        m_lsign = "";
        m_rsign = "";
        break;
    case 1:
        first = "#define";
        middle = "";
        end = "";
        m_lsign = leftkuohaosin;
        m_rsign = rightkuohaosign;
        break;
    case 2:
        first = "#ifdef";
        middle = "";
        end = "#endif";
        m_lsign = enter;
        m_rsign = enter;
        break;
    case 3:
        first = "#ifndef";
        middle = "";
        end = "#endif";
        m_lsign = enter;
        m_rsign = enter;
        break;
    case 4:
        first = "#ifdef";
        middle = "#else";
        end = "#endif";
        m_lsign = enter;
        m_rsign = enter;
        break;
    case 5:
        first = "#ifndef";
        middle = "#else";
        end = "#endif";
        m_lsign = enter;
        m_rsign = enter;
        break;
    case 6:
        first = "#if 0";
        middle = "";
        end = "#endif";
        m_lsign = enter;
        m_rsign = enter;
        break;
    case 7:
        first = "#if 1";
        middle = "";
        end = "#endif";
        m_lsign = enter;
        m_rsign = enter;
        break;


    default:
        return;
        break;
    }

    if(first.isEmpty())
        return;


    foreach (QString string, lst) {
        string = string.simplified();
        QStringList slst = string.split(" ");
        if(slst.size() < 2)
            continue;
        QString defname = slst.at(0).trimmed();
        QString defvalue = slst.at(1).trimmed();
        QString note;
        note.clear();
        if(slst.size() > 2)
        {
            quint32 lp = 0;
            foreach (QString str, slst) {
                qDebug() << "note str:" << str;
                if((lp == 0 ) || (lp == 1))
                {
                    lp++;
                    continue;
                }
                note += str + spacesign;
                lp++;
            }
        }
        tDefInfo.defname  = defname;
        tDefInfo.defvalue = defvalue;
        tDefInfo.note = note;
        dealafter.push_back(tDefInfo);


    }

    //寻找最大长度
    quint32 maxlen = 0;
    quint32 maxlen2 = 0;
    foreach (T_DefineInfo ele, dealafter) {
        if(ele.defname.length() > maxlen)
            maxlen = ele.defname.length();
    }

    foreach (T_DefineInfo ele, dealafter) {
        if(ele.defvalue.length() > maxlen2)
            maxlen2 = ele.defvalue.length();
    }



    foreach (T_DefineInfo el, dealafter) {
        quint32 tmplen = el.defname.length();
        quint32 tmplen2 = el.defvalue.length();
        if(tmplen < maxlen)
        {
            el.defname = QString("%1%2").arg(el.defname).arg(" ", maxlen - tmplen);
        }
        if(tmplen2 < maxlen2)
        {
            el.defvalue = QString("%1%2").arg(el.defvalue).arg(" ", maxlen2 - tmplen2);
        }





        after_nameLst << el;
    }



    foreach (T_DefineInfo el, after_nameLst) {
        if(middle.isEmpty())
            result += first +  spacesign + el.defname + tabsign + m_lsign + el.defvalue + m_rsign + end;
        else
            result += first +  spacesign + el.defname + enter + middle + enter + m_lsign + el.defvalue + m_rsign + end;

        result += tabsign +"/* " + el.note  + " */";
        if(end.isEmpty())
            result += enter ;
        else
            result += enter + enter;

    }


    SetTextEditResult(result);

}

void CodeSophia::Proc_C_IFCONDITION(QStringList &lst)
{
    QString result;
    result.clear();

    QString first;
    QString middle;
    QString middle2;
    QString end;
    QString m_lsign;
    QString m_rsign;
    QList<T_DefineInfo> dealafter;
    QList<T_DefineInfo> after_nameLst;
    T_DefineInfo tDefInfo;


    quint32 index = ui->comboBox_keytips->currentIndex();
    switch(index)
    {
    case 0:

        first = "if";
        middle = "";
        end = enter;
        m_lsign = "{";
        m_rsign = "}";
        break;
    case 1:
        first = "if";
        middle = "else";
        end = enter;
        m_lsign = "{";
        m_rsign = "}";
        break;
    case 2:
        first = "if";
        middle = "else if";
        middle2 = "else";
        end = enter;
        m_lsign = "{";
        m_rsign = "}";
        break;
    case 3:
        first = "switch";
        middle = "";
        end = "";
        m_lsign = "{";
        m_rsign = "}";
        break;
//    case 4:
//        first = "#ifdef";
//        middle = "#else";
//        end = "#endif";
//        m_lsign = enter;
//        m_rsign = enter;
//        break;
//    case 5:
//        first = "#ifndef";
//        middle = "#else";
//        end = "#endif";
//        m_lsign = enter;
//        m_rsign = enter;
//        break;



    default:
        return;
        break;
    }

    if(first.isEmpty())
        return;


    QString breaktips = "break;";
    foreach (QString string, lst) {
        string = string.simplified();

        if(first.contains("switch"))
        {
            result += first + leftkuohaosin + spacesign + string + spacesign + rightkuohaosign;
            result += enter;
            result += m_lsign;
            result += enter;
            result += "case 0:" + enter + enter  + tabsign + breaktips + enter;
            result += "case 1:" + enter + enter + tabsign + breaktips + enter;
            result += "case 2:" + enter + enter + tabsign + breaktips + enter;
            result += "case 3:" + enter + enter + tabsign + breaktips + enter;
            result += "case 4:" + enter + enter + tabsign + breaktips + enter;
            result += "case 5:" + enter + enter + tabsign + breaktips + enter;
            result += "case 6:" + enter + enter + tabsign + breaktips + enter;
            result += "case 7:" + enter + enter + tabsign + breaktips + enter;
            result += "case 8:" + enter + enter + tabsign + breaktips + enter;
            result += "case 9:" + enter + enter + tabsign + breaktips + enter;
            result += "case 10:" + enter + enter + tabsign + breaktips + enter;
            result += "default:" + enter + enter + tabsign + breaktips + enter;
            result += m_rsign;
            result += enter;


            continue;
        }








        result += first ;
        result += leftkuohaosin + spacesign;
        result += string;
        result += spacesign + rightkuohaosign ;
        result += enter;
        result += m_lsign + enter;
        result += enter;
        result += m_rsign + enter;
        if(!middle.isEmpty())
        {
            result += middle + enter;
            result += m_lsign + enter;
            result += enter;
            result += m_rsign + enter;
        }

        if(!middle2.isEmpty())
        {
            result += middle2 + enter;
            result += m_lsign + enter;
            result += enter;
            result += m_rsign + enter;
        }


        result += enter;

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
            << "#define "
            << "#define ()"
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
            << "gen func"
            << "gen func local"
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

void CodeSophia::on_lineEdit_print_textChanged(const QString &arg1)
{
    on_pushButton_gen_clicked();
}

void CodeSophia::on_lineEdit_dataprint_textChanged(const QString &arg1)
{
    on_pushButton_gen_clicked();
}

void CodeSophia::StructPrintMsgUIHide()
{
    ui->label_dataprint->hide();
    ui->label_print->hide();
    ui->lineEdit_dataprint->hide();
    ui->lineEdit_print->hide();
}

void CodeSophia::StructPrintMsgUIShow()
{
    ui->label_dataprint->show();
    ui->label_print->show();
    ui->lineEdit_dataprint->show();
    ui->lineEdit_print->show();
}
