#include "codesophia.h"
#include "ui_codesophia.h"
#include "msgtipsautoshut.h"
#include <QRegExp>
#include <QFile>
#include <QDate>
#include <QTime>
#include <windows.h>
#include "comon.h"

#define SC_LOG

#define STRUCTPRINTUIOP \
    if(current_lan_num == KEY_C &&( current_subtype_num == SUB_STRUCTPRINT\
    || current_subtype_num == SUB_COMMONPRINT))\
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




/*============================================
* FuncName    : CodeSophia::CodeSophia
* Description :
* @parent     :
* Author      :
* Time        : 2017-05-28
============================================*/
CodeSophia::CodeSophia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CodeSophia),
    msgsametimer(new QTimer())
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale()); //设置GBK到本地
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

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
    leftbigkuohaosin = "{";
    rightbigkuohaosign = "}";
    yinhaomsign = "\"";
    douhaosign = ",";
    entersign = "\\n";
    maohaosign = ":";
    qudizhisign = "&";
    xinghao = "*";
    doublexinghao = "**";

    logfilename        ="D:\\files_time.log";
    logreadfilename    ="D:\\\\files_time.re.log";
    pythonexecfilename ="D:\\re.func.py";
    pythonexecfilename_funcdeclare ="D:\\re.func_declare.py";
    pythonexecdir ="D:";




    //combox_print
    QStringList printlst;
    printlst<<""
           <<"mlog_byfunc"
          <<"mlog_msgbyfunc"
         <<"showmlog_byfunc"
           ;
    ui->comboBox_print->addItems(printlst);



    msgsametimer->setInterval(1000);
    connect(msgsametimer,SIGNAL(timeout()), this,SLOT(writemsg()));
    LogInitLog();

    FillStringList();

    ActiveSets();
    ButtonSets();
    ComboBoxSets();
    TextEditSets();
    ReadHistorySettings();
    SETTITLE_STARTUP;
    ReadHistorySettings();

}

/*============================================
* FuncName    : CodeSophia::~CodeSophia
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
CodeSophia::~CodeSophia()
{
    delete ui;
}

/*============================================
* FuncName    : CodeSophia::ActiveSets
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
    connect(ui->action_common_print, SIGNAL(triggered()), this, SLOT(Common_Print()));

}


/*============================================
* FuncName    : CodeSophia::ButtonSets
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::ButtonSets()
{
    connect(ui->pushButton_gen,SIGNAL(clicked()), this, SLOT(on_pushButton_gen_clicked()));
}

/*============================================
* FuncName    : CodeSophia::ComboBoxSets
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::ComboBoxSets()
{
    connect(ui->comboBox_keytips,SIGNAL(clicked()), this, SLOT(on_comboBox_keytips_clicked()));
    connect(ui->comboBox_keytips,SIGNAL(currentIndexChanged(int)), this, SLOT(on_pushButton_gen_clicked()));
    connect(ui->comboBox_print,SIGNAL(currentIndexChanged(int)), this, SLOT(on_pushButton_gen_clicked()));

}
/*============================================
* FuncName    : CodeSophia::TextEditSets
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::TextEditSets()
{
    connect(ui->textEdit_key,SIGNAL(textChanged()), this, SLOT(on_pushButton_gen_clicked()));


}

/*============================================
* FuncName    : CodeSophia::ShowTipsInfo
* Description :
* @s          :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::ShowTipsInfo(QString s)
{
    MsgTipsAutoShut *tipsinfo = new MsgTipsAutoShut(NULL,1000);
    tipsinfo->SetTipsInfo(s);
    tipsinfo->show();
}


/*============================================
* FuncName    : CodeSophia::on_pushButton_gen_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_pushButton_gen_clicked()
{
    static bool isprocessing = false;
    if(isprocessing)
    {
        return;
    }
    isprocessing = true;

    if(current_lan.isEmpty())
    {
        ShowTipsInfo("laguage empty!!!");
        isprocessing = false;
        return;
    }
    if(current_subtype.isEmpty())
    {
        ShowTipsInfo("subtype empty!!!");
        isprocessing = false;
        return;
    }
    //    if(ui->textEdit_key->toPlainText().isEmpty())
    //    {
    //        ui->textEdit_result->clear();
    //        //        ShowTipsInfo("Key TextEdit empty,please input something!!!");
    //        return;
    //    }

    int n;
//    int Number= ui->textEdit_key->document()->lineCount();
    QString orgText = ui->textEdit_key->toPlainText();
    QStringList middlestrList;
//    qDebug() << "Number :" << Number;
    qDebug() << "key    :" << orgText << endl;
    QString str="";



    //write to files
    QString filename = "D:\\funcnames_xxx.txt";
    LogWriteFile(filename, orgText);
    orgText = LogReadFile(filename);

    QStringList list = orgText.split("\n");

    //progressbar for show infos
    QProgressBar *txtBar = NULL;
    if(list.size() > 2000)
    {
        txtBar = new QProgressBar();
        txtBar->show();
        txtBar->setRange(0, list.size());
    }
    foreach (QString str, list) {
        if(!ui->checkBox_showAllText->isChecked() && str.isEmpty())
        {
            if(txtBar)
                txtBar->setValue(n++);

            continue;
        }
        if(txtBar)
            txtBar->setValue(n++);

        middlestrList << str;
        qApp->processEvents();
    }

    if(txtBar)
    {
        txtBar->hide();
        txtBar->deleteLater();
    }
//    update();
    qDebug() << "middle result size:" << middlestrList.size();
    foreach (QString str , middlestrList) {
        qDebug() << str;
    }
    qDebug() << "middle result size: " << middlestrList.size() << "ends";


    switch(current_lan_num)
    {
    case KEY_C:
    {
        switch(current_subtype_num)
        {
        case SUB_HEADER:
            Proc_C_Header(middlestrList);
            break;
        case SUB_DECLARE:
            Proc_C_Declare(middlestrList);
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
        case SUB_STRUCT:
            Proc_C_Struct(middlestrList);
            break;
        case SUB_DEFINE:
            Proc_C_Define(middlestrList);
            break;
        case SUB_IFCONDITION:
            Proc_C_IFCONDITION(middlestrList);
            break;
        case SUB_LOOP:
            Proc_C_LOOP(middlestrList);
            break;
        case SUB_COMMONPRINT:
            Proc_C_COMMONPRINT(middlestrList);
            break;

        default:
            break;
        }
    }
        break;
    default:
        break;
    }


    isprocessing = false;

}

/*============================================
* FuncName    : CodeSophia::on_comboBox_keytips_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_comboBox_keytips_clicked()
{


}


/*============================================
* FuncName    : CodeSophia::CMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::CMsg()
{
    SETTITLE((KEY_C));

}

/*============================================
* FuncName    : CodeSophia::CPPMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::CPPMsg()
{
    SETTITLE((KEY_CPP));


}


/*============================================
* FuncName    : CodeSophia::JavaMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::JavaMsg()
{
    SETTITLE((KEY_JAVA));

}

/*============================================
* FuncName    : CodeSophia::ShellMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::ShellMsg()
{
    SETTITLE((KEY_SHELL));

}

/*============================================
* FuncName    : CodeSophia::DeclareMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::DeclareMsg()
{
    SETSUBTITLE((SUB_DECLARE));
}
/*============================================
* FuncName    : CodeSophia::DefineMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::DefineMsg()
{
    SETSUBTITLE((SUB_DEFINE));

}
/*============================================
* FuncName    : CodeSophia::FuncMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::FuncMsg()
{
    SETSUBTITLE((SUB_FUNCTION));

}
/*============================================
* FuncName    : CodeSophia::StructMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::StructMsg()
{
    SETSUBTITLE((SUB_STRUCT));

}
/*============================================
* FuncName    : CodeSophia::StructPrintMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::StructPrintMsg()
{
    SETSUBTITLE((SUB_STRUCTPRINT));

}

/*============================================
* FuncName    : CodeSophia::HeaderMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::HeaderMsg()
{
    SETSUBTITLE((SUB_HEADER));

}
/*============================================
* FuncName    : CodeSophia::IfConditionMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::IfConditionMsg()
{
    SETSUBTITLE((SUB_IFCONDITION));

}

/*============================================
* FuncName    : CodeSophia::LoopMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::LoopMsg()
{
    SETSUBTITLE((SUB_LOOP));

}
/*============================================
* FuncName    : CodeSophia::NoteMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::NoteMsg()
{
    SETSUBTITLE((SUB_NOTE));

}

/*============================================
* FuncName    : CodeSophia::Common_Print
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::Common_Print()
{
    SETSUBTITLE((SUB_COMMONPRINT));

}


/*============================================
* FuncName    : CodeSophia::SaveMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::SaveMsg()
{
    SETOPTITLE((OP_SAVE));

}

/*============================================
* FuncName    : CodeSophia::getSubType
* Description :
* @type       :
* Author      :
* Time        : 2017-05-28
============================================*/
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
    case SUB_COMMONPRINT:
        return "COMMONPRINT";
    default:
        break;
    }
    return "";
}
/*============================================
* FuncName    : CodeSophia::getOPType
* Description :
* @type       :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : CodeSophia::getKeyClass
* Description :
* @cls        :
* Author      :
* Time        : 2017-05-28
============================================*/
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









