#ifndef UITHREAD_H
#define UITHREAD_H

#include <QThread>

class UiThread : public QThread
{
    Q_OBJECT
public:
    explicit UiThread(QObject *parent = 0);
    void run();
signals:
    
public slots:

};


#endif // UITHREAD_H
