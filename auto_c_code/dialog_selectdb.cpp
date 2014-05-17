#include "dialog_selectdb.h"
#include "ui_dialog_selectdb.h"

Dialog_selectDb::Dialog_selectDb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_selectDb)
{
    ui->setupUi(this);
}

Dialog_selectDb::~Dialog_selectDb()
{
    delete ui;
}