/*============================================
* FuncName    : CodeSophia::FillComBoxKeyTips
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
        case SUB_COMMONPRINT:
            list  = StrLst_COMMONPRINT;
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


/*============================================
* FuncName    : CodeSophia::ReadHistorySettings
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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

    ui->checkBox_showFunc->setChecked(m_settings.value("showFunc").toBool());
    ui->checkBox_showAllText->setChecked(m_settings.value("showAllText").toBool());
    ui->comboBox_keytips->setCurrentIndex(m_settings.value("comboBox_keytips").toInt());
    ui->comboBox_print->setCurrentIndex(m_settings.value("comboBox_print").toInt());
    qDebug() <<  "reading from history settings, of comboBox_keytips :" << m_settings.value("comboBox_keytips").toInt();

    this->restoreGeometry(m_settings.value("CodeSophia_Geometry").toByteArray());
}

/*============================================
* FuncName    : CodeSophia::WriteCurrentSettings
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::WriteCurrentSettings()
{
    QSettings m_settings("codesophia.com.cn", "codesophia");
    m_settings.setValue("current_lan_num", current_lan_num);
    m_settings.setValue("current_subtype_num", current_subtype_num);
    m_settings.setValue("current_optype_num", current_optype_num);
    m_settings.setValue("current_lan", current_lan);
    m_settings.setValue("current_subtype", current_subtype);
    m_settings.setValue("current_optype", current_optype);
    m_settings.setValue("showFunc", ui->checkBox_showFunc->isChecked());
    m_settings.setValue("showAllText", ui->checkBox_showAllText->isChecked());
    m_settings.setValue("comboBox_keytips", ui->comboBox_keytips->currentIndex());
    m_settings.setValue("comboBox_print", ui->comboBox_print->currentIndex());
    qDebug() <<  "writing from history settings, of comboBox_keytips :" << ui->comboBox_keytips->currentIndex();

    m_settings.setValue("CodeSophia_Geometry", this->saveGeometry());

    //    qDebug() << "setting filename:" << m_settings.fileName();

}



/*============================================
* FuncName    : CodeSophia::closeEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::closeEvent(QCloseEvent *event)
{
    qDebug() << "CodeSophia closeEvent";
    WriteCurrentSettings();
    event->accept();
}

/*============================================
* FuncName    : CodeSophia::SetTextEditResult
* Description :
* @str        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::SetTextEditResult(QString &str)
{
    ui->textEdit_result->setText(str);
    QTextCursor cursor = ui->textEdit_result->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit_result->setTextCursor(cursor);
}



/*============================================
* FuncName    : CodeSophia::Proc_C_Header
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::Proc_C_Header(QStringList &lst)
{
    QString result;
    result.clear();

    QString leftsign;
    QString rightsign;
    QString header = "#include ";

//    QString combotext = ui->comboBox_keytips->currentText();
    quint32 comboindex = ui->comboBox_keytips->currentIndex();
//    if(combotext.contains("<"))
//    {
//        leftsign = "<";
//        rightsign = ">";
//    }
//    else if (combotext.contains("\""))
//    {
//        leftsign = "\"";
//        rightsign = "\"";
//    }
//    else
//    {
//        return;
//    }

    switch( comboindex )
    {
    case 0: //"#include <>
        leftsign = "<";
        rightsign = ">";
        break;
    case 1: //"#include ""
        leftsign = "\"";
        rightsign = "\"";
        break;
    case 2: //"#ifdef _cplusplus extern \"C\""
    {
        if(lst.size() == 0 )
            return;
        quint32 cnt = 0;
        foreach (QString string, lst) {
            if(string.isEmpty())
                continue;
            cnt++;
        }
        if(cnt == 0)
            return;

        leftsign = "#ifdef __cplusplus " + enter + "extern \"C\" {" + enter + "#endif" + enter;
        rightsign = "#ifdef __cplusplus " + enter + "}" + enter  + "#endif" + enter;;
        result += leftsign;
        foreach (QString string, lst) {
            if(string.isEmpty())
                continue;
            result += string + enter;
        }
        result += rightsign;
    }
        goto end;

        break;
    case 3:

        break;
    case 4:

        break;
    default:

        break;
    }




    foreach (QString string, lst) {
        if(string.isEmpty())
            continue;
        result += header + leftsign + string + rightsign +"\n";
        qDebug() << "result str :" << result;
    }

end:
    SetTextEditResult(result);

}

/*============================================
* FuncName    : CodeSophia::Proc_C_Declare
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::Proc_C_Declare(QStringList &lst)
{
    QString result;
    result.clear();

    QString leftsign;
    QString rightsign;
    QString header = "";

    QString combotext = ui->comboBox_keytips->currentText();
    if(combotext.contains("extern C"))
    {
        leftsign = "";
        rightsign = "";
        header = "extern \"C\" ";
    }
    else if (combotext.contains("extern"))
    {
        leftsign = "";
        rightsign = "";
        header = "extern ";
    }
    else
    {
        return;
    }

    foreach (QString string, lst) {
        if(string.isEmpty())
            continue;
        if(string.contains(";"))
        {
            string.replace(";","");
            result += header + leftsign + string + rightsign + semisign + "\n";
        }
        else
        {
            result += header + leftsign + string + rightsign + semisign + "\n";
        }
        qDebug() << "result str :" << result;
    }



    SetTextEditResult(result);

}

/*============================================
* FuncName    : CodeSophia::Proc_Note_GetFuncName
* Description :
* @string     :
* Author      :
* Time        : 2017-05-28
============================================*/
QString CodeSophia::Proc_Note_GetFuncName(QString string)
{
//    QString pattern("\\([^(]*\\)"); //static inline void list_del(struct list_head *entry)
    QString pattern("^[^(]*");
    QRegExp rx(pattern);
//            static inline void list_add(struct list_head *new, struct list_head *head)
    if(string.contains(rx))
    {
        int pos = string.indexOf(rx);              // 0, position of the first match.
                                                // Returns -1 if str is not found.
                                                // You can also use rx.indexIn(str);
//        qDebug() << pos;
        if ( pos >= 0 )
        {
//            qDebug() << rx.matchedLength();     // 5, length of the last matched string
//                                                // or -1 if there was no match
//            qDebug() << rx.capturedTexts();     // QStringList("a=100", "a", "100"),
//                                                //   0: text matching pattern
//                                                //   1: text captured by the 1st ()
//                                                //   2: text captured by the 2nd ()

//            qDebug() << rx.cap(0);              // a=100, text matching pattern
//            qDebug() << rx.cap(1);              // a, text captured by the nth ()
//            qDebug() << rx.cap(2);              // 100,

//            qDebug() << rx.pos(0);              // 0, position of the nth captured text
//            qDebug() << rx.pos(1);              // 0
//            qDebug() << rx.pos(2);              // 2
            return rx.cap(0);
        }
    }

    return "";
}

/*============================================
* FuncName    : CodeSophia::Proc_Note_GetFuncNameSize
* Description :
* @string     :
* Author      :
* Time        : 2017-05-28
============================================*/
quint32 CodeSophia::Proc_Note_GetFuncNameSize(QString string)
{
    //    QString pattern("\\([^(]*\\)"); //static inline void list_del(struct list_head *entry)
    QString pattern("^[^(]*");
    QRegExp rx(pattern);
    //            static inline void list_add(struct list_head *new, struct list_head *head)
    if(string.contains(rx))
    {
        int pos = string.indexOf(rx);              // 0, position of the first match.
        // Returns -1 if str is not found.
        // You can also use rx.indexIn(str);
        //        qDebug() << pos;
        if ( pos >= 0 )
        {
            //            qDebug() << rx.matchedLength();     // 5, length of the last matched string
            //                                                // or -1 if there was no match
            //            qDebug() << rx.capturedTexts();     // QStringList("a=100", "a", "100"),
            //                                                //   0: text matching pattern
            //                                                //   1: text captured by the 1st ()
            //                                                //   2: text captured by the 2nd ()

            //            qDebug() << rx.cap(0);              // a=100, text matching pattern
            //            qDebug() << rx.cap(1);              // a, text captured by the nth ()
            //            qDebug() << rx.cap(2);              // 100,

            //            qDebug() << rx.pos(0);              // 0, position of the nth captured text
            //            qDebug() << rx.pos(1);              // 0
            //            qDebug() << rx.pos(2);              // 2
            QString laststr = rx.cap(0).simplified();
            quint32 size = 0;
            if(laststr.contains("::"))
            {
                size = 2;
            }
            else
            {
                size = laststr.split(" ").size();
            }
            return size;
        }
    }

    return 0;
}


/*============================================
* FuncName    : CodeSophia::Proc_Note_GetFuncPara
* Description :
* @string     :
* Author      :
* Time        : 2017-05-28
============================================*/
QString CodeSophia::Proc_Note_GetFuncPara(QString string)
{
    QString pattern("\\([^(]*\\)"); //static inline void list_del(struct list_head *entry)
//    QString pattern("^[^(]*");
    QRegExp rx(pattern);
    //            static inline void list_add(struct list_head *new, struct list_head *head)
    if(string.contains(rx))
    {
        int pos = string.indexOf(rx);              // 0, position of the first match.
        // Returns -1 if str is not found.
        // You can also use rx.indexIn(str);
        qDebug() << pos;
        if ( pos >= 0 )
        {
            qDebug() << rx.matchedLength();     // 5, length of the last matched string
            // or -1 if there was no match
            qDebug() << rx.capturedTexts();     // QStringList("a=100", "a", "100"),
            //   0: text matching pattern
            //   1: text captured by the 1st ()
            //   2: text captured by the 2nd ()

            qDebug() << rx.cap(0);              // a=100, text matching pattern
            qDebug() << rx.cap(1);              // a, text captured by the nth ()
            qDebug() << rx.cap(2);              // 100,

            qDebug() << rx.pos(0);              // 0, position of the nth captured text
            qDebug() << rx.pos(1);              // 0
            qDebug() << rx.pos(2);              // 2
//            QString para = rx.cap(0).replace("(","").replace(")","");
            return rx.cap(0).replace("(","").replace(")","");
        }
    }

    return "";
}
/*============================================
* FuncName    : CodeSophia::Proc_C_Note_IsSpecialSign
* Description : 判断数据是否为特殊字符，如果为特殊，则不处理
* @str        : 原字符
* @funcname   : 函数名
* @funcpara   : 函数参数
* Author      :
* Time        : 2017-05-28
============================================*/
bool CodeSophia::Proc_C_Note_IsSpecialSign(QString &str,QString &funcname,QString &funcpara)
{
    if(str.contains(";")
            || str.contains("!")
            || str.contains("-")
            || str.contains(">")
            || str.contains("#")
//            || str.contains("=")
            || str.contains("<")
            || str.contains("/")
//            || str.contains(QRegExp("\\)[\\s]*[\\S]+"))
            || (str.contains(QRegExp("\\)[\\s]*[^{\\s]+")) && !str.contains("::"))
            || str.contains(QRegExp("return[\\s]+"))
            || str.contains(QRegExp("if[^\\w_]"))
            || funcname.contains("[")
            || funcname.contains("]")
            || funcname.contains(QRegExp("^[^\\w]+")) //开始为特殊字符
            )
        return true;
    return false;
}

