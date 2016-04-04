#include "debugsets.h"

QList<QString> ClsDebugSets::debugsets/* = QString("")*/;

ClsDebugSets::ClsDebugSets(QString *string) :
    QDebug(string)
{

}


void ClsDebugSets::Init()
{
    debugsets.clear();
}

void ClsDebugSets::Push(QString str)
{
    debugsets.append(str);
}

QString ClsDebugSets::Pop()
{
    if(debugsets.size())
    {
        QString retStr = debugsets.at(0);
        debugsets.removeAt(0);
        return retStr;
    }

    return INVALID_STR;
}


quint32 ClsDebugSets::Size()
{
    return debugsets.size();
}

quint8 ClsDebugSets::HaveData()
{
    return (debugsets.size() != 0);
}


