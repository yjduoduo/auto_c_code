#include "toolstabwidget.h"
#include "ui_toolstabwidget.h"
//#include "windns.h"
#include "windows.h"
#include <QDebug>
#include <QSettings>

ToolsTabWidget::ToolsTabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::ToolsTabWidget)
{
    ui->setupUi(this);


    ReadHistorySettings();

}

ToolsTabWidget::~ToolsTabWidget()
{
    delete ui;
}

void ToolsTabWidget::on_pushButton_clicked()
{
}


void ToolsTabWidget::ReadHistorySettings()
{
//    QSettings m_settings("weilaidb.com.cn", "ToolsTabWidget");
//    qDebug() << "ToolsTabWidget read setting filename:" << m_settings.fileName();
//    ui->comboBox_path->addItems(m_settings.value("pathlist").toStringList());

//    this->restoreGeometry(m_settings.value("ToolsTabWidget").toByteArray());
}


void ToolsTabWidget::WriteCurrentSettings()
{
//    QSettings m_settings("weilaidb.com.cn", "ToolsTabWidget");

//    m_settings.setValue("pathlist", pathlist);

//    m_settings.setValue("ToolsTabWidget", this->saveGeometry());

//    qDebug() << "ToolsTabWidget setting filename:" << m_settings.fileName();

}


void ToolsTabWidget::on_pushButton_close_clicked()
{

}
