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
    showtitle = title_org + splitsign + current_lan + splitsign + current_optype;\
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

    ActiveSets();
    ButtonSets();
    ComboBoxSets();




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
    connect(ui->action_save, SIGNAL(triggered()), this, SLOT(SaveMsg()));

}


void CodeSophia::ButtonSets()
{
    connect(ui->pushButton_gen,SIGNAL(clicked()), this, SLOT(on_pushButton_gen_clicked()));
}

void CodeSophia::ComboBoxSets()
{
    connect(ui->comboBox_keytips,SIGNAL(clicked()), this, SLOT(on_comboBox_keytips_clicked()));

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
    if(ui->textEdit_key->toPlainText().isEmpty())
    {
        ShowTipsInfo("Key TextEdit empty,please input something!!!");
        return;
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
    if(current_lan.isEmpty() || current_subtype.isEmpty())
        return;
    QStringList list;
    list.clear();

    switch((current_lan_num))
    {
    case KEY_C:
    {
        switch(( current_subtype_num))
        {
        case SUB_HEADER:
            list
                << "#include <>"
                << "#include \"\""
                ;
            break;
        case SUB_DECLARE:
            list << "extern C"
                    ;
            break;
        case SUB_DEFINE:
            list << "#ifdef "
                 << "#ifndef "
                 << "#ifdef  else "
                 << "#ifndef  else"
                 << "#if 0"
                 << "#if 1"
                ;
            break;
        case SUB_FUNCTION:
            list
                << "extern"
                    ;
            break;
        case SUB_STRUCT:
            list
                << "typedef struct "
                    ;
            break;
        case SUB_STRUCTPRINT:
            list
                << "print %u"
                << "print %d"
                << "print %s"
                << "print %#x"
                << "print %x"
                    ;
            break;
        case SUB_LOOP:
            list
                    << "for"
                    << "while"
                       ;
            break;
        case SUB_IFCONDITION:
            list
                    << "if"
                    << "if else "
                    << "if elseif else"
                       ;
            break;
        default:
            break;
        }

    }
        break;




    default:
        break;
    }

    ui->comboBox_keytips->clear();
    ui->comboBox_keytips->addItems(list);
}



