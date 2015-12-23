#include "autoccode.h"
#include "ui_autoccode.h"
#include "ui_PushDbdialog.h"
#include "ui_dialog_select_database.h"
#include "ui_autoindb.h"
#include "ui_setup1.h"
#include <QtGui>
#include "prefix_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "gencodedatabase.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <QString>
#include <QTime>
#include <QElapsedTimer>

#include "qxtglobalshortcut/qxtglobalshortcut.h"   //add shortcut
#include "emailaddress.h"
#include "sendemail.h"
#include "smtpclient.h"
#include "mimehtml.h"

using namespace std;


//中文Windows的缺省内码是GBK
// ASCII、GB2312、GBK到GB18030的编码方法是向下兼容的。而Unicode只与ASCII兼容（更准确地说，是与
// ISO-8859-1兼容），与GB码不兼容。例如“汉”字的Unicode编码是6C49，而GB码是BABA。
#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )
//QString str;
//QCString cstr;
//str = G2U("中文输入");
//cstr = U2G(str);



#define LIMIT_SHOW_SIZE getLimitNum()/*60*/


#if 1
//编码汉字
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
    ui_setup(new Ui::SetUpDialog),
    sets(NULL),
    index_key_color(0),
    index_note_color(0),
    flag_selectLeft(1),
    listWidget_codeview_row(0),
    CurrentIndex_comboBox_aspect(0),
    CurrentIndex_comboBox_vartype(0),
    CurrentIndex_comboBox_langtype(languagetype_Err_),
    timer(NULL),
    timer_checkbox_sel(NULL),
    lineEdit_search_timer(NULL),
    checkbox_getcliptext_timer(NULL),
    checkbox_AutoGetCon_timer(NULL),
    isCTRLKeyPressed(FALSE),
    is_selected(FALSE)

{
    codec = QTextCodec::codecForName("GBK");//must first used,or is NULL,die
    ui->setupUi(this);
    InDb_Dialog = new QDialog(this);
    ui_dialog->setupUi(InDb_Dialog);

    dialog_selectdb =  new QDialog(this);;
    ui_dia_selectdb->setupUi(dialog_selectdb);

    dialog_autoindb =  new QDialog(this);
    ui_autoindb->setupUi(dialog_autoindb);
    /* set up menu */
    SetUp_Dialog = new QDialog(this);
    ui_setup->setupUi(SetUp_Dialog);

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
    CharFormat();
    setDefaultColor();
    PopMenu();
//    SendMail(str_china("i love you,xiao baobei!!!!!!!!,生命的力量"));
}

void autoCCode::PopMenu()
{
    /* 右键菜单 */
    Act_Maxsize = new QAction(tr("XML"), this);
    Act_Normal = new QAction(tr("SHOW"), this);
    connect(Act_Maxsize, SIGNAL(triggered()), this, SLOT(ADDChange()));
    connect(Act_Normal, SIGNAL(triggered()), this, SLOT(XMLChange()));
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
//在创建了过滤器之后，下面要做的是安装这个过滤器。安装过滤器需要调用installEventFilter()函数。
void autoCCode::InstallEventFilterSets(void)
{
    ui->lineEdit_search->installEventFilter(this);

    ui->listWidget_codeview->installEventFilter(this);
    ui->listWidget_note->installEventFilter(this);
    ui->genshow_textEdit->installEventFilter(this);

}

void autoCCode::ListViewSets()
{
    listView = new QListView(this);
    model = new QStringListModel(this);
    listView->setWindowFlags(Qt::ToolTip);
    //    connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(setCompleter(const QString &)));
    //    connect(listView, SIGNAL(clicked(QModelIndex)), this, SLOT(completeText(const QModelIndex &)));
    QObject::connect(listView,SIGNAL(clicked(QModelIndex)),this,SLOT(completeText(QModelIndex)));
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
    ui_dialog->checkBox_SEL->setChecked(false);
    QObject::connect(ui_dialog->checkBox_SEL,SIGNAL(toggled(bool)),
                     this,SLOT(on_checkBox_SEL_change(bool)));

    QObject::connect(ui_dialog->checkBox_AutoGet_Con,SIGNAL(toggled(bool)),
                     this,SLOT(ui_dialog_AutoGetCon(bool)));
    QObject::connect(ui->checkBox_autogetclipboxtext,SIGNAL(toggled(bool)),
                     this,SLOT(isCheckBox_cliptext_checked(bool)));
    if(!ui_dialog)
        return;

    QObject::connect(ui_dialog->content_textEdit_dia,SIGNAL(textChanged()),
                     this,SLOT(set_index_text()));
    QObject::connect(ui_dialog->content_textEdit_dia,SIGNAL(textChanged()),
                     this,SLOT(set_note_textEdit_firstline()));

    //右选入库处理
    QObject::connect(ui_setup->checkBox_rightTextSelectIndb,SIGNAL(toggled(bool)),
                     this,SLOT(on_checkBox_rightTextSelectIndb_change(bool)));
    //实时查询处理
    QObject::connect(ui_setup->checkBox_rtQuery,SIGNAL(toggled(bool)),
                     this,SLOT(on_checkBox_rtQuery_change(bool)));
}

void autoCCode::on_checkBox_rightTextSelectIndb_change(bool flag)
{
    if(flag)
    {
        ui->pushButton_rightTextSelectIndb->setEnabled(TRUE);
        ui->pushButton_rightTextSelectIndb->show();
    }else{
        ui->pushButton_rightTextSelectIndb->setEnabled(FALSE);
        ui->pushButton_rightTextSelectIndb->hide();
    }
    update();
}

void autoCCode::on_checkBox_rtQuery_change(bool flag)
{

}

quint8 autoCCode::get_rtQuery_enable()
{
    return (Qt::Checked == ui_setup->checkBox_rtQuery->checkState());
}



void autoCCode::on_checkBox_SEL_change(bool flag)
{
    if(!flag)
    {
        //如果没有选中的内容，则默认为第一行文字
        set_note_textEdit_firstline();
    }
}

void autoCCode::set_index_text()
{
    if(ui->checkBox_same->isChecked())
        ui_dialog->index_textEdit_dia->setText(ui_dialog->content_textEdit_dia->toPlainText());
}
void autoCCode::set_note_textEdit_firstline()
{
    //如果有选中的内容，则默认为选中的内容
    QString str_selected = ui_dialog->content_textEdit_dia->textCursor().selectedText();
    QString str_index_sel = ui_dialog->index_textEdit_dia->textCursor().selectedText();
    str_print(str_selected);
    str_print(str_index_sel);
    if(str_selected.length() > 0)
    {
        is_selected = TRUE;
    }
    else
    {
        is_selected = FALSE;
    }

    if(str_selected.length()&&ui_dialog->content_textEdit_dia->hasFocus()){
        ui_dialog->note_textEdit_dia->setText(str_selected);
        selecttext = str_selected;
        return;
        //            QClipboard *clipboard = QApplication::clipboard();
        //            clipboard->setText(selecttext,QClipboard::Clipboard);
    }else if(str_index_sel.length()&&ui_dialog->index_textEdit_dia->hasFocus()){
        ui_dialog->note_textEdit_dia->setText(str_index_sel);
        selecttext = str_index_sel;
        return;
        //            QClipboard *clipboard = QApplication::clipboard();
        //            clipboard->setText(selecttext,QClipboard::Clipboard);
    }

    if(is_selected)
    {
        return;
    }
    //如果没有选中的内容，则默认为第一行文字

    /* 复选框未选中时默认为第一行内容 */
    if(ui_dialog->checkBox_SEL->isChecked())
        return;
    //    int textline = ui_dialog->content_textEdit_dia->document()->lineCount();
    //    qDebug() << "content text linenums:" << textline;

    QString str_firstline = ui_dialog->content_textEdit_dia->document()->findBlockByLineNumber(0).text();
    //    qDebug() << "str_firstline:" << str_firstline;
    if(str_firstline.length())
    {
        ui_dialog->note_textEdit_dia->setText(str_firstline);
    }
}


void autoCCode::lineTextEditSet(void)
{
#if 0//文本改变搜索
    QObject::connect(ui->lineEdit_search,SIGNAL(textChanged(QString)),
                     this,SLOT(SearchText(QString)));
#else//定时搜索
    //    QObject::connect(lineEdit_search_timer,SIGNAL(timeout()),this,SLOT(SearchText_WithTimer()));
    //是否支持实时查询
    QObject::connect(lineEdit_search_timer,SIGNAL(timeout()),this,SLOT(SearchText_WithTimer_Enter()));

#endif
}

void autoCCode::dragDropSet(void)
{
    //允许拖放的文字添加到编辑框中
    ui->codeshow_textEdit->setAcceptDrops(true);
    ui->codeshow_textEdit->hide();
    //    ui->codeshow_textEdit->setHidden(true);
}
void autoCCode::pushButton_setup()
{
    QObject::connect(ui_setup->pushButton_backColor,SIGNAL(clicked()),
                     this,SLOT(on_pushButton_backColor_clicked2()));
    QObject::connect(ui_setup->pushButton_foreColor,SIGNAL(clicked()),
                     this,SLOT(on_pushButton_foreColor_clicked2()));

    QObject::connect(ui_setup->pushButton_font,SIGNAL(clicked()),
                     this,SLOT(setFont()));
}

void autoCCode::pushButtonSet(void)
{
    self_print(pushButtonSet);

    pushButton_setup();
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

    //右清空
    QObject::connect(this->ui->rightclear_btn,SIGNAL(clicked()),
                     this,SLOT(rightClear_textedit()));

    //dialog_select_database.ui add_btn
    QObject::connect(this->ui_dialog->pushButton_add,SIGNAL(clicked()),
                     this,SLOT(add_aspect_totable()));

    //删除按键
    QObject::connect(ui->delete_btn,SIGNAL(clicked()),
                     this,SLOT(delete_btn_clicked_selfdefine()));
    //clean search text
    QObject::connect(ui->pushButton_clean,SIGNAL(clicked()),
                     this,SLOT(cleanLineTextEditSearch()));

#ifdef RELEASE_VERSION
    //更新库,为添加小写的keyworks for search 使用的，默认不显示
    ui->pushButton_updatedb->hide();
    QObject::connect(ui->pushButton_updatedb,SIGNAL(clicked()),
                     this,SLOT(add_column_lowercase_keywords_content()));
#endif

    /*  信号——槽   */
    QObject::connect(ui_dialog->pushButton_note,SIGNAL(clicked()),
                     this,SLOT(setCliptext()));

    QObject::connect(ui_dialog->pushButton_note_clear,SIGNAL(clicked()),
                     this,SLOT(note_clear()));
    QObject::connect(ui->pushButton_search_fromClip,SIGNAL(clicked()),
                     this,SLOT(set_search_text()));
    QObject::connect(ui->pushButton_setUp,SIGNAL(clicked()),
                     this,SLOT(MainSetUp()));

    /*  content按钮操作   */
    QObject::connect(ui_dialog->pushButton_content_paste,SIGNAL(clicked()),
                     this,SLOT(setCliptext_content()));

    QObject::connect(ui_dialog->pushButton_content_clear,SIGNAL(clicked()),
                     this,SLOT(content_clear()));

    /*  min size 操作   */
    QObject::connect(ui_dialog->pushButton_minsize,SIGNAL(clicked()),
                     this,SLOT(minSize_ui_dialog()));
    QObject::connect(ui_dialog->pushButton_widthsize,SIGNAL(clicked()),
                     this,SLOT(widthSize_ui_dialog()));
    QObject::connect(ui_dialog->pushButton_maxsize,SIGNAL(clicked()),
                     this,SLOT(maxSize_ui_dialog()));


    ui->pushButton_updatedb->hide();//隐藏更新库按钮

    QObject::connect(ui->modify_btn,SIGNAL(clicked()),
                     this,SLOT(modify_content()));

    QObject::connect(ui->pushButton_getpaste,SIGNAL(clicked()),
                     this,SLOT(getText_FromRight()));

    //自动入库
    QObject::connect(ui->pushbtn_autoindb,SIGNAL(clicked()),
                     this,SLOT(on_pushbtn_autoindb_clicked_self()));
    QObject::connect(ui_autoindb->pushBtn_Open,SIGNAL(clicked()),
                     this,SLOT(on_ui_autoindb_pushBtn_Open_clicked()));
    QObject::connect(ui_autoindb->pushBtn_process,SIGNAL(clicked()),
                     this,SLOT(on_ui_autoindb_pushBtn_process_clicked()));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(get_autoindb_textedit_cursor_postion()));

    //入库，选择左侧的内容，添加到右侧
    QObject::connect(timer_checkbox_sel,SIGNAL(timeout()),this,SLOT(pushdb_checkbox_if_selected()));
    QObject::connect(timer_checkbox_sel,SIGNAL(timeout()),this,SLOT(set_note_textEdit_firstline()));




}
void autoCCode::set_search_text()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui->lineEdit_search->clear();
    ui->lineEdit_search->setText(clipboard->text());
}
void autoCCode::search_text_clear()
{
    cleanLineTextEditSearch();
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


    //添加current index of ui_dialog

    //左滚动，对应 右滚动
    QObject::connect(this->ui->listWidget_codeview,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(listWidget_note_scroll_sync(QListWidgetItem*)));
    //code view with enter Key

    QObject::connect(this->ui->listWidget_codeview,SIGNAL(currentRowChanged(int)),
                     this,SLOT(listWidget_note_with_currentRowChanged(int)));

    QObject::connect(this->ui->listWidget_codeview,SIGNAL(activated(QModelIndex)),
                     this,SLOT(listWidget_note_with_enter(QModelIndex)));


    QObject::connect(this->ui->listWidget_note,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(listWidget_codeview_scroll_sync(QListWidgetItem*)));
    //    QObject::connect(this->ui->listWidget_note,SIGNAL(activated(QModelIndex)),
    //                     this,SLOT(listWidget_note_with_enter(QModelIndex)));
}


