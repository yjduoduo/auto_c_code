#ifndef AUTOCCODE_H
#define AUTOCCODE_H

#include <QWidget>
#include "prefix_string.h"
#include <QDialog>
#include "gencodedatabase.h"
#include "dialog_selectdb.h"

namespace Ui {
class autoCCode;
class Dialog;
class Dialog_selectDb;
}


class autoCCode : public QWidget
{
    Q_OBJECT
    
public:
    QDialog *InDb_Dialog;
    Dialog_selectDb *ui_dialog_selectdb;
    explicit autoCCode(QWidget *parent = 0);
    ~autoCCode();

private slots:
    void on_save_btn_clicked();

    void on_db_comboBox_activated(const QString &arg1);

    void on_choseCodeDB_btn_clicked(void);

    void on_gencode_btn_clicked(void);

    void on_indb_btn_clicked(void);

    void on_outdb_btn_clicked(void);

    void aboutVersion(void);
public slots:

    void on_ok_btn_dia_clicked(void);

    void on_cancel_btn_dia_clicked(void);


private:
    void pushButtonSet(void);
    void textEditSet(void);
    void addstr_comboBox(void);

    void hide_inBtn(void);
    void hide_OutBtn(void);
    void show_OutBtn(void);
    void show_InBtn(void);

    LanguageType getLanguageType(QString &type);
    QString GetVersion(void);
private:

    Ui::autoCCode *ui;
    Ui::Dialog *ui_dialog;

};


#endif // AUTOCCODE_H
