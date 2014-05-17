#ifndef DIALOG_SELECTDB_H
#define DIALOG_SELECTDB_H

#include <QDialog>

namespace Ui {
class Dialog_selectDb;
}

class Dialog_selectDb : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog_selectDb(QWidget *parent = 0);
    ~Dialog_selectDb();
    
private:
    Ui::Dialog_selectDb *ui;
};

#endif // DIALOG_SELECTDB_H
