#include "autoccode.h"
#include "ui_autoccode.h"

autoCCode::autoCCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoCCode)
{
    ui->setupUi(this);
}

autoCCode::~autoCCode()
{
    delete ui;
}
