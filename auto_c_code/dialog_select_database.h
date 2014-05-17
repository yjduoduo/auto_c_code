#ifndef DIALOG_SELECT_DATABASE_H
#define DIALOG_SELECT_DATABASE_H

#include <QDialog>

namespace Ui {
class Dialog_select_database;
}

class Dialog_select_database : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog_select_database(QWidget *parent = 0);
    ~Dialog_select_database();
    
private:
    Ui::Dialog_select_database *ui;
};

#endif // DIALOG_SELECT_DATABASE_H
