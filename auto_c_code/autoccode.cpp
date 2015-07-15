#include "autoccode.h"
#include "ui_autoccode.h"
#include "ui_PushDbdialog.h"
#include "ui_dialog_select_database.h"
#include "ui_autoindb.h"
#include <QtGui>
#include "prefix_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "version.h"
#include "gencodedatabase.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <QString>
#include <QTime>
#include <QElapsedTimer>

#include "qxtglobalshortcut/qxtglobalshortcut.h"   //add shortcut


using namespace std;


#define LIMIT_SHOW_SIZE getLimitNum()/*60*/


#if 1
//���뺺��
#define str_china(A)     QString::fromLocal8Bit(#A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif

#define UNUSEDVAR(A) (void)A;


autoCCode::autoCCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoCCode),
    ui_dialog(new Ui::Dialog),
    ui_dia_selectdb(new Ui::Dialog_select_database),
    ui_autoindb(new Ui::AutoIndb),
    sets(NULL),
    index_key_color(0),
    index_note_color(0),
    flag_selectLeft(1),
    listWidget_codeview_row(0),
    CurrentIndex_comboBox_aspect(0),
    CurrentIndex_comboBox_vartype(0),
    CurrentIndex_comboBox_langtype(0),
    timer(NULL),
    timer_checkbox_sel(NULL),
    lineEdit_search_timer(NULL),
    checkbox_getcliptext_timer(NULL),
    checkbox_AutoGetCon_timer(NULL)

{
    codec = QTextCodec::codecForName("GBK");//must first used,or is NULL,die
    ui->setupUi(this);
    InDb_Dialog = new QDialog(this);
    ui_dialog->setupUi(InDb_Dialog);

    dialog_selectdb =  new QDialog(this);;
    ui_dia_selectdb->setupUi(dialog_selectdb);

    dialog_autoindb =  new QDialog(this);
    ui_autoindb->setupUi(dialog_autoindb);

    QTimerSet();
    pushButtonSet();
    textEditSet();
    comboBoxSet();
    listWidgetSet();
    addstr_comboBox();
    lineTextEditSet();
    dragDropSet();
    checkboxSet();
    keyPressEventSet();
    shortCutSet();
    ProgressBarSet();
    InstallEventFilterSets();
    ListViewSets();

}
void autoCCode::shortCutSet(void)
{
    //show or hide windows
    QxtGlobalShortcut * sc = new QxtGlobalShortcut(QKeySequence("Shift+Alt+S"), this);
    QObject::connect(sc, SIGNAL(activated()),this, SLOT(toggle()));
    //
    QxtGlobalShortcut * ruku = new QxtGlobalShortcut(QKeySequence("Shift+Alt+R"), this);
    //    QObject::connect(ruku, SIGNAL(activated()),this, SLOT(on_indb_window_show_hide()));
    QObject::connect(ruku, SIGNAL(activated()),this, SLOT(on_indb_window_show_hide()));

    QxtGlobalShortcut * beizhu_fous = new QxtGlobalShortcut(QKeySequence("Shift+Alt+B"), this);
    QObject::connect(beizhu_fous, SIGNAL(activated()),this, SLOT(note_focus()));

    QxtGlobalShortcut * beizhu_clr_fous = new QxtGlobalShortcut(QKeySequence("Shift+Alt+N"), this);
    QObject::connect(beizhu_clr_fous, SIGNAL(activated()),this, SLOT(note_clear_focus()));

    QxtGlobalShortcut * ruku_ok = new QxtGlobalShortcut(QKeySequence("Shift+Alt+O"), this);
    QObject::connect(ruku_ok, SIGNAL(activated()),this, SLOT(ok_btn_dia_clicked_self()));

    QxtGlobalShortcut * ruku_max = new QxtGlobalShortcut(QKeySequence("Shift+Alt+M"), this);
    QObject::connect(ruku_max, SIGNAL(activated()),this, SLOT(maxSize_ui_dialog()));

    QxtGlobalShortcut * ruku_width = new QxtGlobalShortcut(QKeySequence("Shift+Alt+W"), this);
    QObject::connect(ruku_width, SIGNAL(activated()),this, SLOT(widthSize_ui_dialog()));

    QxtGlobalShortcut * ruku_min = new QxtGlobalShortcut(QKeySequence("Shift+Alt+L"), this);
    QObject::connect(ruku_min, SIGNAL(activated()),this, SLOT(minSize_ui_dialog()));

    QxtGlobalShortcut * ruku_paste_left = new QxtGlobalShortcut(QKeySequence("Shift+Alt+P"), this);
    QObject::connect(ruku_paste_left, SIGNAL(activated()),this, SLOT(setCliptext_content()));

    QxtGlobalShortcut * search_paste = new QxtGlobalShortcut(QKeySequence("Shift+Alt+V"), this);
    QObject::connect(search_paste, SIGNAL(activated()),this, SLOT(set_search_text()));

    QxtGlobalShortcut * search_clean = new QxtGlobalShortcut(QKeySequence("Shift+Alt+C"), this);
    QObject::connect(search_clean, SIGNAL(activated()),this, SLOT(search_text_clear()));

}

void autoCCode::ProgressBarSet(void)
{
    self_print(ProgressBarSet);
    if(ui_autoindb->progressBar)
        ui_autoindb->progressBar->setValue(0);

}
//�ڴ����˹�����֮������Ҫ�����ǰ�װ�������������װ��������Ҫ����installEventFilter()������
void autoCCode::InstallEventFilterSets(void)
{
    ui->lineEdit_search->installEventFilter(this);

    ui->listWidget_codeview->installEventFilter(this);
    ui->listWidget_note->installEventFilter(this);


}

void autoCCode::ListViewSets()
{
    listView = new QListView(this);
    model = new QStringListModel(this);
    listView->setWindowFlags(Qt::ToolTip);
    //    connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(setCompleter(const QString &)));
    //    connect(listView, SIGNAL(clicked(QModelIndex)), this, SLOT(completeText(const QModelIndex &)));

}

void autoCCode::ProgressBarSetValue(int value)
{
    self_print(ProgressBarSetValue);
    if(value >=0 && value <=100){
        ui_autoindb->progressBar->setValue(value);
        ui_autoindb->progressBar->update();
    }

}
void autoCCode::QTimerSet(void)
{
    self_print(QTimerSet);
    timer = new QTimer(this);
    timer->start(500);

    timer_checkbox_sel = new QTimer(this);
    timer_checkbox_sel->start(500);

    lineEdit_search_timer = new QTimer(this);
    lineEdit_search_timer->start(500);

    checkbox_getcliptext_timer = new QTimer(this);
    checkbox_getcliptext_timer->start(500);

    QObject::connect(checkbox_getcliptext_timer,SIGNAL(timeout()),this,SLOT(pasteClicpTextToSearchEdit()));

    checkbox_AutoGetCon_timer = new QTimer(this);
    checkbox_AutoGetCon_timer->start(500);
    QObject::connect(checkbox_AutoGetCon_timer,SIGNAL(timeout()),this,SLOT(pasteClicpTextToAutoGetCon_UiDialog()));


    timer_datachangedpopui = new QTimer(this);
    timer_datachangedpopui->start(300);
    QObject::connect(timer_datachangedpopui,SIGNAL(timeout()),this,SLOT(PopInDbUi()));

}
void autoCCode::keyPressEventSet()
{
    //    QObject::connect(btn4,SIGNAL(clicked()),
    //                     this,SLOT(popup()));

    QObject::connect(ui->lineEdit_search,SIGNAL(editingFinished()),
                     this,SLOT(SearchEnter()));
}


void autoCCode::checkboxSet()
{
    QObject::connect(ui_dialog->checkBox_AutoGet_Con,SIGNAL(toggled(bool)),
                     this,SLOT(ui_dialog_AutoGetCon(bool)));
    QObject::connect(ui->checkBox_autogetclipboxtext,SIGNAL(toggled(bool)),
                     this,SLOT(isCheckBox_cliptext_checked(bool)));
    if(!ui_dialog)
        return;

    QObject::connect(ui_dialog->content_textEdit_dia,SIGNAL(textChanged()),
                     this,SLOT(set_index_text()));
}
void autoCCode::set_index_text()
{
    if(ui->checkBox_same->isChecked())
        ui_dialog->index_textEdit_dia->setText(ui_dialog->content_textEdit_dia->toPlainText());
}

void autoCCode::lineTextEditSet(void)
{
#if 0//�ı��ı�����
    QObject::connect(ui->lineEdit_search,SIGNAL(textChanged(QString)),
                     this,SLOT(SearchText(QString)));
#else//��ʱ����
    QObject::connect(lineEdit_search_timer,SIGNAL(timeout()),this,SLOT(SearchText_WithTimer()));

#endif
}

void autoCCode::dragDropSet(void)
{
    //�����Ϸŵ�������ӵ��༭����
    ui->codeshow_textEdit->setAcceptDrops(true);
    //    ui->codeshow_textEdit->setHidden(true);
}


