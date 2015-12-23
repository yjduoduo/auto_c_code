#ifndef SENDEMAIL_H
#define SENDEMAIL_H

#include <QMainWindow>
#include "emailaddress.h"

//namespace Ui {
//class SendEmail;
//}

class SendEmail : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SendEmail(QWidget *parent = 0);
    ~SendEmail();

    static EmailAddress * stringToEmail(const QString & str);
protected:
    void SetDefault(void);
private slots:
    void on_addAttachment_clicked();
    void on_clearAttachment_clicked();

    void on_sendEmail_clicked();
    void onteTextChange(QString strText);

private:
//    Ui::SendEmail *ui;
//    Ui::SendEmail *ui;
    void errorMessage(const QString & message);
};

#endif // SENDEMAIL_H