/*============================================
* FuncName    : CodeSophia::Proc_C_Note
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::Proc_C_Note(QStringList &lst)
{
    QString result;
    result.clear();

    QString leftsign;
    QString rightsign;
    QString header;
    QString xinghaosign;
    bool hassplit = true;
    bool ignorelst = false;
    bool hasenter = true;

    bool hasdouhao = false;
    bool firstcome = false;
    bool isendflag = false;
    QString bindedstr("");
    QString bindedstrlast("");
    QString allbindedstrlast("");//所有数据显示在一个文本里了
///add tip
    QProgressBar *tipProgress = NULL;
    quint32 currentnum = 0;
    quint32 funcnum = 0;
    quint32 totalnum = lst.size();

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
        SetTextEditResult(result);
        return;
        break;
    case 1:
        break;
    case 2:
        hassplit =false;
        header = "";
        leftsign = "/*  \n";
        xinghaosign = doublexinghao;
        rightsign = "*/";
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
    case 4:
        hassplit =true;
        header = "";
        leftsign = "/*  ";
        rightsign = "  */";
        hasenter = false;
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
    case 5: //        function para
        hassplit =false;
        header = "/*============================================";
        leftsign = "";
        rightsign = "";
        hasenter = false;
        if(lst.isEmpty())
        {
            ShowTipsInfo("eg:: static inline void list_add(struct list_head *new, struct list_head *head)");
        }

        if(lst.size() > 2000){
            tipProgress = new QProgressBar();
            tipProgress->setRange(1,totalnum);
        }

        foreach (QString string, lst) {
            if(string.isEmpty())
            {
                allbindedstrlast += enter;
                continue;
            }
            QString orgstring = string;

            if(tipProgress)
            {
                ++currentnum;
                tipProgress->setValue(currentnum);
                tipProgress->show();
            }

            if(hasdouhao && !string.contains(")"))
            {
                bindedstrlast +=string + enter;
                bindedstr +=string;
                continue;
            }
            else if(hasdouhao && string.contains(")"))
            {
                bindedstr +=string;
                bindedstrlast +=string;
                string = bindedstr;
                isendflag = true;

            }

            string = string.simplified();

            QString funcname = Proc_Note_GetFuncName(string);
            QString funcpara = Proc_Note_GetFuncPara(string);
            quint32 funcsize = Proc_Note_GetFuncNameSize(string);
            QStringList paralist = funcpara.split(",");
            QString lastfuncname;
            lastfuncname.clear();
            QStringList lastparalst;
            lastparalst.clear();
            QStringList tmplastparalst;
            tmplastparalst.clear();
            qDebug() << "funcname :" << funcname;
            qDebug() << "funcpara :" << funcpara;
            qDebug() << "funcsize :" << funcsize;
            qDebug() << "string.simplified().split(\"(\").size() :" << string.simplified().split("(").size();
            qDebug() << "Proc_C_Note_IsSpecialSign(string) :" << Proc_C_Note_IsSpecialSign(string, funcname, funcpara);

            if(!funcname.isEmpty()
                    && (string.simplified().split("(").size() == 2)
                    && funcsize > 1 &&
                    !Proc_C_Note_IsSpecialSign(string, funcname, funcpara))
            {
//                if(string.endsWith(",") && !firstcome)
                if(string.contains("(") && !string.contains(")") && !firstcome)
                {
                    hasdouhao = true;
                    firstcome = true;
                    bindedstr +=string;
                    bindedstrlast +=string + enter;
                    continue;
                }
                firstcome = false;
                //proc multi function line
                hasdouhao = false;
                bindedstr = "";

                lastfuncname = funcname.simplified().split(" ").last().replace("*","").replace("&","");
                qDebug() << "funcname :" << lastfuncname;
                //寻找最大长度
                bool continueflag = false;
                continueflag = false;
                quint32 maxlen = 0;
                foreach (QString str, paralist) {
                    QString tmpstr = str.simplified().split(" ").last().replace("*","").replace("&","");
                    if(tmpstr.length() > maxlen)
                        maxlen = tmpstr.length();

                }
//                if(continueflag)
//                    continue;
                foreach (QString str, paralist) {
                    QString tmpstr = str.simplified().split(" ").last().replace("*","").replace("&","");
                    quint32 tmplen = tmpstr.length();
                    QStringList aftersplitlst = str.simplified().split(" ");
                    qDebug() << "string :" << str << ", aftersplitlst size:" << aftersplitlst.size();
                    if(!str.simplified().isEmpty() && (aftersplitlst.size() == 1 /*|| aftersplitlst.size() > 3*/))
                    {
                        continueflag = true;
                    }
                    else
                    {
                        continueflag = false;
                    }

                    if(!continueflag)
                    {
                        tmpstr = QString("%1%2").arg(tmpstr).arg(" ", maxlen - tmplen > 0 ? (maxlen - tmplen) : 0 );
                    }
                    else
                    {
                        tmpstr = QString("--      ");
                    }
                    tmplastparalst << tmpstr;
                    qDebug() << "funcpara :" << tmpstr;

                }
//                qDebug() << "right function parameter"<< endl;
            }
            else
            {
                if(firstcome)
                    allbindedstrlast += bindedstrlast + enter;
                else
                    allbindedstrlast += orgstring + enter;
                firstcome = false;
                isendflag = false;
                hasdouhao = false;
                bindedstr ="";
                bindedstrlast = "";
                continue;
            }

            //寻找最大长度
            quint32 maxlen = 0;
            maxlen = QString("* FuncName    ").length();

            foreach (QString tmpstr, tmplastparalst) {
                if(tmpstr.length() > maxlen)
                    maxlen = tmpstr.length();

            }
            foreach (QString str, tmplastparalst) {
                quint32 tmplen = str.length();
                if(tmplen + 3 < maxlen)
                {
                    str = QString("* @%1%2").arg(str).arg(" ", maxlen - tmplen - 3);
                }
                else
                {
                    str = QString("* @%1%2").arg(str).arg(" ", 0);
                }
                lastparalst << str;

            }

            funcnum++;
            ui->statusbar->showMessage(QString::fromLocal8Bit("函数个数为:%1").arg(funcnum));
            if(ui->checkBox_showFunc->isChecked()) //显示函数名
            {
                if(isendflag)
                {
                    result += bindedstrlast + semisign + enter;
                    allbindedstrlast += bindedstrlast + semisign + enter;
                    isendflag = false;
                    hasdouhao = false;
                    bindedstr ="";
                    bindedstrlast = "";

                }
                else
                {
                    result += string + semisign + enter;
                    allbindedstrlast += string + semisign + enter;
                }
                continue;
            }

            /* 添加注释项 */
            QString allfuncnote = "";
            allfuncnote += QString("") + header + enter;
            allfuncnote += QString("") + "* FuncName    : " + lastfuncname + enter;
            allfuncnote += QString("") + "* Description : " + enter;
            foreach (QString para, lastparalst) {
                allfuncnote += QString("")  + para +    ": "  + enter;
            }
            allfuncnote += QString("") + "* Author      : " + enter;
            QDateTime time;
            allfuncnote += QString("") + "* Time        : " + time.currentDateTime().toString("yyyy-MM-dd") + enter;
            allfuncnote += QString("") + "============================================*/" + enter;

            result += allfuncnote;
            allbindedstrlast += allfuncnote;

            if(isendflag)
            {
                if(ui->checkBox_showAllText->isChecked())
                {
                    result += bindedstrlast + enter + enter + enter;
                    allbindedstrlast += bindedstrlast + enter ;
                }
                else
                {
                    result += bindedstrlast + semisign + enter + enter + enter;
                    allbindedstrlast += bindedstrlast  + semisign + enter ;
                }

                isendflag = false;
                hasdouhao = false;
                bindedstr ="";
                bindedstrlast = "";

            }
            else
            {
                if(ui->checkBox_showAllText->isChecked())
                {
                    result += string + enter + enter + enter;
                    allbindedstrlast += string + enter;

                }
                else
                {
                    result += string + semisign + enter + enter + enter;
                    allbindedstrlast += string + semisign + enter;
                }
            }
            qApp->processEvents();

        }


        if(tipProgress)
        {
            tipProgress->deleteLater();
        }
        if(ui->checkBox_showAllText->isChecked())
            SetTextEditResult(allbindedstrlast);
        else
            SetTextEditResult(result);

        return;

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
        else if(hasenter)
            result +=  xinghaosign + spacesign + string + "\n";
        else
            result +=  string ;


        //        qDebug() << "result str :" << result;
    }

    if(!hassplit)
        result +=  rightsign;





    SetTextEditResult(result);

}

/*============================================
* FuncName    : CodeSophia::Proc_C_Function_SetGet
* Description :
* @lst        :
* @Local      :
* Author      :
* Time        : 2017-05-28
============================================*/
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

typedef struct T_FuncUse
{
    QString string;
    bool hasreturn;
}T_FuncUse;

/*============================================
* FuncName    : CodeSophia::Proc_C_Function_GenFunc
* Description :
* @lst        :
* @Local      :
* Author      :
* Time        : 2017-05-28
============================================*/
QString CodeSophia::Proc_C_Function_GenFunc(QStringList &lst, bool Local)
{
    QList<T_FuncUse> resultlst;
    QString result;
    QString type;
    QString name;
    QString funcname;
    QString parainfo;
    bool hasreturn = false;
    QString staticFlag;
    quint32 loop1 = 0;
    quint32 loop2 = 0;
    quint8 invalid = false;
    T_FuncUse tfunc = {0};
    if(Local)
        staticFlag = "static ";

    if(lst.size())
        result += "/* func_declare_begin */" + enter;
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

        if(!type.simplified().contains("void", Qt::CaseInsensitive))
        {
            hasreturn = true;
        }
        else
        {
            hasreturn = false;
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
        tfunc.string = staticFlag + type + spacesign +funcname + parainfo  + enter;
        tfunc.hasreturn = hasreturn;
        resultlst <<  tfunc;

    }

    if(lst.size())
        result += "/* func_declare_end */" + enter;

    result += enter;
    result += enter;
    if(lst.size())
        result += "/* func_implement */" + enter;

    foreach (T_FuncUse resl, resultlst) {
        result += resl.string;
        result += "{";
        result += enter;
        result += enter;
        if(resl.hasreturn)
        {
            result += tabsign + "return 0;";
        }
        result += enter;
        result += "}";
        result += enter;
        result += enter;

    }

    return result;
}

/*============================================
* FuncName    : CodeSophia::Proc_C_Function
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::Proc_C_Function(QStringList &lst)
{
    QString result;
    result.clear();

    QString header;
    QString end;
    //    QRegExp regExp("\{([^{]*?)\}");
    QRegExp regExp("\s*#include.*");
    QString veryimport_note = QString::fromLocal8Bit("  /* 长度判断非常重要 */");


    /*  make unittest test begin  */

    QString leftsign;
    QString rightsign;
    QString xinghaosign;
    bool hassplit = true;
    bool ignorelst = false;
    bool hasenter = true;

    bool hasdouhao = false;
    bool firstcome = false;
    bool isendflag = false;
    QString bindedstr("");
    QString bindedstrlast("");
    QString allbindedstrlast("");//所有数据显示在一个文本里了
///add tip
    QProgressBar *tipProgress = NULL;
    quint32 currentnum = 0;
    quint32 funcnum = 0;
    quint32 totalnum = lst.size();
    /*  make unittest test end  */



    quint32 index = ui->comboBox_keytips->currentIndex();
    qDebug() << "---index "<< index;
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
    case 5:
        header = "extern \"C\" ";
        foreach(QString string, lst) {
            string = string.simplified();
            if(string.contains(semisign))
                result += header + string + enter;
            else
                result += header + string + semisign + enter;
        }
        break;
    case 6: //提取函数
        header = "";
#if 0
        foreach(QString string, lst) {
            //            string = string.simplified();
            string.replace(QRegExp("(.*#define.*)"),"");
            string.replace(QRegExp("(.*#include.*)"),"");
//            string.replace(QRegExp("(\{([^{]*?)\})"),"");
            string.replace(QRegExp("\\{"),"");
            if(string.simplified().isEmpty())
                continue;
            result += string + enter;
        }

        ui->textEdit_result->setText(result);
#endif
        foreach(QString string, lst) {
            result += string + enter;
        }
        //            string = string.simplified();
        result.replace(QRegExp("(.*#define.*)"),"");
        result.replace(QRegExp("(.*#include.*)"),"");
//            string.replace(QRegExp("(\{([^{]*?)\})"),"");
        result.replace(QRegExp("\\{"),"");
//        if(string.simplified().isEmpty())
//            continue;
        ui->textEdit_result->setText(result);
