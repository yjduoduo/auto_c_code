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
    ui_dia_selectdb(new Ui::Dialog_select_database),
    sets(NULL),
    index_key_color(0),
    index_note_color(0)
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

    //dialog_select_database.ui add_btn
    QObject::connect(this->ui_dialog->pushButton_add,SIGNAL(clicked()),
                     this,SLOT(add_aspect_totable()));
}
void autoCCode::comboBoxSet(void)
{
    self_print(comboBoxSet);
    QObject::connect(this->ui_dia_selectdb->comboBox_selectdb,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(on_comboBox_selectdb_currentIndexChanged(QString)));
    QObject::connect(this->ui->comboBox_vartype,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(on_ui_comboBox_vartype_currentIndexChanged(QString)));
    QObject::connect(this->ui_dia_selectdb->comboBox_aspect,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(on_comboBox_aspect_currentIndexChanged(QString)));

    //左滚动，对应 右滚动
    QObject::connect(this->ui->listWidget_codeview,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(listWidget_note_scroll_sync(QListWidgetItem*)));
    QObject::connect(this->ui->listWidget_note,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(listWidget_codeview_scroll_sync(QListWidgetItem*)));
}


void autoCCode::on_comboBox_selectdb_currentIndexChanged(const QString &arg1)
{
    self_print(on_comboBox_selectdb_currentIndexChanged);
    str_print(arg1);

    ui->listWidget_codeview->clear();
    ui->listWidget_note->clear();

    selected_langtype = arg1;
    //str_print(selected_langtype);
    LanguageType langtype = getLanguageType(selected_langtype);
    sets = get_table_sets_bytype(langtype);
    if(!sets)
        return;

    QString select_express;
    select_express.clear();
    //str_print(sets->talbename);
    QString aspect = ui_dialog->comboBox_aspect->currentText();
    str_print(aspect);
    if(aspect == "")
    {
        select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2'")
                .arg(sets->talbename)
                .arg(selected_langtype);
    }else{
        select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2' and aspect_field='%3'")
                .arg(sets->talbename)
                .arg(selected_langtype)
                .arg(aspect);
    }
    clr_selectresult();
    str_print(select_express);
    //gencode str clear
    GenCode_str.clear();

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult,
                     ASPECT_NONE);

    ui->codeshow_textEdit->setText(selectresult.contentstr);
    ui->listWidget_codeview->addItems(selectresult.keyword_list);
    ui->listWidget_note->addItems(selectresult.note_list);
    dialog_selectdb->close();
}

void autoCCode::textEditSet(void)
{
    self_print(textEditSet);

    //    QObject::connect(ui->db_comboBox,SIGNAL(activated(QString)),
    //                     this,SLOT(on_db_comboBox_activated(QString)));

}
void autoCCode::addstr_aspect_comboBox(void)
{
    LanguageType langtype = languagetype_Aspect_;
    sets = get_table_sets_bytype(langtype);
    if(!sets)
        return;
    QString select_express = QString("select distinct aspect_field from aspect_table;");
    clr_selectresult();
    str_print(select_express);
    selectresult.aspect_list<<str_china();
    b.selectdatabase(sets->databasename,
                     select_express.toUtf8().data(),
                     selectresult,
                     ASPECT_HAVE);
    ui_dia_selectdb->comboBox_aspect->clear();
    ui_dia_selectdb->comboBox_aspect->addItems(selectresult.aspect_list);

    ui_dialog->comboBox_aspect->clear();
    ui_dialog->comboBox_aspect->addItems(selectresult.aspect_list);

}