void autoCCode::comboBox_selectdb_currentIndexChanged(const QString &arg1)
{
    self_print(comboBox_selectdb_currentIndexChanged);
    str_print(arg1);

    if(arg1.isEmpty())
    {
        setWindowTitle_Main("AutoCCode");
    }else{
        setWindowTitle_Main(arg1);
    }

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
            select_express = QString("select content,lantype,keywords,note,vartype,aspect_field from %1 where lantype='%2' and aspect_field='%3' and delflag=0 order by ID desc limit %4")
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
    selectresult.keyword_subshowlist = listWidget_codeview_subShow(selectresult.keyword_list);
    ui->listWidget_codeview->addItems(selectresult.keyword_subshowlist);
    //    ui->listWidget_codeview->addItems(selectresult.keyword_list);

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
    //    selectresult.aspect_list<<str_china();
    b.selectdatabase(sets->databasename,
                     select_express.toUtf8().data(),
                     selectresult,
                     ASPECT_HAVE);
    selectresult.aspect_list.push_front(str_china());//范畴首位填写为空
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
    <<str_china(Patchs)
    <<str_china(Postgresql)
    <<str_china(Rust)
    <<str_china(shell)
    <<str_china(Sqlite3)
    <<str_china(Swift)
    <<str_china(Php)
    <<str_china(Python)
    <<str_china(Qt)
    <<str_china(Qtquick)
    <<str_china(Unittest);



    ui_dialog->langtype_comboBox->addItems(strlist);

    //select db dialog add strlist;
    ui_dia_selectdb->comboBox_selectdb->addItems(strlist);

    //范畴
    addstr_aspect_comboBox();

    //解决当前数据库选择或者插入时存在的不一致问题。
    connect(ui_dialog->langtype_comboBox,SIGNAL(currentIndexChanged(int)),
            ui_dia_selectdb->comboBox_selectdb,SLOT(setCurrentIndex(int)));
    connect(ui_dia_selectdb->comboBox_selectdb,SIGNAL(currentIndexChanged(int)),
            ui_dialog->langtype_comboBox,SLOT(setCurrentIndex(int)));



    //默认内容
    ui_dialog->comboBox_vartype->setCurrentIndex(4);  //入库 变量类型
    ui_dialog->langtype_comboBox->setCurrentIndex(2); //入库 选择语言，C，C++，Python etc.
    //    ui_dialog->comboBox_aspect->setCurrentIndex();  //入库 范畴 ，默认为linux
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
    QString savefileName;
    savefileName.clear();

    if(!ui->checkBox_AutoSave->isChecked())//自动保存未被选中
    {
        savefileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"), getCurrentDateTimeTxt(), tr("All Files (*.*);;Txt(*.txt);;Img Files(*.png);;Code Files(*.c)"));

    }
    else //自动保存选中
    {
        savefileName = getCurrentDateTimeTxt() +QString::fromLocal8Bit(".txt");
    }

    if (savefileName.isNull())
    {
        //fileName是文件名
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
//选择数据库
void autoCCode::on_choseCodeDB_btn_clicked(void)
{
    self_print(on_choseCodeDB_btn_clicked);
    dialog_selectdb->show();

}
//生成代码库
void autoCCode::on_gencode_btn_clicked(void)
{
    self_print(on_gencode_btn_clicked);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDesktopServices::storageLocation(QDesktopServices::DesktopLocation),
                                                    tr("Txt (*.txt)"
                                                       ";;Ctype (*.c *.C *.cc *.h)"
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
    QString text_china;
    text_china.clear();

    text_china += QString::fromLocal8Bit(file.readAll().data());
    if(ui_setup->checkBox_showpath->isChecked())
    {
        text_china += "\n======[end]========\n" + fileName + "\n";
    }

    if(showcode_textEdit_AtBotton())
    {
        ui->codeshow_textEdit->setText(text_china);
    }
    //code Editor设置读取文本
    ui->genshow_textEdit->setPlainText(text_china);

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
void autoCCode::SaveUiMove()
{
    quint16 usWidth = (this->width() - InDb_Dialog->width())/2;
    quint16 usHeight = abs(this->height() - InDb_Dialog->height())/2;
    usHeight = usHeight > 80 ? usHeight:80;
    InDb_Dialog->move(this->pos().x() + usWidth,
                      this->pos().y() + usHeight);
}

//入库函数
void autoCCode::on_indb_btn_clicked(void)
{
    self_print(on_indb_btn_clicked);
    QString select_text = ui->codeshow_textEdit->textCursor().selectedText();
    ui_dialog->content_textEdit_dia->setText(select_text);

    SaveUiMove();

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
        {
            SaveUiMove();
            InDb_Dialog->show();
        }
        else
            InDb_Dialog->hide();
        //        InDb_Dialog->exec();
    }else{
        if(InDb_Dialog->isHidden())
        {
            SaveUiMove();
            InDb_Dialog->show();
        }
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
    else if(type == "Patchs")
    {
        return languagetype_Patchs_;
    }
    else if(type == "Unittest")
    {
        return languagetype_Unittest_;
    }
    else if(type == "Swift")
    {
        return languagetype_Swift_;
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

    //获取内容
    QString contentHtml = ui_dialog->content_textEdit_dia->toHtml();
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
        QMessageBox::information(NULL, str_china(类型), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
        return;
    }
    if(content.isEmpty())
    {
        QMessageBox::information(NULL, str_china(内容), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
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
        QMessageBox::information(NULL, str_china(声明), str_china(内容已经存在), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(vartype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(变量类型), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(index_keyword.isEmpty())
    {
        QMessageBox::information(NULL, str_china(检索), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(note.isEmpty())
    {
        QMessageBox::information(NULL, str_china(注释), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    //    if(aspect.isEmpty())
    //    {
    //        QMessageBox::information(NULL, str_china(范围), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
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
        //对话框不关闭
        ui_dialog->content_textEdit_dia->clear();
    }else{
        InDb_Dialog->update();
        InDb_Dialog->close();
    }


#else
    //对话框不关闭
    ui_dialog->content_textEdit_dia->clear();
#endif

    b.creatable(&insertcontent);
    b.inserttable(&insertcontent);


    //内容添加后，更新控件中内容的相关显示
    update_show_after_insert();

    is_selected = FALSE;//插入数据后，把此置为FALSE

    //插入数据是否发送
    if(ui_setup->checkBox_email->isChecked())
    {/* 发送HTML内容，否则为连续的内容 */
        SendMail(contentHtml);
    }
}

void autoCCode::cancel_btn_dia_clicked_self(void)
{
    self_print(cancel_btn_dia_clicked_self);
    InDb_Dialog->close();
}
void autoCCode::aboutVersion(void)
{
    QMessageBox::information(NULL, str_china(版本), GetVersion(),NULL,NULL);
    return;
}
QString autoCCode::GetVersion(void)
{
    return str_china(自动生成代码)
            +"\n"
            +str_china(by小魏莱)
            +"\n"
            +version_autoccode;
}

void autoCCode::listWidgetSet(void)
{
    self_print(listWidget);
#if 0 //双击
    QObject::connect(ui->listWidget_codeview,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_keyword(QListWidgetItem*)));
    QObject::connect(ui->listWidget_note,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_note(QListWidgetItem*)));
#else//单击
    QObject::connect(ui->listWidget_codeview,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_keyword(QListWidgetItem*)));
    //双击
    QObject::connect(ui->listWidget_note,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_note(QListWidgetItem*)));
#if 0//多选

    QObject::connect(ui->listWidget_codeview,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(add_to_gen_code_textedit_by_keyword(QListWidgetItem*)));
#endif//end 多选


#endif //end 双击


}
//添加到右边的内容中
void autoCCode::add_to_gen_code_textedit_by_keyword(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit_by_keyword);
    rightTextShowClear_oncheched();
    QString str = item->text();
    unsigned int index = 0;
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.keyword_subshowlist.at(i))
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


    ui->genshow_textEdit->setPlainText(GenCode_str);
    SearchTextResWithColor(GenCode_str);
    ui->genshow_textEdit->moveCursor(QTextCursor::End);
    ui->listWidget_codeview->setFocus();
    //    update();
    //    ui->listWidget_codeview->update();
    ui->genshow_textEdit->update();
}
//添加到右边的内容中
void autoCCode::add_to_gen_code_textedit_by_note(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit_by_note);
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


    ui->genshow_textEdit->setPlainText(GenCode_str);
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
    result.keyword_subshowlist.clear();
    result.note_list.clear();
    result.vartype_list.clear();
    result.existflag = 0;
    result.aspect_list.clear();
    result.aspect_field.clear();

    //每次重新开始记录，否则滚动时点击会死机
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
    selectresult.keyword_subshowlist = listWidget_codeview_subShow(selectresult.keyword_list);
    ui->listWidget_codeview->addItems(selectresult.keyword_subshowlist);
    //    ui->listWidget_codeview->addItems(selectresult.keyword_list);
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

    //范畴
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

//右边的注释同步滚动，选择
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
//note滚动点击
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
    //开机删除死机bug
    if(0 == selectresult.existflag )
        return;

    //    if()
    str_print(index_key_color);
    //先判定第一个不删除吧，点击note右侧的内容进行删除时出现问题
    if( flag_selectLeft )
    {
        /*  标准对话框——警示消息框   */
        QMessageBox::warning(NULL,"Warning",
                             str_china(请选择右侧进行删除),
                             QMessageBox::Yes,QMessageBox::Yes);
        //        /*  标准对话框——警示消息框   */
        //        QMessageBox::warning(NULL,"Warning",
        //                             str_china(请选择左侧进行删除),
        //                             QMessageBox::Yes,QMessageBox::Yes);
        return;
    }

    /*  输入对话框   */
    bool isOK;
    QString text = QInputDialog::getText(this,"Input Dialog",
                                         "Please Press Ok to delete",
                                         QLineEdit::Normal,
                                         selectresult.keyword_list.at(index_key_color)
                                         .left(100),
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

void autoCCode::SearchText_WithTimer_Enter(void)
{
    //    qDebug() << "get_rtQuery_enable():" << get_rtQuery_enable();
    if(!get_rtQuery_enable())
    {
        return;
    }
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

    /* 长度为1不搜索 */
    if(searchStr.length() <= 1)
        return;

    //添加，当前是否有选中库，如果没有则直接返回，可当作一个函数接口。
    if(!currentDbHaved())
    {
        return;
    }

    if(searchStr.isEmpty()){
        update_show_after_insert();
        return;
    }
    judge_color_index();
    str_print(selectresult.content_list.size());

    clr_selectresult(selectresult);

    //保存查找关键字 begin
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
    {//插入一条数据
        //qDebug() << "insert looktimes:" << looktexthistoryres.looktimes;
        looktexthis_express.clear();
        looktexthis_express = QString("insert into %1([looktextname],[lowercase_looktextname] ,[looktimes], [relatedtblname])  VALUES('%2','%3','%4','%5')")
                .arg(setsLookHis->talbename)
                .arg(searchStr)
                .arg(searchStr.toLower())
                .arg(looktexthistoryres.looktimes+1)
                .arg(sets->talbename);

        //        //qDebug() << "databasename:" << setsLookHis->databasename;
        //        //qDebug() << "express:" << setsLookHis->creat_table_express;

        b.opendatabase(setsLookHis->databasename,setsLookHis->creat_table_express);
        b.insertdatabase(setsLookHis->databasename,looktexthis_express.toLocal8Bit().data());
    }
    else
    {//更新数据
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

    //保存查找关键字 end



    //查找并插入数据
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
        selectresult.keyword_subshowlist = listWidget_codeview_subShow(selectresult.keyword_list);
        ui->listWidget_codeview->addItems(selectresult.keyword_subshowlist);
        //        ui->listWidget_codeview->addItems(selectresult.keyword_list);
        ui->listWidget_note->addItems(selectresult.note_list);
    }
}
void autoCCode::cleanLineTextEditSearch(void)
{
    ui->lineEdit_search->setFocus();
    if(ui->lineEdit_search->text().isEmpty())
    {
        return;
    }
    if(ui->lineEdit_search->text().length() <= 1)
    {
        ui->lineEdit_search->clear();
        return;
    }
    ui->lineEdit_search->clear();
    update_show_after_insert();
}



//为所有表中的lowercase_keyword添加内容
void autoCCode::add_column_lowercase_keywords_content(void)
{
    self_print(add_column_lowercase_keywords);
    if(!sets)
        return;
    //开机删除死机bug
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
    selectresult.keyword_subshowlist = listWidget_codeview_subShow(selectresult.keyword_list);
    ui->listWidget_codeview->addItems(selectresult.keyword_subshowlist);
    //    ui->listWidget_codeview->addItems(selectresult.keyword_list);
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
#define MAXREADFILESIZE 10*1024*1024  /* 最大读取文件10M */
    str_print(fileName);
    /*  读取文件 只读   */
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly) && file.size() < MAXREADFILESIZE)
    {
        //        qDebug() << "file opend!!";
        QTextStream stream(&file);
        //        ui->codeshow_textEdit->setText(stream.readAll());
        ui->genshow_textEdit->setPlainText(stream.readAll());
        //        qDebug() << "content:" << stream.readAll();
    }
    file.close();
}

void autoCCode::SearchEnter()
{
    //    qDebug() << "search enter!!";
    self_print(SearchEnter);
    //    if(get_rtQuery_enable()) //如果支持实时查询，屏蔽此功能
    //        return;
    //    SearchText_WithTimer();
    SearchText(ui->lineEdit_search->text());
    //    SetlistWidget_codeview_row(0);
    //    this->ui->listWidget_codeview->setFocus();
    //    this->ui->listWidget_codeview->setModelColumn(GetlistWidget_codeview_row());
}


void autoCCode::listWidget_note_with_currentRowChanged(int row)
{
    //    qt计算耗时
    static QElapsedTimer elapseTimer;
    static quint8 IsTimerStarted = FALSE;
    static qint64 time_begin = 0; //返回从上次start()或restart()开始以来的时间差，单位ms
    static qint64 time_end   = 0; //返回从上次start()或restart()开始以来的时间差，单位ms;

    if(!IsTimerStarted)
    {
        elapseTimer.start();
        IsTimerStarted = TRUE;
    }

    //开始记录时间
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

        ui->genshow_textEdit->setPlainText(GenCode_str);
        SearchTextResWithColor(GenCode_str);
        //    setCharColor(10);
        //    ui->genshow_textEdit->setHtml(GenCode_str);
        //        ui->genshow_textEdit->moveCursor(QTextCursor::End);
        //        ui->listWidget_codeview->setFocus();
    }
    /* 记录结束时间 */
    if(elapseTimer.isValid())
    {
        time_end = elapseTimer.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms;
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

/*  QT:设置textedit文本框中某个字符的格式 */
void autoCCode::setCharColor(unsigned int pos)
{
    if(pos <= 0)return ;
    QTextCursor cursor = ui->genshow_textEdit->textCursor();//ui->view1->textCursor();
    cursor.movePosition( QTextCursor::StartOfLine );//行首
    cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, pos-1);//向右移动到Pos
    cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
    ui->genshow_textEdit->setTextCursor( cursor ); // added
    QTextCharFormat defcharfmt = ui->genshow_textEdit->currentCharFormat();
    QTextCharFormat newcharfmt = defcharfmt;
    newcharfmt.setFontUnderline( true );
    newcharfmt.setUnderlineColor( QColor( Qt::red ) );
    newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );
    ui->genshow_textEdit->setCurrentCharFormat( newcharfmt );

    cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
    ui->genshow_textEdit->setTextCursor( cursor ); // added
    // ui->genshow_textEdit->setCurrentCharFormat( defcharfmt );
    ui->genshow_textEdit->setFocus();
}

/*  QT:设置textedit文本框中某个字符串的格式 */
void autoCCode::setStringColor(unsigned int pos,unsigned int len)
{
    unsigned int i = 0;

    if((0 == len)||(0 == pos))
    {
        //        cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
        //        ui->genshow_textEdit->setTextCursor( cursor ); // added
        //    ui->genshow_textEdit->setCurrentCharFormat( defcharfmt );
        ui->genshow_textEdit->setCurrentCharFormat( defcharfmt );
        return;
    }

    QTextCursor cursor = ui->genshow_textEdit->textCursor();//ui->view1->textCursor();

    QTextCharFormat newcharfmt = defcharfmt;

    cursor.movePosition(QTextCursor::Start);//整体首
    //    cursor.movePosition(QTextCursor::StartOfBlock);
    //    cursor.movePosition( QTextCursor::StartOfLine);//行首
    cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::MoveAnchor, pos >=1? (pos-1):0);//向右移动到Pos
    for(i = 0;i < len;i++){
        cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );

    }





    if(len >0)
    {
        ui->genshow_textEdit->setTextCursor( cursor ); // added
        newcharfmt.setBackground(QBrush(Qt::cyan,Qt::SolidPattern));

        //        newcharfmt.setFontUnderline( true );
        //        newcharfmt.setBackground(QBrush(Qt::cyan,Qt::SolidPattern));
        //        newcharfmt.setUnderlineColor( QColor( Qt::red ) );
        //        newcharfmt.setUnderlineStyle( QTextCharFormat::WaveUnderline );
    }
    else
    {

        //        newcharfmt.setFontUnderline( false );
        //        newcharfmt.setUnderlineColor( QColor( Qt::red ) );
        //        newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );
    }
    //    cursor.mergeCharFormat(newcharfmt);

    ui->genshow_textEdit->setCurrentCharFormat( newcharfmt );

    cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
    //    cursor.movePosition( QTextCursor::End);
    ui->genshow_textEdit->setTextCursor( cursor ); // added
    //    ui->genshow_textEdit->setCurrentCharFormat( defcharfmt );
    ui->genshow_textEdit->setCurrentCharFormat( defcharfmt );

    //    cursor = ui->genshow_textEdit->textCursor();
    //    cursor.movePosition(QTextCursor::End);
    //    ui->genshow_textEdit->setTextCursor(cursor);
    //    //    ui->genshow_textEdit->setFocus();
    ui->genshow_textEdit->updateGeometry();
    ui->genshow_textEdit->updatesEnabled();
    ui->genshow_textEdit->update();


    //修改数据显示不全的问题，为啥更改下大小就可以了？
    int h = ui->genshow_textEdit->height();
    int w = ui->genshow_textEdit->width();
    //    QPoint p();
    ui->genshow_textEdit->resize(w/2,h/2);
    ui->genshow_textEdit->resize(w,h);
    ui->genshow_textEdit->setFocus();
    ui->genshow_textEdit->update();

}

enum
{
    CHAR_ENGLISH,
    CHAR_LOCA8Bit,
    CHAR_NONE
};
void autoCCode::SearchTextResWithColor(QString &resStr)
{

    //颜色框是否选中
    if(!ui->checkBox_ResWithColor->isChecked())
    {
        //        //背景白色，前景黑色
        //        ui->genshow_textEdit->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);");
        setStringColor(0, 0);
        return;
    }
    //    //背景浅绿色，前景黑色
    //    ui->genshow_textEdit->setStyleSheet("background-color: rgb(60, 243, 243);color: rgb(0, 0, 0);");
    QString searchText = ui->lineEdit_search->text();
    if(searchText.isEmpty() || (searchText.length() <= 1))     /* 长度为1不搜索 */
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
    //    int k = 0;
    //    int q = 0;
    int goflat = CHAR_ENGLISH;
    if(resStr.contains(searchText))
    {
        int strutf8len = strlen(searchText.toUtf8().data());
        int strlocal8bitlen = strlen(searchText.toLocal8Bit().data());
        //        qDebug() << "-->>>> :"<< searchText;
        //        qDebug() << "searchText len: " <<  searchText.length();
        //        qDebug() << "searchText toAscii     strlen(text):" << strlen(searchText.toAscii().data());
        //        qDebug() << "searchText toUtf8      strlen(text):" << strutf8len;
        //        qDebug() << "searchText toLocal8Bit strlen(text):" << strlocal8bitlen;
#if 0
        while ((j = resStr.indexOf(searchText.toLatin1().data(), j, Qt::CaseInsensitive)) != -1) {
            //            qDebug() << "Found "+ searchText + " tag at index position:  " << j;
            setStringColor(j + 1, searchText.length());
            ++j;
        }

#else
        //        QChar c;
        ////        QString str = searchText.unicode();
        //        QChar *pws = (QChar *)searchText.unicode();
        //        c = *pws;
        //        quint16 length = searchText.toAscii().length();
        //        qDebug() << "lenth:"<< length;
        //        qDebug() << "c:"<< c;

        //        qDebug() << "str format:" <<resStr;

        //        QLatin1String latinstr(searchText.toLatin1().data());

        ////        QString resStrUnicode = G2U(resStr.toLocal8Bit().data());

        //        while ((j = resStr.indexOf(latinstr, j, Qt::CaseInsensitive)) != -1) {
        //            qDebug() << "Found "+ searchText + " tag at index position:  " << j;
        //            setStringColor(j + 1, length);
        //            ++j;
        //        }

#define SHOWMAXCHARNUMS_ENG 20   /* 英文最多颜色显示的个数 */
#define SHOWMAXCHARNUMS_CHI 40   /* 汉语最多颜色显示的个数 */

        int showmaxnums = 0;

        //根据引用的内容来判断吧，既然无法同时满足，只能这样了，根据字符是否为汉字来走不同的路径
        if(strutf8len == strlocal8bitlen)
        {
            goflat = CHAR_ENGLISH;
            //qDebug() << "english letter!!!";
        }
        else
        {
            goflat = CHAR_LOCA8Bit;
            //qDebug() << "!!!not english letter";
        }

        if( goflat == CHAR_ENGLISH)
        {
            while ((j = resStr.indexOf(searchText.toLatin1().data(), j, Qt::CaseInsensitive)) != -1) {
                //qDebug() << "Found "+ searchText + " tag at index position:  " << j;
                setStringColor(j + 1, searchText.length());
                ++j;
                if(showmaxnums > SHOWMAXCHARNUMS_ENG)
                {
                    break;
                }
                showmaxnums++;
            }

        }else{
            //汉字
            QChar c;
            QChar *pws = (QChar *)searchText.unicode();
            c = *pws;
            quint16 length = searchText.toAscii().length();
            qDebug() << "lenth:"<< length;
            //            qDebug() << "c:"<< c;

            //打印所有搜索的字符
            int i;
            for(i = 0;i< length;i++){
                qDebug() << "c:"<< *(pws + i);
            }
            //思路:按汉字的位置依次获取一下。
            /*
  1.比如搜索“外文名”
  2.先搜索‘外’，获取位置
  3.再搜索‘文’，依次获取
  4.最后再把内容进行处理，确认哪些显示哪些不显示
  5.如果字符串大于3个，则按搜索一、三个来计算
  6.按首字符、最后一个字符来计算，效果不佳
*/
            showmaxnums = 0;
            QVector<int> firstcharposvec;
            firstcharposvec.clear();
            //查找第一个字符位置
            while (((j = resStr.indexOf(*(pws + 0), j, Qt::CaseInsensitive)) != -1))
            {
                qDebug() << "Found "+ searchText + " tag at index jjjjjjjj:  " << j;
                //                setStringColor(j + 1, length);
                firstcharposvec.push_back(j);
                ++j;
                if(showmaxnums > SHOWMAXCHARNUMS_CHI)
                {
                    break;
                }
                showmaxnums++;
            }
            qDebug()<< "\n";

            showmaxnums = 0;
            QVector<int> seccharposvec;
            seccharposvec.clear();

            int searchCharPos = 0;
            if(length >=3)
            {
                searchCharPos = /*length -*/ 2;
            }else{
                searchCharPos = 1;
            }


            //查找第二个字符位置
            j = 0;
            while (((j = resStr.indexOf(*(pws + searchCharPos), j, Qt::CaseInsensitive)) != -1))
            {
                qDebug() << "Found "+ searchText + " tag at index jjjjjjjj:  " << j;
                //                setStringColor(j + 1, length);
                seccharposvec.push_back(j);
                ++j;
                if(showmaxnums > SHOWMAXCHARNUMS_CHI)
                {
                    break;
                }
                showmaxnums++;
            }
            QVector<int> reltposvec;
            reltposvec.clear();

            //比较两次所得，查找首字符在其中的位置
            for(QVector<int>::iterator iter=firstcharposvec.begin(); iter!=firstcharposvec.end(); iter++)
            {
                //                  if( *iter == 3)
                //                         veci.erase(iter);
                for (QVector<int>::iterator siter=seccharposvec.begin(); siter!=seccharposvec.end(); siter++)
                {
                    qDebug() << "firstcharposvec pos:" << *iter;
                    qDebug() << "seccharposvec   pos:" << *siter;
                    if(*iter + searchCharPos == *siter)
                    {
                        reltposvec.push_back(*iter);
                        qDebug() << "find pos:" << *iter;
                        seccharposvec.erase(siter);
                        break;
                    }
                }
            }

            firstcharposvec.clear();
            seccharposvec.clear();


            for (int i = 0; i < reltposvec.size(); ++i)
            {
                //                qDebug() << "last pos:" << reltposvec.at(i);
                setStringColor(reltposvec.at(i) + 1, length);
            }
            reltposvec.clear();


            //            while (((j = resStr.indexOf(*(pws + 0), j, Qt::CaseInsensitive)) != -1)
            //                   &&((k = resStr.indexOf(*(pws + 1), k, Qt::CaseInsensitive)) != -1))
            //            {
            //                //                for(i = 0;i< length;i++){
            //                //                    qDebug() << "c:"<< *(pws + i);
            //                //                }
            //                qDebug() << "Found "+ searchText + " tag at index jjjjjjjj:  " << j;
            //                qDebug() << "Found "+ searchText + " tag at index kkkkkkkk:  " << k;
            //                if(j + 1 == k)
            //                {
            //                    setStringColor(j + 1, length);
            //                    ++j;
            //                    k=j;
            //                    //                    ++k;
            //                }else{
            //                    ++j;
            //                    k=j;
            //                    //                    ++k;
            //                }
            //            }
        }






        ////        qDebug() << "str format:" <<resStr;

        //        QLatin1String latinstr(searchText.toLatin1().data());

        ////        QString resStrUnicode = G2U(resStr.toLocal8Bit().data());

        //        while ((j = resStr.indexOf(latinstr, j, Qt::CaseInsensitive)) != -1) {
        //            qDebug() << "Found "+ searchText + " tag at index position:  " << j;
        //            setStringColor(j + 1, length);
        //            ++j;
        //        }


#endif


    }
    else
    {
        //qDebug() << "-->>>> not found";
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
    //    if(resStrWithHtml.contains(searchText, Qt::CaseInsensitive)) //不管大小写
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

    //    QString resStrUnicode = G2U(GenCode_str.toLocal8Bit().data());
    //    ui->genshow_textEdit->setText(resStrUnicode);



    ui->genshow_textEdit->setPlainText(GenCode_str);
    SearchTextResWithColor(GenCode_str);

    //    setCharColor(10);
    //    ui->genshow_textEdit->setHtml(GenCode_str);
    //    ui->genshow_textEdit->moveCursor(QTextCursor::End);
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
    //开机删除死机bug
    if(0 == selectresult.existflag )
        return;

    //    if()
    str_print(index_key_color);
    //先判定第一个不删除吧，点击note右侧的内容进行删除时出现问题
    if( flag_selectLeft )
    {
        /*  标准对话框——警示消息框   */
        QMessageBox::warning(NULL,"Warning",
                             str_china(请选择右侧进行修改),
                             QMessageBox::Yes,QMessageBox::Yes);
        //        /*  标准对话框——警示消息框   */
        //        QMessageBox::warning(NULL,"Warning",
        //                             str_china(请选择左侧进行删除),
        //                             QMessageBox::Yes,QMessageBox::Yes);
        return;
    }

    str_print(index_key_color);


    ui_dialog->note_textEdit_dia->setText(selectresult.note_list.at(index_key_color));
    ui_dialog->content_textEdit_dia->setText(selectresult.content_list.at(index_key_color));
    ui_dialog->index_textEdit_dia->setText(selectresult.keyword_list.at(index_key_color));

#if 0/* 此两处打印打挂了，为啥？ */
    //    str_print(selectresult.vartype_list.at(index_key_color));
    //    str_print(selectresult.aspect_field.at(index_key_color));
    //    str_print(get_aspect_list_index(selectresult.aspect_field.at(index_key_color)));
#endif

    //    ui_dialog->comboBox_aspect->setCurrentIndex(get_aspect_list_index(selectresult.aspect_field.at(index_key_color)));

    ui_dialog->comboBox_vartype->setCurrentIndex(get_CurrentIndex_comboBox_vartype(selectresult.vartype_list.at(index_key_color)));
    /* 选择的数据库表往右窜呢 */
    ui_dialog->langtype_comboBox->setCurrentIndex(CurrentIndex_comboBox_langtype + 1);
    //    ui_dia_selectdb->comboBox_selectdb->setCurrentIndex(CurrentIndex_comboBox_langtype);

    SaveUiMove();
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
    if(ui->checkBox_rightTextClean->isChecked())//右边清空内容
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
                                                    QDesktopServices::storageLocation(QDesktopServices::DesktopLocation),
                                                    tr("Txt (*.txt)"
                                                       ";;Ctype (*.c *.C *.cc *.h)"
                                                       ";;Cpptype(*.cpp *.CPP *.h)"
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

    int retbtn = QMessageBox::information(this,str_china(请确认入库共有标题内容),
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
#if 0 //出现乱码，不知道什么情况
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
        //内容添加后，更新控件中内容的相关显示
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
        int linenumber = ui_autoindb->textEdit_show->textCursor().blockNumber();//光标所在行
        if(linenumber == linenumber_old)
            return;
        str_print(linenumber);
        //获取光标左侧文本内容
        cursor_left_text.clear();

        textlist_indb_content.clear();
        for(int i = 0; i< linenumber ; i++){
            QTextBlock textBlock = ui_autoindb->textEdit_show->document()->findBlockByLineNumber(i);//通过行号找到指定行 数据块
            cursor_left_text += textBlock.text()+"\n";
        }
        textline_total = ui_autoindb->textEdit_show->document()->lineCount();//文本总行数
        str_print(textline_total);
        str_print(cursor_left_text);
        for(int i = linenumber; i< textline_total-1 ; i++){
            QTextBlock textBlock = ui_autoindb->textEdit_show->document()->findBlockByLineNumber(i);//通过行号找到指定行 数据块
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

    //获取内容toUtf8
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
        QMessageBox::information(NULL, str_china(类型), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
        *ret = 1;
        return;
    }
    if(content.isEmpty())
    {
        QMessageBox::information(NULL, str_china(内容), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
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
        //消除内容存在的时候的提示
        //        QMessageBox::information(NULL, str_china(声明), str_china(内容已经存在), QMessageBox::Yes, QMessageBox::Yes);
        //        *ret = 1;
        return;
    }

    if(vartype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(变量类型), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        *ret = 1;
        return;
    }

    if(index_keyword.isEmpty())
    {
        QMessageBox::information(NULL, str_china(检索), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        *ret = 1;
        return;
    }
    if(note.isEmpty())
    {
        QMessageBox::information(NULL, str_china(注释), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
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
    //        //对话框不关闭
    //        ui_dialog->content_textEdit_dia->clear();
    //    }else{
    //        InDb_Dialog->close();
    //    }


    //#else
    //    //对话框不关闭
    //    ui_dialog->content_textEdit_dia->clear();
    //#endif

    //    //内容添加后，更新控件中内容的相关显示
    //    update_show_after_insert();
}

void autoCCode::pushdb_checkbox_if_selected()
{
    //    self_print(pushdb_checkbox_if_selected);
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
    //    self_print(pasteClicpTextToSearchEdit);
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
    //    self_print(pasteClicpTextToAutoGetCon_UiDialog);
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

    //如果选中连续输入，则下面不处理
    if(ui->checkBox_inbox->isChecked())
    {
        return;
    }


    if(ui->checkBox_popupindb->isChecked())
    {
        if(STATE_CLIPBORD_CHAGED == IsClipboardChanged())
        {
            if(!isMinimized())//主窗口最小化时不操作任何数据写入
            {//窗口显示后，如果剪切板内容有变化，则进行窗口显示
                if(InDb_Dialog->isHidden() && //窗口为隐藏状态，则弹出
                        (!ui_dialog->langtype_comboBox->currentText().isEmpty()))
                {
                    if(!ui_dialog->content_textEdit_dia->toPlainText().isEmpty())
                    {
                        ui_dialog->content_textEdit_dia->clear();
                        SaveUiMove();
                        InDb_Dialog->show();
                    }else{
                        InDb_Dialog->hide();
                    }
                    //                    }

                }else{//窗口为显示状态
                    if(ui_dialog->content_textEdit_dia->toPlainText().isEmpty())
                    {//如果其内容为空，则隐藏
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
//是否显示界面左侧的内容了
int autoCCode::showcode_textEdit_AtBotton()
{
    //codeshow_textEdit
    if(ui->radioButton_showall->isChecked())
    {
        return TRUE;
    }
    //如果自动入库下面的里的数字为1，则显示，否则不显示
    return (ui_autoindb->spinBox_notenumber->text().toInt() == 1);
}

void autoCCode::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
    int numSteps = numDegrees / 15;//滚动的步数，*15就是鼠标滚动的角度
    if (event->orientation() == Qt::Horizontal) {
        //        scrollHorizontally(numSteps);       //水平滚动
        //        qDebug() << "horizontal";
    } else {
        //        scrollVertically(numSteps);       //垂直滚动
        //        qDebug() << "vectorial numSteps:" <<numSteps << ",numDegrees:" << numDegrees;
    }

    if(isCTRLKeyPressed && IsCursorInGenShowUi())
    {
        if(numSteps > 0)
        {
            ZoomInFont();
        }
        else if(numSteps < 0)
        {
            ZoomOutFont();
        }
    }
    event->accept();      //接收该事件
}

bool autoCCode::eventFilter(QObject *obj, QEvent *event)
{
    //    qDebug() << "eventFilter";
    if (obj == ui->lineEdit_search) //ui->lineEdit_search
    {

        //双击出现listView界面
        if (event->type() == QEvent::MouseButtonDblClick) {
            //qDebug()<<"double clicked!!";
            if(listView->isHidden())
            {
                on_lineEdit_search_MouseButtonDblClick();
            }
            else
            {
            }
            return true;
        }
        //单击隐藏listView界面
        if (event->type() == QEvent::MouseButtonPress) {
            //            qDebug()<<" clicked!!";

            //隐藏双击出来的listView列表框
            on_lineEdit_search_Key_Escape();
            return true;
        }
        //按键处理
        if(event->type() == QEvent::KeyPress)
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
            }
            else {
                //qDebug()<<"else Key !!";

            }

        }
    }

    //    qDebug()<<"isCTRLKeyPressed:" << isCTRLKeyPressed;
    //想添加一个Ctrl+滚轮放大字体的功能
    if(obj == ui->genshow_textEdit)
    {
        if (event->type()==QEvent::FocusIn)     //然后再判断控件的具体事件 (这里指获得焦点事件)
        {
            //            QPalette p=QPalette();
            //            p.setColor(QPalette::Base,Qt::green);
            //            ui->lineEdit1->setPalette(p);
            //隐藏双击出来的listView列表框
            on_lineEdit_search_Key_Escape();
            //             return true;

        }
        else if (event->type()==QEvent::FocusOut)    // 这里指 lineEdit1 控件的失去焦点事件
        {
            //            QPalette p=QPalette();
            //            p.setColor(QPalette::Base,Qt::white);
            //            ui->lineEdit1->setPalette(p);
            //            if(completeTextFlag == FLAG_YES)
            //            {
            //                return true;
            //            }
            //            qDebug()<<"lose focus,so  hidden!!";
            //            on_lineEdit_search_Key_Escape();
            //            return true;
        }

        //按键处理
        if(event->type() == QEvent::KeyRelease)
        {
            isCTRLKeyPressed = FALSE;
        }
        //按键处理
        if(event->type() == QEvent::KeyPress)
        {
            //qDebug()<<"KeyPress ed!!";
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();
            if (Qt::Key_Control == key){
                qDebug()<<"Key_Control   Pressed";
                isCTRLKeyPressed = TRUE;
            }
            else {
                //qDebug()<<"else Key !!";
            }
        }


        //        //按键处理
        //        if(event->type() == QEvent::KeyPress)
        //        {
        //            //qDebug()<<"KeyPress ed!!";
        //            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //            int key = keyEvent->key();
        //            if (Qt::Key_Down == key) {
        //                //qDebug()<<"Key_Down !!";
        //            } else if (Qt::Key_Up == key) {
        //                //qDebug()<<"Key_Up !!";

        //            } else if (Qt::Key_Escape == key) {
        //                //qDebug()<<"Key_Escape !!";
        //                on_lineEdit_search_Key_Escape();

        //            } else if (Qt::Key_Enter == key || Qt::Key_Return == key) {
        //                //qDebug()<<"Key_Enter   Key_Return!!";
        //            } else {
        //                //qDebug()<<"else Key !!";

        //            }
        ////            return true;
        //        }
    }
    return QObject::eventFilter(obj, event);
}

void autoCCode::on_lineEdit_search_MouseButtonDblClick()
{
    qDebug() << "on_lineEdit_search_MouseButtonDblClick";


    //添加，当前是否有选中库，如果没有则直接返回，可当作一个函数接口。
    if(!currentDbHaved())
    {
        return;
    }

    int minwidth = 200;
    int setWidth = minwidth > ui->lineEdit_search->width() ? minwidth : ui->lineEdit_search->width();
    int setHeight = this->height()*3/4;
    listView->setMinimumWidth(setWidth);
    listView->setMaximumWidth(setWidth);
    listView->setMinimumHeight(setHeight);
    listView->setMaximumHeight(setHeight);

    int lineeditH = ui->lineEdit_search->height();
    QPoint p(ui->lineEdit_search->mapToGlobal(QPoint(0,0+lineeditH)));//弹出列表listView的位置

    QStringList s1;
    s1.clear();

#define LIMITDATA_MAX 100

#if 1//从数据库表looktexthis_table中查找前LIMITDATA_MAX个最常用的数据 begin
    //保存查找关键字 begin
    codestructSets *setsLookHis = get_table_sets_bytype(languagetype_LookTextHis_);
    if(!setsLookHis)
        return;
    LookTextHistoryResult looktexthistoryres;
    clr_looktexthisresult(looktexthistoryres);

    QString looktexthis_express;
    looktexthis_express.clear();
    looktexthis_express = QString("select * from %1 where relatedtblname='%2'  order by looktimes desc limit %3")
            .arg(setsLookHis->talbename)
            .arg(sets->talbename)
            .arg(LIMITDATA_MAX);

    b.searchdatabase_lookTextHisTbl(setsLookHis->databasename,looktexthis_express.toLocal8Bit().data(),
                                    looktexthistoryres,
                                    "");//searchStr置空，表示所有全查询

    //    qDebug() << "count:"<< looktexthistoryres.looktextarry.count();
    for(int i=0;i< looktexthistoryres.looktextarry.count();i++)
    {
        //qDebug() << "arry:"<< looktexthistoryres.looktextarry.at(i);
        //只显示记录小于30个字节的字段
        if(looktexthistoryres.looktextarry.at(i).length() < 30)
        {
            s1.append(looktexthistoryres.looktextarry.at(i));
        }
    }

#endif //从数据库表looktexthis_table中查找前10个最常用的数据 begin


    //    model->setStringList(looktexthistoryres.looktextarry);
    model->setStringList(s1);
    listView->setModel(model);
    if (model->rowCount() == 0)
    {
        return;
    }

    listView->move(p);
    listView->show();
}

//隐藏双击出来的listView列表框
void autoCCode::on_lineEdit_search_Key_Escape()
{
    if(listView->isVisible())
        listView->hide();
}

void autoCCode::completeText(QModelIndex index)
{
    qDebug() << "completeText";
    //    qDebug() << index.data().string();
    qDebug() << index.data().toString();

    QString lookText = index.data().toString();
    ui->lineEdit_search->clear();
    ui->lineEdit_search->setText(lookText);

    //完成后关闭显示view
    listView->hide();
}

//当前是否有选中库，如果没有则直接返回，可当作一个函数接口。
bool autoCCode::currentDbHaved(void)
{
    str_print(CurrentIndex_comboBox_langtype);
    sets = get_table_sets_bytype(CurrentIndex_comboBox_langtype);
    if(!sets)
    {
        return FALSE;
    }

    return TRUE;
}

//ui->listWidget_codeview->addItems(s1); 显示内容缩短优化
QStringList autoCCode::listWidget_codeview_subShow(QStringList &strlist)
{
#define SHOW_MAX_CHARS 60
    QStringList s1;
    s1.clear();
    for(int i=0;i< strlist.count();i++)
    {
        //qDebug() << "arry:"<< looktexthistoryres.looktextarry.at(i);
        //只显示记录小于30个字节的字段
        if(strlist.at(i).length() > SHOW_MAX_CHARS)
        {
            s1.append(strlist.at(i).left(SHOW_MAX_CHARS)+" ...");
        }
        else
        {
            s1.append(strlist.at(i));
        }

    }
    return s1;
}


void autoCCode::CharFormat(void)
{
    defcharfmt = ui->genshow_textEdit->currentCharFormat();
}

void autoCCode::setWindowTitle_Main(const QString &arg1)
{
    setWindowTitle(arg1);
}
//    logfilename +="C:/";
//    logfilename +=QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
//    logfilename +=QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
//    logfilename +=QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
//    logfilename +=QDesktopServices::storageLocation(QDesktopServices::ApplicationsLocation);
//    QDesktopServices::storageLocation(QDesktopServices::HomeLocation)
//    QDesktopServices::storageLocation(QDesktopServices::ApplicationsLocation)
//    QDesktopServices::storageLocation(QDesktopServices::TempLocation)
QString autoCCode::getCurrentDateTimeTxt()
{
    QDate date;
    QTime time;
    QString logfilename;
    logfilename.clear();
    /* 默认保存到桌面上 */
    logfilename +=QDesktopServices::storageLocation(QDesktopServices::DesktopLocation) + "/";
    logfilename += date.currentDate().toString("sclogyyyy-MM-dd");
    logfilename += time.currentTime().toString("_HH-mm-ss");
    logfilename +=".txt";
    //    qDebug() << "save path:" << logfilename;
    return logfilename;
}

void autoCCode::MainSetUp()
{
    self_print(MainSetUp);
    SetUp_Dialog->show();
}

void autoCCode::on_pushButton_foreColor_clicked2()
{
    self_print(on_pushButton_foreColor_clicked);
    setforegroudColor();
}

void autoCCode::on_pushButton_backColor_clicked2()
{
    self_print(on_pushButton_backColor_clicked);
    setbackgroudColor();
}
//颜色对话框设置-前景色
void autoCCode::setforegroudColor()
{
    QPalette palette    = ui->genshow_textEdit->palette();
    QPalette palettebtn = ui_setup->pushButton_foreColor->palette();
    const QColor &color=QColorDialog::getColor(palette.color(QPalette::Base),this);
    if(color.isValid()){
        qDebug() <<"textedit set color";
        palette.setColor(QPalette::Text,color);
        palettebtn.setColor(QPalette::Button,color);
        ui->genshow_textEdit->setPalette(palette);
        ui_setup->pushButton_foreColor->setPalette(palettebtn);

    }
    update();
}
//颜色对话框设置-背景色
void autoCCode::setbackgroudColor()
{
    QPalette palette=ui->genshow_textEdit->palette();
    QPalette palettebtn = ui_setup->pushButton_backColor->palette();
    const QColor &color=QColorDialog::getColor(palette.color(QPalette::Base),this);
    if(color.isValid()){
        palette.setColor(QPalette::Base,color);
        palettebtn.setColor(QPalette::Button,color);
        ui->genshow_textEdit->setPalette(palette);
        ui_setup->pushButton_backColor->setPalette(palettebtn);
    }

}
//setup 界面，默认设置的前景色和背景色
void autoCCode::setDefaultColor()
{
    QPalette palette=ui->genshow_textEdit->palette();
    QPalette palette_backcolorbtn = ui_setup->pushButton_backColor->palette();
    QPalette palette_foreColorbtn = ui_setup->pushButton_foreColor->palette();
    palette.setColor(QPalette::Base,Qt::white);//背景色
    palette.setColor(QPalette::Text,Qt::black);//前景色
    palette_backcolorbtn.setColor(QPalette::Button,Qt::white);
    palette_foreColorbtn.setColor(QPalette::Button,Qt::black);
    ui->genshow_textEdit->setPalette(palette);
    ui_setup->pushButton_backColor->setPalette(palette_backcolorbtn);
    ui_setup->pushButton_foreColor->setPalette(palette_foreColorbtn);
    ui->genshow_textEdit->setFont(QFont("Times",14,QFont::Bold));
    ui_setup->pushButton_font->setText("Times");
}
//设置字体对话框
void autoCCode::setFont()
{
    bool ok;
    const QFont& font = QFontDialog::getFont(&ok,
                                             ui->genshow_textEdit->font(),
                                             this,
                                             tr("字体对话框"));
    if(ok) {// 如果<确定>,设置字体.
        ui->genshow_textEdit->setFont(font);
        ui_setup->pushButton_font->setText(font.family());
    }
}
#define MAXFONTSIZE 72
#define MINFONTSIZE 1
void autoCCode::ZoomInFont()
{

    qDebug() << "ZoomInFont";
    QFont font = ui->genshow_textEdit->font();
    int fontsize = font.pointSize();
    //    qDebug() << "OldFontFamily:" << OldFontFamily;
    qDebug() << "fontsize:" << fontsize;
    fontsize += 1;
    if(fontsize > MAXFONTSIZE)
        fontsize = MAXFONTSIZE;

    ui->genshow_textEdit->setFont(QFont(font.family(),fontsize,font.weight()));
}

void autoCCode::ZoomOutFont()
{
    qDebug() << "ZoomOutFont";
    QFont font = ui->genshow_textEdit->font();
    int fontsize = font.pointSize();
    //    qDebug() << "OldFontFamily:" << OldFontFamily;
    qDebug() << "fontsize:" << fontsize;
    fontsize -= 1;
    if(fontsize < MINFONTSIZE)
        fontsize = MINFONTSIZE;

    ui->genshow_textEdit->setFont(QFont(font.family(),fontsize,font.weight()));
}

int autoCCode::IsCursorInGenShowUi(void)
{
    int x = ui->genshow_textEdit->x();
    int y = ui->genshow_textEdit->y();
    int w = ui->genshow_textEdit->width();
    int h = ui->genshow_textEdit->height();
    int left = x;
    int right = x + w;
    int top = y;
    int bot = y + h;
    qDebug() << "x:" <<x << " y:" <<y;
    qDebug() << "w:" <<x << " h:" <<y;
    qDebug() << "left:" <<left << " right:" <<right;
    qDebug() << "top:" <<top << " bot:" <<bot;
    QPoint a = QCursor::pos();
    qDebug() << "QCursor x:" <<a.x() << " y:" <<a.y();
    if(a.x() < right && a.x() > left && a.y() > top && a.y() < bot)
    {
        qDebug() << "in scope!!!!!!!!!!!!!!!";
        return TRUE;
    }
    else
    {
        qDebug() << "out scope";
        return FALSE;
    }
}

/* 右键菜单 */
void autoCCode::contextMenuEvent(QContextMenuEvent *event)
{
    QCursor cur=this->cursor();
    QMenu *menu=new QMenu(this);
    menu->addAction(Act_Maxsize); //添加菜单项1
    menu->addAction(Act_Normal); //添加菜单项2
    menu->exec(cur.pos()); //关联到光标
}


//右选入库 按钮按下时
//实现自动入库功能
void autoCCode::on_pushButton_rightTextSelectIndb_clicked()
{
    QString cotext = ui->genshow_textEdit->toPlainText().trimmed();
    QString cotextHtml = ui->genshow_textEdit->toPlainText();
    if(cotext.isEmpty())
    {
        qDebug() << "context. empty";
        return;
    }
    //光标选中的文本
    QString str_selected = ui->genshow_textEdit->textCursor().selectedText().trimmed();
    if(str_selected.isEmpty())
    {
        qDebug() << "select text. empty";
        return;
    }
    ok_btn_dia_clicked_self_another(cotext, str_selected);
    //插入数据是否发送
    if(ui_setup->checkBox_email->isChecked())
    {/* 发送HTML内容，否则为连续的内容 */
        SendMail(cotextHtml);
    }
}
//根据定义的数据，直接入库并显示
void autoCCode::ok_btn_dia_clicked_self_another(QString con,QString str_sel)
{
    self_print(ok_btn_dia_clicked_self_another);

    //获取内容
    QString content = con;
    QString lanaugetype = ui_dialog->langtype_comboBox->currentText();
    QString index_keyword   = con;
    index_keyword = index_keyword.replace("\n"," ");
    //    index_keyword.trimmed();
    QString note   = str_sel;
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
        QMessageBox::information(NULL, str_china(类型), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
        return;
    }
    if(content.isEmpty())
    {
        QMessageBox::information(NULL, str_china(内容), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
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
        QMessageBox::information(NULL, str_china(声明), str_china(内容已经存在), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(vartype.isEmpty())
    {
        QMessageBox::information(NULL, str_china(变量类型), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(index_keyword.isEmpty())
    {
        QMessageBox::information(NULL, str_china(检索), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(note.isEmpty())
    {
        QMessageBox::information(NULL, str_china(注释), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    //    if(aspect.isEmpty())
    //    {
    //        QMessageBox::information(NULL, str_china(范围), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
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

    //    if(ui_dialog->checkBox_EOR->isChecked())
    //        ui_dialog->note_textEdit_dia->clear();

    //    if(ui->checkBox_inbox->isChecked())
    //    {
    //        //对话框不关闭
    //        ui_dialog->content_textEdit_dia->clear();
    //    }else{
    //        InDb_Dialog->update();
    //        InDb_Dialog->close();
    //    }


#else
    //对话框不关闭
    ui_dialog->content_textEdit_dia->clear();
#endif

    b.creatable(&insertcontent);
    b.inserttable(&insertcontent);


    //内容添加后，更新控件中内容的相关显示
    update_show_after_insert();

    //    is_selected = FALSE;//插入数据后，把此置为FALSE
}



//发送邮件
void autoCCode::SendMail(QString str)
{
//    ui->sendmailab->clear();
    //----------------------
    //检查必要条件:
    QMessageBox msgWarning (this);

    QString defaultsend="allinfosend@126.com";
    QString defaultpass="lmr2973194+-*/";
    QString defaultrecv="allinfosets@126.com;wxjlmr@126.com";
    QString host="smtp.126.com";
    int port=25;
    bool ssl = false;
    bool auth = true;
#if 1
    QString user = defaultsend/*ui->username->text().trimmed()*/;
    if(user.isEmpty())
    {
        msgWarning.setText("please set user");
        msgWarning.exec();
        //        ui->username->setFocus();
        return;
    }

    QString password = defaultpass/*ui->password->text().trimmed()*/;
    if(password.isEmpty())
    {
        msgWarning.setText("please set password");
        msgWarning.exec();
        //        ui->password->setFocus();
        return;
    }

    //发送者，与用户名相同
    QString strSender = defaultsend/*ui->sender->text().trimmed()*/;
    if(strSender.isEmpty())
    {
        msgWarning.setText("please set sender!!!");
        msgWarning.exec();
        //        ui->sender->setFocus();
        return;
    }

    //接收者
    QString strRecipient = defaultrecv/*ui->recipients->text()*/;
    if(strRecipient.isEmpty())
    {
        msgWarning.setText("please set sender!!!");
        msgWarning.exec();
        //        ui->recipients->setFocus();
        return;
    }
#endif


    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));


    EmailAddress *sender = SendEmail::stringToEmail(strSender/*ui->sender->text()*/);

    //主题,默认以时间为主题
    QString subject = gettimeofCurrent();
    //内容
    QString &html = str;/*ui->contentText->toHtml()*/;

    SmtpClient smtp (host, port, ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

    MimeMessage message;

    message.setSender(sender);
    message.setSubject(subject);

    //接收人列表
    QStringList rcptStringList = strRecipient.trimmed().split(';');/*ui->recipients->text().trimmed().split(';')*/;

    //删除空的联系人
    while(rcptStringList.size() > 0 && rcptStringList.last().trimmed().isEmpty())
    {
        rcptStringList.removeLast();
    }

    for (int i = 0; i < rcptStringList.size(); ++i)
        message.addRecipient(SendEmail::stringToEmail(rcptStringList.at(i)));

    MimeHtml content;
    content.setHtml(html);

    message.addPart(&content);

    if (!smtp.connectToHost())
    {
        ui->sendmailab->setText("SMTP link error.");
        return;
    }

    if (auth)
    {
        if (!smtp.login(user, password))
        {
            ui->sendmailab->setText("login fail.");
            return;
        }
    }

    if (!smtp.sendMail(message))
    {
        ui->sendmailab->setText("Send Fail");
        return;
    }
    else
    {
        ui->sendmailab->setText("Send OK");
    }

    smtp.quit();
//不知道什么问题？如果没有下面或者上面的设置UTF-8,发送为乱码，但是此处不更改，本地显示有问题
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));


}

QString autoCCode::gettimeofCurrent()
{
    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    return dt.toString("yyyy/MM/dd  hh:mm:ss");
}
