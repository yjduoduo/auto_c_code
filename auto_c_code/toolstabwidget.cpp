#include "toolstabwidget.h"
#include "ui_toolstabwidget.h"
//#include "windns.h"
#include "windows.h"
#include <QDebug>

ToolsTabWidget::ToolsTabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::ToolsTabWidget)
{
    ui->setupUi(this);
}

ToolsTabWidget::~ToolsTabWidget()
{
    delete ui;
}

void ToolsTabWidget::on_pushButton_clicked()
{
}
