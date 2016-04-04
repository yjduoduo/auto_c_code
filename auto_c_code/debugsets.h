#ifndef DEBUGSETS_H
#define DEBUGSETS_H

#include <QList>
#include <QString>
#include <QDebug>

#ifndef INVALID_STR
#define INVALID_STR "UNKNOWN"
#endif

class ClsDebugSets:public QDebug
{
public:
    ClsDebugSets(QString *string = NULL);
    static void Init();
    static void Push(QString str);
    static QString Pop();
    static quint32 Size();
    static quint8 HaveData();
    static QList<QString> debugsets;
    inline ClsDebugSets &operator<<(const QString & t){debugsets.append(t);}

private:


};

#endif // DEBUGSETS_H
