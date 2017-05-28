#include "debugsets.h"

QList<QString> ClsDebugSets::debugsets/* = QString("")*/;

/*============================================
* FuncName    : ClsDebugSets::ClsDebugSets
* Description :
* @string     :
* Author      :
* Time        : 2017-05-28
============================================*/
ClsDebugSets::ClsDebugSets(QString *string) :
    QDebug(string)
{

}


/*============================================
* FuncName    : ClsDebugSets::Init
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void ClsDebugSets::Init()
{
    debugsets.clear();
}

/*============================================
* FuncName    : ClsDebugSets::Push
* Description :
* @str        :
* Author      :
* Time        : 2017-05-28
============================================*/
void ClsDebugSets::Push(QString str)
{
    debugsets.append(str);
}

/*============================================
* FuncName    : ClsDebugSets::Pop
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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


/*============================================
* FuncName    : ClsDebugSets::Size
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
quint32 ClsDebugSets::Size()
{
    return debugsets.size();
}

/*============================================
* FuncName    : ClsDebugSets::HaveData
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
quint8 ClsDebugSets::HaveData()
{
    return (debugsets.size() != 0);
}