//        LogWriteFile(ui->textEdit_key->toPlainText());

        //        ShellExecuteA(NULL,"open", exepath,filepath,NULL,SW_SHOWNORMAL);

        ////        ShellExecuteA(NULL,"open", exepath,filepath,NULL,SW_SHOWMINIMIZED);

        //        //python.exe
        ////        LPCSTR exepath = "python.exe";
        ////        ShellExecuteA(NULL,"open", exepath,NULL,NULL,SW_SHOWNORMAL);
        //        //python.exe
        ////        QFile *pFile = NULL;
        ////        pFile = new QFile(filepath);
        ////        if(!pFile->exists())
        ////        {
        ////            pFile->open(QIODevice::WriteOnly);
        ////            pFile->write(headerPython.toLocal8Bit().data());
        ////        }
        ////        pFile->close();
        //        //notepad++.exe打开此文件
        ////        LPCSTR notepadexe = "notepad++.exe";
        ////        ShellExecuteA(NULL,"open", notepadexe,filepath,NULL,SW_SHOWNORMAL);

        ////        Sleep(3000);
        ////        pythonexecfilename
        //        result = LogReadFile(logreadfilename);
        //        LogDeleteFile(logreadfilename);



        //        if(!regExp.exactMatch(result))
        //        {
        ////            QMessageBox::warning(NULL, tr("提示"), tr("ip地址错误"),NULL,NULL);
        //            ShowTipsInfo(tr("无匹配函数"));
        //            return;
        //        }

        //        p = re.compile(r'.*[\w&*]+\s+(?P<functionname>([\w:*&~]+))\((?P<paramterlist>([\s\S]*?))\)')
        //        astr = re.sub(r'\s*\/\/.*','', text) ##del //
        //        astr = re.sub(r'\s*\/\*([\s\S]*?)\*\/', '', astr) ##del /* * in multi line
        //        astr = re.sub(r'#ifdef\s+__cplusplus([\s\S]*?)#endif', '', astr)  ##del __cplusplus
        //        astr = re.sub(r'#if 0([\s\S]*?)#endif', '', astr)  ##del  #if 0 ~ #endif
        //        astr = re.sub(r'\s*#include.*', '', astr)  ##del #include
        //        astr = re.sub(r'"(.*)"', '',astr)  ##del "   "
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}
        //        astr = re.sub(r'\{([^{]*?)\}', '',astr)  ##del {}

        //        result = result.replace(QRegExp("\s*//[^\n]*"), "");
        //        result = result.replace(QRegExp("\s*\/\*([\s\S]*)\*\/"), "");
        //        result = result.replace(QRegExp("\s*/*([\s\S]*)*//"), "");
        //        result = result.replace(QRegExp("\s*/*([\s\S]*)*/"), "");
        //        result.replace();
        //        result = result.replace(QRegExp("#ifdef\s+__cplusplus([\s\S]*?)#endif"), "");
        //        result = result.replace(QRegExp("#if 0([\s\S]*?)#endif"), "");
        //        result = result.replace(QRegExp("\s*#include.*"), "");
        //        result = result.replace(QRegExp("\"(.*)\""), "");
        //        result = result.replace(QRegExp("\{([^{]*?)\}"), "");
        //        result = result.replace(QRegExp("\{([^{]*?)\}"), "");
        //        result = result.replace(QRegExp("\{([^{]*?)\}"), "");
        //        result = result.replace(QRegExp("\{([^{]*?)\}"), "");
        //        result = result.replace(QRegExp("\{([^{]*?)\}"), "");
        //        result = result.replace(QRegExp("\{([^{]*?)\}"), "");
        //        result = result.replace(QRegExp("\{([^{]*?)\}"), "");
        //        result = result.replace(QRegExp(""), "");
        //        result = result.replace(QRegExp(""), "");

        //        result = result.replace("{([^{]*?)}", "");
        //        result = result.replace("{([^{]*?)}", "");
        //        result = result.replace("{([^{]*?)}", "");
        //        result = result.replace("{([^{]*?)}", "");
        //        result = result.replace("{([^{]*?)}", "");
        //        result = result.replace("{([^{]*?)}", "");
        //        result = result.replace("{([^{]*?)}", "");
        //        result = result.replace("{([^{]*?)}", "");
        //        result += header + string + semisign + enter;
        return;
        break;
    case 7://malloc free
    {
        header = "malloc";
        end = "free";
        if(lst.size() == 0)
            ShowTipsInfo("char *p size");

        foreach (QString string, lst) {
            QStringList inlst = string.simplified().split(" ");
            if(inlst.size() < 3)
                continue;
            QString type = inlst.at(0).simplified().replace("*", "");
            QString name = inlst.at(1).simplified().replace("*", "");
            QString size = inlst.at(2).simplified();
            result += type + spacesign;
            result += "*" + name + spacesign + "=" + spacesign;
            result += "(" + type + " *" + ")" + header + "(" + size + ")" + semisign + enter;
            result += "para_checkpointer(" + name + ")" + semisign + enter;
            result += "para_checklen(" + QString("dwActLen > ") + size + ")" + semisign + veryimport_note + enter;
            result += enter;
            result += enter;
            result += enter;
            result += "if(" + name + ")" + enter;
            result += "{" + enter;
            result += tabsign + end +"(" + name +")" + semisign + enter;
            result += tabsign + name +" = NULL" + semisign + enter;
            result += "}" + enter;
            result += enter;
            result += enter;
            result += enter;


        }

    }
        break;
    case 8://new delete
    {
        header = "new";
        end = "delete";
        if(lst.size() == 0)
            ShowTipsInfo("char *p size");

        foreach (QString string, lst) {
            QStringList inlst = string.simplified().split(" ");
            if(inlst.size() < 3)
                continue;
            QString type = inlst.at(0).simplified().replace("*", "");
            QString name = inlst.at(1).simplified().replace("*", "");
            QString size = inlst.at(2).simplified();
            result += type + spacesign;
            result += "*" + name + spacesign + "=" + spacesign;
            result += /*"(" + type + " *" + ")" + */header + "(" + size + ")" + semisign + enter;
            result += "para_checkpointer(" + name + ")" + semisign + enter;
            result += "para_checklen(" + QString("dwActLen > ") + size + ")" + semisign + veryimport_note + enter;
            result += enter;
            result += enter;
            result += enter;
            result += "if(" + name + ")" + enter;
            result += "{" + enter;
            result += tabsign + end +"(" + name +")" + semisign + enter;
            result += tabsign + name +" = NULL" + semisign + enter;
            result += "}" + enter;
            result += enter;
            result += enter;
            result += enter;


        }

    }
        break;

    case 9: //"make unittest"
    {
        hassplit =false;
        header = "/*============================================";
        leftsign = "";
        rightsign = "";
        hasenter = false;
        if(lst.isEmpty())
        {
            ShowTipsInfo("eg:: static void ut_function(struct list_head *new, struct list_head *head)");
        }

        if(lst.size() > 2000){
            tipProgress = new QProgressBar();
            tipProgress->setRange(1,totalnum);
        }

        foreach (QString string, lst) {
            if(string.isEmpty())
            {
                allbindedstrlast += enter;
                continue;
            }
            QString orgstring = string;

            if(tipProgress)
            {
                ++currentnum;
                tipProgress->setValue(currentnum);
                tipProgress->show();
            }

            if(hasdouhao && !string.contains(")"))
            {
                bindedstrlast +=string + enter;
                bindedstr +=string;
                continue;
            }
            else if(hasdouhao && string.contains(")"))
            {
                bindedstr +=string;
                bindedstrlast +=string;
                string = bindedstr;
                isendflag = true;

            }

            string = string.simplified();

            QString funcname = Proc_Note_GetFuncName(string);
            QString funcpara = Proc_Note_GetFuncPara(string);
            quint32 funcsize = Proc_Note_GetFuncNameSize(string);
            QStringList paralist = funcpara.split(",");
            QString lastfuncname;
            lastfuncname.clear();
            QStringList lastparalst;
            lastparalst.clear();
            QStringList tmplastparalst;
            tmplastparalst.clear();
            qDebug() << "funcname :" << funcname;
            qDebug() << "funcpara :" << funcpara;
            qDebug() << "funcsize :" << funcsize;
            qDebug() << "string.simplified().split(\"(\").size() :" << string.simplified().split("(").size();
            qDebug() << "Proc_C_Note_IsSpecialSign(string) :" << Proc_C_Note_IsSpecialSign(string, funcname, funcpara);

            if(!funcname.isEmpty()
                    && (string.simplified().split("(").size() == 2)
                    && funcsize > 1 &&
                    !Proc_C_Note_IsSpecialSign(string, funcname, funcpara))
            {
//                if(string.endsWith(",") && !firstcome)
                if(string.contains("(") && !string.contains(")") && !firstcome)
                {
                    hasdouhao = true;
                    firstcome = true;
                    bindedstr +=string;
                    bindedstrlast +=string + enter;
                    continue;
                }
                firstcome = false;
                //proc multi function line
                hasdouhao = false;
                bindedstr = "";

                lastfuncname = funcname.simplified().split(" ").last().replace("*","").replace("&","");
                //add unit test function name begin,注释部分添加
                lastfuncname = "ut_" + lastfuncname;
                paralist.clear();
                //add unit test function name end
                qDebug() << "lastfuncname :" << lastfuncname;
                //寻找最大长度
                bool continueflag = false;
                continueflag = false;
                quint32 maxlen = 0;
                foreach (QString str, paralist) {
                    QString tmpstr = str.simplified().split(" ").last().replace("*","").replace("&","");
                    if(tmpstr.length() > maxlen)
                        maxlen = tmpstr.length();

                }
//                if(continueflag)
//                    continue;
                foreach (QString str, paralist) {
                    QString tmpstr = str.simplified().split(" ").last().replace("*","").replace("&","");
                    quint32 tmplen = tmpstr.length();
                    QStringList aftersplitlst = str.simplified().split(" ");
                    qDebug() << "string :" << str << ", aftersplitlst size:" << aftersplitlst.size();
                    if(!str.simplified().isEmpty() && (aftersplitlst.size() == 1 /*|| aftersplitlst.size() > 3*/))
                    {
                        continueflag = true;
                    }
                    else
                    {
                        continueflag = false;
                    }

                    if(!continueflag)
                    {
                        tmpstr = QString("%1%2").arg(tmpstr).arg(" ", maxlen - tmplen > 0 ? (maxlen - tmplen) : 0 );
                    }
                    else
                    {
                        tmpstr = QString("--      ");
                    }
                    tmplastparalst << tmpstr;
                    qDebug() << "funcpara :" << tmpstr;

                }
//                qDebug() << "right function parameter"<< endl;
            }
            else
            {
                if(firstcome)
                    allbindedstrlast += bindedstrlast + enter;
                else
                    allbindedstrlast += orgstring + enter;
                firstcome = false;
                isendflag = false;
                hasdouhao = false;
                bindedstr ="";
                bindedstrlast = "";
                continue;
            }

            //寻找最大长度
            quint32 maxlen = 0;
            maxlen = QString("* FuncName    ").length();

            foreach (QString tmpstr, tmplastparalst) {
                if(tmpstr.length() > maxlen)
                    maxlen = tmpstr.length();

            }
            foreach (QString str, tmplastparalst) {
                quint32 tmplen = str.length();
                if(tmplen + 3 < maxlen)
                {
                    str = QString("* @%1%2").arg(str).arg(" ", maxlen - tmplen - 3);
                }
                else
                {
                    str = QString("* @%1%2").arg(str).arg(" ", 0);
                }
                lastparalst << str;

            }

            funcnum++;
            ui->statusbar->showMessage(QString::fromLocal8Bit("函数个数为:%1").arg(funcnum));
            if(ui->checkBox_showFunc->isChecked()) //显示函数名
            {
                if(isendflag)
                {
                    result += bindedstrlast + semisign + enter;
                    allbindedstrlast += bindedstrlast + semisign + enter;
                    isendflag = false;
                    hasdouhao = false;
                    bindedstr ="";
                    bindedstrlast = "";

                }
                else
                {
                    result += string + semisign + enter;
                    allbindedstrlast += string + semisign + enter;
                }
                continue;
            }

            /* 添加注释项 */
            QString allfuncnote = "";
            allfuncnote += QString("") + header + enter;
            allfuncnote += QString("") + "* FuncName    : " + lastfuncname + enter;
            allfuncnote += QString("") + "* Description : " + enter;
            foreach (QString para, lastparalst) {
                allfuncnote += QString("")  + para +    ": "  + enter;
            }
            allfuncnote += QString("") + "* Author      : " + enter;
            QDateTime time;
            allfuncnote += QString("") + "* Time        : " + time.currentDateTime().toString("yyyy-MM-dd") + enter;
            allfuncnote += QString("") + "============================================*/" + enter;


            QString unitnote2("");
            QString unitnote4("");
            unitnote2 += QString("") + "void " + lastfuncname + "(void)" + enter;
            unitnote2 += QString("") + "{" + enter;
            unitnote2 += QString("") + tabsign + enter;
            unitnote4 += QString("") + tabsign + enter;
            unitnote4 += QString("") + "}" + enter;

            allfuncnote+=unitnote2;

            result += allfuncnote;
            allbindedstrlast += allfuncnote;

            if(isendflag)
            {
                result += tabsign + bindedstrlast + semisign  +  enter + unitnote4 + enter + enter;
                allbindedstrlast += tabsign + bindedstrlast + semisign  + enter + unitnote4 + enter ;
                isendflag = false;
                hasdouhao = false;
                bindedstr ="";
                bindedstrlast = "";

            }
            else
            {
                result += tabsign + string + semisign + enter + unitnote4 + enter;
                allbindedstrlast += tabsign + string + enter + unitnote4;
            }
            qApp->processEvents();

        }


        if(tipProgress)
        {
            tipProgress->deleteLater();
        }
        if(ui->checkBox_showAllText->isChecked())
            SetTextEditResult(allbindedstrlast);
        else
            SetTextEditResult(result);

        return;

    }
        break;
    default:
        return;
        break;
    }



    SetTextEditResult(result);

}

/*============================================
* FuncName    : CodeSophia::needqudizhi
* Description :
* @single     :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::needqudizhi(T_DataFormat &single)
{
    if(single.string.contains("*") || single.string.contains("[") ||single.string.contains("]"))
    {
        single.haveyihao = false;
    }
    else
    {
        single.haveyihao = true;
    }
}

/*============================================
* FuncName    : CodeSophia::FormatLst
* Description :
* @inlst      :
* @header     :
* @havedouhao  :
* @result     :
* @fmt        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::FormatLst(QStringList &inlst, QString &header,bool &havedouhao,
                           QString &result, QString fmt)
{
    QString format;
    format.clear();
    if(havedouhao)
        format += header /*+ leftkuohaosin*/ + yinhaomsign;
    else
        format += header + leftkuohaosin + yinhaomsign;
    foreach (QString f, inlst) {
//        format += QString("0x%-0%1x ").arg(f.length() >2 ? (f.length() - 2) : f.length());
        if(fmt.contains("0x", Qt::CaseInsensitive))
            format += QString(fmt).arg(f.length() >2 ? (f.length() - 2) : f.length());
        else
            format += QString(fmt).arg(f.length() >2 ? (f.length()) : f.length());

    }
    format += entersign + yinhaomsign;
    format += spacesign + douhaosign + enter + tabsign;
    foreach (QString f, inlst) {
        if(f == inlst.at(0))
            continue;
        format += douhaosign + spacesign;
    }

    format += rightkuohaosign  + semisign;
    result += format + enter;
    result += enter;
}