void autoCCode::pushButtonSet(void)
{
    self_print(pushButtonSet);
    //btn list
    //ui
    QObject::connect(ui->close_btn,SIGNAL(clicked()),this,SLOT(close()));
    QObject::connect(this->ui->about_btn,SIGNAL(clicked()),
                     this,SLOT(aboutVersion()));
    //    QObject::connect(ui->save_btn,SIGNAL(clicked()),this,SLOT(on_save_btn_clicked()));
    //    QObject::connect(ui->tocode_btn,SIGNAL(clicked()),this,SLOT(on_tocode_btn_clicked()));
    //ui_dialog
    QObject::connect(this->ui_dialog->ok_btn_dia,SIGNAL(clicked()),
                     this,SLOT(ok_btn_dia_clicked_self()));
    QObject::connect(this->ui_dialog->cancel_btn_dia,SIGNAL(clicked()),
                     this,SLOT(cancel_btn_dia_clicked_self()));

    //�����
    QObject::connect(this->ui->rightclear_btn,SIGNAL(clicked()),
                     this,SLOT(rightClear_textedit()));

    //dialog_select_database.ui add_btn
    QObject::connect(this->ui_dialog->pushButton_add,SIGNAL(clicked()),
                     this,SLOT(add_aspect_totable()));

    //ɾ������
    QObject::connect(ui->delete_btn,SIGNAL(clicked()),
                     this,SLOT(delete_btn_clicked_selfdefine()));
    //clean search text
    QObject::connect(ui->pushButton_clean,SIGNAL(clicked()),
                     this,SLOT(cleanLineTextEditSearch()));

#ifdef RELEASE_VERSION
    //���¿�,Ϊ���Сд��keyworks for search ʹ�õģ�Ĭ�ϲ���ʾ
    ui->pushButton_updatedb->hide();
    QObject::connect(ui->pushButton_updatedb,SIGNAL(clicked()),
                     this,SLOT(add_column_lowercase_keywords_content()));
#endif

    /*  �źš�����   */
    QObject::connect(ui_dialog->pushButton_note,SIGNAL(clicked()),
                     this,SLOT(setCliptext()));

    QObject::connect(ui_dialog->pushButton_note_clear,SIGNAL(clicked()),
                     this,SLOT(note_clear()));
    QObject::connect(ui->pushButton_search_fromClip,SIGNAL(clicked()),
                     this,SLOT(set_search_text()));

    /*  content��ť����   */
    QObject::connect(ui_dialog->pushButton_content_paste,SIGNAL(clicked()),
                     this,SLOT(setCliptext_content()));

    QObject::connect(ui_dialog->pushButton_content_clear,SIGNAL(clicked()),
                     this,SLOT(content_clear()));

    /*  min size ����   */
    QObject::connect(ui_dialog->pushButton_minsize,SIGNAL(clicked()),
                     this,SLOT(minSize_ui_dialog()));
    QObject::connect(ui_dialog->pushButton_widthsize,SIGNAL(clicked()),
                     this,SLOT(widthSize_ui_dialog()));
    QObject::connect(ui_dialog->pushButton_maxsize,SIGNAL(clicked()),
                     this,SLOT(maxSize_ui_dialog()));


    ui->pushButton_updatedb->hide();//���ظ��¿ⰴť

    QObject::connect(ui->modify_btn,SIGNAL(clicked()),
                     this,SLOT(modify_content()));

    QObject::connect(ui->pushButton_getpaste,SIGNAL(clicked()),
                     this,SLOT(getText_FromRight()));

    //�Զ����
    QObject::connect(ui->pushbtn_autoindb,SIGNAL(clicked()),
                     this,SLOT(on_pushbtn_autoindb_clicked_self()));
    QObject::connect(ui_autoindb->pushBtn_Open,SIGNAL(clicked()),
                     this,SLOT(on_ui_autoindb_pushBtn_Open_clicked()));
    QObject::connect(ui_autoindb->pushBtn_process,SIGNAL(clicked()),
                     this,SLOT(on_ui_autoindb_pushBtn_process_clicked()));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(get_autoindb_textedit_cursor_postion()));

    //��⣬ѡ���������ݣ���ӵ��Ҳ�
    QObject::connect(timer_checkbox_sel,SIGNAL(timeout()),this,SLOT(pushdb_checkbox_if_selected()));


}
void autoCCode::set_search_text()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui->lineEdit_search->clear();
    ui->lineEdit_search->setText(clipboard->text());
}
void autoCCode::search_text_clear()
{
    ui->lineEdit_search->clear();
    ui->lineEdit_search->setFocus();
}
void autoCCode::note_clear()
{
    if(!InDb_Dialog->isHidden())
        ui_dialog->note_textEdit_dia->clear();
}
void autoCCode::note_focus()
{
    if(!InDb_Dialog->isHidden())
        ui_dialog->note_textEdit_dia->setFocus();
}
void autoCCode::note_clear_focus()
{
    note_clear();
    note_focus();
}

void autoCCode::content_clear()
{
    ui_dialog->content_textEdit_dia->clear();
}
void autoCCode::setCliptext()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui_dialog->note_textEdit_dia->clear();
    ui_dialog->note_textEdit_dia->setText(clipboard->text());
}
void autoCCode::setCliptext_content()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui_dialog->content_textEdit_dia->clear();
    ui_dialog->content_textEdit_dia->setText(clipboard->text());
}
void autoCCode::setDesktop_center(QDialog *dialog)
{
    QDesktopWidget *desk=QApplication::desktop();
    int wd=desk->width();
    int ht=desk->height();
    UNUSEDVAR(wd);
    dialog->move(0,(ht-height()/2)/2);
}

void autoCCode::minSize_ui_dialog()
{
    InDb_Dialog->resize(QSize());
    setDesktop_center(InDb_Dialog);

}
void autoCCode::widthSize_ui_dialog()
{

    InDb_Dialog->resize(QSize(QApplication::desktop()->width(),Qt::MinimumSize));
    QDesktopWidget *desk=QApplication::desktop();
    //    int wd=desk->width();
    int ht=desk->height();
    InDb_Dialog->move(0,(ht-height()/2)/2);
}
void autoCCode::maxSize_ui_dialog()
{

    InDb_Dialog->setMaximumSize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()));
    //    InDb_Dialog->setWindowFlags(InDb_Dialog->windowFlags()& Qt::WindowMaximizeButtonHint & Qt::WindowMinimizeButtonHint);

    //    InDb_Dialog->resize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()));
    InDb_Dialog->resize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()-60));
    InDb_Dialog->move(0,0);
}
void autoCCode::comboBoxSet(void)
{
    self_print(comboBoxSet);
    QObject::connect(this->ui_dia_selectdb->comboBox_selectdb,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(comboBox_selectdb_currentIndexChanged(QString)));
    QObject::connect(this->ui->comboBox_vartype,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(ui_comboBox_vartype_currentIndexChanged(QString)));
    QObject::connect(this->ui_dia_selectdb->comboBox_aspect,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(comboBox_aspect_currentIndexChanged(QString)));


    //���current index of ui_dialog

    //���������Ӧ �ҹ���
    QObject::connect(this->ui->listWidget_codeview,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(listWidget_note_scroll_sync(QListWidgetItem*)));
    //code view with enter Key

    QObject::connect(this->ui->listWidget_codeview,SIGNAL(currentRowChanged(int)),
                     this,SLOT(listWidget_note_with_currentRowChanged(int)));

    QObject::connect(this->ui->listWidget_codeview,SIGNAL(activated(QModelIndex)),
                     this,SLOT(listWidget_note_with_enter(QModelIndex)));


    QObject::connect(this->ui->listWidget_note,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(listWidget_codeview_scroll_sync(QListWidgetItem*)));
}


void autoCCode::comboBox_selectdb_currentIndexChanged(const QString &arg1)
{
    self_print(comboBox_selectdb_currentIndexChanged);
    str_print(arg1);

    selected_langtype = arg1;
    str_print(selected_langtype);
    LanguageType langtype = getLanguageType(selected_langtype);

    CurrentIndex_comboBox_langtype = langtype;

    str_print(CurrentIndex_comboBox_langtype);


    sets = get_table_sets_bytype(langtype);
    if(!sets)
        return;

    QString select_express;
    select_express.clear();
    //str_print(sets->talbename);
    QString aspect = ui_dialog->comboBox_aspect->currentText();
    str_print(aspect);
    if(aspect == "")
    {
        if(ui->radioButton_showall->isChecked())
            select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where lantype='%2' and delflag=0  order by ID desc")
                    .arg(sets->talbename)
                    .arg(selected_langtype);
        else
            select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where lantype='%2' and delflag=0  order by ID desc limit %3")
                    .arg(sets->talbename)
                    .arg(selected_langtype)
                    .arg(LIMIT_SHOW_SIZE);
    }else{
        if(ui->radioButton_showall->isChecked())
            select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where lantype='%2' and aspect_field='%3' and delflag=0 order by ID desc")
                    .arg(sets->talbename)
                    .arg(selected_langtype)
                    .arg(aspect);
        else
            select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where lantype='%2' and aspect_field='%3' and delflag=0 order by ID desc limit %3")
                    .arg(sets->talbename)
                    .arg(selected_langtype)
                    .arg(aspect)
                    .arg(LIMIT_SHOW_SIZE);
    }
    clr_selectresult(selectresult);
    str_print(select_express);
    //gencode str clear
    GenCode_str.clear();

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult,
                     ASPECT_NONE);
    if(showcode_textEdit_AtBotton())
    {
        ui->codeshow_textEdit->setText(selectresult.contentstr);
    }
    clear_listWidget_beforecall();
    ui->listWidget_codeview->addItems(selectresult.keyword_list);
    ui->listWidget_note->addItems(selectresult.note_list);
    dialog_selectdb->close();
}

