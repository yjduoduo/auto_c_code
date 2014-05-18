#include "autoccode.h"
#include "ui_autoccode.h"
#include "ui_PushDbdialog.h"
#include "ui_dialog_select_database.h"
#include <QtGui>
#include "prefix_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "version.h"
#include "gencodedatabase.h"
#include <iostream>





//编码汉字
#define str_china(A)     QString::fromLocal8Bit(#A)




autoCCode::autoCCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoCCode),
    ui_dialog(new Ui::Dialog),
    ui_dia_selectdb(new Ui::Dialog_select_database)
{
    ui->setupUi(this);
    InDb_Dialog = new QDialog(this);
    ui_dialog->setupUi(InDb_Dialog);

    dialog_selectdb =  new QDialog(this);;
    ui_dia_selectdb->setupUi(dialog_selectdb);

    pushButtonSet();
    textEditSet();
    comboBoxSet();
    listWidgetSet();
    addstr_comboBox();
}

void autoCCode::pushButtonSet(void)
{
    self_print(pushButtonSet);
    //btn list
    //ui
    QObject::connect(ui->close_btn,SIGNAL(clicked()),this,SLOT(close()));
    QObject::connect(this->ui->about_btn,SIGNAL(clicked()),
                     this,SLOT(aboutVersion()));
    //    QObject::connect(ui->save_btn,SIGNAL(clicked()),this,SLOT(on_save_btn_clicked()));
    //    QObject::connect(ui->tocode_btn,SIGNAL(clicked()),this,SLOT(on_tocode_btn_clicked()));
    //ui_dialog
    QObject::connect(this->ui_dialog->ok_btn_dia,SIGNAL(clicked()),
                     this,SLOT(on_ok_btn_dia_clicked()));
    QObject::connect(this->ui_dialog->cancel_btn_dia,SIGNAL(clicked()),
                     this,SLOT(on_cancel_btn_dia_clicked()));

    //右清空
    QObject::connect(this->ui->rightclear_btn,SIGNAL(clicked()),
                     this,SLOT(rightClear_textedit()));

}
void autoCCode::comboBoxSet(void)
{
    self_print(comboBoxSet);
    QObject::connect(this->ui_dia_selectdb->comboBox_selectdb,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(on_comboBox_selectdb_currentIndexChanged(QString)));
}


void autoCCode::on_comboBox_selectdb_currentIndexChanged(const QString &arg1)
{
    self_print(on_comboBox_selectdb_currentIndexChanged);
    str_print(arg1);
    selected_langtype = arg1;
    //str_print(selected_langtype);
    LanguageType langtype = getLanguageType(selected_langtype);
    codestructSets* sets = get_table_sets_bytype(langtype);
    if(!sets)
        return;
    //str_print(sets->talbename);

    QString select_express = QString("select content,lantype,keywords,note from %1 where lantype='%2'")
            .arg(sets->talbename)
            .arg(selected_langtype);


    selectresult.contentList.clear();
    selectresult.contentstr.clear();
    selectresult.Keyword_list.clear();
    selectresult.note_list.clear();
    //gencode str clear
    GenCode_str.clear();

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult.contentstr,
                     selectresult.contentList,
                     selectresult.Keyword_list,
                     selectresult.note_list);

    ui->codeshow_textEdit->setText(selectresult.contentstr);
    ui->listWidget_codeview->clear();
    ui->listWidget_codeview->addItems(selectresult.Keyword_list);

    dialog_selectdb->close();
}

void autoCCode::textEditSet(void)
{
    self_print(textEditSet);

    //    QObject::connect(ui->db_comboBox,SIGNAL(activated(QString)),
    //                     this,SLOT(on_db_comboBox_activated(QString)));

}

void autoCCode::addstr_comboBox(void)
{

    self_print(addstr_comboBox);
    QStringList strlist;
    strlist.clear();
    strlist<<str_china(头文件)
          <<str_china(函数)
         <<str_china(结构体)
        <<str_china(变量);
    ui->db_comboBox->addItems(strlist);


    strlist.clear();
    strlist<<str_china()
          <<str_china(C)
         <<str_china(C++)
        <<str_china(Qt)
       <<str_china(Python)
      <<str_china(Jave);

    ui_dialog->langtype_comboBox->addItems(strlist);

    //select db dialog add strlist;
    ui_dia_selectdb->comboBox_selectdb->addItems(strlist);

}

autoCCode::~autoCCode()
{
    delete ui;
}

void autoCCode::on_save_btn_clicked()
{
    self_print(on_save_btn_clicked);
    QString savefileName = QFileDialog::getSaveFileName(this,
                                                        tr("Open Files"), "", tr("All Files (*.*)"));

    if (savefileName.isNull())
    {
        //fileName是文件名
        return;
    }
    str_print(savefileName);

    QFile file(savefileName);
    if (!file.open(QIODevice::ReadWrite)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }

    QTextStream out(&file);
    out << ui->genshow_textEdit->toPlainText();

    file.close();

    //点的是取消    qDebug()<<"fileName:"<<fileName;
}