/*============================================
* FuncName    : CodeSophia::Proc_C_COMMONPRINT_LOOPS
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::Proc_C_COMMONPRINT_LOOPS(QStringList &lst)
{
    QString result;
    result.clear();

    QString header;
    bool havedouhao = false;



    QString lineEditText = ui->lineEdit_print->text().trimmed();

    if(!lineEditText.isEmpty() && lineEditText.contains(","))
    {
        header = ui->lineEdit_print->text().trimmed();
        havedouhao = true;
    }
    else if(!lineEditText.isEmpty())
        header = ui->lineEdit_print->text().trimmed();
    else
        header = "printf";


    foreach (QString str, lst) {
        if(havedouhao)
        {
            result += header + yinhaomsign;
        }
        else
        {
            result += header + leftkuohaosin + yinhaomsign;
        }
        result += str.simplified() ;
        result += entersign + yinhaomsign + rightkuohaosign + semisign;
        result += enter;
        QStringList inlst = str.simplified().split(" ");
        FormatLst(inlst, header, havedouhao,  result, "0x%-0%1x ");
        FormatLst(inlst, header, havedouhao,  result, "%-0%1u ");
        FormatLst(inlst, header, havedouhao,  result, "%-0%1llu ");
        FormatLst(inlst, header, havedouhao,  result, "%-0%1d ");
        FormatLst(inlst, header, havedouhao,  result, "%-0%1lld ");
        FormatLst(inlst, header, havedouhao,  result, "%-0%1f ");
        FormatLst(inlst, header, havedouhao,  result, "%-0%1p ");
        FormatLst(inlst, header, havedouhao,  result, "%-0%1s ");
//        FormatLst(inlst, header, havedouhao,  result, "%p ");

        result += enter;
        result += enter;
        result += enter;





    }

    int loop = 0;
//    qDebug("name addr shelf slot\n");
//    qDebug("name addr shelf slotjjjfdsafdsafdsaffffffffffffffffffasdfasdf\n");
//    qDebug("0x%-02x 0x%-02x 0x%-03x 0x%-043x \n");
//    qDebug("afdname fdfsdfdsfaddr xxshelf fdfefsfdsfdsfdsslot\n");
//    qDebug("0x%-05x 0x%-011x 0x%-05x 0x%-017x \n");
    qDebug("noafd nameafd addrfd shelfaa\n");
//    qDebug("0x%-03x 0x%-05x 0x%-04x 0x%-05x \n" , , , , );

//    qDebug("%-05u %-07u %-06u %-07u \n" , , , , );

//    qDebug("%-05llu %-07llu %-06llu %-07llu \n" , , , , );

//    qDebug("%-05d %-07d %-06d %-07d \n" , , , , );

//    qDebug("%-05lld %-07lld %-06lld %-07lld \n" , , , , );

//    qDebug("%-05f %-07f %-06f %-07f \n" , , , , );

//    qDebug("%-05p %-07p %-06p %-07p \n" , , , , );

//    qDebug("%p %p %p %p \n" , , , , );









    for( loop = 0 ; loop < 10; loop++ )
    {
        qDebug("0x%-03x 0x%-05x 0x%-04x 0x%-05x " ,
                loop, loop, loop, loop);
        qDebug("%-05u %-07u %-06u %-07u \n" ,
               loop, loop, loop, loop);

//        qDebug("%-05llu %-07llu %-06llu %-07llu \n" ,
//              loop, loop, loop, loop);

        qDebug("%-05d %-07d %-06d %-07d \n" ,
               loop, loop, loop, loop);

//        qDebug("%-05lld %-07lld %-06lld %-07lld \n" ,
//               loop, loop, loop, loop);

//        qDebug("%-05f %-07f %-06f %-07f \n" ,
//               loop, loop, loop, loop);

        qDebug("%-05p %-07p %-06p %-07p \n" ,
               loop, loop, loop, loop);

//        qDebug("%p %p %p %p \n" ,
//               loop, loop, loop, loop);

        char *pt = "this";
        qDebug("%-05s %-07s %-06s %-07s \n" ,
               pt, pt, pt, pt);
    }






    SetTextEditResult(result);
}


void CodeSophia::Proc_C_COMMONPRINT_ComboPrint(QStringList &lst)
{
    QString result;
    result.clear();
}


/*============================================
* FuncName    : CodeSophia::Proc_C_COMMONPRINT
* Description : deal with common print
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::Proc_C_COMMONPRINT(QStringList &lst)
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
    bool havedouhao = false;
    bool havecomboprint = false; //combobox print flag



    QString lineEditText = ui->lineEdit_print->text().trimmed();

    if(!lineEditText.isEmpty() && lineEditText.contains(","))
    {
        header = ui->lineEdit_print->text().trimmed();
        havedouhao = true;
    }
    else if(!lineEditText.isEmpty())
        header = ui->lineEdit_print->text().trimmed();
    else if(!ui->comboBox_print->currentText().isEmpty())
    {
        header = ui->comboBox_print->currentText().trimmed();
        havecomboprint = true;
    }
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
    case 0://var print

        format = "%u   ";
        break;
    case 1:
        format = "%lu  ";
        break;
    case 2://loops print
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
    case 16:
        format = "%p   ";
        //    << "print %-08x"
        break;
        break;
    case 17: //loops
        format = "%p   ";
        //    << "print %-08x"
        if(lst.size() == 0)
            ShowTipsInfo("name addr shelf slot");
        Proc_C_COMMONPRINT_LOOPS(lst);
        return;
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

//    if(!ui->comboBox_print->currentText().isEmpty()) //comboxprint不为空时，处理其它内容
//    {

//        Proc_C_COMMONPRINT_ComboPrint(lst);
//        return;
//    }


//    foreach (QString string, lst) {
//        string = string.simplified();
//        if (string.contains("struct") && string.contains("{"))
//        {
//            structname = string.split("struct").at(1).split("{").at(0).trimmed();
//        }
//        if (string.contains("}") && structname.isEmpty())
//        {
//            structname = string.split("}").at(1).split(";").at(0).trimmed();
//            qDebug() << "string split name:" << string.split("}").at(0);
//            qDebug() << "string split name:" << string.split("}").at(1);
//        }
//        if(!structname.isEmpty())
//        {
//            qDebug() << "strucgt name:" << structname;
//            break;
//        }
//    }

//    if(structname.isEmpty())
//        return;

//    if(havedouhao)
//    {
//        structinfo += header; /*  有逗号不加左括号  */
//    }
//    else
//    {
//        structinfo += header + leftkuohaosin;
//    }
//    structinfo += yinhaomsign;
//    structinfo += "Struct" + spacesign;
//    structinfo += structname;
//    structinfo += " info following ===== ";
//    structinfo += "[" + var_name + "]" ;
//    structinfo += entersign;
//    structinfo += yinhaomsign;
//    structinfo += rightkuohaosign;
//    structinfo += semisign;
//    structinfo += enter;


    //过滤元素
    foreach(QString string, lst) {
        string = string.simplified();
//        if(!string.contains(";"))
//            continue;
        if(string.length() >= 2)
        {
            if(string.left(1) == "*")
                continue;
            if(string.left(1) == "/" && string.left(2) == "/")
                continue;
            if(string.left(1) == "/" && string.left(2) == "*")
                continue;
        }

        if(string.contains("{") || string.contains("}"))
            continue;
        //        string = string.replace(QRegExp("(\\d+)"),"");
        //        string = string.replace(QRegExp("(\\s+);"),"");
        string.replace(QRegExp("(\\s+;)"),";");
        if(string.contains(","))
        {
            QStringList inlst = string.split(",");
//            string.replace(QRegExp(","),";");
            QString type = inlst.at(0).split(" ").at(0);
            QString inres;
            foreach (QString str, inlst) {
                if(str.split(" ").size() >=2)
                    inres +=type+ spacesign + str.split(" ").at(1);
                else
                    inres +=type + spacesign + str.split(" ").at(0);

                inres += semisign;

            }
            string = inres;
        }

        qDebug() << "commprint--> string:" << string;
        string.replace(QRegExp("\\s*=.*,"),";");
        string.replace(QRegExp("\\s*=.*;"),";");
        string = string.replace(QRegExp("(\\/\\*.*\\*\\/)"),""); //去除/* */
        string = string.replace(QRegExp("(\\/\\/)"),""); //去除//
        QStringList tmps = string.split(";");
        //        qDebug() << "; split size :" << tmps.size();
        //去除变量和;之间的空格
        //        foreach (QString s, tmps) {
        //            if(s != tmps.last())
        //                string += s;
        //        }
        string.clear();
        //分隔;号
        foreach (QString s, tmps) {
//            if(s != tmps.last())
                string += s;
        }
        //        qDebug() << "string filter :" << string;
        if(string.isEmpty())
            continue;
        if(string.contains("*") || (string.contains("[") && string.contains("]")))
        {
            single.format = format_p;
        }
        else
        {
            single.format = format;
        }

        if(string.contains("*"))
        {
            QString tmpstr2 = string;
            tmpstr2.replace(QRegExp("(\\[.*\\])"),"") ;
            if(tmpstr2.contains("*"))
                string.replace(QRegExp("(.*\\*)"),"");
        }

        //        string.replace("*", "");
//        tmps = string.split(",");
        tmps = string.split(";");
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
                    single.stringright = m_name.replace(QRegExp("(\\[.*\\])"),"") ;
                    needqudizhi(single);
                    m_nameLst << single;
                }
                else
                {
                    qDebug() << ", m_name split name:" <<s;
                    single.string = s.simplified();
                    single.stringright = s.simplified().replace(QRegExp("(\\[.*\\])"),"") ;
                    needqudizhi(single);
                    m_nameLst << single;
                }
            }
        }
        else if(havecomboprint && ui->comboBox_print->currentText().contains("msg"))
        {
            single.stringlst = string.split(" ");
            single.string.clear();
            single.stringright = m_name.simplified().replace(QRegExp("(\\[.*\\])"),"") ;
            needqudizhi(single);
            m_nameLst << single;

        }
        else
        {

            m_name = string.split(" ").last();
            single.string = m_name.simplified();
            single.stringright = m_name.simplified().replace(QRegExp("(\\[.*\\])"),"") ;
            needqudizhi(single);
            m_nameLst << single;
        }
    }

    //寻找最大长度
    quint32 maxlen = 0;
    quint32 maxlen2 = 0;
    foreach (T_DataFormat ele, m_nameLst) {
        if(ele.string.length() > maxlen)
            maxlen = ele.string.length();
        if(ele.stringright.length() > maxlen2)
            maxlen2 = ele.stringright.length();
    }

    foreach (T_DataFormat el, m_nameLst) {
        quint32 tmplen = el.string.length();
        quint32 tmplen2 = el.stringright.length();
        if(tmplen < maxlen)
        {
            el.string = QString("%1%2").arg(el.string).arg(" ", maxlen - tmplen);
        }
        if(tmplen2 < maxlen2)
        {
            el.stringright = QString("%1%2").arg(el.stringright).arg(" ", maxlen2 - tmplen2);
        }
        after_nameLst << el;
    }



    result += structinfo ;

    foreach (T_DataFormat el, after_nameLst) {
        result += header ;
        if(!havedouhao)
            result += leftkuohaosin ;
        if(el.string.isEmpty() && el.stringlst.size())
        {
            var_name = el.stringlst.last();
            int seq = 0;
            foreach (QString tmpstr, el.stringlst) {
                if( seq == 0)
                    result += "&" +tmpstr + douhaosign + " sizeof(" + tmpstr + ")" + douhaosign ;
                else if(seq == el.stringlst.size()  - 1)
                    break;
                else
                    result += tmpstr + douhaosign;

                seq++;
            }
            result += yinhaomsign ;
            result += var_name ;
        }else
        {
            result += yinhaomsign ;
            result += el.string ;
        }
        result += spacesign ;
        result += maohaosign ;
        result += tabsign ;
        result += el.format ;
        if(havecomboprint)
        {

        }
        else
        {
            result += entersign ;
        }
        result += yinhaomsign ;
        result += douhaosign ;
        result += spacesign ;
        if(el.haveyihao && (16 == index ))
        {

            result += qudizhisign;
            result += var_name ;
        }
        else
        {
            result += var_name ;
        }
        result += el.stringright;
        result += rightkuohaosign ;
        result += semisign ;
        result += enter ;
    }


    SetTextEditResult(result);

}