void autoCCode::addstr_comboBox(void)
{

    self_print(addstr_comboBox);
    QStringList strlist;
    strlist.clear();
    strlist<<str_china()
          <<str_china(header)
         <<str_china(function)
        <<str_china(struct)
       <<str_china(variable);
    ui->comboBox_vartype->addItems(strlist);
    ui_dialog->comboBox_vartype->addItems(strlist);


    strlist.clear();
    strlist<<str_china()
          <<str_china(C)
         <<str_china(C++)
        <<str_china(Qt)
       <<str_china(Python)
      <<str_china(shell)
     <<str_china(Jave);

    ui_dialog->langtype_comboBox->addItems(strlist);

    //select db dialog add strlist;
    ui_dia_selectdb->comboBox_selectdb->addItems(strlist);

    //范畴
    addstr_aspect_comboBox();

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
    //    QString text_file = file.readAll();
    QString text_china = QString::fromLocal8Bit(file.readAll().data());

    ui->codeshow_textEdit->setText(text_china);

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

    //    InDb_Dialog->exec();
    InDb_Dialog->show();

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
    else if(type == "shell")
    {
        return languagetype_Shell_;
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
    QString vartype = ui_dialog->comboBox_vartype->currentText();
    QString aspect = ui_dialog->comboBox_aspect->currentText();

    str_print(vartype);

    //str_print(content);
    //str_print(lanaugetype);
    //str_print(index_keyword);
    //str_print(note);

    if(lanaugetype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(类型), str_china(不能为空), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if(content.isEmpty())
    {
        QMessageBox::information(NULL, str_china(内容), str_china(不能为空), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    //检测内容是否已经存在了。
    QString exitstr;
    exitstr.clear();


    LanguageType langtype = getLanguageType(lanaugetype);
    sets = get_table_sets_bytype(langtype);
    if(!sets)
        return;
    //str_print(sets->talbename);

    QString select_express = QString("select content from %1 where lantype='%2' and content='%3' and vartype='%4'")
            .arg(sets->talbename)
            .arg(lanaugetype)
            .arg(content)
            .arg(vartype);
    clr_selectresult();
    str_print(select_express);
    b.selectdatabase(sets->databasename,
                     select_express.toUtf8().data(),
                     selectresult,
                     ASPECT_NONE);

    if(selectresult.existflag)
    {
        QMessageBox::information(NULL, str_china(声明), str_china(内容已经存在), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }

    if(vartype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(变量类型), str_china(不能为空), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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
    //    if(aspect.isEmpty())
    //    {
    //        QMessageBox::information(NULL, str_china(范围), str_china(不能为空), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    //        return;
    //    }

    InsertCon insertcontent;
    insertcontent.content = content;
    insertcontent.languagetype = getLanguageType(lanaugetype);
    insertcontent.keyword   = index_keyword;
    insertcontent.note      = note;
    insertcontent.vartype   = vartype;
    insertcontent.aspect    = aspect;


    b.creatable(&insertcontent);
    b.inserttable(&insertcontent);

#ifdef RELEASE_VERSION
    InDb_Dialog->close();
#else
    //    InDb_Dialog->close();
    ui_dialog->content_textEdit_dia->clear();
#endif

    //内容添加后，更新控件中内容的相关显示
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
                     this,SLOT(add_to_gen_code_textedit_by_keyword(QListWidgetItem*)));
    QObject::connect(ui->listWidget_note,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_note(QListWidgetItem*)));

}
//添加到右边的内容中
void autoCCode::add_to_gen_code_textedit_by_keyword(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit);
    QString str = item->text();
    unsigned int index = 0;
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.keyword_list.at(i))
            index = i;
        //        qDebug()<<"note list:"<<selectresult.note_list.at(i);
    }

    str_print(str);

    GenCode_str+="/*  ";
    GenCode_str+=selectresult.note_list.at(index);
    GenCode_str+="   */";
    GenCode_str+="\n";
    GenCode_str+=selectresult.content_list.at(index);
    GenCode_str+="\n";
    GenCode_str+="\n";
    ui->genshow_textEdit->setText(GenCode_str);

}
//添加到右边的内容中
void autoCCode::add_to_gen_code_textedit_by_note(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit);
    QString str = item->text();
    unsigned int index = 0;
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.note_list.at(i))
            index = i;
        //        qDebug()<<"note list:"<<selectresult.note_list.at(i);
    }

    str_print(str);

    GenCode_str+="/*  ";
    GenCode_str+=selectresult.note_list.at(index);
    GenCode_str+="   */";
    GenCode_str+="\n";
    GenCode_str+=selectresult.content_list.at(index);
    GenCode_str+="\n";
    GenCode_str+="\n";
    ui->genshow_textEdit->setText(GenCode_str);

}


