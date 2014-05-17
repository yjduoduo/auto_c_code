#include "autoccode.h"
#include "ui_autoccode.h"
#include <QtGui>
#include "prefix_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "version.h"



#ifndef RELEASE_VERSION
//声明变量并打印
#define str_print(Var) \
EXPORT_SYMBOL(Var)\
qDebug()<<FUNCDEBUG(Var);
#endif




autoCCode::autoCCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoCCode)
{
    ui->setupUi(this);


    pushButtonSet();
    textEditSet();
}

void autoCCode::pushButtonSet(void)
{
    str_print(pushButtonSet);
    //btn list
    QObject::connect(ui->close_btn,SIGNAL(clicked()),this,SLOT(close()));
}

void autoCCode::textEditSet(void)
{
    str_print(textEditSet);


}


autoCCode::~autoCCode()
{
    delete ui;
}
