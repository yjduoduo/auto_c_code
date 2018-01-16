#ifndef TOOLSTABWIDGET_H
#define TOOLSTABWIDGET_H

#include <QTabWidget>

namespace Ui {
class ToolsTabWidget;
}

class ToolsTabWidget : public QTabWidget
{
    Q_OBJECT
    
public:
    explicit ToolsTabWidget(QWidget *parent = 0);
    ~ToolsTabWidget();
    
private slots:
    void on_pushButton_clicked();

    void ReadHistorySettings();

    void WriteCurrentSettings();
    void on_pushButton_close_clicked();

private:
    Ui::ToolsTabWidget *ui;


};

#endif // TOOLSTABWIDGET_H
