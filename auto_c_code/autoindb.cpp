#include "autoindb.h"
#include "ui_autoindb.h"

AutoIndb::AutoIndb(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoIndb)
{
    ui->setupUi(this);
}

AutoIndb::~AutoIndb()
{
    delete ui;
}
