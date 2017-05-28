#include "sockthread.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <windows.h>


#if 1
//编码汉字
#define str_china(A)     QString::fromLocal8Bit(#A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif


/*============================================
* FuncName    : sockthread::sockthread
* Description :
* @parent     :
* Author      :
* Time        : 2017-05-28
============================================*/
sockthread::sockthread(QObject *parent) :
    QThread(parent),clientConnection(NULL)
{

    //读数据统计
    TotalReadBytes = 0;
    bytesReceived  = 0;
    bytesNeedRecv  = 0;

//    clientConnection = /*cltConnection*/;
}

//    QObject::connect(clientConnection,SIGNAL(readyRead()),
//                     this,SLOT(updateReadMsgProgress()));
//    QObject::connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)),
//                     this,SLOT(displayErr(QAbstractSocket::SocketError)));


/*============================================
* FuncName    : sockthread::run
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void sockthread::run()
{
    while(1)
    {
//        qDebug() << "emit signal.....";
//        emit emitMsgBoxSignal();
        Sleep(1000);
    }
}


/*============================================
* FuncName    : sockthread::displayErr
* Description :
* @socketError  :
* Author      :
* Time        : 2017-05-28
============================================*/
void sockthread::displayErr(QAbstractSocket::SocketError socketError)
{
    if(socketError == QTcpSocket::RemoteHostClosedError)
        return;
    QMessageBox::information(NULL,str_china("网络"),
                             str_china("产生如下错误： %1")
                             .arg(clientConnection->errorString()));
//    tcpserver->close();
//    tcpserver = NULL;

    if(clientConnection)
    {
        clientConnection->close();
        clientConnection = NULL;
    }
#ifdef SHOWCURSOR
    QApplication::restoreOverrideCursor();
#endif
}

/*============================================
* FuncName    : sockthread::updateReadMsgProgress
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void sockthread::updateReadMsgProgress()
{
    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_4_0);

    static bool recvdone = READ_DONE;

    if(bytesReceived <= sizeof(qint64)*1  && (recvdone == READ_DONE)){
        if((clientConnection->bytesAvailable() >= sizeof(qint64)*1)){
            in>>TotalReadBytes;
//            bytesReceived += sizeof(qint64)*1;
            inBlock.resize(0);
            recvdone = READING;
        }
    }


    if (bytesReceived < TotalReadBytes){
        /* 实际需要接收的字节数 */
        bytesNeedRecv = TotalReadBytes - bytesReceived;
        bytesReceived += clientConnection->bytesAvailable();

        if(bytesReceived >= TotalReadBytes){
            inBlock.append(clientConnection->read(bytesNeedRecv));
            bytesReceived = TotalReadBytes;
        }else{
            inBlock.append(clientConnection->readAll());
        }

        qDebug() << "bytesReceived:"<< bytesReceived;
        qDebug() << "TotalReadBytes   :"<< TotalReadBytes;
    }

    if (bytesReceived == TotalReadBytes) {
        QString  bigmsg = inBlock;
        //入库
//        readfromremote(bigmsg);
        emit emitMsgDoneSignal(bigmsg);

        TotalReadBytes = 0;
        bytesReceived = 0;
//        fileNameSize = 0;
        bytesNeedRecv = 0;
        inBlock.resize(0);

        recvdone = READ_DONE;
    }


}



/*============================================
* FuncName    : sockthread::setSocketConnect
* Description :
* @cltConnet  :
* Author      :
* Time        : 2017-05-28
============================================*/
void sockthread::setSocketConnect(QTcpSocket *cltConnet)
{
    clientConnection = cltConnet;
    QObject::connect(clientConnection,SIGNAL(readyRead()),
                     this,SLOT(updateReadMsgProgress()));
    QObject::connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)),
                     this,SLOT(displayErr(QAbstractSocket::SocketError)));


}