void autoCCode::textEditSet(void)
{
    self_print(textEditSet);

    //    QObject::connect(ui->db_comboBox,SIGNAL(activated(QString)),
    //                     this,SLOT(on_db_comboBox_activated(QString)));

}
void autoCCode::addstr_aspect_comboBox(void)
{
    LanguageType langtype = languagetype_Aspect_;
    sets = get_table_sets_bytype(langtype);
    if(!sets)
        return;
    QString select_express = QString("select distinct aspect_field from aspect_table;");
    clr_selectresult(selectresult);
    str_print(select_express);
    selectresult.aspect_list<<str_china();
    b.selectdatabase(sets->databasename,
                     select_express.toUtf8().data(),
                     selectresult,
                     ASPECT_HAVE);
    ui_dia_selectdb->comboBox_aspect->clear();
    ui_dia_selectdb->comboBox_aspect->addItems(selectresult.aspect_list);

    ui_dialog->comboBox_aspect->clear();
    ui_dialog->comboBox_aspect->addItems(selectresult.aspect_list);


    //
    aspect_list_mem = selectresult.aspect_list;
    //    update();

}

void autoCCode::addstr_comboBox(void)
{

    self_print(addstr_comboBox);
    QStringList strlist;
    strlist.clear();
    strlist<<str_china()
          <<str_china(header)
         <<str_china(function)
        <<str_china(struct)
       <<str_china(variable);
    ui->comboBox_vartype->addItems(strlist);
    ui_dialog->comboBox_vartype->addItems(strlist);


    strlist.clear();
    strlist<<str_china()
          <<str_china(Android)
         <<str_china(C)
        <<str_china(C++)
       <<str_china(Debug)
      <<str_china(Erlang)
     <<str_china(Jave)
    <<str_china(JavaScript)
    <<str_china(Mysql)
    <<str_china(Oracle)
    <<str_china(Postgresql)
    <<str_china(Rust)
    <<str_china(Sqlite3)
    <<str_china(shell)
    <<str_china(Php)
    <<str_china(Python)
    <<str_china(Qt)
    <<str_china(Qtquick);



    ui_dialog->langtype_comboBox->addItems(strlist);

    //select db dialog add strlist;
    ui_dia_selectdb->comboBox_selectdb->addItems(strlist);

    //����
    addstr_aspect_comboBox();

    //Ĭ������
    ui_dialog->comboBox_vartype->setCurrentIndex(4);  //��� ��������
    ui_dialog->langtype_comboBox->setCurrentIndex(2); //��� ѡ�����ԣ�C��C++��Python etc.
    //    ui_dialog->comboBox_aspect->setCurrentIndex();  //��� ���� ��Ĭ��Ϊlinux
    //    if(ui_dialog->comboBox_aspect->currentText() == "linux")
    //    {

    //    }

}

autoCCode::~autoCCode()
{
    delete ui;
}

void autoCCode::on_save_btn_clicked()
{
    self_print(on_save_btn_clicked);
    QString savefileName = QFileDialog::getSaveFileName(this,
                                                        tr("Open Files"), "", tr("All Files (*.*)"));

    if (savefileName.isNull())
    {
        //fileName���ļ���
        return;
    }
    str_print(savefileName);

    QFile file(savefileName);
    if (!file.open(QIODevice::ReadWrite)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }

    QTextStream out(&file);
    out << ui->genshow_textEdit->toPlainText();

    file.close();

}

void autoCCode::db_comboBox_activated(const QString &arg1)
{
    self_print(db_comboBox_activated);
}
//ѡ�����ݿ�
void autoCCode::on_choseCodeDB_btn_clicked(void)
{
    self_print(on_choseCodeDB_btn_clicked);
    dialog_selectdb->show();

}
//���ɴ����
void autoCCode::on_gencode_btn_clicked(void)
{
    self_print(on_gencode_btn_clicked);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "../",
                                                    tr("Ctype (*.c *.C *.cc *.h)"
                                                       ";;Cpptype(*.cpp *.CPP *.h)"
                                                       ";;QTtype(*.c *.C *.cpp *.CPP *.ui *.rc *.pro *.h)"
                                                       ";;Pythontype(*.py *.PY)"
                                                       ";;JavaType(*.java)"
                                                       ";;All Files(*.*)"));
    //    qDebug()<<"fileName:"<<fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }
    //    QString text_file = file.readAll();
    QString text_china = QString::fromLocal8Bit(file.readAll().data());

    if(showcode_textEdit_AtBotton())
    {
        ui->codeshow_textEdit->setText(text_china);
    }

    //    QTextStream out(&file);
    //    out << "Thomas M. Disch: " << 334 << endl;

    file.close();
}
void autoCCode::hide_inBtn(void)
{
    ui->indb_btn->hide();
}

void autoCCode::show_InBtn(void)
{
    ui->indb_btn->show();
}
//��⺯��
void autoCCode::on_indb_btn_clicked(void)
{
    self_print(on_indb_btn_clicked);
    QString select_text = ui->codeshow_textEdit->textCursor().selectedText();
    ui_dialog->content_textEdit_dia->setText(select_text);

    if(ui->checkBox_inbox->isChecked())
    {
        InDb_Dialog->show();
        //        InDb_Dialog->exec();
    }else{
        InDb_Dialog->show();
    }
}

void autoCCode::on_indb_window_show_hide()
{
    self_print(on_indb_btn_clicked);
    QString select_text = ui->codeshow_textEdit->textCursor().selectedText();
    ui_dialog->content_textEdit_dia->setText(select_text);

    if(ui->checkBox_inbox->isChecked())
    {
        if(InDb_Dialog->isHidden())
            InDb_Dialog->show();
        else
            InDb_Dialog->hide();
        //        InDb_Dialog->exec();
    }else{
        if(InDb_Dialog->isHidden())
            InDb_Dialog->show();
        else
            InDb_Dialog->hide();
    }
}

//void autoCCode::on_outdb_btn_clicked(void)
//{
//    self_print(on_outdb_btn_clicked);
//}

LanguageType autoCCode::getLanguageType(QString &type)
{

    if(type == "C"){
        return languagetype_C_;
    }else if(type == "Qt"){
        return languagetype_Qt_;
    }
    else if(type == "Python"){
        return languagetype_Python_;
    }
    else if(type == "Php"){
        return languagetype_Php_;
    }
    else if(type == "Qtquick"){
        return languagetype_Qtquick_;
    }
    else if(type == "shell")
    {
        return languagetype_Shell_;
    }
    else if(type == "Jave"){
        return languagetype_Jave_;
    }else if(type == "C++"){
        return languagetype_Cpp_;
    }
    else if(type == "Oracle")
    {
        return languagetype_Oracle_;
    }
    else if(type == "Erlang")
    {
        return languagetype_Erlang_;
    }

    else if(type == "Mysql")
    {
        return languagetype_Mysql_;
    }
    else if(type == "Sqlite3")
    {
        return languagetype_Sqlite3_;
    }
    else if(type == "Postgresql")
    {
        return languagetype_Postgresql_;
    }
    else if(type == "Rust")
    {
        return languagetype_Rust_;
    }
    else if(type == "Android")
    {
        return languagetype_Android_;
    }
    else if(type == "JavaScript")
    {
        return languagetype_JavaScript_;
    }
    else if(type == "Debug")
    {
        return languagetype_Debug_;
    }

    else{
        return languagetype_Err_;
    }
}

void autoCCode::save_before_ops(void)
{
    selectresult_tmp = selectresult;
    index_key_color_tmp = index_key_color;
    index_note_color_tmp = index_note_color;
}
void autoCCode::restore_before_ops(void)
{
    selectresult = selectresult_tmp;
    index_key_color = index_key_color_tmp ;
    index_note_color = index_note_color_tmp ;
}

