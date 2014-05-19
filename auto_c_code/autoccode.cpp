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





//���뺺��
#define str_china(A)     QString::fromLocal8Bit(#A)




autoCCode::autoCCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoCCode),
    ui_dialog(new Ui::Dialog),
    ui_dia_selectdb(new Ui::Dialog_select_database),
    sets(NULL)
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

    //�����
    QObject::connect(this->ui->rightclear_btn,SIGNAL(clicked()),
                     this,SLOT(rightClear_textedit()));

}
void autoCCode::comboBoxSet(void)
{
    self_print(comboBoxSet);
    QObject::connect(this->ui_dia_selectdb->comboBox_selectdb,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(on_comboBox_selectdb_currentIndexChanged(QString)));
    QObject::connect(this->ui->comboBox_vartype,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(on_ui_comboBox_vartype_currentIndexChanged(QString)));
}


void autoCCode::on_comboBox_selectdb_currentIndexChanged(const QString &arg1)
{
    self_print(on_comboBox_selectdb_currentIndexChanged);
    str_print(arg1);
    selected_langtype = arg1;
    //str_print(selected_langtype);
    LanguageType langtype = getLanguageType(selected_langtype);
    sets = get_table_sets_bytype(langtype);
    if(!sets)
        return;
    //str_print(sets->talbename);

    QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2'")
            .arg(sets->talbename)
            .arg(selected_langtype);


    clr_selectresult();
    //gencode str clear
    GenCode_str.clear();

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult);

    ui->codeshow_textEdit->setText(selectresult.contentstr);
    ui->listWidget_codeview->clear();
    ui->listWidget_codeview->addItems(selectresult.keyword_list);

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
        //fileName���ļ���
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
//ѡ�����ݿ�
void autoCCode::on_choseCodeDB_btn_clicked(void)
{
    self_print(on_choseCodeDB_btn_clicked);
    dialog_selectdb->show();

}
//���ɴ����
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
//��⺯��
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

    //��ȡ����
    QString content = ui_dialog->content_textEdit_dia->toPlainText();
    QString lanaugetype = ui_dialog->langtype_comboBox->currentText();
    QString index_keyword   = ui_dialog->index_textEdit_dia->toPlainText();
    QString note   = ui_dialog->note_textEdit_dia->toPlainText();
    QString vartype = ui_dialog->comboBox_vartype->currentText();

    str_print(vartype);

    //str_print(content);
    //str_print(lanaugetype);
    //str_print(index_keyword);
    //str_print(note);

    if(lanaugetype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if(content.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    //��������Ƿ��Ѿ������ˡ�
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
    b.selectdatabase(sets->databasename,select_express.toUtf8().data(),selectresult);

    if(selectresult.existflag)
    {
        QMessageBox::information(NULL, str_china(����), str_china(�����Ѿ�����), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }

    if(vartype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(��������), str_china(����Ϊ��), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }

    if(index_keyword.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if(note.isEmpty())
    {
        QMessageBox::information(NULL, str_china(ע��), str_china(����Ϊ��), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }

    InsertCon insertcontent;
    insertcontent.content = content;
    insertcontent.languagetype = getLanguageType(lanaugetype);
    insertcontent.keyword   = index_keyword;
    insertcontent.note      = note;
    insertcontent.vartype   = vartype;


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
    QMessageBox::information(NULL, str_china(�汾), GetVersion(),NULL,NULL);
    return;
}
QString autoCCode::GetVersion(void)
{
    return str_china(�Զ����ɴ���)
            +"\n"
            +str_china(byСκ��)
            +"\n"
            +version_autoccode;
}

void autoCCode::listWidgetSet(void)
{
    self_print(listWidget);
    QObject::connect(ui->listWidget_codeview,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit(QListWidgetItem*)));
}
//��ӵ��ұߵ�������
void autoCCode::add_to_gen_code_textedit(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit);
    QString str = item->text();
    unsigned int index = 0;
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.keyword_list.at(i))
            index = i;
        qDebug()<<"note list:"<<selectresult.note_list.at(i);
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
}

void autoCCode::select_db_by_vartype(QString &select_express)
{
    if(!sets)
        return;
    //str_print(sets->talbename);
    clr_selectresult();

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult);

    ui->codeshow_textEdit->setText(selectresult.contentstr);
    ui->listWidget_codeview->clear();
    ui->listWidget_codeview->addItems(selectresult.keyword_list);
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