/*============================================
* FuncName    : CodeSophia::Proc_C_StructPrint
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
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
    bool havedouhao = false;



    QString lineEditText = ui->lineEdit_print->text().trimmed();

    if(!lineEditText.isEmpty() && lineEditText.contains(","))
    {
        header = ui->lineEdit_print->text().trimmed();
        havedouhao = true;
    }
    else if(!lineEditText.isEmpty())
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
    case 16:
        format = "%p   ";
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

    if(havedouhao)
    {
        structinfo += header; /*  有逗号不加左括号  */
    }
    else
    {
        structinfo += header + leftkuohaosin;
    }
    structinfo += yinhaomsign;
    structinfo += "Struct" + spacesign;
    structinfo += structname;
    structinfo += " info following ===== ";
    structinfo += "[" + var_name + "]" ;
    structinfo += entersign;
    structinfo += yinhaomsign;
    structinfo += rightkuohaosign;
    structinfo += semisign;
    structinfo += enter;


    //过滤元素
    foreach(QString string, lst) {
        string = string.simplified();
        if(!string.contains(";"))
            continue;
        if(string.length() < 2)
            continue;
        if(string.left(1) == "*")
            continue;
        if(string.left(1) == "/" && string.left(2) == "/")
            continue;
        if(string.left(1) == "/" && string.left(2) == "*")
            continue;
        if(string.contains("{") || string.contains("}"))
            continue;
        //        string = string.replace(QRegExp("(\\d+)"),"");
        //        string = string.replace(QRegExp("(\\s+);"),"");
        string.replace(QRegExp("(\\s+;)"),";");
        string = string.replace(QRegExp("(\\/\\*.*\\*\\/)"),""); //去除/* */
        string = string.replace(QRegExp("(\\/\\/)"),""); //去除//
        QStringList tmps = string.split(";");
        //        qDebug() << "; split size :" << tmps.size();
        //去除变量和;之间的空格
        //        foreach (QString s, tmps) {
        //            if(s != tmps.last())
        //                string += s;
        //        }
        string.clear();
        //分隔;号
        foreach (QString s, tmps) {
            if(s != tmps.last())
                string += s;
        }
        //        qDebug() << "string filter :" << string;
        if(string.isEmpty())
            continue;
        if(string.contains("*") || (string.contains("[") && string.contains("]")))
        {
            single.format = format_p;
        }
        else
        {
            single.format = format;
        }

        if(string.contains("*"))
        {
            QString tmpstr2 = string;
            tmpstr2.replace(QRegExp("(\\[.*\\])"),"") ;
            if(tmpstr2.contains("*"))
                string.replace(QRegExp("(.*\\*)"),"");
        }

        //        string.replace("*", "");
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
                    single.stringright = m_name.replace(QRegExp("(\\[.*\\])"),"") ;
                    needqudizhi(single);
                    m_nameLst << single;
                }
                else
                {
                    qDebug() << ", m_name split name:" <<s;
                    single.string = s.simplified();
                    single.stringright = s.simplified().replace(QRegExp("(\\[.*\\])"),"") ;
                    needqudizhi(single);
                    m_nameLst << single;
                }
            }
        }
        else
        {

            m_name = string.split(" ").last();
            single.string = m_name.simplified();
            single.stringright = m_name.simplified().replace(QRegExp("(\\[.*\\])"),"") ;
            needqudizhi(single);
            m_nameLst << single;
        }
    }

    //寻找最大长度
    quint32 maxlen = 0;
    quint32 maxlen2 = 0;
    foreach (T_DataFormat ele, m_nameLst) {
        if(ele.string.length() > maxlen)
            maxlen = ele.string.length();
        if(ele.stringright.length() > maxlen2)
            maxlen2 = ele.stringright.length();
    }

    foreach (T_DataFormat el, m_nameLst) {
        quint32 tmplen = el.string.length();
        quint32 tmplen2 = el.stringright.length();
        if(tmplen < maxlen)
        {
            el.string = QString("%1%2").arg(el.string).arg(" ", maxlen - tmplen);
        }
        if(tmplen2 < maxlen2)
        {
            el.stringright = QString("%1%2").arg(el.stringright).arg(" ", maxlen2 - tmplen2);
        }
        after_nameLst << el;
    }



    result += structinfo ;

    foreach (T_DataFormat el, after_nameLst) {
        result += header ;
        if(!havedouhao)
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
        if(el.haveyihao && (16 == index ))
        {

            result += qudizhisign;
            result += var_name ;
        }
        else
        {
            result += var_name ;
        }
        result += el.stringright;
        result += rightkuohaosign ;
        result += semisign ;
        result += enter ;
    }


    SetTextEditResult(result);

}

/*============================================
* FuncName    : CodeSophia::Proc_C_Struct
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::Proc_C_Struct(QStringList &lst)
{
    QString result;
    QString header;
    QString structname;
    T_StructFormat data;
    QList<T_StructFormat> elementlst;
    QList<T_StructFormat> elementlastlst;
    QString prefix_T = "T_";
    QString prefix_P = "*P_";
    QString notes = "/*    */";

    header = "typedef struct " + prefix_T;


    quint16 ulp = 0;
    foreach (QString string, lst) {
        ulp++;
        string = string.simplified();
        if(ulp == 1)
        {
            structname = string;
            continue;
        }
        if(string.isEmpty())
            continue;

        if(string.split(";").size() > 1)
        {
            data.string = string.split(";").at(0).simplified();
            data.note = string.split(";").at(1).simplified();
        }
        else
        {
            string.replace(";", "");
            string.replace(":", "");
            data.string = string;
        }
        elementlst.push_back(data);
    }

    //寻找最大长度
    quint32 maxlenstr = 0;
    foreach (T_StructFormat data, elementlst) {
        if(data.string.length() > maxlenstr)
            maxlenstr = data.string.length();
    }
    quint32 maxlennote = 0;
    foreach (T_StructFormat data, elementlst) {
        if(data.note.length() > maxlennote)
            maxlennote = data.note.length();
    }

    foreach (T_StructFormat data, elementlst) {
        quint32 tmplen = data.string.length();
        if(tmplen < maxlenstr)
        {
            data.string = QString("%1%2").arg(data.string).arg(" ", maxlenstr - tmplen);
        }
        quint32 tmplennote = data.note.length();
        if(tmplennote < maxlennote)
        {
            data.note = QString("%1%2").arg(data.note).arg(" ", maxlennote - tmplennote);
        }

        elementlastlst << data;
    }

    result = header + structname + leftbigkuohaosin + enter;
    foreach (T_StructFormat data2, elementlastlst) {
        result += tabsign + data2.string + semisign + tabsign + "/* " + data2.note  + " */" + enter;
    }
    result += rightbigkuohaosign + prefix_T + structname +
            douhaosign + spacesign + prefix_P + structname + semisign + enter;


    SetTextEditResult(result);

}

/*============================================
* FuncName    : CodeSophia::Proc_C_Define
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : CodeSophia::Proc_C_IFCONDITION
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
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

        if(first.contains("switch") && (lst.size() > 1))
        {

            result += first + leftkuohaosin + spacesign + string + spacesign + rightkuohaosign;
            result += enter;
            result += m_lsign;
            result += enter;
            uint8_t cnt = 0;
            foreach (QString instr, lst) {
                cnt ++;
                if(cnt == 1 )
                {
                    continue;
                }
                result += "case " + instr.simplified().replace(";","") + ":" + enter + enter  + tabsign + breaktips + enter;
            }
            result += "default:" + enter + enter + tabsign + breaktips + enter;
            result += m_rsign;
            result += enter;

            break;
        }else if(first.contains("switch")){
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

/*============================================
* FuncName    : CodeSophia::Proc_C_LOOP
* Description :
* @lst        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::Proc_C_LOOP(QStringList &lst)
{
    QString result;
    result.clear();

    QString first;
    QString middle;
    QString middle2;
    QString end;
    QString m_lsign;
    QString m_rsign;
    QString sign_con;//condition
    QString sign_sizhe; //si ze yunsuan
    QList<T_DefineInfo> dealafter;
    QList<T_DefineInfo> after_nameLst;
    T_DefineInfo tDefInfo;


    quint32 index = ui->comboBox_keytips->currentIndex();
    switch(index)
    {
    case 0://for ++
        if(lst.size() == 0)
            ShowTipsInfo("int i = 0; 100");
        first = "for";
        middle = "";
        sign_con = "<";
        sign_sizhe = "++";
        end = enter;
        m_lsign = "{";
        m_rsign = "}";
        break;
    case 1://for --
        if(lst.size() == 0)
            ShowTipsInfo("int i = 100; 0");
        first = "for";
        middle = "";
        sign_con = ">";
        sign_sizhe = "--";
        end = enter;
        m_lsign = "{";
        m_rsign = "}";
        break;
    case 2:
        if(lst.size() == 0)
            ShowTipsInfo("conditons");
        first = "while";
        middle = "";
        end = enter;
        m_lsign = "{";
        m_rsign = "}";
        break;
    case 3:
        first = "do";
        middle = "while(0)";
        end = enter;
        m_lsign = "{";
        m_rsign = "}";
        break;
    case 4:
        first = "do";
        middle = "while(1)";
        end = enter;
        m_lsign = "{";
        m_rsign = "}";
        break;

    default:
        return;
        break;
    }

    if(first.isEmpty())
        return;


    foreach (QString string, lst) {
        string = string.simplified();
        string.replace("="," = ");
        QStringList tmplst = string.split(";");
        if(tmplst.size() < 2)
        {
            continue;
        }
        QString initval = tmplst.at(0).trimmed();
        QString initval2;
        QString conditionval = tmplst.at(1).trimmed();
        QStringList firstlst = initval.split(" ");
        if(firstlst.size() < 2)
        {
            continue;
        }
        QString firstval  = firstlst.at(1);
        firstlst.removeAt(0);

        foreach (QString str, firstlst) {
            initval2 += str + spacesign;
        }



        result += initval +  semisign + enter ;

        result += first ;
        result += leftkuohaosin + spacesign;
        result += initval2 + semisign + spacesign;
        result += firstval + spacesign + sign_con + spacesign+ conditionval+ semisign + spacesign;
        result += firstval + sign_sizhe;
        result += spacesign + rightkuohaosign ;
        result += enter;
        result += m_lsign + enter;
        result += enter;
        result += m_rsign + enter;

        result += enter;

    }

    foreach (QString string, lst) {
        if(!first.contains("while"))
            break;
        string = string.simplified();


        result += first ;
        result += leftkuohaosin + spacesign;
        result += string + spacesign;
        result += rightkuohaosign ;
        result += enter;
        result += m_lsign + enter;
        result += enter;
        result += m_rsign + enter;

        result += enter;

    }
    //do while(0)
    foreach (QString string, lst) {
        if(!(first.contains("do")) && !(first.contains("while(0)")))
            break;
        string = string.simplified();


        result = first ;
        result += leftkuohaosin + spacesign;
        result += string + spacesign;
        result += rightkuohaosign ;
        result += enter;
        result += m_lsign + enter;
        result += enter;
        result += m_rsign + middle;
        result +=  enter;

        result += enter;

    }
    //do while(1)
    foreach (QString string, lst) {
        if(!(first.contains("do")) && !(first.contains("while(1)")))
            break;
        string = string.simplified();


        result = first ;
        result += leftkuohaosin + spacesign;
        result += string + spacesign;
        result += rightkuohaosign ;
        result += enter;
        result += m_lsign + enter;
        result += enter;
        result += m_rsign + middle;
        result +=  enter;

        result += enter;

    }

    SetTextEditResult(result);

}

/*============================================
* FuncName    : CodeSophia::on_pushButton_leftclear_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_pushButton_leftclear_clicked()
{
    ui->textEdit_key->clear();
    ui->textEdit_key->setFocus();
    ui->statusbar->showMessage("");
}

/*============================================
* FuncName    : CodeSophia::on_pushButton_rightclear_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_pushButton_rightclear_clicked()
{
    ui->textEdit_result->clear();
}



/*============================================
* FuncName    : CodeSophia::FillStringList
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::FillStringList()
{
    //    QStringList StrLst_KEYC_HEADER;

    StrLst_KEYC_HEADER
            << "#include <>"
            << "#include \"\""
            << "#ifdef _cplusplus extern \"C\""
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
            << "make func"
            << "make func local"
            << "extern \"C\""
            << str_china(提取函数)
            << "malloc free"
            << "new delete"
            << "make unittest"
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
            << "print %p"
               ;
    //    QStringList StrLst_KEYC_LOOP;
    StrLst_KEYC_LOOP
            << "for ++"
            << "for --"
            << "while"
            << "do while(0)"
            << "do while(1)"
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
            << "/* */ multi line"
            << "//"
            << "/* */ single"
            << "function para"
               ;


    //普通打印
    StrLst_COMMONPRINT
            << StrLst_KEYC_STRUCTPRINT
            << "loops"
               ;
}

