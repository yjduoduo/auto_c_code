#include "uithread.h"
#include <windows.h>
#include <QDebug>
#include <QCoreApplication>


UiThread::UiThread(QObject *parent) :
    QThread(parent)
{
}

void UiThread::run()
{
//    QProcess::execute("notepad.exe");
    while(1)
    {
//        qDebug() << "thread running...";
        Sleep(3000);
        qApp->processEvents();
    }
}
