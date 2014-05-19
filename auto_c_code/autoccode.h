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

    void add_to_gen_code_textedit(QListWidgetItem* item);

    void rightClear_textedit(void);

    void on_ui_comboBox_vartype_currentIndexChanged(const QString &str);

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
    void listWidgetSet(void);

    void select_db_by_vartype(QString &select_exp);

    void clr_selectresult(void);

private:

    Ui::autoCCode *ui;
    Ui::Dialog *ui_dialog;
    Ui::Dialog_select_database *ui_dia_selectdb;

protected:
    QString GenCode_str;
private://QString
    QString selected_langtype;

    SelectResult selectresult;

    codestructSets* sets;


};


#endif // AUTOCCODE_H
