//#include "udppkg.h"

//UdpPkg::UdpPkg()
//{
////    ui->setupUi(this);
//    configFlag = false;        //��ʼ�����Ӳ��� Ϊδ����
////    ui->getTextEdit->ensureCursorVisible();
////    ui->sendTextEdit->setFocus();      //��������ʱ������ͣ�ڷ��ͶԻ���
////    ui->ipEdit->setText("192.168.1.103");   //����Ĭ�ϵ�Զ�̶�Ip
////    ui->portEdit->setText("6665");           //����Ĭ�϶˿ں�

////    //������ݼ�
////    ui->udpSendButton->setShortcut(tr("Alt+F"));
//////    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
//////    hostaddr1 = info.addresses().takeFirst();

////    //  hostaddr1 = new QHostAddress("192.168.2.214"); //���ص�Ip��ַ
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
//        box.setText(tr("��ʼ����socket����"));
//        box.exec();
//    }
//    else
//    {
//        connect(udpSocket1, SIGNAL(readyRead()), this, SLOT(receive()));
//        connect(ui->udpSendButton, SIGNAL(clicked()), this, SLOT(send()));
//    }
//    this->setWindowTitle(tr("����Qt��UDP�������-------����׼���ĸ"));


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
