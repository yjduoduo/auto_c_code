#include "autoccode.h"
#include "ui_autoccode.h"
#include "ui_PushDbdialog.h"
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
    ui(new Ui::autoCCode)
{
    ui->setupUi(this);
    InDb_Dialog = new QDialog(this);
    ui_dialog->setupUi(InDb_Dialog);

    pushButtonSet();
    textEditSet();
    addstr_comboBox();
}

void autoCCode::pushButtonSet(void)
{
    self_print(pushButtonSet);
    //btn list
    QObject::connect(ui->close_btn,SIGNAL(clicked()),this,SLOT(close()));
    //    QObject::connect(ui->save_btn,SIGNAL(clicked()),this,SLOT(on_save_btn_clicked()));
    //    QObject::connect(ui->tocode_btn,SIGNAL(clicked()),this,SLOT(on_tocode_btn_clicked()));
    QObject::connect(this->ui_dialog->ok_btn_dia,SIGNAL(clicked()),
                     this,SLOT(on_ok_btn_dia_clicked()));
    QObject::connect(this->ui_dialog->cancel_btn_dia,SIGNAL(clicked()),
                     this,SLOT(on_cancel_btn_dia_clicked()));


    QObject::connect(this->ui->about_btn,SIGNAL(clicked()),
                     this,SLOT(aboutVersion()));

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
    strlist<<str_china(ͷ�ļ�)
          <<str_china(����)
         <<str_china(�ṹ��)
        <<str_china(����);
    ui->db_comboBox->addItems(strlist);


    strlist.clear();
    strlist<<str_china(C)
          <<str_china(C++)
         <<str_china(Qt)
        <<str_china(Python)
       <<str_china(Jave);

    ui_dialog->langtype_comboBox->addItems(strlist);
}

autoCCode::~autoCCode()
{
    delete ui;
}

void autoCCode::on_save_btn_clicked()
{
    self_print(on_save_btn_clicked);
}

void autoCCode::on_db_comboBox_activated(const QString &arg1)
{
    self_print(on_db_comboBox_activated);
}
//ѡ�����ݿ�
void autoCCode::on_choseCodeDB_btn_clicked(void)
{
    self_print(on_choseCodeDB_btn_clicked);

}
//���ɴ����
void autoCCode::on_gencode_btn_clicked(void)
{
    self_print(on_gencode_btn_clicked);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "../",
                                                    tr("Images (*.c *.h)"));
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




    str_print(content);
    str_print(lanaugetype);
    str_print(index_keyword);
    str_print(note);


    if(content.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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
    insertcontent.languageType = getLanguageType(lanaugetype);
    insertcontent.keyword   = index_keyword;
    insertcontent.note      = note;

    GenCodeDatabase b;
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
    return str_china(�Զ����ɴ���)+"\n"+version_autoccode;
}
