#include "sendemail.h"
//#include "ui_sendemail.h"
#include "emailaddress.h"
#include <QFileDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include <iostream>
#include <QtGlobal>
#include <QtGui>
#include "smtpclient.h"
#include "mimehtml.h"
#include "mimeattachment.h"

#if 1
SendEmail::SendEmail(QWidget *parent) :
    QMainWindow(parent)/*,
    ui(new Ui::SendEmail)*/
{
//    ui->setupUi(this);
//    this->setFixedSize(710, 556);
//    //设置: 发送邮件服务器 SMTP
//    ui->host->setEnabled(false);
//    //    ui->host->setText("smtp.exmail.qq.com");
//    ui->host->setText("smtp.126.com");//设置邮箱服务器

//    //设置端口
//    ui->port->setEnabled(false);
//    ui->port->setValue(25);
//    //ui->port->setValue(465);

//    //发送的方式
//    ui->ssl->setEnabled(false);
//    ui->ssl->setVisible(false);
//    //ui->ssl->setChecked(true);
//    ui->label_7->setVisible(false);

//    //隐藏认证
//    ui->label_4->setVisible(false);
//    ui->auth->setVisible(false);
//    ui->auth->setChecked(true);

//    ui->username->setFixedHeight(20);


//    ui->username->setFocus();
//    ui->username->selectAll();

//    ui->username->setToolTip("设置: 发件人的邮箱");
//    ui->password->setToolTip("设置: 发件人的密码");

//    ui->sender->setReadOnly(true);
//    ui->sender->setToolTip("sender:sdd@sdf.com");
//    ui->recipients->setToolTip("rec1:sdfsd@sdfsdf.com; rec2:sdfsdf@sdfsdf.com"); //设置自己的邮箱
//    ui->clearAttachment->setFixedSize(ui->addAttachment->width(), 23);

//    connect(ui->username, SIGNAL(textChanged(QString)), this, SLOT(onteTextChange(QString)));
//    SetDefault();
}
//set default username and passwd
void SendEmail::SetDefault(void)
{
    QString defaultsend="allinfosend@126.com";
    QString defaultpass="lmr2973194+-*/";
    QString defaultrecv="allinfosets@126.com;wxjlmr@126.com";
//    ui->username->setText(defaultsend);
//    ui->password->setText(defaultpass);
//    ui->recipients->setText(defaultrecv);
//    onteTextChange(defaultsend);
}

SendEmail::~SendEmail()
{
//    delete ui;
}

EmailAddress* SendEmail::stringToEmail(const QString &str)
{
    int p1 = str.indexOf("<");
    int p2 = str.indexOf(">");

    if (p1 == -1)
    {
        // no name, only email address
        return new EmailAddress(str);
    }
    else
    {
        return new EmailAddress(str.mid(p1 + 1, p2 - p1 - 1), str.left(p1));
    }

}

void SendEmail::on_addAttachment_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);

//    if (dialog.exec())
//        ui->attachments->addItems(dialog.selectedFiles());
}

void SendEmail::on_clearAttachment_clicked()
{
//    ui->attachments->clear();
}


void SendEmail::onteTextChange(QString strText)
{
//    ui->sender->setText(strText);
}
#endif
//发送
void SendEmail::on_sendEmail_clicked()
{
    //----------------------
    //检查必要条件:
    QMessageBox msgWarning (this);
#if 1
    QString user = ""/*ui->username->text().trimmed()*/;
    if(user.isEmpty())
    {
        msgWarning.setText("please set user");
        msgWarning.exec();
//        ui->username->setFocus();
        return;
    }

    QString password = ""/*ui->password->text().trimmed()*/;
    if(password.isEmpty())
    {
        msgWarning.setText("please set password");
        msgWarning.exec();
//        ui->password->setFocus();
        return;
    }

    QString strSender = ""/*ui->sender->text().trimmed()*/;
    if(strSender.isEmpty())
    {
        msgWarning.setText("please set sender!!!");
        msgWarning.exec();
//        ui->sender->setFocus();
        return;
    }

    QString strRecipient = ""/*ui->recipients->text()*/;
    if(strRecipient.isEmpty())
    {
        msgWarning.setText("please set sender!!!");
        msgWarning.exec();
//        ui->recipients->setFocus();
        return;
    }
#endif
    //----------------------
    //准备发送邮件
//    QString host = ui->host->text();
//    int port = ui->port->value();
//    bool ssl = ui->ssl->isChecked();
//    bool auth = ui->auth->isChecked();

    QString host = "";
    int port = 25;
    bool ssl = true;
    bool auth = true;

    EmailAddress *sender = stringToEmail(""/*ui->sender->text()*/);

    //主题
    QString subject = "";
    //内容
    QString html = "";

    SmtpClient smtp (host, port, ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

    MimeMessage message;

    message.setSender(sender);
    message.setSubject(subject);

    //接收人列表
    QStringList rcptStringList = QString("").trimmed().split(';');/*ui->recipients->text().trimmed().split(';')*/;

    //删除空的联系人
    while(rcptStringList.size() > 0 && rcptStringList.last().trimmed().isEmpty())
    {
        rcptStringList.removeLast();
    }

    for (int i = 0; i < rcptStringList.size(); ++i)
         message.addRecipient(stringToEmail(rcptStringList.at(i)));

    MimeHtml content;
    content.setHtml(html);

    message.addPart(&content);

//    for (int i = 0; i < ui->attachments->count(); ++i)
//    {
//        MimeAttachment *temp = new MimeAttachment(new QFile(ui->attachments->item(i)->text().toUtf8()));
//        //temp->setEncoding(MimeAttachment::QuotedPrintable);
//        message.addPart(temp);
//    }

    if (!smtp.connectToHost())
    {
        errorMessage("SMTP link error.\nplease make sure SMTP server is right.");
        return;
    }

    if (auth)
        if (!smtp.login(user, password))
        {
//            ui->username->setFocus();
//            ui->username->selectAll();
            errorMessage("login fail.\nmake sure user,passwd ok.");
            return;
        }

    if (!smtp.sendMail(message))
    {
        errorMessage("mail send fail.\nmake sure recvier ok.");
//        ui->recipients->setFocus();
//        ui->recipients->selectAll();
        return;
    }
    else
    {
        QMessageBox okMessage (this);
        okMessage.setText("email send succ.");
        okMessage.exec();
    }

    smtp.quit();
}

void SendEmail::errorMessage(const QString &message)
{
//    QErrorMessage err (this);

//    err.showMessage(message);

//    err.exec();
    QMessageBox errMessage (this);
    errMessage.setText(message);
    errMessage.exec();
}
