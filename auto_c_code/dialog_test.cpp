#include "dialog_test.h"
#include "ui_dialog_test.h"

Dialog_test::Dialog_test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_test)
{
    ui->setupUi(this);
}

Dialog_test::~Dialog_test()
{
    delete ui;
}