/*============================================
* FuncName    : CodeSophia::on_lineEdit_print_textChanged
* Description :
* @arg1       :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_lineEdit_print_textChanged(const QString &arg1)
{
    on_pushButton_gen_clicked();
}

/*============================================
* FuncName    : CodeSophia::on_lineEdit_dataprint_textChanged
* Description :
* @arg1       :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_lineEdit_dataprint_textChanged(const QString &arg1)
{
    on_pushButton_gen_clicked();
}

/*============================================
* FuncName    : CodeSophia::StructPrintMsgUIHide
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::StructPrintMsgUIHide()
{
    ui->label_dataprint->hide();
    ui->label_print->hide();
    ui->lineEdit_dataprint->hide();
    ui->lineEdit_print->hide();
}

/*============================================
* FuncName    : CodeSophia::StructPrintMsgUIShow
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::StructPrintMsgUIShow()
{
    ui->label_dataprint->show();
    ui->label_print->show();
    ui->lineEdit_dataprint->show();
    ui->lineEdit_print->show();
}




/************************************************/
/*函 数:LogInitLog                               */
/*入 参:无                                        */
/*出 参:无                                        */
/*返 回:无                                        */
/*功 能:保存log信息                                */
/*author :wxj                                    */
/*version:1.0                                    */
/*时 间:2015.4.25                                 */
/*************************************************/
/*============================================
* FuncName    : CodeSophia::LogInitLog
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::LogInitLog()
{
#ifdef SC_LOG  //将发送网络的数据有效信息进行保存
    //    QDate date;
    //    QTime time;
    //    logfilename = date.currentDate().toString("sclogyyyy-MM-dd");
    //    logfilename += time.currentTime().toString("_HH-mm-ss");

    plogFile = new QFile(logfilename);
    if(!plogFile)
    {
        qDebug() <<"Open file Err:" << logfilename;
        return ;
    }


    //    LogWriteFile("\nTcpWrite NetData Log!!\n==============>>>>Starting:\n");
    qDebug() <<"Log file:" << logfilename;
#endif

}


/************************************************/
/*函 数:LogWriteFile                              */
/*入 参:str-写入log的数据                           */
/*出 参:无                                        */
/*返 回:                                          */
/*功 能:将str写入文件                              */
/*author :wxj                                    */
/*version:1.0                                    */
/*时 间:2015.4.25                                 */
/*************************************************/
/*============================================
* FuncName    : CodeSophia::LogWriteFile
* Description :
* @str        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::LogWriteFile(QString str)
{
#ifdef SC_LOG
    if(plogFile)
    {
        plogFile->open(QIODevice::WriteOnly);
        //        plogFile->open(QIODevice::Append);
        plogFile->write(str.toLocal8Bit());
        plogFile->close();
    }
#endif
}
/*============================================
* FuncName    : CodeSophia::LogWriteFile
* Description :
* @filename   :
* @str        :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::LogWriteFile(QString filename, QString str)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.write(str.toLocal8Bit());
    file.close();
}


/*============================================
* FuncName    : CodeSophia::LogReadFile
* Description :
* @filename   :
* Author      :
* Time        : 2017-05-28
============================================*/
QString CodeSophia::LogReadFile(QString filename)
{
    QFile file(filename);

    if(file.exists())
    {
        QByteArray dataFromFile;
        file.open(QIODevice::ReadOnly);
        dataFromFile=file.readAll();
        file.close();
        return dataFromFile;
    }
    return "";
}

