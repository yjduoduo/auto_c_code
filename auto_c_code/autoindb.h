#ifndef AUTOINDB_H
#define AUTOINDB_H

#include <QWidget>

namespace Ui {
class AutoIndb;
}

class AutoIndb : public QWidget
{
    Q_OBJECT
    
public:
    explicit AutoIndb(QWidget *parent = 0);
    ~AutoIndb();
    
private:
    Ui::AutoIndb *ui;
};

#endif // AUTOINDB_H
