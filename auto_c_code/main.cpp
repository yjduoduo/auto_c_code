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
//    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

//    QApplication a(argc, argv);


    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale()); //设置GBK到本地
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    autoCCode w;
    w.show();
    
//    qDebug()<<"auto c code\n";
    return a.exec();
}

