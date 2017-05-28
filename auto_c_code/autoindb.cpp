#include "autoindb.h"
#include "ui_autoindb.h"

/*============================================
* FuncName    : AutoIndb::AutoIndb
* Description :
* @parent     :
* Author      :
* Time        : 2017-05-28
============================================*/
AutoIndb::AutoIndb(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoIndb)
{
    ui->setupUi(this);
}

/*============================================
* FuncName    : AutoIndb::~AutoIndb
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
AutoIndb::~AutoIndb()
{
    delete ui;
}

