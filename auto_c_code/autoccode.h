#ifndef AUTOCCODE_H
#define AUTOCCODE_H

#include <QWidget>

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
    Ui::autoCCode *ui;
};

#endif // AUTOCCODE_H
