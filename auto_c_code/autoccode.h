#ifndef AUTOCCODE_H
#define AUTOCCODE_H

#include <QWidget>
#include "prefix_string.h"
#include <QDialog>
#include "gencodedatabase.h"

namespace Ui {
class autoCCode;
class Dialog;
class Dialog_select_database;
}

class autoCCode : public QWidget,public GenCodeDatabase
{
    Q_OBJECT

public:

    explicit autoCCode(QWidget *parent = 0);
    ~autoCCode();

public:
    QDialog *InDb_Dialog;
    QDialog *dialog_selectdb;

    GenCodeDatabase b;


private slots:
    void on_save_btn_clicked();

    void on_db_comboBox_activated(const QString &arg1);

    void on_choseCodeDB_btn_clicked(void);

    void on_gencode_btn_clicked(void);

    void on_indb_btn_clicked(void);

    void on_outdb_btn_clicked(void);

    void aboutVersion(void);

    void on_comboBox_selectdb_currentIndexChanged(const QString &arg1);

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
//    QString getTablename(QString &type);
    QString GetVersion(void);

    void comboBoxSet(void);


private:

    Ui::autoCCode *ui;
    Ui::Dialog *ui_dialog;
    Ui::Dialog_select_database *ui_dia_selectdb;

private://QString
    QString selected_langtype;
};


#endif // AUTOCCODE_H
