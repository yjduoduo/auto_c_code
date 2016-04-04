#include "uithread.h"
#include <windows.h>
#include <QDebug>
#include <QCoreApplication>


#include "debugsets.h"

UiThread::UiThread(QObject *parent) :
    QThread(parent)
{
}

void UiThread::run()
{
//    QProcess::execute("notepad.exe");
    while(1)
    {
        if(ClsDebugSets::HaveData())
        {
            qDebug() <<"----thread printf:"<<ClsDebugSets::Pop();
        }
        else
        {
            Sleep(3000);
            qApp->processEvents();
        }
//        qDebug() << "thread running...";
    }
}
