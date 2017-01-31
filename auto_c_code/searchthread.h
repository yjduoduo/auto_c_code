#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QString>
#include <QStringList>


typedef struct
{
    QString contentstr;
    QStringList content_list;
    QStringList keyword_list;
    QStringList keyword_subshowlist;
    QStringList note_list;
    QStringList vartype_list;
    int existflag;//����Ƿ�������� ��־
    QStringList aspect_list;
    QStringList aspect_field;//�Զ�������
}SelectResult;

typedef struct SelInPara_Selectdatabase
{
    const char *databases_name;
    char *selecttableexpress;
    SelectResult selectres;
    int aspeactflag;
}SelInPara_Selectdatabase;





class SearchThread : public QThread
{
    Q_OBJECT
public:
    explicit SearchThread(QObject *parent = 0);
    
signals:
    void emitMsgDoneSignal(SelectResult);

public slots:
    void updateSearchWord(SelInPara_Selectdatabase & info);
protected:
    void run();
    
};

#endif // SEARCHTHREAD_H
