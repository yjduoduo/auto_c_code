//#include "udppkg.h"

//UdpPkg::UdpPkg()
//{
////    ui->setupUi(this);
//    configFlag = false;        //初始化连接参数 为未连接
////    ui->getTextEdit->ensureCursorVisible();
////    ui->sendTextEdit->setFocus();      //程序启动时，焦点停在发送对话框
////    ui->ipEdit->setText("192.168.1.103");   //设置默认的远程端Ip
////    ui->portEdit->setText("6665");           //设置默认端口号

////    //关联快捷键
////    ui->udpSendButton->setShortcut(tr("Alt+F"));
//////    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
//////    hostaddr1 = info.addresses().takeFirst();

////    //  hostaddr1 = new QHostAddress("192.168.2.214"); //本地的Ip地址
////    //    hostaddrStr = hostaddr1.toString();
////    //   hostaddrStr = QNetworkInterface::allAddresses().first().toString();
//    localIpStr = getIp();
////    ui->ipEdit->setText(localIpStr);
//    localHostAddr = new QHostAddress(localIpStr);
//    udpSocket1 = new QUdpSocket(this);


//    bool bindFlag = udpSocket1->bind(*localHostAddr, 6665, QUdpSocket::ShareAddress);
//    if(!bindFlag)
//    {
//        QMessageBox box;
//        box.setText(tr("初始化绑定socket错误！"));
//        box.exec();
//    }
//    else
//    {
//        connect(udpSocket1, SIGNAL(readyRead()), this, SLOT(receive()));
//        connect(ui->udpSendButton, SIGNAL(clicked()), this, SLOT(send()));
//    }
//    this->setWindowTitle(tr("基于Qt的UDP聊天界面-------南无准提佛母"));


////    timerSendPkt = new QTimer(this);
////    connect(timerSendPkt,SIGNAL(timeout()),this,SLOT(WriteData()));
////    timerSendPkt->start(10);
//}

//void UdpPkg::WriteData()
//{
//    if(!configFlag)
//    {
//        qDebug() << "no config";
//        return;
//    }
////    sendStr = ui->sendTextEdit->toPlainText();
////    if(sendStr.length()==0)
////    {
////        qDebug() << "str len zero";
////        return;
////    }
////    send();
////    ui->sendTextEdit->clear();
//}