//dialog ok button
void autoCCode::ok_btn_dia_clicked_self(void)
{
    self_print(ok_btn_dia_clicked_self);

    //��ȡ����
    QString content = ui_dialog->content_textEdit_dia->toPlainText().trimmed();
    QString lanaugetype = ui_dialog->langtype_comboBox->currentText();
    QString index_keyword   = ui_dialog->index_textEdit_dia->toPlainText().trimmed();
    index_keyword = index_keyword.replace("\n"," ");
    //    index_keyword.trimmed();
    QString note   = ui_dialog->note_textEdit_dia->toPlainText().trimmed();
    note = note.replace("\n"," ");
    note += "\t\t\t\t";
    note += QDateTime::currentDateTime().toString("yyyy MMM d ddd,hh:mm:ss");

    //    QDateTime d = lo.toDateTime("Mon,26 Apr 2010, 08:21:03","ddd,d MMM yyyy, hh:mm:ss");
    //    Q_ASSERT(d.isValid());

    //    note.trimmed();
    QString vartype = ui_dialog->comboBox_vartype->currentText();
    QString aspect = ui_dialog->comboBox_aspect->currentText();


    if(lanaugetype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes , QMessageBox::Yes);
        return;
    }
    if(content.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes , QMessageBox::Yes);
        return;
    }

    LanguageType langtype = getLanguageType(lanaugetype);
    sets = get_table_sets_bytype(langtype);
    if(!sets)
        return;

    QString select_express = QString("select content from %1 where lantype='%2' and content='%3' and vartype='%4' and delflag=0 order by ID desc")
            .arg(sets->talbename)
            .arg(lanaugetype)
            .arg(content)
            .arg(vartype);
    save_before_ops();

    clr_selectresult(selectresult);
    str_print(select_express);
    b.selectdatabase(sets->databasename,
                     select_express.toUtf8().data(),
                     selectresult,
                     ASPECT_NONE);

    if(selectresult.existflag)
    {
        restore_before_ops();
        QMessageBox::information(NULL, str_china(����), str_china(�����Ѿ�����), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(vartype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(��������), str_china(����Ϊ��), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(index_keyword.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(note.isEmpty())
    {
        QMessageBox::information(NULL, str_china(ע��), str_china(����Ϊ��), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    //    if(aspect.isEmpty())
    //    {
    //        QMessageBox::information(NULL, str_china(��Χ), str_china(����Ϊ��), QMessageBox::Yes, QMessageBox::Yes);
    //        return;
    //    }


    clr_selectresult(selectresult_tmp);

    InsertCon insertcontent;
    insertcontent.content = content;
    insertcontent.languagetype = getLanguageType(lanaugetype);
    insertcontent.keyword   = index_keyword;
    insertcontent.note      = note;
    insertcontent.vartype   = vartype;
    insertcontent.aspect    = aspect;

#ifndef DEBUG_V

    if(ui_dialog->checkBox_EOR->isChecked())
        ui_dialog->note_textEdit_dia->clear();

    if(ui->checkBox_inbox->isChecked())
    {
        //�Ի��򲻹ر�
        ui_dialog->content_textEdit_dia->clear();
    }else{
        InDb_Dialog->update();
        InDb_Dialog->close();
    }


#else
    //�Ի��򲻹ر�
    ui_dialog->content_textEdit_dia->clear();
#endif

    b.creatable(&insertcontent);
    b.inserttable(&insertcontent);


    //������Ӻ󣬸��¿ؼ������ݵ������ʾ
    update_show_after_insert();
}

void autoCCode::cancel_btn_dia_clicked_self(void)
{
    self_print(cancel_btn_dia_clicked_self);
    InDb_Dialog->close();
}
void autoCCode::aboutVersion(void)
{
    QMessageBox::information(NULL, str_china(�汾), GetVersion(),NULL,NULL);
    return;
}
QString autoCCode::GetVersion(void)
{
    return str_china(�Զ����ɴ���)
            +"\n"
            +str_china(byСκ��)
            +"\n"
            +version_autoccode;
}

void autoCCode::listWidgetSet(void)
{
    self_print(listWidget);
#if 0 //˫��
    QObject::connect(ui->listWidget_codeview,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_keyword(QListWidgetItem*)));
    QObject::connect(ui->listWidget_note,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_note(QListWidgetItem*)));
#else//����
    QObject::connect(ui->listWidget_codeview,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_keyword(QListWidgetItem*)));
    //˫��
    QObject::connect(ui->listWidget_note,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_note(QListWidgetItem*)));
#if 0//��ѡ

    QObject::connect(ui->listWidget_codeview,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_keyword(QListWidgetItem*)));
#endif//end ��ѡ


#endif //end ˫��


}
//��ӵ��ұߵ�������
void autoCCode::add_to_gen_code_textedit_by_keyword(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit);
    rightTextShowClear_oncheched();
    QString str = item->text();
    unsigned int index = 0;
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.keyword_list.at(i))
            index = i;
        //        qDebug()<<"note list:"<<selectresult.note_list.at(i);
    }

    str_print(str);

    GenCode_str+="/*  ";
    GenCode_str+=selectresult.note_list.at(index);
    GenCode_str+="   */";
    GenCode_str+="\n";
    GenCode_str+=selectresult.content_list.at(index);
    GenCode_str+="\n";
    GenCode_str+="\n";


    ui->genshow_textEdit->setText(GenCode_str);
    SearchTextResWithColor(GenCode_str);
    ui->genshow_textEdit->moveCursor(QTextCursor::End);
    ui->listWidget_codeview->setFocus();
    //    update();
}
//��ӵ��ұߵ�������
void autoCCode::add_to_gen_code_textedit_by_note(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit);
    rightTextShowClear_oncheched();
    QString str = item->text();
    unsigned int index = 0;
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.note_list.at(i))
            index = i;
        //        qDebug()<<"note list:"<<selectresult.note_list.at(i);
    }

    str_print(str);

    GenCode_str+="/*  ";
    GenCode_str+=selectresult.note_list.at(index);
    GenCode_str+="   */";
    GenCode_str+="\n";
    GenCode_str+=selectresult.content_list.at(index);
    GenCode_str+="\n";
    GenCode_str+="\n";


    ui->genshow_textEdit->setText(GenCode_str);
    SearchTextResWithColor(GenCode_str);
    //    update();
}


void autoCCode::rightClear_textedit(void)
{
    GenCode_str.clear();
    ui->genshow_textEdit->clear();
    //    update();
}
void autoCCode::clr_selectresult(SelectResult &result)
{
    result.content_list.clear();
    result.contentstr.clear();
    result.keyword_list.clear();
    result.note_list.clear();
    result.vartype_list.clear();
    result.existflag = 0;
    result.aspect_list.clear();
    result.aspect_field.clear();

    //ÿ�����¿�ʼ��¼���������ʱ���������
    index_key_color = 0;
    index_note_color = 0;


}
void autoCCode::clr_looktexthisresult(LookTextHistoryResult &result)
{
    result.looktextarry.clear();
    result.looktimes = 0;
}

void autoCCode::clear_listWidget_beforecall(void)
{
    ui->listWidget_codeview->clear();
    ui->listWidget_note->clear();
    //    update();
}
void autoCCode::listWidget_scrollToBottom(void)
{
    ui->listWidget_codeview->scrollToBottom();
    ui->listWidget_note->scrollToBottom();
    //    update();
}
void autoCCode::listWidget_scrollToTop(void)
{
    ui->listWidget_codeview->scrollToTop();
    ui->listWidget_note->scrollToTop();
    //    update();
}
void autoCCode::select_db_by_vartype(QString &select_express)
{

    if(!sets)
        return;
    //str_print(sets->talbename);
    clr_selectresult(selectresult);

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult,
                     ASPECT_NONE);


    if(showcode_textEdit_AtBotton())
    {
        ui->codeshow_textEdit->setText(selectresult.contentstr);
    }

    clear_listWidget_beforecall();
    ui->listWidget_codeview->addItems(selectresult.keyword_list);
    ui->listWidget_note->addItems(selectresult.note_list);

    listWidget_scrollToTop();
    //    update();
}

void autoCCode::ui_comboBox_vartype_currentIndexChanged(const QString &str)
{
    self_print(ui_comboBox_vartype_currentIndexChanged);
    str_print(str);

    if(!sets)
        return;

    if(str.contains("header")){
        QString select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where vartype='%2' and delflag=0 order by ID desc")
                .arg(sets->talbename)
                .arg("header");
        select_db_by_vartype(select_express);
    }else if(str.contains("function")){
        QString select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where vartype='%2' and delflag=0 order by ID desc")
                .arg(sets->talbename)
                .arg("function");
        select_db_by_vartype(select_express);
    }else if(str.contains("struct")){
        QString select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where vartype='%2' and delflag=0 order by ID desc")
                .arg(sets->talbename)
                .arg("struct");
        select_db_by_vartype(select_express);
    }
    else if(str.contains("variable")){
        QString select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where vartype='%2' and delflag=0 order by ID desc")
                .arg(sets->talbename)
                .arg("variable");
        select_db_by_vartype(select_express);
    }
    else{
        //        str_print(sets->langtype);
        QString select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where lantype='%2' and delflag=0 order by ID desc")
                .arg(sets->talbename)
                .arg(getLanguageStr(sets->langtype));
        select_db_by_vartype(select_express);
    }

}

void autoCCode::comboBox_aspect_currentIndexChanged(const QString &str)
{
    self_print(comboBox_aspect_currentIndexChanged);
    str_print(str);

    if(!str.isEmpty())
    {

    }

}
void autoCCode::add_aspect_totable(void)
{
    QString aspect_str = ui_dialog->lineEdit_add_aspect->text();
    str_print(aspect_str);
    if(aspect_str.isEmpty())
    {
        return;
    }

    InsertCon insertcontent;
    //    insertcontent.content = content;
    insertcontent.languagetype = languagetype_Aspect_;
    insertcontent.aspect       = aspect_str;
    //    insertcontent.keyword   = index_keyword;
    //    insertcontent.note      = note;
    //    insertcontent.vartype   = vartype;

    b.creatable(&insertcontent);
    b.inserttable(&insertcontent);

    //����
    addstr_aspect_comboBox();

}

void autoCCode::judge_color_index(void)
{
    if(index_key_color > selectresult.content_list.size())
    {
        index_key_color=0;
        index_note_color = 0;
        index_key_color_tmp = 0;
        index_note_color_tmp = 0;
    }

}

//�ұߵ�ע��ͬ��������ѡ��
void autoCCode::listWidget_note_scroll_sync(QListWidgetItem* item)
{
    self_print(listWidget_note_scroll_sync);
    unsigned int index = 0;
    QString str = item->text();
    if(str.isEmpty())
        return;
    if(selectresult.content_list.size() == 0)
        return;
    judge_color_index();
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.keyword_list.at(i))
            index = i;
    }

    str_print(str);
    str_print(index);
    ui->listWidget_note->setCurrentRow(index);
    ui->listWidget_note->item(index_key_color)->setBackgroundColor(Qt::white);
    ui->listWidget_note->item(index)->setBackgroundColor(Qt::green);
    index_key_color = index; //
    str_print(index_key_color);

    ui->listWidget_codeview->item(index_note_color)->setBackgroundColor(Qt::white);
    flag_selectLeft = 0 ;
    //    update();
}
//note�������
void autoCCode::listWidget_codeview_scroll_sync(QListWidgetItem* item)
{
    self_print(listWidget_codeview_scroll_sync);
    unsigned int index = 0;
    QString str = item->text();
    str_print(selectresult.existflag);
    if(str.isEmpty())
        return;
    if(selectresult.content_list.size() == 0)
        return;
    judge_color_index();
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.note_list.at(i))
            index = i;
    }
    ui->listWidget_codeview->setCurrentRow(index);
    ui->listWidget_codeview->item(index_note_color)->setBackgroundColor(Qt::white);
    ui->listWidget_codeview->item(index)->setBackgroundColor(Qt::red);

    index_note_color = index; //


    ui->listWidget_note->item(index_key_color)->setBackgroundColor(Qt::white);


    flag_selectLeft = 1 ;
    str_print( flag_selectLeft );
    //    update();
}

