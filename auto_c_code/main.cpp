#include <QtGui/QApplication>
#include "autoccode.h"
#include <QtGui>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    autoCCode w;
    w.show();
    
    qDebug()<<"auto c code\n";
    return a.exec();
}
