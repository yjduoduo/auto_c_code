#include "searchthread.h"
#include <windows.h>
#include <QDebug>

SearchThread::SearchThread(QObject *parent) :
    QThread(parent)
{
}


void SearchThread::run()
{
    while(1)
    {
        qDebug() <<"search thread processing...";
        Sleep(1000);
    }
}


void SearchThread::updateSearchWord(SelInPara_Selectdatabase & info)
{
    qDebug() <<"search databases_name :" << info.databases_name;
}