void autoCCode::update_show_after_insert(void)
{
    if(!sets)
        return;
    QString select_express;
    select_express.clear();
    if(ui->radioButton_showall->isChecked())
        select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2' and delflag=0 order by ID desc")
                .arg(sets->talbename)
                .arg(getLanguageStr(sets->langtype));
    else
        select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2' and delflag=0 order by ID desc limit %3")
                .arg(sets->talbename)
                .arg(getLanguageStr(sets->langtype))
                .arg(LIMIT_SHOW_SIZE);
    select_db_by_vartype(select_express);
}
void autoCCode::delete_btn_clicked_selfdefine(void)
{
    self_print(delete_btn_clicked);
    if(!sets)
        return;
    //����ɾ������bug
    if(0 == selectresult.existflag )
        return;

    //    if()
    str_print(index_key_color);
    //���ж���һ����ɾ���ɣ����note�Ҳ�����ݽ���ɾ��ʱ��������
    if( flag_selectLeft )
    {
        /*  ��׼�Ի��򡪡���ʾ��Ϣ��   */
        QMessageBox::warning(NULL,"Warning",
                             str_china(��ѡ���Ҳ����ɾ��),
                             QMessageBox::Yes,QMessageBox::Yes);
        //        /*  ��׼�Ի��򡪡���ʾ��Ϣ��   */
        //        QMessageBox::warning(NULL,"Warning",
        //                             str_china(��ѡ��������ɾ��),
        //                             QMessageBox::Yes,QMessageBox::Yes);
        return;
    }

    /*  ����Ի���   */
    bool isOK;
    QString text = QInputDialog::getText(this,"Input Dialog",
                                         "Please Press Ok to delete",
                                         QLineEdit::Normal,
                                         selectresult.keyword_list.at(index_key_color),
                                         &isOK);
    if(isOK)
    {
        str_print(index_key_color);
        QString select_express = QString("update %1 set delflag=1 where keywords='%2'")
                .arg(sets->talbename)
                .arg(selectresult.keyword_list.at(index_key_color));

        b.updatetable(sets->langtype,select_express);

        update_show_after_insert();
        QMessageBox::information(this,"Information",
                                 "Your comment:<h1><font color=red>" +text +"</font></h1>"+" deleted!",
                                 QMessageBox::Yes,QMessageBox::Yes);
    }





}

void autoCCode::SearchText_WithTimer(void)
{
    QString searchStr = ui->lineEdit_search->text();
    static QString oldStr;
    if(oldStr != searchStr)
        SearchText(searchStr);
    oldStr = searchStr;

}


void autoCCode::SearchText(const QString &searchStr)
{
    self_print(SearchText);
    str_print(searchStr);

    if(!sets)
        return;
    if(searchStr.isEmpty()){
        update_show_after_insert();
        return;
    }
    judge_color_index();
    str_print(selectresult.content_list.size());

    clr_selectresult(selectresult);

    //������ҹؼ��� begin
    codestructSets *setsLookHis = get_table_sets_bytype(languagetype_LookTextHis_);
    if(!setsLookHis)
        return;

    LookTextHistoryResult looktexthistoryres;
    clr_looktexthisresult(looktexthistoryres);

    QString looktexthis_express;
    looktexthis_express.clear();
    looktexthis_express = QString("select * from %1 order by ID desc")
            .arg(setsLookHis->talbename);

    b.searchdatabase_lookTextHisTbl(setsLookHis->databasename,looktexthis_express.toLocal8Bit().data(),
                     looktexthistoryres,
                     searchStr);

    if(0 == looktexthistoryres.looktimes)
    {//����һ������
        //qDebug() << "insert looktimes:" << looktexthistoryres.looktimes;
        looktexthis_express.clear();
        looktexthis_express = QString("insert into %1([looktextname],[lowercase_looktextname] ,[looktimes])  VALUES('%2','%3','%4')")
                .arg(TBL_LOOKTEXTHIS)
                .arg(searchStr)
                .arg(searchStr.toLower())
                .arg(looktexthistoryres.looktimes+1);

//        //qDebug() << "databasename:" << setsLookHis->databasename;
//        //qDebug() << "express:" << setsLookHis->creat_table_express;

        b.opendatabase(setsLookHis->databasename,setsLookHis->creat_table_express);
        b.insertdatabase(setsLookHis->databasename,looktexthis_express.toLocal8Bit().data());
    }
    else
    {//��������
        //qDebug() << "else branch";
        //qDebug() << "insert looktimes:" << looktexthistoryres.looktimes;

        looktexthis_express = QString("update %1 set looktimes=%2 where looktextname='%3'")
                .arg(setsLookHis->talbename)
                .arg(looktexthistoryres.looktimes+1)
                .arg(searchStr);

        //qDebug() << "databasename:" << setsLookHis->databasename;
        //qDebug() << "express:" << setsLookHis->creat_table_express;

        b.opendatabase(setsLookHis->databasename,setsLookHis->creat_table_express);
        b.insertdatabase(setsLookHis->databasename,looktexthis_express.toLocal8Bit().data());
    }


    qDebug() << "looktimes:" << looktexthistoryres.looktimes;

    //������ҹؼ��� end



    //���Ҳ���������
    QString select_express;
    select_express.clear();
    select_express = QString("select lowercase_keyworks,keywords,content,lantype,note,vartype from %1 where lantype='%2' and delflag=0 order by ID desc")
            .arg(sets->talbename)
            .arg(getLanguageStr(sets->langtype));


    clr_selectresult(selectresult);

    str_print(select_express);

    b.searchdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult,
                     searchStr.toLower());
    if(showcode_textEdit_AtBotton())
    {
        ui->codeshow_textEdit->setText(selectresult.contentstr);
    }

    clear_listWidget_beforecall();
    if(0 == selectresult.keyword_list.length()){
        alert();
    }else{
        ui->listWidget_codeview->addItems(selectresult.keyword_list);
        ui->listWidget_note->addItems(selectresult.note_list);
    }
}
void autoCCode::cleanLineTextEditSearch(void)
{
    ui->lineEdit_search->setFocus();
    if(ui->lineEdit_search->text().isEmpty())
        return;
    ui->lineEdit_search->clear();
    update_show_after_insert();
}



//Ϊ���б��е�lowercase_keyword�������
void autoCCode::add_column_lowercase_keywords_content(void)
{
    self_print(add_column_lowercase_keywords);
    if(!sets)
        return;
    //����ɾ������bug
    if(0 == selectresult.existflag )
        return;


    QString select_express;
    select_express.clear();
    select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2' order by ID desc")
            .arg(sets->talbename)
            .arg(getLanguageStr(sets->langtype));
    clr_selectresult(selectresult);
    str_print(select_express);
    GenCode_str.clear();

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult,
                     ASPECT_NONE);
    if(showcode_textEdit_AtBotton())
    {
        ui->codeshow_textEdit->setText(selectresult.contentstr);
    }

    clear_listWidget_beforecall();
    ui->listWidget_codeview->addItems(selectresult.keyword_list);
    ui->listWidget_note->addItems(selectresult.note_list);

    //    return;

    QStringList::const_iterator iterator = selectresult.keyword_list.begin();

    while( iterator != selectresult.keyword_list.end()){
        //        cout << (*iterator).toAscii().data() << endl;
        QString tmp =(*iterator).toLower();
        QString repalceafter = tmp.replace("\'","\'\'");
        select_express = QString("update %1 set lowercase_keyworks='%2' where keywords='%3' and lantype='%4'")
                .arg(sets->talbename)
                .arg(repalceafter)
                .arg((*iterator))
                .arg(getLanguageStr(sets->langtype));

        str_print(select_express);
        b.updatetable(sets->langtype,select_express);

        ++iterator;
    }




}

void autoCCode::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }
}
void autoCCode::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty()){
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty()){
        return;
    }
    this->setWindowTitle(fileName);
    readTextFile(fileName);
}

void autoCCode::readTextFile(const QString &fileName)
{
    str_print(fileName);
    /*  ��ȡ�ļ� ֻ��   */
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        ui->codeshow_textEdit->setText(stream.readAll());
    }
    file.close();
}

void autoCCode::SearchEnter()
{
    self_print(SearchEnter);
    SetlistWidget_codeview_row(0);
    this->ui->listWidget_codeview->setFocus();
    this->ui->listWidget_codeview->setModelColumn(GetlistWidget_codeview_row());


}


