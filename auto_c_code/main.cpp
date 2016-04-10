//#include <QtGui/QApplication>
#include <QApplication>
#include <QtGui>
#include "autoccode.h"
#include "globalapplication.h"
#include <QtGui>



int main(int argc, char *argv[])
{
    GlobalApplication  a(argc, argv);
//    QApplication a(argc, argv);
    autoCCode w;
    w.show();
    
//    qDebug()<<"auto c code\n";
    return a.exec();
}