void autoCCode::on_db_comboBox_activated(const QString &arg1)
{
    self_print(on_db_comboBox_activated);
}
//选择数据库
void autoCCode::on_choseCodeDB_btn_clicked(void)
{
    self_print(on_choseCodeDB_btn_clicked);
    dialog_selectdb->show();

}
//生成代码库
void autoCCode::on_gencode_btn_clicked(void)
{
    self_print(on_gencode_btn_clicked);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "../",
                                                    tr("Ctype (*.c *.C *.cc *.h)"
                                                       ";;Cpptype(*.cpp *.CPP *.h)"
                                                       ";;QTtype(*.c *.C *.cpp *.CPP *.ui *.rc *.pro *.h)"
                                                       ";;Pythontype(*.py *.PY)"
                                                       ";;JavaType(*.java)"
                                                       ";;All Files(*.*)"));
    qDebug()<<"fileName:"<<fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }
    QString text_file = file.readAll();

    ui->codeshow_textEdit->setText(text_file);

    //    QTextStream out(&file);
    //    out << "Thomas M. Disch: " << 334 << endl;

    file.close();
}
void autoCCode::hide_inBtn(void)
{
    ui->indb_btn->hide();
}
void autoCCode::hide_OutBtn(void)
{
    ui->outdb_btn->hide();
}
void autoCCode::show_OutBtn(void)
{
    ui->outdb_btn->show();
}
void autoCCode::show_InBtn(void)
{
    ui->indb_btn->show();
}
//入库函数
void autoCCode::on_indb_btn_clicked(void)
{
    self_print(on_indb_btn_clicked);
    QString select_text = ui->codeshow_textEdit->textCursor().selectedText();
    ui_dialog->content_textEdit_dia->setText(select_text);

    InDb_Dialog->exec();

}

void autoCCode::on_outdb_btn_clicked(void)
{
    self_print(on_outdb_btn_clicked);
}

LanguageType autoCCode::getLanguageType(QString &type)
{
#if 1
    if(type == "C"){
        return languagetype_C_;
    }else if(type == "Qt"){
        return languagetype_Qt_;
    }
    else if(type == "Python"){
        return languagetype_Python_;
    }
    else if(type == "Jave"){
        return languagetype_Jave_;
    }else if(type == "C++"){
        return languagetypeCpp_;
    }else{
        return languagetype_Err_;
    }
#else //type must be integer
    switch(type)
    {
    case "C":
        return languagetype_C_;
    case "Qt":
        return languagetype_Qt_;
    case "Python":
        return languagetype_Python_;
    case "Jave":
        return languagetype_Jave_;
    case "C++":
        return languagetypeCpp_;
    default:
        return languagetype_Err_;
    }
#endif
}

//dialog ok button
void autoCCode::on_ok_btn_dia_clicked(void)
{
    self_print(on_ok_btn_dia_clicked);

    //获取内容
    QString content = ui_dialog->content_textEdit_dia->toPlainText();
    QString lanaugetype = ui_dialog->langtype_comboBox->currentText();
    QString index_keyword   = ui_dialog->index_textEdit_dia->toPlainText();
    QString note   = ui_dialog->note_textEdit_dia->toPlainText();




    //str_print(content);
    //str_print(lanaugetype);
    //str_print(index_keyword);
    //str_print(note);


    if(content.isEmpty())
    {
        QMessageBox::information(NULL, str_china(内容), str_china(不能为空), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if(lanaugetype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(类型), str_china(不能为空), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if(index_keyword.isEmpty())
    {
        QMessageBox::information(NULL, str_china(检索), str_china(不能为空), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if(note.isEmpty())
    {
        QMessageBox::information(NULL, str_china(注释), str_china(不能为空), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }

    InsertCon insertcontent;
    insertcontent.content = content;
    insertcontent.languageType = getLanguageType(lanaugetype);
    insertcontent.keyword   = index_keyword;
    insertcontent.note      = note;


    b.creatable(&insertcontent);
    b.inserttable(&insertcontent);

    InDb_Dialog->close();
}

void autoCCode::on_cancel_btn_dia_clicked(void)
{
    self_print(on_cancel_btn_dia_clicked);
    InDb_Dialog->close();
}
void autoCCode::aboutVersion(void)
{
    QMessageBox::information(NULL, str_china(版本), GetVersion(),NULL,NULL);
    return;
}
QString autoCCode::GetVersion(void)
{
    return str_china(自动生成代码)
            +"\n"
            +str_china(by小魏莱)
            +"\n"
            +version_autoccode;
}

void autoCCode::listWidgetSet(void)
{
    self_print(listWidget);
    QObject::connect(ui->listWidget_codeview,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit(QListWidgetItem*)));
}
//添加到右边的内容中
void autoCCode::add_to_gen_code_textedit(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit);
    QString str = item->text();
    unsigned index = 0;
    for(unsigned i=0;i<selectresult.contentList.size();i++){
        if(str == selectresult.Keyword_list.at(i))
            index = i;
        qDebug()<<"note list:"<<selectresult.note_list.at(i);
    }

    str_print(str);

    GenCode_str+="/*  ";
    GenCode_str+=selectresult.note_list.at(index);
    GenCode_str+="   */";
    GenCode_str+="\n";
    GenCode_str+=selectresult.contentList.at(index);
    GenCode_str+="\n";
    GenCode_str+="\n";
    ui->genshow_textEdit->setText(GenCode_str);

}

void autoCCode::rightClear_textedit(void)
{
    GenCode_str.clear();
    ui->genshow_textEdit->clear();
}
