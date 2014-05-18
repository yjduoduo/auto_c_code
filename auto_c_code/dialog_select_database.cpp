#include "dialog_select_database.h"
#include "ui_dialog_select_database.h"

Dialog_select_database::Dialog_select_database(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_select_database)
{
    ui->setupUi(this);
}

Dialog_select_database::~Dialog_select_database()
{
    delete ui;
}

