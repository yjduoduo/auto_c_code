#ifndef MSGTIPSAUTOSHUT_H
#define MSGTIPSAUTOSHUT_H

#include <QWidget>
#include <QtGui>

class MsgTipsAutoShut : public QWidget
{
    Q_OBJECT
public:
    explicit MsgTipsAutoShut(QWidget *parent = 0);
    void SetTipsInfo(QString s);
    void WidgetXYsetDesktop_center(QWidget *widget);

signals:
    
public slots:
    void closeEvent(QCloseEvent *event);
private:
    QString str;
    QTimer *timer;
    QLabel *label;

};

#endif // MSGTIPSAUTOSHUT_H
