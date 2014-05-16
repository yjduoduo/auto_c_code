#include <QtGui/QApplication>
#include "autoccode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    autoCCode w;
    w.show();
    
    return a.exec();
}
