#ifndef CODESOPHIA_H
#define CODESOPHIA_H

#include <QMainWindow>
//#include "codesophia.h"

namespace Ui {
    class CodeSophia;
} // namespace Ui


class CodeSophia : public QMainWindow
{
    Q_OBJECT
public:
    explicit CodeSophia(QWidget *parent = 0);
    
signals:
    
public slots:


private:
    Ui::CodeSophia *ui;
    
};

#endif // CODESOPHIA_H
