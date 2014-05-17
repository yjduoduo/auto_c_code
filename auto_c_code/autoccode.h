#ifndef AUTOCCODE_H
#define AUTOCCODE_H

#include <QWidget>
#include "prefix_string.h"






namespace Ui {
class autoCCode;
}

class autoCCode : public QWidget
{
    Q_OBJECT
    
public:
    explicit autoCCode(QWidget *parent = 0);
    ~autoCCode();
private:
    void pushButtonSet(void);
    void textEditSet(void);
private:

    Ui::autoCCode *ui;
};

#endif // AUTOCCODE_H
