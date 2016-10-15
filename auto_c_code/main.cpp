//#include <QtGui/QApplication>
#include <QApplication>
#include <QtGui>
#include "autoccode.h"
#include "globalapplication.h"
#include <QtGui>



int main(int argc, char *argv[])
{
    GlobalApplication  a(argc, argv);
//    QTranslator translator(0);
//    translator.load("calendar.qm",":/");
//    a.installTranslator(&translator);
//    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTranslator translator(0);
    translator.load("autoccode.qm",":/");
    a.installTranslator(&translator);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

//    QApplication a(argc, argv);
    autoCCode w;
    w.show();
    
//    qDebug()<<"auto c code\n";
    return a.exec();
}