/*============================================
* FuncName    : CodeSophia::LogDeleteFile
* Description :
* @filename   :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::LogDeleteFile(QString filename)
{
    //    QFile file(filename);

    //    QFile::remove();
    qDebug() << " will delte file :" << filename;
    QFile::remove(filename.toLatin1().data());
    //    QFile::unlink(filename.toLatin1().data());

    //    if(file.exists())
    //    {
    ////        file.remove();
    //    }

}

/*============================================
* FuncName    : CodeSophia::get_funcname_qregexp
* Description :
* @text       :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::get_funcname_qregexp(QString &text)
{
    QStringList result = text.split("\n");
    text.clear();
    foreach (QString str, result) {
        text +=str.replace(QRegExp("(.*#define.*)"),"");
        text +=str.replace(QRegExp("(.*#include.*)"),"");
    }
//    text.replace(QRegExp("\\{([^{]*?)\\}"),"");

//    text.replace(QRegExp("(\\s*\\/\\/.*)"),"");
//    text.replace(QRegExp("(\\s*\\/\\*[\\s\\S]*?\\*\\/)"),"");


}

/*============================================
* FuncName    : CodeSophia::get_funcname_and_declare_qregexp
* Description :
* @text       :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::get_funcname_and_declare_qregexp(QString &text)
{
//    text.replace(QRegExp("\\{([^{]*?)\\}"),"");
//    text = text.replace("\n",""); //去除换行
    text = text.replace(QRegExp("\\{([^{]*?)\\}"),"");

}





/*============================================
* FuncName    : CodeSophia::on_pushButton_load_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_pushButton_load_clicked()
{
#if 0
    QString result;
    LPCSTR exepath = "python.exe";
    LPCSTR filepath = "";
    //python使用的版本必须是2.7的，如果是3.4以上的可能还不行。
    if(ui->checkBox_showFunc->isChecked())
    {
        filepath = pythonexecfilename;
        writepythonexecfuncfilename(pythonexecfilename);
    }
    else
    {
        filepath = pythonexecfilename_funcdeclare;
        writepythonexecfuncfilename_funcdeclare(pythonexecfilename_funcdeclare);
    }

    LogDeleteFile(logreadfilename);
    //    ShellExecuteA(NULL,"open", exepath,filepath,pythonexecdir,SW_SHOWNORMAL);
    ShellExecuteA(NULL,"open", exepath,filepath,pythonexecdir,SW_HIDE);
    //    Sleep(1000);
    while(1)
    {
        QFile file(logreadfilename);
        if(file.exists())
        {
            result = LogReadFile(logreadfilename);
            //            LogDeleteFile(logreadfilename);
            break;
        }
        Sleep(500);
        //        SetTextEditResult(result);

    }
    QStringList lst = result.split("\n");
    result = "";
    foreach (QString str, lst) {
        if(!str.simplified().length())
            continue;
        str = str.simplified();
        if(str.right(1) == ")" && !str.contains(";"))
            result += str + semisign + enter;
        else
        {
            result += str + enter;
        }
    }

    ui->textEdit_result->setText(result);

#else

    QString result = ui->textEdit_key->toPlainText();
    LPCSTR filepath = "";
    //python使用的版本必须是2.7的，如果是3.4以上的可能还不行。
    if(ui->checkBox_showFunc->isChecked())
    {
//        get_funcname_qregexp(result);
    }
    else
    {
        get_funcname_and_declare_qregexp(result);
    }

//    QStringList lst = result.split("\n");
//    result = "";
//    foreach (QString str, lst) {
//        if(!str.simplified().length())
//            continue;
//        str = str.simplified();
//        if(str.right(1) == ")" && !str.contains(";"))
//            result += str + semisign + enter;
//        else
//        {
//            result += str + enter;
//        }
//    }

    ui->textEdit_result->setText(result);

#endif
}

/*============================================
* FuncName    : CodeSophia::writepythonexecfuncfilename
* Description :
* @filename   :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::writepythonexecfuncfilename(QString filename)
{
    QString headerPython =
            QString("#!/usr/bin/env python") + "\n" +
            "# -*- coding: utf-8 -*-" + "\n"
            "##file function: 提取C CPP文件中的函数名，宏定义，结构体等" + "\n"
            "##filename:re.py" + "\n"
            "" + "\n"
            "import re" + "\n"
            "import sys" + "\n"
            "import os" + "\n"
            "import time" + "\n"
            "from time import ctime,sleep;" + "\n"
            "" + "\n"
            "" + "\n"
            "" + "\n"
            "tips = \"regular expression\"" + "\n"
            "print '-' * 30 + tips + '-' * 30" + "\n"
            "" + "\n"
            "print 'Number of arguments:', len(sys.argv)" + "\n"
            "print 'They are:', str(sys.argv)" + "\n"
            "if (len(sys.argv) > 1):" + "\n"
            "	filename=str(sys.argv[1])" + "\n"
            "	print 'filename is ' + filename" + "\n"
            "	" + "\n"
            "###open and read file" + "\n"
            "def openfilereturncontent(filename):" + "\n"
            "	# try:" + "\n"
            "	fh = open(filename, \"r\")" + "\n"
            "	filecontent = fh.read()" + "\n"
            "	fh.close()" + "\n"
            "	return filecontent" + "\n"
            "	# finally:" + "\n"
            "		# print \"Error:没有找到文件或读取文件失败\"" + "\n"
            "	" + "\n"
            "" + "\n"
            "def writefilewithcontent(filename, text):" + "\n"
            "	# try:" + "\n"
            "	print filename" + "\n"
            "	print os.path.dirname(filename)" + "\n"
            "	print os.path.exists(os.path.dirname(filename))" + "\n"
            "	if False == os.path.exists(os.path.dirname(filename)):" + "\n"
            "		os.makedirs(os.path.dirname(filename))" + "\n"
            "	fw = open(filename,\"w\")" + "\n"
            "	fw.write(text)" + "\n"
            "	fw.close()" + "\n"
            "	# finally:" + "\n"
            "		# print \"Error:没有找到文件或读取文件失败\"" + "\n"
            "	" + "\n"
            "def f2(m2):" + "\n"
            "	d = m2.groupdict()" + "\n"
            "	return d['functionname']" + "\n"
            "" + "\n"
            "###regular expression for header and source files" + "\n"
            "def regrexheader(text):" + "\n"
            "	p = re.compile(r'.*[\\w&*]+\\s+(?P<functionname>([\\w:*&~]+))\\((?P<paramterlist>([\\s\\S]*?))\\)')" + "\n"
            "	astr = re.sub(r'\\s*\\/\\/.*','', text) ##del //" + "\n"
            "	astr = re.sub(r'\\s*\\/\\*([\\s\\S]*?)\\*\\/', '', astr) ##del /* * in multi line" + "\n"
            "	astr = re.sub(r'#ifdef\\s+__cplusplus([\\s\\S]*?)#endif', '', astr)  ##del __cplusplus" + "\n"
            "	astr = re.sub(r'#if 0([\\s\\S]*?)#endif', '', astr)  ##del  #if 0 ~ #endif" + "\n"
            "	astr = re.sub(r'\\s*#include.*', '', astr)  ##del #include" + "\n"
            "	astr = re.sub(r'\"(.*)\"', '',astr)  ##del "   "" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}	" + "\n"
            "" + "\n"
            "	astr = p.sub(f2,astr)   ##提取函数名称   效率高" + "\n"
            "" + "\n"
            "	astr = re.sub(r'#pragma.*', '', astr)  ##del " + "\n"
            "	astr = re.sub(r'\\s*#endif', '', astr)  ##del " + "\n"
            "	astr = re.sub(r'\\s*#ifndef.*', '', astr)  ##del " + "\n"
            "	astr = re.sub(r'\\s*#ifdef.*', '', astr)  ##del " + "\n"
            "	astr = re.sub(r'\\s*#else', '', astr)  ##del " + "\n"
            "	# print astr" + "\n"
            "	return astr" + "\n"
            "	" + "\n"
            "def delete(filepath):" + "\n"
            "	f = open(filepath, 'a+')" + "\n"
            "	fnew = open(filepath + '.re.txt','wb')" + "\n"
            "	for line in f.readlines():" + "\n"
            "		data = line.strip()" + "\n"
            "		if len(data) != 0:" + "\n"
            "			fnew.write(data)" + "\n"
            "			fnew.write('\\n')" + "\n"
            "	f.close()" + "\n"
            "	fnew.close()" + "\n"
            "	" + "\n"
            "	" + "\n"
            "	" + "\n"
            "	" + "\n"
            "if __name__==\"__main__\":" + "\n"
            "	" + "\n"
            "	print '=' * 3 + \"begin\" + '=' * 3" + "\n"
            "	str=\"\"" + "\n"
            "	savedir=\"D:\\\\\"" + "\n"
            "	savefilename=\"files_time.re.log\"" + "\n"
            "	readfile=\"files_time.log\"" + "\n"
            "" + "\n"
            "	str = openfilereturncontent(savedir + readfile)" + "\n"
            "	print \"file[%s] content:\" % (savedir + readfile)" + "\n"
            "	savefilename = os.path.realpath(savefilename)	" + "\n"
            "	print \"savefilename[%s] content:\" % (savefilename)" + "\n"
            "	text = regrexheader(str)" + "\n"
            "	print '-' *100" + "\n"
            "	print \"deal result:\" + text" + "\n"
            "	" + "\n"
            "	writefilewithcontent(savefilename, text)" + "\n"
            "	" + "\n"
            "	# delete(savefilename)" + "\n"
            "	# os.remove(savefilename)" + "\n"
            "	# os.rename(savefilename +'.re.txt', savefilename)" + "\n"
            "	" + "\n"
            "	print '=' * 3 + \"end\" + '=' * 3" + "\n"
            "	# sleep(100)" + "\n"
            "	" + "\n"
            "	" + "\n"
            "	" + "\n";

    QFile file(filename);
    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        file.write(headerPython.toLocal8Bit().data());
        file.close();
    }

}

/*============================================
* FuncName    : CodeSophia::writepythonexecfuncfilename_funcdeclare
* Description :
* @filename   :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::writepythonexecfuncfilename_funcdeclare(QString filename)
{
    QString headerPython =
            QString("#!/usr/bin/env python") + "\n" +
            "# -*- coding: utf-8 -*-" + "\n"
            "##file function: 提取C CPP文件中的函数名，宏定义，结构体等" + "\n"
            "##filename:re.py" + "\n"
            "" + "\n"
            "import re" + "\n"
            "import sys" + "\n"
            "import os" + "\n"
            "import time" + "\n"
            "from time import ctime,sleep;" + "\n"
            "" + "\n"
            "" + "\n"
            "" + "\n"
            "tips = \"regular expression\"" + "\n"
            "print '-' * 30 + tips + '-' * 30" + "\n"
            "" + "\n"
            "print 'Number of arguments:', len(sys.argv)" + "\n"
            "print 'They are:', str(sys.argv)" + "\n"
            "if (len(sys.argv) > 1):" + "\n"
            "	filename=str(sys.argv[1])" + "\n"
            "	print 'filename is ' + filename" + "\n"
            "	" + "\n"
            "###open and read file" + "\n"
            "def openfilereturncontent(filename):" + "\n"
            "	# try:" + "\n"
            "	fh = open(filename, \"r\")" + "\n"
            "	filecontent = fh.read()" + "\n"
            "	fh.close()" + "\n"
            "	return filecontent" + "\n"
            "	# finally:" + "\n"
            "		# print \"Error:没有找到文件或读取文件失败\"" + "\n"
            "	" + "\n"
            "" + "\n"
            "def writefilewithcontent(filename, text):" + "\n"
            "	# try:" + "\n"
            "	print filename" + "\n"
            "	print os.path.dirname(filename)" + "\n"
            "	print os.path.exists(os.path.dirname(filename))" + "\n"
            "	if False == os.path.exists(os.path.dirname(filename)):" + "\n"
            "		os.makedirs(os.path.dirname(filename))" + "\n"
            "	fw = open(filename,\"w\")" + "\n"
            "	fw.write(text)" + "\n"
            "	fw.close()" + "\n"
            "	# finally:" + "\n"
            "		# print \"Error:没有找到文件或读取文件失败\"" + "\n"
            "	" + "\n"
            "def f2(m2):" + "\n"
            "	d = m2.groupdict()" + "\n"
            "	return d['functionname']" + "\n"
            "" + "\n"
            "###regular expression for header and source files" + "\n"
            "def regrexheader(text):" + "\n"
            "	p = re.compile(r'.*[\\w&*]+\\s+(?P<functionname>([\\w:*&~]+))\\((?P<paramterlist>([\\s\\S]*?))\\)')" + "\n"
            "	astr = re.sub(r'\\s*\\/\\/.*','', text) ##del //" + "\n"
            "	astr = re.sub(r'\\s*\\/\\*([\\s\\S]*?)\\*\\/', '', astr) ##del /* * in multi line" + "\n"
            "	astr = re.sub(r'#ifdef\\s+__cplusplus([\\s\\S]*?)#endif', '', astr)  ##del __cplusplus" + "\n"
            "	astr = re.sub(r'#if 0([\\s\\S]*?)#endif', '', astr)  ##del  #if 0 ~ #endif" + "\n"
            "	astr = re.sub(r'\\s*#include.*', '', astr)  ##del #include" + "\n"
            "	astr = re.sub(r'\"(.*)\"', '',astr)  ##del "   "" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}" + "\n"
            "	astr = re.sub(r'\\{([^{]*?)\\}', '',astr)  ##del {}	" + "\n"
            "" + "\n"
            "	astr = re.sub(r'#pragma.*', '', astr)  ##del " + "\n"
            "	astr = re.sub(r'\\s*#endif', '', astr)  ##del " + "\n"
            "	astr = re.sub(r'\\s*#ifndef.*', '', astr)  ##del " + "\n"
            "	astr = re.sub(r'\\s*#ifdef.*', '', astr)  ##del " + "\n"
            "	astr = re.sub(r'\\s*#else', '', astr)  ##del " + "\n"
            "	# print astr" + "\n"
            "	return astr" + "\n"
            "	" + "\n"
            "def delete(filepath):" + "\n"
            "	f = open(filepath, 'a+')" + "\n"
            "	fnew = open(filepath + '.re.txt','wb')" + "\n"
            "	for line in f.readlines():" + "\n"
            "		data = line.strip()" + "\n"
            "		if len(data) != 0:" + "\n"
            "			fnew.write(data)" + "\n"
            "			fnew.write('\\n')" + "\n"
            "	f.close()" + "\n"
            "	fnew.close()" + "\n"
            "	" + "\n"
            "	" + "\n"
            "	" + "\n"
            "	" + "\n"
            "if __name__==\"__main__\":" + "\n"
            "	" + "\n"
            "	print '=' * 3 + \"begin\" + '=' * 3" + "\n"
            "	str=\"\"" + "\n"
            "	savedir=\"D:\\\\\"" + "\n"
            "	savefilename=\"files_time.re.log\"" + "\n"
            "	readfile=\"files_time.log\"" + "\n"
            "" + "\n"
            "	str = openfilereturncontent(savedir + readfile)" + "\n"
            "	print \"file[%s] content:\" % (savedir + readfile)" + "\n"
            "	savefilename = os.path.realpath(savefilename)	" + "\n"
            "	print \"savefilename[%s] content:\" % (savefilename)" + "\n"
            "	text = regrexheader(str)" + "\n"
            "	print '-' *100" + "\n"
            "	print \"deal result:\" + text" + "\n"
            "	" + "\n"
            "	writefilewithcontent(savefilename, text)" + "\n"
            "	" + "\n"
            "	# delete(savefilename)" + "\n"
            "	# os.remove(savefilename)" + "\n"
            "	# os.rename(savefilename +'.re.txt', savefilename)" + "\n"
            "	" + "\n"
            "	print '=' * 3 + \"end\" + '=' * 3" + "\n"
            "	# sleep(100)" + "\n"
            "	" + "\n"
            "	" + "\n"
            "	" + "\n";

    QFile file(filename);
    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        file.write(headerPython.toLocal8Bit().data());
        file.close();
    }

}




/*============================================
* FuncName    : CodeSophia::on_checkBox_showFunc_toggled
* Description :
* @checked    :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_checkBox_showFunc_toggled(bool checked)
{
    on_pushButton_gen_clicked();
//    on_pushButton_load_clicked();

}

/*============================================
* FuncName    : CodeSophia::dragEnterEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }
}

/*============================================
* FuncName    : CodeSophia::dropEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::dropEvent(QDropEvent *event)
{
    urls = event->mimeData()->urls();
    filedraged(urls);
}


void CodeSophia::filedraged(QList<QUrl> &urls)
{
    if(urls.isEmpty()){
        return;
    }
    qDebug() << "-->>urls cout:" << urls.size();

//#if 0
//    QString fileName = urls.first().toLocalFile();
//    if(fileName.isEmpty()){
//        return;
//    }
//    this->setWindowTitle(fileName);
//    readTextFile(fileName);
//#else
//    QString oldTitle = windowTitle();
//    QString showTitle(oldTitle + "  ");
//    QString fileName("");
//    rightClear_textedit();//清空显示数据
//    foreach (QUrl url, urls)
//    {
//        fileName = url.toLocalFile();
//        showTitle += url.toLocalFile() + "  ";
//        if(fileName.isEmpty()){
//            continue;
//        }
//        this->setWindowTitle(showTitle);
//        qDebug() << "~~filename :" << fileName;

//        QFileInfo fileinfo(fileName);
//        if(fileinfo.isDir())
//        {
//            this->setWindowTitle(url.toLocalFile());
//            qDebug() << "~~is dir!!,filename :" << fileName;
////            QString msgstr(str_china(不能读取文件夹 %1.).arg(fileName));
////            ShowTipsInfoWithShowTime(msgstr, 1000);
//            //遍历文件夹
////            QDir dir(fileName);
//            zeropathFileNums();
//            zeroloopFileNums();
////            qDebug() << "文件夹["<< fileName << "]里文件数量为:" << iteratorDirectory_FileNums(fileName);
//            zeropathFileNums();
//            zeroloopFileNums();
//            quint32 IteratorNums = iteratorDirectory_FileNums(fileName);
//            ShowTipsInfoWithShowTime(QString("文件数量为:%2,文件夹[%1]").arg(fileName).arg(IteratorNums),
//                                     3000);
//            zeroloopFileNums();
//            textprogress_init();
//            key_escaple_pressed = false;
//            if(IteratorNums > 500)
//            {
//                if(QMessageBox::Yes == QMessageBox::warning(NULL, "warning", "文件数量较大，是否继续？", QMessageBox::Yes , QMessageBox::No))
//                {
//                    iteratorDirectory_Saveui(fileName);
//                }
//            }
//            else
//            {
//                iteratorDirectory_Saveui(fileName);
//            }
////
//            key_escaple_pressed = false;
//            zeropathFileNums();
//            zeroloopFileNums();

//        }
//        else
//        {
//            qDebug() << "~~ is file!!,filename :" << fileName;
//            readTextFileAppend(fileName);
//        }


//    }
//#endif
}



/*============================================
* FuncName    : CodeSophia::setChildUI
* Description :
* @s          :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::setChildUI(QString s)
{
    msgsametimer->start();
//    static QString oldData("");
    towritemsgs.clear();
    towritemsgs = s;
//    if(s != oldData)
//        ui->textEdit_key->setText(s);

//    oldData = s;
}


/*============================================
* FuncName    : CodeSophia::writemsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::writemsg()
{
    msgsametimer->stop();
    ui->textEdit_key->setText(towritemsgs);
}






/*============================================
* FuncName    : CodeSophia::on_pushButton_paster2left_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_pushButton_paster2left_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui->textEdit_key->setText(clipboard->text());
}

/*============================================
* FuncName    : CodeSophia::on_pushButton_fetchright_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_pushButton_fetchright_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->textEdit_result->toPlainText(),QClipboard::Clipboard);
}

/*============================================
* FuncName    : CodeSophia::on_checkBox_showAllText_toggled
* Description :
* @checked    :
* Author      :
* Time        : 2017-05-28
============================================*/
void CodeSophia::on_checkBox_showAllText_toggled(bool checked)
{
    on_pushButton_gen_clicked();
}