void autoCCode::listWidget_note_with_currentRowChanged(int row)
{
    //    qt�����ʱ
    static QElapsedTimer elapseTimer;
    static quint8 IsTimerStarted = FALSE;
    static qint64 time_begin = 0; //���ش��ϴ�start()��restart()��ʼ������ʱ����λms
    static qint64 time_end   = 0; //���ش��ϴ�start()��restart()��ʼ������ʱ����λms;

    if(!IsTimerStarted)
    {
        elapseTimer.start();
        IsTimerStarted = TRUE;
    }

    //��ʼ��¼ʱ��
    time_begin = elapseTimer.elapsed();
    qint64 time_interval = (time_end > time_begin)?(time_end - time_begin):(time_begin - time_end);
    //    qDebug() << "time_begin :" << time_begin;
    //    qDebug() << "time_end   :" << time_end;
    qDebug() << "time diff:" << time_interval;
    if(time_interval > 100)
    {
        //        qDebug() << "time_begin :" << time_begin;


        self_print(listWidget_note_with_currentRowChanged);
        //    qDebug()<<"row:"<<row<<endl;
        SetlistWidget_codeview_row(row);
        //change color
        unsigned int index = GetlistWidget_codeview_row();
        if(selectresult.content_list.size() == 0)
            return;
        judge_color_index();

        ui->listWidget_note->setCurrentRow(index);
        ui->listWidget_note->item(index_key_color)->setBackgroundColor(Qt::white);
        ui->listWidget_note->item(index)->setBackgroundColor(Qt::green);
        index_key_color = index; //
        str_print(index_key_color);

        ui->listWidget_codeview->item(index_note_color)->setBackgroundColor(Qt::white);
        flag_selectLeft = 0 ;

        GenCode_str.clear();
        GenCode_str+="/*  ";
        GenCode_str+=selectresult.note_list.at(index);
        GenCode_str+="   */";
        GenCode_str+="\n";
        GenCode_str+=selectresult.content_list.at(index);
        GenCode_str+="\n";
        GenCode_str+="\n";

        ui->genshow_textEdit->setText(GenCode_str);
        SearchTextResWithColor(GenCode_str);
        //    setCharColor(10);
        //    ui->genshow_textEdit->setHtml(GenCode_str);
        //        ui->genshow_textEdit->moveCursor(QTextCursor::End);
        //        ui->listWidget_codeview->setFocus();
    }
    /* ��¼����ʱ�� */
    if(elapseTimer.isValid())
    {
        time_end = elapseTimer.elapsed(); //���ش��ϴ�start()��restart()��ʼ������ʱ����λms;
        //        qDebug() << "time_end   :" << time_end;
    }

}

void autoCCode::SetlistWidget_codeview_row(int row)
{
    self_print(SetlistWidget_codeview_row);
    if(row < 0)
        row = 0;
    listWidget_codeview_row = row;
}
int autoCCode::GetlistWidget_codeview_row(void)
{
    self_print(GetlistWidget_codeview_row);
    return listWidget_codeview_row;
}

/*  QT:����textedit�ı�����ĳ���ַ��ĸ�ʽ */
void autoCCode::setCharColor(unsigned int pos)
{
    if(pos <= 0)return ;
    QTextCursor cursor = ui->genshow_textEdit->textCursor();//ui->view1->textCursor();
    cursor.movePosition( QTextCursor::StartOfLine );//����
    cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, pos-1);//�����ƶ���Pos
    cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
    ui->genshow_textEdit->setTextCursor( cursor ); // added
    QTextCharFormat defcharfmt = ui->genshow_textEdit->currentCharFormat();
    QTextCharFormat newcharfmt = defcharfmt;
    newcharfmt.setFontUnderline( true );
    newcharfmt.setUnderlineColor( QColor( Qt::red ) );
    newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );
    ui->genshow_textEdit->setCurrentCharFormat( newcharfmt );

    cursor.movePosition( QTextCursor::PreviousCharacter );//���������Ϊ��ȥ�����selected
    ui->genshow_textEdit->setTextCursor( cursor ); // added
    // ui->genshow_textEdit->setCurrentCharFormat( defcharfmt );
    ui->genshow_textEdit->setFocus();
}

/*  QT:����textedit�ı�����ĳ���ַ����ĸ�ʽ */
void autoCCode::setStringColor(unsigned int pos,unsigned int len)
{
    unsigned int i = 0;

    if((0 == len)||(0 == pos))
    {
        return;
    }

    QTextCursor cursor = ui->genshow_textEdit->textCursor();//ui->view1->textCursor();
    cursor.movePosition( QTextCursor::StartOfLine);//����
    cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::MoveAnchor, pos >=1? (pos-1):0);//�����ƶ���Pos
    for(i = 0;i < len;i++){
        cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
    }
    ui->genshow_textEdit->setTextCursor( cursor ); // added

    QTextCharFormat defcharfmt = ui->genshow_textEdit->currentCharFormat();
    QTextCharFormat newcharfmt = defcharfmt;


    if(len >0)
    {

        newcharfmt.setFontUnderline( true );
        newcharfmt.setUnderlineColor( QColor( Qt::red ) );
        newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );
    }
    else
    {

        newcharfmt.setFontUnderline( false );
        //        newcharfmt.setUnderlineColor( QColor( Qt::red ) );
        //        newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );
    }


    ui->genshow_textEdit->setCurrentCharFormat( newcharfmt );

    cursor.movePosition( QTextCursor::PreviousCharacter );//���������Ϊ��ȥ�����selected
    ui->genshow_textEdit->setTextCursor( cursor ); // added
    ui->genshow_textEdit->setCurrentCharFormat( defcharfmt );
    //    ui->genshow_textEdit->setFocus();
}

void autoCCode::SearchTextResWithColor(QString &resStr)
{
    //��ɫ���Ƿ�ѡ��
    if(!ui->checkBox_ResWithColor->isChecked())
    {
        //        //������ɫ��ǰ����ɫ
        //        ui->genshow_textEdit->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);");
        setStringColor(0, 0);
        return;
    }
    //    //����ǳ��ɫ��ǰ����ɫ
    //    ui->genshow_textEdit->setStyleSheet("background-color: rgb(60, 243, 243);color: rgb(0, 0, 0);");
    QString searchText = ui->lineEdit_search->text();
    if(searchText.isEmpty())
    {
        return;
    }
#if 1
    //    if(resStr.contains(searchText))
    //    {
    //        int pos = resStr.indexOf(searchText);
    //        qDebug() <<"pos:" << pos;
    //        setStringColor(pos + 1, searchText.length());
    //    }

    //    QString str = "We must be <b>bold</b>, very <b>bold</b>";
    //    qDebug() <<"defaultCursorMoveStyle" << ui->genshow_textEdit->document()->defaultCursorMoveStyle();
    int j = 0;
    if(resStr.contains(searchText))
    {
        qDebug() << "-->>>> ";
        qDebug() << "searchText len: " <<  searchText.length();
        while ((j = resStr.indexOf(searchText.toLatin1(), j, Qt::CaseInsensitive)) != -1) {
            //            qDebug() << "Found "+ searchText + " tag at index position:  " << j;
            setStringColor(j + 1, searchText.length());
            ++j;
        }
    }

#else

    QString str = "We must be <b>bold</b>, very <b>bold</b>";
    int j = 0;

    while ((j = str.indexOf("<b>", j)) != -1) {
        qDebug() << "Found <b> tag at index position" << j;
        ++j;
    }
#endif

    return;

    //    QString searchTextWithColor = "<font color=blue size=4>" + searchText +"</font>";
    //    QString resStrWithHtml = QString::fromLocal8Bit("<html><body>%1</body></html>").arg(resStr);
    //    //    QString resStrWithHtml = resStr;
    //    if(resStrWithHtml.contains(searchText, Qt::CaseInsensitive)) //���ܴ�Сд
    //    {
    //        resStrWithHtml.replace(searchText, searchTextWithColor);
    //        resStrWithHtml.replace("\r", "<p>");
    //        int pos = resStrWithHtml.indexOf(searchText);
    //        qDebug() <<"pos:" << pos;
    //        setCharColor(pos);
    //        resStr = resStrWithHtml;
    //    }
}

void autoCCode::listWidget_note_with_enter(const QModelIndex &modelindex)
{
    self_print(listWidget_note_with_enter);
    rightTextShowClear_oncheched();
    //    qDebug()<<"index:"<<modelindex.row();
    unsigned int index = GetlistWidget_codeview_row();



    GenCode_str+="/*  ";
    GenCode_str+=selectresult.note_list.at(index);
    GenCode_str+="   */";
    GenCode_str+="\n";
    GenCode_str+=selectresult.content_list.at(index);
    GenCode_str+="\n";
    GenCode_str+="\n";

    ui->genshow_textEdit->setText(GenCode_str);
    SearchTextResWithColor(GenCode_str);
    //    setCharColor(10);
    //    ui->genshow_textEdit->setHtml(GenCode_str);
    ui->genshow_textEdit->moveCursor(QTextCursor::End);
    ui->listWidget_codeview->setFocus();

}

void autoCCode::contentSetFocus(void)
{
    ui->lineEdit_search->clear();
    ui->lineEdit_search->setFocus();
}

void autoCCode::modify_content()
{
    self_print(modify_content);
    if(!sets)
        return;
    //����ɾ������bug
    if(0 == selectresult.existflag )
        return;

    //    if()
    str_print(index_key_color);
    //���ж���һ����ɾ���ɣ����note�Ҳ�����ݽ���ɾ��ʱ��������
    if( flag_selectLeft )
    {
        /*  ��׼�Ի��򡪡���ʾ��Ϣ��   */
        QMessageBox::warning(NULL,"Warning",
                             str_china(��ѡ���������޸�),
                             QMessageBox::Yes,QMessageBox::Yes);
        //        /*  ��׼�Ի��򡪡���ʾ��Ϣ��   */
        //        QMessageBox::warning(NULL,"Warning",
        //                             str_china(��ѡ��������ɾ��),
        //                             QMessageBox::Yes,QMessageBox::Yes);
        return;
    }

    str_print(index_key_color);


    ui_dialog->note_textEdit_dia->setText(selectresult.note_list.at(index_key_color));
    ui_dialog->content_textEdit_dia->setText(selectresult.content_list.at(index_key_color));
    ui_dialog->index_textEdit_dia->setText(selectresult.keyword_list.at(index_key_color));

    str_print(selectresult.vartype_list.at(index_key_color));
    str_print(selectresult.aspect_field.at(index_key_color));
    str_print(get_aspect_list_index(selectresult.aspect_field.at(index_key_color)));
    //    ui_dialog->comboBox_aspect->setCurrentIndex(get_aspect_list_index(selectresult.aspect_field.at(index_key_color)));

    ui_dialog->comboBox_vartype->setCurrentIndex(get_CurrentIndex_comboBox_vartype(selectresult.vartype_list.at(index_key_color)));

    ui_dialog->langtype_comboBox->setCurrentIndex(CurrentIndex_comboBox_langtype);


    InDb_Dialog->show();

}