void autoCCode::rightClear_textedit(void)
{
    GenCode_str.clear();
    ui->genshow_textEdit->clear();
}
void autoCCode::clr_selectresult(void)
{
    selectresult.content_list.clear();
    selectresult.contentstr.clear();
    selectresult.keyword_list.clear();
    selectresult.note_list.clear();
    selectresult.vartype_list.clear();
    selectresult.existflag = 0;
    selectresult.aspect_list.clear();

    //每次重新开始记录，否则滚动时点击会死机
    index_key_color = 0;
    index_note_color = 0;


}

void autoCCode::select_db_by_vartype(QString &select_express)
{
    ui->listWidget_codeview->clear();
    ui->listWidget_note->clear();
    if(!sets)
        return;
    //str_print(sets->talbename);
    clr_selectresult();

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult,
                     ASPECT_NONE);

    ui->codeshow_textEdit->setText(selectresult.contentstr);
    ui->listWidget_codeview->addItems(selectresult.keyword_list);
    ui->listWidget_note->addItems(selectresult.note_list);
}

void autoCCode::on_ui_comboBox_vartype_currentIndexChanged(const QString &str)
{
    self_print(on_ui_comboBox_vartype_currentIndexChanged);
    str_print(str);

    if(!sets)
        return;

    if(str.contains("header")){
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where vartype='%2'")
                .arg(sets->talbename)
                .arg("header");
        select_db_by_vartype(select_express);
    }else if(str.contains("function")){
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where vartype='%2'")
                .arg(sets->talbename)
                .arg("function");
        select_db_by_vartype(select_express);
    }else if(str.contains("struct")){
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where vartype='%2'")
                .arg(sets->talbename)
                .arg("struct");
        select_db_by_vartype(select_express);
    }
    else if(str.contains("variable")){
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where vartype='%2'")
                .arg(sets->talbename)
                .arg("variable");
        select_db_by_vartype(select_express);
    }
    else{
        //        str_print(sets->langtype);
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2'")
                .arg(sets->talbename)
                .arg(getLanguageStr(sets->langtype));
        select_db_by_vartype(select_express);
    }

}

void autoCCode::on_comboBox_aspect_currentIndexChanged(const QString &str)
{
    self_print(on_comboBox_aspect_currentIndexChanged);
    str_print(str);

    if(!str.isEmpty())
    {

    }

}
void autoCCode::add_aspect_totable(void)
{
    QString aspect_str = ui_dialog->lineEdit_add_aspect->text();
    str_print(aspect_str);
    if(aspect_str.isEmpty())
    {
        return;
    }

    InsertCon insertcontent;
    //    insertcontent.content = content;
    insertcontent.languagetype = languagetype_Aspect_;
    insertcontent.aspect       = aspect_str;
    //    insertcontent.keyword   = index_keyword;
    //    insertcontent.note      = note;
    //    insertcontent.vartype   = vartype;

    b.creatable(&insertcontent);
    b.inserttable(&insertcontent);

    //范畴
    addstr_aspect_comboBox();

}
//右边的注释同步滚动，选择
void autoCCode::listWidget_note_scroll_sync(QListWidgetItem* item)
{
    self_print(listWidget_note_scroll_sync);
    unsigned int index = 0;
    QString str = item->text();
    if(str.isEmpty())
        return;
    if(selectresult.content_list.size() == 0)
        return;
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.keyword_list.at(i))
            index = i;
    }

    //    str_print(str);
    //    str_print(index);
    ui->listWidget_note->setCurrentRow(index);
    ui->listWidget_note->item(index_key_color)->setBackgroundColor(Qt::white);
    ui->listWidget_note->item(index)->setBackgroundColor(Qt::green);
    index_key_color = index; //

    ui->listWidget_codeview->item(index_note_color)->setBackgroundColor(Qt::white);
}

void autoCCode::listWidget_codeview_scroll_sync(QListWidgetItem* item)
{
    self_print(listWidget_codeview_scroll_sync);
    unsigned int index = 0;
    QString str = item->text();
    str_print(selectresult.existflag);
    if(str.isEmpty())
        return;
    if(selectresult.content_list.size() == 0)
        return;
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.note_list.at(i))
            index = i;
    }
    ui->listWidget_codeview->setCurrentRow(index);
    ui->listWidget_codeview->item(index_note_color)->setBackgroundColor(Qt::white);
    ui->listWidget_codeview->item(index)->setBackgroundColor(Qt::red);
    index_note_color = index; //

    ui->listWidget_note->item(index_key_color)->setBackgroundColor(Qt::white);

}
