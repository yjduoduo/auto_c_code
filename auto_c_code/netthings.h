#ifndef NETTHINGS_H
#define NETTHINGS_H


#include "zmq.h"
#include <QString>
#include <QObject>
#include <QThread>

class NetThings:public QObject, public QThread
{
//    Q_OBJECT
public:
    NetThings(QObject *parent = 0);
//signals:
//    void emitMsg(QString &msg);
    int helloword_server (void);
protected:
    void run();

public slots:
    void updateMsg(QString strmsg);
private:
    QString msg;

};

#endif // NETTHINGS_H