int autoCCode::get_CurrentIndex_comboBox_vartype(const QString & vartype)
{
    if(vartype ==  str_china())
        return 0;
    else if (vartype ==  str_china(header))
        return 1;
    else if (vartype ==  str_china(function))
        return 2;
    else if (vartype ==  str_china(struct))
        return 3;
    else if (vartype ==  str_china(variable))
        return 4;
    else
        return 0;
}

int autoCCode::get_aspect_list_index(const QString &index_str)
{

    int index = 0;
    QStringList list = aspect_list_mem;

    QStringList::const_iterator iterator = list.begin();
    while( iterator != list.end()){
        if((*iterator) == index_str)
            return index;
        ++iterator;
        index++;
    }
    return 0;
}

void autoCCode::rightTextShowClear_oncheched()
{
    if(ui->checkBox_rightTextClean->isChecked())//�ұ��������
        rightClear_textedit();
}

void autoCCode::getText_FromRight(void)
{
    self_print(getText_FromRight);
    QString str;
    str.clear();
    str = ui->genshow_textEdit->toPlainText();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(str,QClipboard::Clipboard);
}

void autoCCode::on_pushbtn_autoindb_clicked_self()
{
    self_print(on_pushbtn_autoindb_clicked_self);
    //    if(ui_autoindb->)
    if(dialog_autoindb->isHidden()){
        dialog_autoindb->show();
        dialog_autoindb->update();
    }

}
void autoCCode::on_ui_autoindb_pushBtn_Open_clicked()
{
    self_print(on_ui_autoindb_pushBtn_Open_clicked);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "../",
                                                    tr("Ctype (*.c *.C *.cc *.h)"
                                                       ";;Cpptype(*.cpp *.CPP *.h)"
                                                       ";;QTtype(*.c *.C *.cpp *.CPP *.ui *.rc *.pro *.h)"
                                                       ";;Pythontype(*.py *.PY)"
                                                       ";;JavaType(*.java)"
                                                       ";;All Files(*.*)"));
    //    qDebug()<<"fileName:"<<fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }
    //    QString text_file = file.readAll();
    QString text_china = QString::fromLocal8Bit(file.readAll().data());

    ui_autoindb->textEdit_show->setText(text_china);

    //    QTextStream out(&file);
    //    out << "Thomas M. Disch: " << 334 << endl;

    file.close();
}
void autoCCode::on_ui_autoindb_pushBtn_process_clicked()
{
    self_print(on_ui_autoindb_pushBtn_process_clicked);

    int retbtn = QMessageBox::information(this,str_china(��ȷ����⹲�б�������),
                                          cursor_left_text,
                                          QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    str_print(retbtn);
    if(0x00004000 == retbtn)//0x00004000 yesbutton
    {
        str_print(dialog_autoindb);
#if 1
        QStringList::const_iterator iterator = textlist_indb_content.begin();
        int counter = 1;
        int returnval = 0;
#if 0 //�������룬��֪��ʲô���
        while( iterator != textlist_indb_content.end()){
            //            qDebug() << (*iterator).toAscii().data();
            ok_btn_dia_clicked_self_autoindb((*iterator),&returnval);
            if(returnval >0)
                break;
            //            qDebug()<<"current value:"<< counter <<endl;
            ProgressBarSetValue((counter*100)/textlist_indb_content.count());
            ++iterator;
            ++counter;
        }
#else
        for(int i = 0; i< textlist_indb_content.count() ; i++){
            ok_btn_dia_clicked_self_autoindb(cursor_left_text,textlist_indb_content.at(i),&returnval);
            if(returnval >0)
                break;
            //            qDebug()<<"current value:"<< counter <<endl;
            ProgressBarSetValue((counter*100)/textlist_indb_content.count());
            ++counter;
        }

#endif
        //������Ӻ󣬸��¿ؼ������ݵ������ʾ
        update_show_after_insert();
#endif
    }
}
void autoCCode::get_autoindb_textedit_cursor_postion()
{
    //    self_print(get_autoindb_textedit_cursor_postion);
    if(!dialog_autoindb->isHidden())
    {
        static int linenumber_old = -1;
        int linenumber = ui_autoindb->textEdit_show->textCursor().blockNumber();//���������
        if(linenumber == linenumber_old)
            return;
        str_print(linenumber);
        //��ȡ�������ı�����
        cursor_left_text.clear();

        textlist_indb_content.clear();
        for(int i = 0; i< linenumber ; i++){
            QTextBlock textBlock = ui_autoindb->textEdit_show->document()->findBlockByLineNumber(i);//ͨ���к��ҵ�ָ���� ���ݿ�
            cursor_left_text += textBlock.text()+"\n";
        }
        textline_total = ui_autoindb->textEdit_show->document()->lineCount();//�ı�������
        str_print(textline_total);
        str_print(cursor_left_text);
        for(int i = linenumber; i< textline_total-1 ; i++){
            QTextBlock textBlock = ui_autoindb->textEdit_show->document()->findBlockByLineNumber(i);//ͨ���к��ҵ�ָ���� ���ݿ�
            QString tmp_text = /*cursor_left_text +*/ textBlock.text();
            if(textBlock.text().isEmpty())
                continue;
            textlist_indb_content.append(tmp_text);
        }
#if 0
        //        QStringList::const_iterator iterator = textlist_indb_content.begin();
        //        while( iterator != textlist_indb_content.end()){
        //            qDebug() << "================================\n";
        //            qDebug() << (*iterator).toAscii().data();
        //            ++iterator;
        //        }
#endif

        //        str_print(text_tmp);
        int stringlistsize = textlist_indb_content.count();
        str_print(stringlistsize);
        UNUSEDVAR(stringlistsize);
        ProgressBarSetValue(0);
        linenumber_old = linenumber;
    }
}
void autoCCode::ok_btn_dia_clicked_self_autoindb(QString begintext,QString combinetext,int *ret)
{
    self_print(ok_btn_dia_clicked_self_autoindb);

    //��ȡ����toUtf8
    QString content = (begintext + combinetext).trimmed();
    QString lanaugetype = ui_dialog->langtype_comboBox->currentText();
    QString index_keyword   = content;
    index_keyword = index_keyword.replace("\n"," ");

    QString note;
    QString time;
    static QString time_old;
    static int counter_time = 1;
    note.clear();
    if(ui_autoindb->checkBox_nullornot->isChecked()){
        note = "";
        note = note.replace("\n"," ");
        note += "\t\t\t\t";
        time = QDateTime::currentDateTime().toString("yyyy MMM d ddd,hh:mm:ss");
        note += time ;

        //        char buf[20];
        //        memset(buf,0,sizeof(buf));
        //        sprintf(buf,"%d",counter_time);
        //        note += "\t"+QString::fromLocal8Bit(buf);
        //        counter_time++;

        if(time == time_old){
            char buf[20];
            memset(buf,0,sizeof(buf));
            sprintf(buf,"%d",counter_time);
            note += "  " +QString::fromLocal8Bit(buf);
            counter_time++;
        }else{
            counter_time = 1;
        }

        time_old = time;
    }else{
        char buf[100];
        memset(buf,0,sizeof(buf));
        strncpy(buf,combinetext.toLocal8Bit().data(),ui_autoindb->spinBox_notenumber->text().toInt());
        note   = QString::fromLocal8Bit(buf);
        note = note.replace("\n"," ");
        note = note.trimmed();
        note += "\t\t\t\t";
        time = QDateTime::currentDateTime().toString("yyyy MMM d ddd,hh:mm:ss");
        note += time ;

    }

    QString vartype = ui_dialog->comboBox_vartype->currentText();
    QString aspect = ui_dialog->comboBox_aspect->currentText();


    if(lanaugetype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes , QMessageBox::Yes);
        *ret = 1;
        return;
    }
    if(content.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes , QMessageBox::Yes);
        *ret = 1;
        return;
    }

    LanguageType langtype = getLanguageType(lanaugetype);
    sets = get_table_sets_bytype(langtype);
    if(!sets){
        *ret = 1;
        return;
    }

    QString select_express = QString("select content from %1 where lantype='%2' and content='%3' and vartype='%4' and delflag=0 order by ID desc")
            .arg(sets->talbename)
            .arg(lanaugetype)
            .arg(content)
            .arg(vartype);
    save_before_ops();

    clr_selectresult(selectresult);
    str_print(select_express);
    b.selectdatabase(sets->databasename,
                     select_express.toUtf8().data(),
                     selectresult,
                     ASPECT_NONE);

    if(selectresult.existflag)
    {
        restore_before_ops();
        //�������ݴ��ڵ�ʱ�����ʾ
        //        QMessageBox::information(NULL, str_china(����), str_china(�����Ѿ�����), QMessageBox::Yes, QMessageBox::Yes);
        //        *ret = 1;
        return;
    }

    if(vartype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(��������), str_china(����Ϊ��), QMessageBox::Yes, QMessageBox::Yes);
        *ret = 1;
        return;
    }

    if(index_keyword.isEmpty())
    {
        QMessageBox::information(NULL, str_china(����), str_china(����Ϊ��), QMessageBox::Yes, QMessageBox::Yes);
        *ret = 1;
        return;
    }
    if(note.isEmpty())
    {
        QMessageBox::information(NULL, str_china(ע��), str_china(����Ϊ��), QMessageBox::Yes, QMessageBox::Yes);
        *ret = 1;
        return;
    }



    clr_selectresult(selectresult_tmp);

    InsertCon insertcontent;
    insertcontent.content = content;
    insertcontent.languagetype = getLanguageType(lanaugetype);
    insertcontent.keyword   = index_keyword;
    insertcontent.note      = note;
    insertcontent.vartype   = vartype;
    insertcontent.aspect    = aspect;


    b.creatable(&insertcontent);
    b.inserttable(&insertcontent);

    //#ifndef DEBUG_V

    //    if(ui_dialog->checkBox_EOR->isChecked())
    //        ui_dialog->note_textEdit_dia->clear();

    //    if(ui->checkBox_inbox->isChecked())
    //    {
    //        //�Ի��򲻹ر�
    //        ui_dialog->content_textEdit_dia->clear();
    //    }else{
    //        InDb_Dialog->close();
    //    }


    //#else
    //    //�Ի��򲻹ر�
    //    ui_dialog->content_textEdit_dia->clear();
    //#endif

    //    //������Ӻ󣬸��¿ؼ������ݵ������ʾ
    //    update_show_after_insert();
}

