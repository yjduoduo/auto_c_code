#ifndef DIALOG_TEST_H
#define DIALOG_TEST_H

#include <QDialog>

namespace Ui {
class Dialog_test;
}

class Dialog_test : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog_test(QWidget *parent = 0);
    ~Dialog_test();
    
private:
    Ui::Dialog_test *ui;
};

#endif // DIALOG_TEST_H
