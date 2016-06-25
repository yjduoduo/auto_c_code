#ifndef SOCKTHREAD_H
#define SOCKTHREAD_H

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QAbstractSocket>


enum
{
    READ_DONE,
    READING,
    READ_OTHER
};


class sockthread : public QThread
{
    Q_OBJECT
public:
    explicit sockthread(QObject *parent = 0);
    void setSocketConnect(QTcpSocket *cltConnet);
    
signals:
    void emitMsgDoneSignal(QString);
    
public slots:
    void displayErr(QAbstractSocket::SocketError socketError);
    void updateReadMsgProgress();

protected:
    void run();

protected:

    QTcpSocket *clientConnection;


    //读数据统计
//    qint64 TotalReadBytes;
//    qint64 byteReadden;
//    qint64 bytesToRead;
//    QByteArray outReadBlock;

    qint64  TotalReadBytes;
    qint64  bytesReceived;
//    qint64  fileNameSize;
    qint64  bytesNeedRecv;
    QByteArray inBlock;
};

#endif // SOCKTHREAD_H