void autoCCode::pushdb_checkbox_if_selected()
{
    self_print(pushdb_checkbox_if_selected);
    if(InDb_Dialog->isHidden())
        return;
    if(!ui_dialog->checkBox_SEL->isChecked())
        return;
    if((!ui_dialog->content_textEdit_dia->hasFocus()) &&
            (!ui_dialog->index_textEdit_dia->hasFocus()))
        return;
    QString str_selected = ui_dialog->content_textEdit_dia->textCursor().selectedText();
    QString str_index_sel = ui_dialog->index_textEdit_dia->textCursor().selectedText();
    str_print(str_selected);
    str_print(str_index_sel);
    if(str_selected.length()&&ui_dialog->content_textEdit_dia->hasFocus()){
        ui_dialog->note_textEdit_dia->setText(str_selected);
        selecttext = str_selected;
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(selecttext,QClipboard::Clipboard);
    }else if(str_index_sel.length()&&ui_dialog->index_textEdit_dia->hasFocus()){
        ui_dialog->note_textEdit_dia->setText(str_index_sel);
        selecttext = str_index_sel;
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(selecttext,QClipboard::Clipboard);
    }

}

void autoCCode::pasteClicpTextToSearchEdit()
{
    self_print(pasteClicpTextToSearchEdit);
    if(!ui->checkBox_autogetclipboxtext->isChecked())
        return;
    QString linetext = ui->lineEdit_search->text();
    QString cliptext =  QApplication::clipboard()->text();
    str_print(linetext);
    str_print(cliptext);

    if(linetext != cliptext)
        ui->lineEdit_search->setText(cliptext);
}

void autoCCode::isCheckBox_cliptext_checked(bool checked)
{
    self_print(pasteClicpTextToSearchEdit);
    str_print(checked);
    if(checked){
        if(!checkbox_getcliptext_timer->isActive())
            checkbox_getcliptext_timer->start();
    }else
        checkbox_getcliptext_timer->stop();
    if(!checked)
        ui->lineEdit_search->setText("");


}
void autoCCode::ui_dialog_AutoGetCon(bool checked)
{
    self_print(ui_dialog_AutoGetCon);
    str_print(checked);
    if(checked){
        if(!checkbox_AutoGetCon_timer->isActive())
            checkbox_AutoGetCon_timer->start();
    }else
        checkbox_AutoGetCon_timer->stop();
    //    if(!checked)
    //        ui_dialog->content_textEdit_dia->setText("");

}
void autoCCode::pasteClicpTextToAutoGetCon_UiDialog()
{
    self_print(pasteClicpTextToAutoGetCon_UiDialog);
    if((!ui_dialog->checkBox_AutoGet_Con->isChecked()) || InDb_Dialog->isHidden())
        return;
    QString linetext = ui_dialog->content_textEdit_dia->toPlainText();
    QString cliptext =  QApplication::clipboard()->text();
    str_print(linetext);
    str_print(cliptext);

    if(linetext.isEmpty()&& cliptext != selecttext)
        ui_dialog->content_textEdit_dia->setText(cliptext);
}

quint8 autoCCode::IsClipboardChanged()
{
    static QString text;
    QClipboard *clipboard = QApplication::clipboard();
    if(text != clipboard->text())
    {
        text = clipboard->text();
        return STATE_CLIPBORD_CHAGED;
    }else{
        return STATE_CLIPBORD_NOCHAGED;
    }
}

void autoCCode::PopInDbUi()
{
    static uint8_t firstin = FLAG_YES;
    UNUSEDVAR(firstin);

    //���ѡ���������룬�����治����
    if(ui->checkBox_inbox->isChecked())
    {
        return;
    }


    if(ui->checkBox_popupindb->isChecked())
    {
        if(STATE_CLIPBORD_CHAGED == IsClipboardChanged())
        {
            if(!isMinimized())//��������С��ʱ�������κ�����д��
            {//������ʾ��������а������б仯������д�����ʾ
                if(InDb_Dialog->isHidden() && //����Ϊ����״̬���򵯳�
                        (!ui_dialog->langtype_comboBox->currentText().isEmpty()))
                {
                    if(!ui_dialog->content_textEdit_dia->toPlainText().isEmpty())
                    {
                        ui_dialog->content_textEdit_dia->clear();
                        InDb_Dialog->show();
                    }else{
                        InDb_Dialog->hide();
                    }
                    //                    }

                }else{//����Ϊ��ʾ״̬
                    if(ui_dialog->content_textEdit_dia->toPlainText().isEmpty())
                    {//���������Ϊ�գ�������
                        InDb_Dialog->hide();
                    }
                }
            }
            else
            {
                InDb_Dialog->hide();
                //                qDebug() << "main ui hide!!";
            }
        }
    }
    else
    {
        InDb_Dialog->hide();
        //                qDebug() << "main ui hide!!";
    }
}

int autoCCode::getLimitNum()
{
    return ui_autoindb->spinBox_notenumber->text().toInt()?ui_autoindb->spinBox_notenumber->text().toInt():10;
}
//�Ƿ���ʾ��������������
int autoCCode::showcode_textEdit_AtBotton()
{
    //codeshow_textEdit
    if(ui->radioButton_showall->isChecked())
    {
        return TRUE;
    }
    //����Զ����������������Ϊ1������ʾ��������ʾ
    return (ui_autoindb->spinBox_notenumber->text().toInt() == 1);
}


bool autoCCode::eventFilter(QObject *obj, QEvent *event)
{
    //    qDebug() << "eventFilter";
    if (obj == ui->lineEdit_search) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            //qDebug()<<"double clicked!!";
            on_lineEdit_search_MouseButtonDblClick();
            return true;
        }
        else if(event->type() == QEvent::KeyPress)
        {
            //qDebug()<<"KeyPress ed!!";
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();
            if (Qt::Key_Down == key) {
                //qDebug()<<"Key_Down !!";
            } else if (Qt::Key_Up == key) {
                //qDebug()<<"Key_Up !!";

            } else if (Qt::Key_Escape == key) {
                //qDebug()<<"Key_Escape !!";
                on_lineEdit_search_Key_Escape();

            } else if (Qt::Key_Enter == key || Qt::Key_Return == key) {
                //qDebug()<<"Key_Enter   Key_Return!!";
            } else {
                //qDebug()<<"else Key !!";

            }

        }

    }
    else
    {
        on_lineEdit_search_Key_Escape();
    }
    return QObject::eventFilter(obj, event);
}

void autoCCode::on_lineEdit_search_MouseButtonDblClick()
{
    qDebug() << "on_lineEdit_search_MouseButtonDblClick";

    int setWidth = 150;
    listView->setMinimumWidth(setWidth);
    listView->setMaximumWidth(setWidth);

    int lineeditH = ui->lineEdit_search->height();
    QPoint p(ui->lineEdit_search->mapToGlobal(QPoint(0,0+lineeditH)));//�����б�listView��λ��



    #if 1//�����ݿ��looktexthis_table�в���ǰ10����õ����� begin
    //������ҹؼ��� begin
    codestructSets *setsLookHis = get_table_sets_bytype(languagetype_LookTextHis_);
    if(!setsLookHis)
        return;
    LookTextHistoryResult looktexthistoryres;
    clr_looktexthisresult(looktexthistoryres);

    QString looktexthis_express;
    looktexthis_express.clear();
    looktexthis_express = QString("select * from %1 order by looktimes desc limit 10")
            .arg(setsLookHis->talbename);

    b.searchdatabase_lookTextHisTbl(setsLookHis->databasename,looktexthis_express.toLocal8Bit().data(),
                     looktexthistoryres,
                                    "");//searchStr�ÿգ���ʾ����ȫ��ѯ

    qDebug() << "count:"<< looktexthistoryres.looktextarry.count();
    for(int i=0;i< looktexthistoryres.looktextarry.count();i++)
    {
        qDebug() << "arry:"<< looktexthistoryres.looktextarry.at(i);
    }


    #endif //�����ݿ��looktexthis_table�в���ǰ10����õ����� begin


    model->setStringList(looktexthistoryres.looktextarry);
    listView->setModel(model);
    if (model->rowCount() == 0)
    {
        return;
    }

    listView->move(p);
    listView->show();
}

void autoCCode::on_lineEdit_search_Key_Escape()
{
    if(listView->isVisible())
        listView->hide();
}

