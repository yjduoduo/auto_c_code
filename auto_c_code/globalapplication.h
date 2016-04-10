#ifndef GLOBALAPPLICATION_H
#define GLOBALAPPLICATION_H

#include <QApplication>

class GlobalApplication : public QApplication
{
public:
     GlobalApplication(int&argc,char **argv);
     ~GlobalApplication();

     bool notify(QObject*, QEvent *);
     void setWindowInstance(QWidget*wnd);
//      bool qwsEventFilter(QWSEvent * event);
private:
     QWidget *widget;
};

#endif // GLOBALAPPLICATION_H
