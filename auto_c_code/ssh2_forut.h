#ifndef SSH2_FORUT_H
#define SSH2_FORUT_H
#include <QtGui>

class ssh2_forut
{
//    Q_OBJECT
public:
    ssh2_forut(char *ipaddr, QTextEdit *showresult);
    void whenconnected();
    void settext(QString text);

//signals:
//    void sendResult(char *str);
private:
    char m_ipaddr[128];
    QTextEdit *m_showresult;
};

#endif // SSH2_FORUT_H
