#include "netthings.h"
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#include <QDebug>

#define sleep(n)    Sleep(n*1000)
#endif

int NetThings::helloword_server (void)
{
    //  Hello World server

    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
//    int rc = zmq_bind (responder, "tcp://eth0:5555");
//    int rc = zmq_bind (responder, "tcp://1.2.3.4:5555");
//    int rc = zmq_bind (responder, "inproc://my_publisher");
    assert (rc == 0);

    while (1) {
        char buffer [10];

        zmq_recv (responder, buffer, 10, 0);
        qDebug ("Received Hello\n");
        sleep (1);          //  Do some 'work'
//        zmq_send (responder, "World", 5, 0);
        zmq_send (responder, msg.toLocal8Bit().data(), strlen(msg.toLocal8Bit().data()), 0);
    }
    zmq_close(responder);
    zmq_ctx_destroy(context);
    return 0;
}


NetThings::NetThings(QObject *parent)
    :QObject(parent),QThread(parent)
{
//    connect(this, SIGNAL(emitMsg(QString&)), this, SLOT(updateMsg(QString&)));

}

void NetThings::updateMsg(QString  strmsg)
{
    msg = strmsg;
    qDebug() << "---- hello world server ,msg:" << strmsg;

}


void NetThings::run()
{
    qDebug() << "============>>>>start Hello World Server.....";
    helloword_server();

    while(1)
    {
        qDebug() << "emit signal.....";
//        emit emitMsgBoxSignal();
        Sleep(1000);
    }
}
