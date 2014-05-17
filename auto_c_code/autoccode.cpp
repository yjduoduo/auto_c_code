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





//编码汉字
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
    str_print(pushButtonSet);
    //btn list
    QObject::connect(ui->close_btn,SIGNAL(clicked()),this,SLOT(close()));
    //    QObject::connect(ui->save_btn,SIGNAL(clicked()),this,SLOT(on_save_btn_clicked()));
    //    QObject::connect(ui->tocode_btn,SIGNAL(clicked()),this,SLOT(on_tocode_btn_clicked()));
    QObject::connect(this->ui_dialog->ok_btn_dia,SIGNAL(clicked()),
                     this,SLOT(on_ok_btn_dia_clicked()));
    QObject::connect(this->ui_dialog->cancel_btn_dia,SIGNAL(clicked()),
                     this,SLOT(on_cancel_btn_dia_clicked()));




}

void autoCCode::textEditSet(void)
{
    str_print(textEditSet);

    //    QObject::connect(ui->db_comboBox,SIGNAL(activated(QString)),
    //                     this,SLOT(on_db_comboBox_activated(QString)));

}

void autoCCode::addstr_comboBox(void)
{

    str_print(addstr_comboBox);
    QStringList strlist;
    strlist.clear();
    strlist<<str_china(头文件)
          <<str_china(函数)
         <<str_china(结构体)
        <<str_china(变量);
    ui->db_comboBox->addItems(strlist);


    strlist.clear();
    strlist<<str_china(C)
          <<str_china(C++)
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
    str_print(on_save_btn_clicked);
}

void autoCCode::on_db_comboBox_activated(const QString &arg1)
{
    str_print(on_db_comboBox_activated);
}
//选择数据库
void autoCCode::on_choseCodeDB_btn_clicked(void)
{
    str_print(on_choseCodeDB_btn_clicked);

}
//生成代码库
void autoCCode::on_gencode_btn_clicked(void)
{
    str_print(on_gencode_btn_clicked);
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
//入库函数
void autoCCode::on_indb_btn_clicked(void)
{
    str_print(on_indb_btn_clicked);
    QString select_text = ui->codeshow_textEdit->textCursor().selectedText();
    ui_dialog->content_textEdit_dia->setText(select_text);

    InDb_Dialog->exec();

}

void autoCCode::on_outdb_btn_clicked(void)
{
    str_print(on_outdb_btn_clicked);
}


void autoCCode::on_ok_btn_dia_clicked(void)
{
    str_print(on_ok_btn_dia_clicked);

    InDb_Dialog->close();
}

void autoCCode::on_cancel_btn_dia_clicked(void)
{
    str_print(on_cancel_btn_dia_clicked);
    InDb_Dialog->close();
}
