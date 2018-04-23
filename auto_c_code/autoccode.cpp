#include "autoccode.h"
#include "ui_autoccode.h"
#include "ui_PushDbdialog.h"
#include "ui_dialog_select_database.h"
#include "ui_autoindb.h"
#include "ui_setup1.h"
#include "ui_toolstabwidget.h"
#include "ui_codetools.h"
#include <QtGui>
#include "prefix_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>

#include "gencodedatabase.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <QString>
#include <QTime>
#include <QElapsedTimer>

#include "qxtglobalshortcut/qxtglobalshortcut.h"   //add shortcut
#include "debugsets.h"

#include <QtNetwork/QUdpSocket>
#include<QtNetwork/QHostAddress>
#include <QMessageBox>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QTimer>
#include "ui_udppkgdialog.h"
#include "sockthread.h"
#include "msgtipsautoshut.h"
//calender
#include "window_calender.h"
#include <QProgressDialog>
#include <QProgressBar>
#include "ssh2_forut.h"
#include <QShortcut>


#define DEFAULT_PORT 22222
//#define DEFAULT_PORT   "16689"

#define DEFAULT_HOSTADDR "192.168.1.100"

enum{
    RET_SUCESS,
    RET_FAIL,
};



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

#define DEBUGINFO(A)  ClsDebugSets::Push(A)



#if 1
//编码汉字
#define str_china(A)     QString::fromLocal8Bit(#A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif

#define UNUSEDVAR(A) (void)A;

//ui genshow_textedit设置文本，另外需要设置到子层的数据。可能传递的数据存在问题，双击了。
#define UI_GENSHOW_TEXTEDIT_SETTEXT(string)\
do{\
SWITCH_genshow_textEdit_SetText(string);\
 SendParent2ChildUI(0);\
}while(0)

#define SWITCH_genshow_textEdit_SetText(string)\
do{\
    showbeautycontent(string, 1);\
}while(0)

//ui tools界面下Ctrl按键是否按下
static bool isCTRLKeyPressed_uitools = FALSE;
static bool isFKeyPressed_uitools = FALSE;
static bool isCKeyPressed_uitools = FALSE;




/*============================================
* FuncName    : autoCCode::autoCCode
* Description :
* @parent     :
* Author      :
* Time        : 2017-05-28
============================================*/
autoCCode::autoCCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoCCode),
    ui_dialog(new Ui::Dialog),
    ui_dia_selectdb(new Ui::Dialog_select_database),
    ui_autoindb(new Ui::AutoIndb),
    ui_setup(new Ui::SetUpDialog),
    ui_toolsets(new Ui::ToolsTabWidget),
    ui_codetool(new Ui::WizardPage),
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
    Isgenshow_textEdit_Enter(FALSE),
    isToolsContent_Enter(FALSE),
    isToolsSuffix_Enter(FALSE),
    is_selected(FALSE),
    pthread_event(NULL),
    socket(NULL),
    hostaddr(NULL),
    tcpserver(NULL),
    window_calender(NULL),
    helloworldNet(NULL),
    isDropFileEnd(true)
{
//    codec = QTextCodec::codecForName("GBK");//must first used,or is NULL,die
    codec = QTextCodec::codecForName("UTF-8");//must first used,or is NULL,die
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale()); //设置GBK到本地
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());


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
    /* tools sets menu */  //独立的
    toolsTabWidget = new QTabWidget();
    ui_toolsets->setupUi(toolsTabWidget);
    ui_toolsets->comboBox_path->installEventFilter(NULL);

    //    ui_toolsets->textEdit_main_content->installEventFilter(NULL);
    //    ui_toolsets->textEdit_suffix->installEventFilter(NULL);
    toolsTabWidget->hide();


    codetool = new QWizardPage();
    ui_codetool->setupUi(codetool);
    //    codetool->show();

    //network
    remoteip = ReadRemoteIpAddr();
    ui_setup->lineEdit_remoteIP->setText(remoteip);

    localip = ReadLocalIpAddr();
    ui_setup->comboBox_localIp->setEditText(localip);

    //network read and write
    //写数据统计
    TotalBytes   = 0;
    byteWritten  = 0;
    bytesToWrite = 0;

    //    //读数据统计
    //    TotalReadBytes = 0;
    //    bytesReceived  = 0;
    //    bytesNeedRecv  = 0;

    /*
    ** 处理扫描夹
    */
    pathFileNums = 0;
    //增加进度条
    progress=new QProgressDialog(this);
//    progress->setLabelText(QString("scaning").arg(""));
    //添加setWindowFlags之后进度条在window中显示，无法单独出来
    //progress->setWindowFlags(Qt::FramelessWindowHint);
//    progress->setRange(0,idmainmaxnums);
    progress->setModal(true);
    progress->setFocus();
    //必须显示出来
    progress->hide();


    key_escaple_pressed = false;




    //测试hello world netthings
    helloworldNet = new NetThings(this);
//    connect(helloworldNet, SIGNAL(emitMsg(QString&)), helloworldNet, SLOT(updateMsg(QString&)));
    connect(ui->genshow_textEdit, SIGNAL(textChanged()), this, SLOT(updateHelloMsg()),
            Qt::DirectConnection);

    helloworldNet->start();


    pathlist.clear();


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
    ThreadSets();
    DebugSets();
    ReadHistorySettings();

    UISets();

}

/*============================================
* FuncName    : autoCCode::PopMenu
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::PopMenu()
{
    /* 右键菜单 */
    Act_Maxsize = new QAction(tr("XML"), this);
    Act_Normal = new QAction(tr("SHOW"), this);
    connect(Act_Maxsize, SIGNAL(triggered()), this, SLOT(ADDChange()));
    connect(Act_Normal, SIGNAL(triggered()), this, SLOT(XMLChange()));
}


/*============================================
* FuncName    : autoCCode::shortCutSet
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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

    QxtGlobalShortcut * search_clean = new QxtGlobalShortcut(QKeySequence("Ctrl+C"), this);
    QObject::connect(search_clean, SIGNAL(activated()),this, SLOT(search_text_clear()));

    QxtGlobalShortcut * search_text = new QxtGlobalShortcut(QKeySequence("Ctrl+F"), this);
    QObject::connect(search_text, SIGNAL(activated()),this, SLOT(procFindShortCut()));

    /**
      **
快捷键
Ctrl +F 查找
Ctrl +C 清空
Shift+Alt+V 粘贴查找

      **/

//    QShortcut *findShortCut = new QShortcut(this);
////    ui->lineEdit_search->setKey(tr("Ctrl+F"));
//    ui->pushButton_search->setShortcut(Qt::CTRL&&Qt::Key_F);

////    ui->lineEdit_search->setPlaceholderText("请输入查找的数据");
//    ProcShowText("");

//    connect(findShortCut, SIGNAL(activated()),this,SLOT(procFindShortCut()));

//    QShortcut *clearcontShortCut = new QShortcut(this);
////    ui->lineEdit_search->setKey(tr("Ctrl+C"));
//    ui->pushButton_clean->setShortcut(Qt::CTRL&&Qt::Key_C);
//    connect(clearcontShortCut, SIGNAL(activated()),this,SLOT(procClearShortCut()));

}

/*============================================
* FuncName    : autoCCode::ProgressBarSet
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ProgressBarSet(void)
{
    self_print(ProgressBarSet);
    if(ui_autoindb->progressBar)
        ui_autoindb->progressBar->setValue(0);

}
//在创建了过滤器之后，下面要做的是安装这个过滤器。安装过滤器需要调用installEventFilter()函数。
/*============================================
* FuncName    : autoCCode::InstallEventFilterSets
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::InstallEventFilterSets(void)
{
    ui->lineEdit_search->installEventFilter(this);

    ui->listWidget_codeview->installEventFilter(this);
    ui->listWidget_note->installEventFilter(this);
    ui->genshow_textEdit->installEventFilter(this);
    //    ui_toolsets->textEdit_main_content->installEventFilter(this);
    //    ui_toolsets->textEdit_suffix->installEventFilter(this);

    ui->choseCodeDB_btn->installEventFilter(this);

    ui_dia_selectdb->comboBox_selectdb->installEventFilter(this);

    ui->indb_btn->installEventFilter(this);

    ui_dialog->cancel_btn_dia->installEventFilter(this);
    ui_dialog->ok_btn_dia->installEventFilter(this);

    ui_dialog->langtype_comboBox->installEventFilter(this);
}

/*============================================
* FuncName    : autoCCode::ListViewSets
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ListViewSets()
{
    listView = new QListView(this);
    model = new QStringListModel(this);
    listView->setWindowFlags(Qt::ToolTip);
    //    connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(setCompleter(const QString &)));
    //    connect(listView, SIGNAL(clicked(QModelIndex)), this, SLOT(completeText(const QModelIndex &)));
    QObject::connect(listView,SIGNAL(clicked(QModelIndex)),this,SLOT(completeText(QModelIndex)));
    //network localipaddr
    QObject::disconnect(ui_setup->comboBox_localIp,SIGNAL(currentIndexChanged(QString)),
                        this,SLOT(saveBindLocalIP(QString)));
    ui_setup->comboBox_localIp->addItems(getLstIp());

    QStringList ipsets = getLstIp();
    int curindex = 0;
    foreach (QString ipaddr, ipsets) {
        if (ipaddr == localip)
        {
            break;
        }
        curindex ++;
    }
    qDebug() <<"loca addr index:" << curindex;
    //network localipaddr
    QObject::connect(ui_setup->comboBox_localIp,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(saveBindLocalIP(QString)));
    //network localip
    ui_setup->comboBox_localIp->setCurrentIndex(curindex);

}

/*============================================
* FuncName    : autoCCode::ProgressBarSetValue
* Description :
* @value      :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ProgressBarSetValue(int value)
{
    self_print(ProgressBarSetValue);
    if(value >=0 && value <=100){
        ui_autoindb->progressBar->setValue(value);
        ui_autoindb->progressBar->update();
    }

}

/*============================================
* FuncName    : autoCCode::ThreadSets
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ThreadSets()
{
    qDebug() << "thread sets";
    /* pthread线程，防止界面假死 */
    pthread_event = new UiThread(this);
    qDebug() << "thread event:" << pthread_event;
    pthread_event->start();

}

/*============================================
* FuncName    : autoCCode::DebugSets
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::DebugSets()
{
    /* 打印信息的数据包 printfsets */
    ClsDebugSets::Push("DebugSets");
    //    ClsDebugSets qCDebug;
    qCDebug << "DebugSets2222222222222";
}



/*============================================
* FuncName    : autoCCode::QTimerSet
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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

    //    timer_genshow_textEdit_checkContent_append = new QTimer(this);
    //    timer_genshow_textEdit_checkContent_append->start(500);
    //    QObject::connect(timer_genshow_textEdit_checkContent_append,
    //                     SIGNAL(timeout()),this,SLOT(check_genshow_textEdit_is_append()));
    QClipboard *clipboard = QApplication::clipboard();
    //    QString clipboardtext = clipboard->text();
    //检测剪切板内容是否变化

    QObject::connect(clipboard,SIGNAL(dataChanged()),this,
                     SLOT(check_genshow_textEdit_is_append()));


}

/*============================================
* FuncName    : autoCCode::UISets
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::UISets()
{
    woUi = new CodeSophia(this);

//    Ui::CodeSophia *codeUI = new Ui::CodeSophia(woUi);
//    codeUI->setupUi(woUi);
//    codeUI->
    //转换格式为CodeSophia
//     = new Ui::CodeSophia;
//    uico->setupUi(woUi);

    QObject::connect(ui_setup->checkBox_codeshortcut,SIGNAL(stateChanged(int)),
                     this, SLOT(on_checkBox_codeshortcut_stateChanged(int)));

    QObject::connect(ui->checkBox_senddata2subui, SIGNAL(stateChanged(int)),
                     this, SLOT(SendParent2ChildUI(int)));
    if (ui_setup->checkBox_codeshortcut->isChecked())
    {
//        woUi->show();
//        woUi->showMinimized();
        //setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
//        setWindowState(Qt::WindowMinimizeButtonHint);
//        woUi->show();

        qDebug() << "windowFlags() :" << windowFlags();
        woUi->showMinimized();
        woUi->update();
//        woUi->hide();
    }
    else
        woUi->hide();

}

/*============================================
* FuncName    : autoCCode::keyPressEventSet
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::keyPressEventSet()
{
    //    QObject::connect(btn4,SIGNAL(clicked()),
    //                     this,SLOT(popup()));

    QObject::connect(ui->lineEdit_search,SIGNAL(editingFinished()),
                     this,SLOT(SearchEnter()));
    //鼠标有效
    setMouseTracking(true);

    QObject::connect(qApp->clipboard(),SIGNAL(selectionChanged()),
                     this,SLOT(on_selectionChanged()));
    QObject::connect(qApp->clipboard(),SIGNAL(dataChanged()),
                     this,SLOT(on_selectionChanged()));
}


/*============================================
* FuncName    : autoCCode::checkboxSet
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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

    /* setup send pkg checkbox */
    QObject::connect(ui_setup->checkBox_SeverFlag,SIGNAL(toggled(bool)),
                     this,SLOT(on_checkBox_checkBox_SeverFlag_change(bool)));
}

/*============================================
* FuncName    : autoCCode::on_checkBox_rightTextSelectIndb_change
* Description :
* @flag       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_checkBox_rightTextSelectIndb_change(bool flag)
{
    qDebug() << "on_checkBox_rightTextSelectIndb_change, flag: " << flag << endl  ;
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

/*============================================
* FuncName    : autoCCode::on_checkBox_rtQuery_change
* Description :
* @flag       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_checkBox_rtQuery_change(bool flag)
{

}

/*============================================
* FuncName    : autoCCode::on_checkBox_checkBox_SeverFlag_change
* Description :
* @flag       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_checkBox_checkBox_SeverFlag_change(bool flag)
{
    //    qDebug() << "checkbox sendpkg change:" << flag << endl;
    if(!flag)
    {
        return;
    }

    saveBindLocalIP(localip);

    //    pUdp_ui = new Ui::UdpPkgDialog;
    ////    QDialog *dialog = new QDialog;
    //    QWidget *pUdpdialog = new QWidget;
    //    pUdp_ui->setupUi(pUdpdialog);
    //    pUdpdialog->show();
    //    QObject::connect(pUdp_ui->cancelButton,SIGNAL(clicked()),
    //                     pUdpdialog,SLOT(close()));
    //    QObject::connect(pUdp_ui->okButton,SIGNAL(clicked()),
    //                     this,SLOT(on_pUdpdialog_okBtn_clicked()));
    //    QObject::connect(pUdp_ui->okButton,SIGNAL(clicked()),
    //                     pUdpdialog,SLOT(close()));

}

/*============================================
* FuncName    : autoCCode::get_rtQuery_enable
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
quint8 autoCCode::get_rtQuery_enable()
{
    return (Qt::Checked == ui_setup->checkBox_rtQuery->checkState());
}



/*============================================
* FuncName    : autoCCode::on_checkBox_SEL_change
* Description :
* @flag       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_checkBox_SEL_change(bool flag)
{
    if(!flag)
    {
        //如果没有选中的内容，则默认为第一行文字
        set_note_textEdit_firstline();
    }
}

/*============================================
* FuncName    : autoCCode::set_index_text
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::set_index_text()
{
    if(ui->checkBox_same->isChecked())
        ui_dialog->index_textEdit_dia->setText(ui_dialog->content_textEdit_dia->toPlainText());
}
/*============================================
* FuncName    : autoCCode::set_note_textEdit_firstline
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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


/*============================================
* FuncName    : autoCCode::lineTextEditSet
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::lineTextEditSet(void)
{
    QObject::connect(ui->lineEdit_search,SIGNAL(textChanged(QString)),
                     this,SLOT(ProcShowText(QString)));

#if 0//文本改变搜索
    QObject::connect(ui->lineEdit_search,SIGNAL(textChanged(QString)),
                     this,SLOT(SearchText(QString)));
#else//定时搜索
    //    QObject::connect(lineEdit_search_timer,SIGNAL(timeout()),this,SLOT(SearchText_WithTimer()));
    //是否支持实时查询
    QObject::connect(lineEdit_search_timer,SIGNAL(timeout()),this,SLOT(SearchText_WithTimer_Enter()));

#endif


    //ip地址
    QObject::connect(ui_setup->lineEdit_remoteIP,SIGNAL(textChanged(QString)),
                     this,SLOT(saveRemoteIP(QString)));
}

/*============================================
* FuncName    : autoCCode::dragDropSet
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::dragDropSet(void)
{
    //允许拖放的文字添加到编辑框中
    ui->codeshow_textEdit->setAcceptDrops(true);
    ui->codeshow_textEdit->hide();
    //    ui->codeshow_textEdit->setHidden(true);
}
/*============================================
* FuncName    : autoCCode::pushButton_setup
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::pushButton_setup()
{
    QObject::connect(ui_setup->pushButton_backColor,SIGNAL(clicked()),
                     this,SLOT(on_pushButton_backColor_clicked2()));
    QObject::connect(ui_setup->pushButton_foreColor,SIGNAL(clicked()),
                     this,SLOT(on_pushButton_foreColor_clicked2()));

    QObject::connect(ui_setup->pushButton_font,SIGNAL(clicked()),
                     this,SLOT(setFont()));
}

/*============================================
* FuncName    : autoCCode::pushButtonSet
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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



    //计算器
    QObject::connect(ui_toolsets->pushButton_calc,SIGNAL(clicked()),this,SLOT(on_pushButton_calc_exe_clicked()));
    //记事本
    QObject::connect(ui_toolsets->pushButton_notepad,SIGNAL(clicked()),this,SLOT(on_pushButton_notepad_exe_clicked()));
    //notepad++.exe
    QObject::connect(ui_toolsets->pushButton_notepadplus,SIGNAL(clicked()),this,SLOT(on_pushButton_notepadplus_exe_clicked()));
    //Python pushButton_python
    QObject::connect(ui_toolsets->pushButton_python,SIGNAL(clicked()),this,SLOT(on_pushButton_python_exe_clicked()));
    //Python python D:\test.py
    QObject::connect(ui_toolsets->pushButton_python_Dtest,SIGNAL(clicked()),this,SLOT(on_pushButton_python_Dpathtestfile_exe_clicked()));
    //Calender
    QObject::connect(ui_toolsets->pushButton_calender,SIGNAL(clicked()),this,SLOT(on_pushButton_show_calender_clicked()));


    //source insight
    QObject::connect(ui_toolsets->pushButton_sourceinsight,SIGNAL(clicked()),this,SLOT(on_pushButton_sourceinsight_exe_clicked()));

    //命令行CMD
    QObject::connect(ui_toolsets->pushButton_cmd,SIGNAL(clicked()),this,SLOT(on_pushButton_cmd_exe_clicked()));
    //添加路径
    QObject::connect(ui_toolsets->pushButton_addpath,SIGNAL(clicked()),this,SLOT(on_tools_pushButton_addpath_clicked()));

    QObject::connect(ui_toolsets->pushButton_open,SIGNAL(clicked()),this,SLOT(on_tools_pushButton_openpath_clicked()));

    QObject::connect(ui_toolsets->pushButton_delpath,SIGNAL(clicked()),this,SLOT(on_tools_pushButton_delpath_clicked()));

//    QObject::connect(ui_toolsets->pushButton_opendir,SIGNAL(clicked()),this,SLOT(on_tools_pushButton_opendir_clicked()));

    QObject::connect(ui_toolsets->comboBox_path,SIGNAL(currentIndexChanged(int)),this,SLOT(on_tools_pushButton_openpath_auto(int)));




//
    QMenu *mymenu=new QMenu;
    QAction *opendir=new QAction("打开文件夹",this);
    QAction *openfile=new QAction("打开文件",this);
    QObject::connect(opendir,SIGNAL(triggered()),this,SLOT(on_tools_pushButton_opendir_clicked()));
    QObject::connect(openfile,SIGNAL(triggered()),this,SLOT(on_tools_pushButton_openfile_clicked()));

    mymenu->addAction(opendir);
    mymenu->addAction(openfile);
    ui_toolsets->pushButton_opendir->setMenu(mymenu);
    ui_toolsets->pushButton_opendir->setStyleSheet("QPushButton::menu-indicator{image:none}");
    ui_toolsets->pushButton_opendir->installEventFilter(this);

    ui_toolsets->comboBox_path->installEventFilter(this);

    //连接ssh
    QObject::connect(ui_toolsets->pushButton_connectssh,SIGNAL(clicked()),this,SLOT(on_tools_pushButton_connectssh_clicked()));


}
/*============================================
* FuncName    : autoCCode::set_search_text
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::set_search_text()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui->lineEdit_search->clear();
    ui->lineEdit_search->setText(clipboard->text());
}
/*============================================
* FuncName    : autoCCode::search_text_clear
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::search_text_clear()
{
    if(!InDb_Dialog->isHidden())
    {
        ui_dialog->content_textEdit_dia->clear();
        return;
    }
    cleanLineTextEditSearch();
    ui->lineEdit_search->clear();
    ui->lineEdit_search->setFocus();
}
/*============================================
* FuncName    : autoCCode::note_clear
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::note_clear()
{
    if(!InDb_Dialog->isHidden())
        ui_dialog->note_textEdit_dia->clear();
}
/*============================================
* FuncName    : autoCCode::note_focus
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::note_focus()
{
    if(!InDb_Dialog->isHidden())
        ui_dialog->note_textEdit_dia->setFocus();
}
/*============================================
* FuncName    : autoCCode::content_focus
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::content_focus()
{
    if(!InDb_Dialog->isHidden())
        ui_dialog->content_textEdit_dia->setFocus();
}

/*============================================
* FuncName    : autoCCode::note_clear_focus
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::note_clear_focus()
{
    note_clear();
    note_focus();
}

/*============================================
* FuncName    : autoCCode::content_clear
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::content_clear()
{
    ui_dialog->content_textEdit_dia->clear();
}
/*============================================
* FuncName    : autoCCode::setCliptext
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::setCliptext()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui_dialog->note_textEdit_dia->clear();
    ui_dialog->note_textEdit_dia->setText(clipboard->text());
}
/*============================================
* FuncName    : autoCCode::setCliptext_content
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::setCliptext_content()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui_dialog->content_textEdit_dia->clear();
    ui_dialog->content_textEdit_dia->setText(clipboard->text());
}
/*============================================
* FuncName    : autoCCode::setDesktop_center
* Description :
* @dialog     :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::setDesktop_center(QDialog *dialog)
{
    QDesktopWidget *desk=QApplication::desktop();
    int wd=desk->width();
    int ht=desk->height();
    UNUSEDVAR(wd);
    dialog->move(0,(ht-height()/2)/2);
}

/*============================================
* FuncName    : autoCCode::WidgetYsetDesktop_center
* Description :
* @widget     :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::WidgetYsetDesktop_center(QWidget *widget)
{
    QDesktopWidget *desk=QApplication::desktop();
    int wd=desk->width();
    int ht=desk->height();
    UNUSEDVAR(wd);
    widget->move(0,(ht-height()/2)/2);
}

/*============================================
* FuncName    : autoCCode::WidgetXsetDesktop_center
* Description :
* @widget     :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::WidgetXsetDesktop_center(QWidget *widget)
{
    QDesktopWidget *desk=QApplication::desktop();
    int wd=desk->width();
    int ht=desk->height();
    UNUSEDVAR(wd);
    widget->move((wd - width()/2)/2,0);
}


/*============================================
* FuncName    : autoCCode::WidgetXYsetDesktop_center
* Description :
* @widget     :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::WidgetXYsetDesktop_center(QWidget *widget)
{
    QDesktopWidget *desk=QApplication::desktop();
    int wd=desk->width();
    int ht=desk->height();
    UNUSEDVAR(wd);
    widget->move((wd - width()/2)/2,(ht-height()/2)/2);
}


/*============================================
* FuncName    : autoCCode::minSize_ui_dialog
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::minSize_ui_dialog()
{
    InDb_Dialog->resize(QSize());
    setDesktop_center(InDb_Dialog);

}
/*============================================
* FuncName    : autoCCode::widthSize_ui_dialog
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::widthSize_ui_dialog()
{

    InDb_Dialog->resize(QSize(QApplication::desktop()->width(),Qt::MinimumSize));
    QDesktopWidget *desk=QApplication::desktop();
    //    int wd=desk->width();
    int ht=desk->height();
    InDb_Dialog->move(0,(ht-height()/2)/2);
}
/*============================================
* FuncName    : autoCCode::maxSize_ui_dialog
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::maxSize_ui_dialog()
{

    InDb_Dialog->setMaximumSize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()));
    //    InDb_Dialog->setWindowFlags(InDb_Dialog->windowFlags()& Qt::WindowMaximizeButtonHint & Qt::WindowMinimizeButtonHint);

    //    InDb_Dialog->resize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()));
    InDb_Dialog->resize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()-60));
    InDb_Dialog->move(0,0);
}
/*============================================
* FuncName    : autoCCode::comboBoxSet
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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

    //network localipaddr
    QObject::connect(ui_setup->comboBox_localIp,SIGNAL(currentIndexChanged(QString)),
                     this,SLOT(saveBindLocalIP(QString)));

    //support remote flag
    QObject::connect(ui_setup->checkBox_supportRemote,SIGNAL(clicked(bool)),
                     this,SLOT(checkSupportRemote(bool)));
}


/*============================================
* FuncName    : autoCCode::comboBox_selectdb_currentIndexChanged
* Description :
* @arg1       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::comboBox_selectdb_currentIndexChanged(const QString &arg1)
{
    self_print(comboBox_selectdb_currentIndexChanged);
    str_print(arg1);

    if(arg1.isEmpty())
    {
        setWindowTitle_Main("AutoCCode");
    }
    else if(ui_setup->checkBox_SeverFlag->isChecked())
    {
        setWindowTitle_Main(QString("Server--") + arg1);
    }
    else{
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

    if(!ui->lineEdit_search->text().replace(" ","").isEmpty())
    {
        SearchText(ui->lineEdit_search->text());
        dialog_selectdb->close();
        return;
    }

    dialog_selectdb->close();

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
}

/*============================================
* FuncName    : autoCCode::textEditSet
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::textEditSet(void)
{
    self_print(textEditSet);

    //    QObject::connect(ui->db_comboBox,SIGNAL(activated(QString)),
    //                     this,SLOT(on_db_comboBox_activated(QString)));


    //    QObject::connect(ui_toolsset)

}
/*============================================
* FuncName    : autoCCode::addstr_aspect_comboBox
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::addstr_comboBox
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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
     << str_china(Go)
     <<str_china(Hadoop)
    <<str_china(Jave)
    <<str_china(JavaScript)
    <<str_china(Mysql)
    <<str_china(Oracle)
    <<str_china(Patchs)
    <<str_china(Perl)
    <<str_china(Postgresql)
    <<str_china(Php)
    <<str_china(Python)
    <<str_china(Qt)
    <<str_china(Qtquick)
    <<str_china(Rust)
    <<str_china(shell)
    <<str_china(Sqlite3)
    <<str_china(Swift)
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

/*============================================
* FuncName    : autoCCode::~autoCCode
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
autoCCode::~autoCCode()
{
//    delete woUi;
//    delete ui_dialog;//入库
//    delete ui_dia_selectdb;//选择数据库
//    delete ui_autoindb;//自动入库界面
//    delete ui_setup;//设置界面
//    delete ui_toolsets;//工具集
//    delete pUdp_ui; //UDP
//    delete ui_codetool;

    delete ui;//最后删除
}

/*============================================
* FuncName    : autoCCode::on_save_btn_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::db_comboBox_activated
* Description :
* @arg1       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::db_comboBox_activated(const QString &arg1)
{
    self_print(db_comboBox_activated);
}
//选择数据库
/*============================================
* FuncName    : autoCCode::on_choseCodeDB_btn_clicked
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_choseCodeDB_btn_clicked(void)
{
    self_print(on_choseCodeDB_btn_clicked);
    dialog_selectdb->move(ui->choseCodeDB_btn->x(), ui->choseCodeDB_btn->y());
    dialog_selectdb->show();
    update();

}
//生成代码库
/*============================================
* FuncName    : autoCCode::on_gencode_btn_clicked
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_gencode_btn_clicked(void)
{
    self_print(on_gencode_btn_clicked);
    //保存打开的路径
    static QString savedPath = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    savedPath,
                                                    tr(";;All Files(*.*)"
                                                       ";;Ctype (*.c *.C *.cc *.h)"
                                                       ";;Cpptype(*.cpp *.CPP *.h)"
                                                       ";;QTtype(*.c *.C *.cpp *.CPP *.ui *.rc *.pro *.h)"
                                                       ";;Pythontype(*.py *.PY)"
                                                       ";;JavaType(*.java)"
                                                       "Txt (*.txt)"));
    //    qDebug()<<"fileName:"<<fileName;
    QFileInfo fileinfo = QFileInfo(fileName);
    //文件名
    QString file_name = fileinfo.fileName();
    //文件后缀
    QString file_suffix = fileinfo.suffix();
    //绝对路径
    QString file_path = fileinfo.absolutePath();

    savedPath = file_path;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }

    readTextFile(fileName);
    file.close();
    return;

//    //before
//    if(ui_setup->checkBox_showpath->isChecked())
//    {
//        QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
//        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

//    }
//    else
//    {
//        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
//        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//    }

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
    SWITCH_genshow_textEdit_SetText(text_china);

    //    QTextStream out(&file);
    //    out << "Thomas M. Disch: " << 334 << endl;

    file.close();

//    //after
//    if(ui_setup->checkBox_showpath->isChecked())
//    {
//        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
//        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//    }
//    else
//    {
//        QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
//        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
//    }
}
/*============================================
* FuncName    : autoCCode::hide_inBtn
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::hide_inBtn(void)
{
    ui->indb_btn->hide();
}

/*============================================
* FuncName    : autoCCode::show_InBtn
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::show_InBtn(void)
{
    ui->indb_btn->show();
}
/*============================================
* FuncName    : autoCCode::SaveUiMove
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SaveUiMove()
{
    quint16 usWidth = (this->width() - InDb_Dialog->width())/2;
    quint16 usHeight = abs(this->height() - InDb_Dialog->height())/2;
    usHeight = usHeight > 80 ? usHeight:80;
    InDb_Dialog->move(this->pos().x() + usWidth,
                      this->pos().y() + usHeight);
    content_focus();
}

//入库函数
/*============================================
* FuncName    : autoCCode::on_indb_btn_clicked
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_indb_btn_clicked(void)
{
    self_print(on_indb_btn_clicked);
    QString select_text = ui->codeshow_textEdit->textCursor().selectedText();
    ui_dialog->content_textEdit_dia->setText(select_text);

    SaveUiMove();

    if(isMainServer())
    {
        return;
    }
    if(ui->checkBox_inbox->isChecked())
    {
        InDb_Dialog->show();
        //        InDb_Dialog->exec();
    }else{
        InDb_Dialog->show();
    }
}

/*============================================
* FuncName    : autoCCode::on_indb_window_show_hide
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
            if(!isMainServer())
            {
                InDb_Dialog->show();
            }
        }
        else
            InDb_Dialog->hide();
        //        InDb_Dialog->exec();
    }else{
        if(InDb_Dialog->isHidden())
        {
            SaveUiMove();
            if(!isMainServer())
            {
                InDb_Dialog->show();
            }
        }
        else
            InDb_Dialog->hide();
    }
}

//void autoCCode::on_outdb_btn_clicked(void)
//{
//    self_print(on_outdb_btn_clicked);
//}

/*============================================
* FuncName    : autoCCode::getLanguageType
* Description :
* @type       :
* Author      :
* Time        : 2017-05-28
============================================*/
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
    else if(type == "Perl")
    {
        return languagetype_Perl_;
    }
    else if(type == "Hadoop")
    {
        return languagetype_Hadoop_;
    }
    else if(type == "Go")
    {
        return languagetype_Go_;
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

/*============================================
* FuncName    : autoCCode::save_before_ops
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::save_before_ops(void)
{
    selectresult_tmp = selectresult;
    index_key_color_tmp = index_key_color;
    index_note_color_tmp = index_note_color;
}
/*============================================
* FuncName    : autoCCode::restore_before_ops
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::restore_before_ops(void)
{
    selectresult = selectresult_tmp;
    index_key_color = index_key_color_tmp ;
    index_note_color = index_note_color_tmp ;
}

//dialog ok button
/*============================================
* FuncName    : autoCCode::ok_btn_dia_clicked_self
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ok_btn_dia_clicked_self(void)
{
    self_print(ok_btn_dia_clicked_self);

    //获取内容
    QString content = ui_dialog->content_textEdit_dia->toPlainText().trimmed();
    QString lanaugetype = ui_dialog->langtype_comboBox->currentText();
    QString index_keyword   = ui_dialog->index_textEdit_dia->toPlainText().trimmed();
    index_keyword = index_keyword.replace("\n"," ");
    //    index_keyword.trimmed();
    QString note   = ui_dialog->note_textEdit_dia->toPlainText().trimmed();
    note = note.replace(" ","");
    note = note.replace("\n"," ");
    if(note.isEmpty())
    {
        note = "nokey!";
    }

    note += "\t\t\t\t";
    note += QDateTime::currentDateTime().toString("yyyy MMM d ddd,hh:mm:ss");

    //    QDateTime d = lo.toDateTime("Mon,26 Apr 2010, 08:21:03","ddd,d MMM yyyy, hh:mm:ss");
    //    Q_ASSERT(d.isValid());

    //    note.trimmed();
    QString vartype = ui_dialog->comboBox_vartype->currentText();
    QString aspect = ui_dialog->comboBox_aspect->currentText();


    if(lanaugetype.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("类型!! 不能为空"));
//        QMessageBox::information(NULL, str_china(类型), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
        return;
    }
    if(content.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("内容!! 不能为空"));
//        QMessageBox::information(NULL, str_china(内容), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
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
        if(isMainServer())
            return;
        close_indb_dialog();
        ShowTipsInfo(QString::fromLocal8Bit("内容已经存在"));
//        QMessageBox::information(NULL, str_china(声明), str_china(内容已经存在), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(vartype.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("变量类型 不能为空"));
//        QMessageBox::information(NULL, str_china(变量类型), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(index_keyword.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("检索 不能为空"));
//        QMessageBox::information(NULL, str_china(检索), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(note.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("注释 不能为空"));
//        QMessageBox::information(NULL, str_china(注释), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    //    if(aspect.isEmpty())
    //    {
    //        QMessageBox::information(NULL, str_china(范围), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
    //        return;
    //    }

    close_indb_dialog();

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


    //内容添加后，更新控件中内容的相关显示
    update_show_after_insert();

    is_selected = FALSE;//插入数据后，把此置为FALSE
}

/*============================================
* FuncName    : autoCCode::close_indb_dialog
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::close_indb_dialog()
{
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

}



/*============================================
* FuncName    : autoCCode::cancel_btn_dia_clicked_self
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::cancel_btn_dia_clicked_self(void)
{
    self_print(cancel_btn_dia_clicked_self);
    InDb_Dialog->close();
}
/*============================================
* FuncName    : autoCCode::aboutVersion
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::aboutVersion(void)
{
    ShowTipsInfo(QString::fromLocal8Bit("版本 %1").arg(GetVersion()));
//    QMessageBox::information(NULL, str_china(版本), GetVersion(),NULL,NULL);
    return;
}
/*============================================
* FuncName    : autoCCode::GetVersion
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
QString autoCCode::GetVersion(void)
{
    QString shortcuttips = str_china("快捷键 ") +
            str_china("Ctrl +F 查找") +
            str_china("Ctrl +C 清空") +
            str_china("Shift+Alt+V 粘贴查找");


    return str_china(自动生成代码)
            +"\n"
            + shortcuttips
            +"\n"
            +str_china(by小魏莱)
            +"\n"
            +version_autoccode;
}

/*============================================
* FuncName    : autoCCode::listWidgetSet
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::add_to_gen_code_textedit_by_keyword
* Description :
* @item       :
* Author      :
* Time        : 2017-05-28
============================================*/
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

    //内容前缀头 内容头
    if (ui_setup->checkBox_content_withheader->isChecked())
    {
        GenCode_str+="/*  ";
        GenCode_str+=selectresult.note_list.at(index);
        GenCode_str+="   */";
        GenCode_str+="\n";
    }
    GenCode_str+=selectresult.content_list.at(index);
    GenCode_str+="\n";
    GenCode_str+="\n";


    SWITCH_genshow_textEdit_SetText(GenCode_str);
    SearchTextResWithColor2(GenCode_str);
    ui->genshow_textEdit->moveCursor(QTextCursor::End);
    ui->listWidget_codeview->setFocus();
    //    update();
    //    ui->listWidget_codeview->update();
    ui->genshow_textEdit->update();
}
//添加到右边的内容中
/*============================================
* FuncName    : autoCCode::add_to_gen_code_textedit_by_note
* Description :
* @item       :
* Author      :
* Time        : 2017-05-28
============================================*/
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
    //内容前缀头 内容头
    if (ui_setup->checkBox_content_withheader->isChecked())
    {
        GenCode_str+="/*  ";
        GenCode_str+=selectresult.note_list.at(index);
        GenCode_str+="   */";
        GenCode_str+="\n";
    }

    GenCode_str+=selectresult.content_list.at(index);
    GenCode_str+="\n";
    GenCode_str+="\n";


    SWITCH_genshow_textEdit_SetText(GenCode_str);
    SearchTextResWithColor2(GenCode_str);
    //    update();
}


/*============================================
* FuncName    : autoCCode::rightClear_textedit
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::rightClear_textedit(void)
{
    GenCode_str.clear();
    ui->genshow_textEdit->clear();
    //    update();
}
/*============================================
* FuncName    : autoCCode::clr_selectresult
* Description :
* @result     :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::clr_looktexthisresult
* Description :
* @result     :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::clr_looktexthisresult(LookTextHistoryResult &result)
{
    result.looktextarry.clear();
    result.looktimes = 0;
}

/*============================================
* FuncName    : autoCCode::clear_listWidget_beforecall
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::clear_listWidget_beforecall(void)
{
    ui->listWidget_codeview->clear();
    ui->listWidget_note->clear();
    //    update();
}
/*============================================
* FuncName    : autoCCode::listWidget_scrollToBottom
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::listWidget_scrollToBottom(void)
{
    ui->listWidget_codeview->scrollToBottom();
    ui->listWidget_note->scrollToBottom();
    //    update();
}
/*============================================
* FuncName    : autoCCode::listWidget_scrollToTop
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::listWidget_scrollToTop(void)
{
    ui->listWidget_codeview->scrollToTop();
    ui->listWidget_note->scrollToTop();
    //    update();
}
/*============================================
* FuncName    : autoCCode::select_db_by_vartype
* Description :
* @select_express  :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::ui_comboBox_vartype_currentIndexChanged
* Description :
* @str        :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::comboBox_aspect_currentIndexChanged
* Description :
* @str        :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::comboBox_aspect_currentIndexChanged(const QString &str)
{
    self_print(comboBox_aspect_currentIndexChanged);
    str_print(str);

    if(!str.isEmpty())
    {

    }

}
/*============================================
* FuncName    : autoCCode::add_aspect_totable
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::judge_color_index
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::listWidget_note_scroll_sync
* Description :
* @item       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::listWidget_note_scroll_sync(QListWidgetItem* item)
{
    self_print(listWidget_note_scroll_sync);
    unsigned int index = 0;
    unsigned int cmplen = 0;
    QString str = item->text();
    if(str.isEmpty())
        return;
    if(selectresult.content_list.size() == 0)
        return;

    cmplen = str.length() <= 30 ? str.length() : 30;
    judge_color_index();
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str.left(cmplen) == selectresult.keyword_list.at(i).left(cmplen))
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
/*============================================
* FuncName    : autoCCode::listWidget_codeview_scroll_sync
* Description :
* @item       :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::update_show_after_insert
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::delete_btn_clicked_selfdefine
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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
        QString keyword = selectresult.keyword_list.at(index_key_color);
        keyword = keyword.replace("'", "''");
        QString select_express = QString("update %1 set delflag=1 where keywords='%2'")
                .arg(sets->talbename)
                .arg(keyword);

        b.updatetable(sets->langtype,select_express);

        update_show_after_insert();
QMessageBox::information(this,"Information",
                                 "Your comment:<h1><font color=red>" +text +"</font></h1>"+" deleted!",
                                 QMessageBox::Yes,QMessageBox::Yes);
    }





}

/*============================================
* FuncName    : autoCCode::SearchText_WithTimer
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SearchText_WithTimer(void)
{
    QString searchStr = ui->lineEdit_search->text();
    static QString oldStr;
    if(oldStr != searchStr)
        SearchText(searchStr);
    oldStr = searchStr;

}

/*============================================
* FuncName    : autoCCode::SearchText_WithTimer_Enter
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SearchText_WithTimer_Enter(void)
{
    qApp->processEvents();
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


/*============================================
* FuncName    : autoCCode::SearchText
* Description :
* @searchStr  :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SearchText(const QString &searchStr)
{
    qApp->processEvents();
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

    //    qCDebug <<"SearchText:" << searchStr;

    /* 判断查询字符和实际的字符串是否相同 */
    if(searchStr != ui->lineEdit_search->text())
    {
        qCDebug << "searchstr not same with line edit text,may changeing !!";
        qCDebug << "searchstr:" << searchStr;
        qCDebug << "line text:" << ui->lineEdit_search->text();
        return;
    }
    qCDebug << "searchstr:" << searchStr;
    qCDebug << "line text:" << ui->lineEdit_search->text();

    if(showcode_textEdit_AtBotton())
    {
        ui->codeshow_textEdit->setText(selectresult.contentstr);
    }

    clear_listWidget_beforecall();
    if(0 == selectresult.keyword_list.length()){
        alert();
        return;
    }else{
        selectresult.keyword_subshowlist = listWidget_codeview_subShow(selectresult.keyword_list);
        ui->listWidget_codeview->addItems(selectresult.keyword_subshowlist);
        //        ui->listWidget_codeview->addItems(selectresult.keyword_list);
        ui->listWidget_note->addItems(selectresult.note_list);
    }


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
}
/*============================================
* FuncName    : autoCCode::cleanLineTextEditSearch
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::add_column_lowercase_keywords_content
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::dragEnterEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }
}


/*============================================
* FuncName    : autoCCode::iteratorDirectory_FileNums
* Description :
* @path       :
* Author      :
* Time        : 2017-05-28
============================================*/
quint32 autoCCode::iteratorDirectory_FileNums(QString path)
{
    QDir dir(path);

    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        if(mfi.isFile())
        {
            pathFileNums +=1;
            qDebug()<< "File :" << mfi.fileName();
        }else
        {
            if(mfi.fileName()=="." || mfi.fileName() == "..")continue;
            qDebug() << "Entry Dir" << mfi.absoluteFilePath();
            iteratorDirectory_FileNums(mfi.absoluteFilePath());
//            ShowTipsInfo("扫描中...");
        }
    }
    return pathFileNums;
}

/*============================================
* FuncName    : autoCCode::zeropathFileNums
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::zeropathFileNums()
{
    pathFileNums = 0;
}
/*============================================
* FuncName    : autoCCode::getpathFileNums
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
quint32 autoCCode::getpathFileNums()
{
    return pathFileNums;
}


/*============================================
* FuncName    : autoCCode::zeroloopFileNums
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::zeroloopFileNums()
{
    loopFileNums = 0;
}

/*============================================
* FuncName    : autoCCode::iteratorDirectory_Print
* Description :
* @path       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::iteratorDirectory_Print(QString path)
{
    QDir dir(path);

    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        if(mfi.isFile())
        {
            qDebug()<< "File :" << mfi.fileName();
        }else
        {
            if(mfi.fileName()=="." || mfi.fileName() == "..")continue;
            qDebug() << "Entry Dir" << mfi.absoluteFilePath();
            iteratorDirectory_Print(mfi.absoluteFilePath());
        }
    }
}

char g_textbar[102];
char tipsstr[] = "|\\-/";

/*============================================
* FuncName    : autoCCode::textprogress_init
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::textprogress_init(void)
{
    memset(g_textbar, 0 ,sizeof(g_textbar));
}


/*============================================
* FuncName    : autoCCode::textprogress
* Description :
* @loop       :
* @total      :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::textprogress(quint32 loop, quint32 total)
{
    int i = 0;

    i = (loop * 100 ) / total;


//    while(i <= 100)
    {
        g_textbar[i]='#';
        printf("[%-101s][%d%%][%c]\r", g_textbar, i, tipsstr[i%4]);
        ShowTipsInfo(QString("[%-101s][%d%%][%c]\r").arg(g_textbar).arg(i).arg(tipsstr[i%4]));
//        fflush(stdout);
//        i++;
//        usleep(100000);
    }
//    printf("\n");

}

/*============================================
* FuncName    : autoCCode::dirprogress
* Description :
* @filename   :
* @loop       :
* @total      :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::dirprogress(QString filename, quint32 loop, quint32 total)
{
//    //增加进度条
//    progress=new QProgressDialog(this);
//    progress->setLabelText(tr("Search %1").arg("database"));
//    //添加setWindowFlags之后进度条在window中显示，无法单独出来
//    //progress->setWindowFlags(Qt::FramelessWindowHint);
//    progress->setRange(0,idmainmaxnums);
//    progress->setModal(true);
//    progress->setFocus();
//    //必须显示出来
    progress->setLabelText(QString("scaning %1").arg(filename));
    progress->show();
    progress->setRange(0,total);
    progress->setValue(loop);

}





/*============================================
* FuncName    : autoCCode::iteratorDirectory_Saveui
* Description :
* @path       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::iteratorDirectory_Saveui(QString path)
{
    QDir dir(path);

    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        if(mfi.isFile() && !mfi.isSymLink())
        {
//            qDebug()<< "File :" << mfi.fileName();
            qDebug()<< "File :" << mfi.absoluteFilePath();
            readTextFileAppend(mfi.absoluteFilePath());
            loopFileNums +=1;
//            textprogress(loopFileNums , getpathFileNums());
            dirprogress(mfi.absoluteFilePath(), loopFileNums , getpathFileNums());
        }else
        {
            if(mfi.fileName()=="." || mfi.fileName() == "..")continue;
            qDebug() << "Entry Dir" << mfi.absoluteFilePath();
            iteratorDirectory_Saveui(mfi.absoluteFilePath());
        }

        if(key_escaple_pressed)
        {
            break;
        }
    }
}

void autoCCode::filedraged(QList<QUrl> &urls)
{
    if(urls.isEmpty()){
        return;
    }
    qDebug() << "-->>urls cout:" << urls.size();

#if 0
    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty()){
        return;
    }
    this->setWindowTitle(fileName);
    readTextFile(fileName);
#else
    QString oldTitle = windowTitle();
    QString showTitle(oldTitle + "  ");
    QString fileName("");
    rightClear_textedit();//清空显示数据
    foreach (QUrl url, urls)
    {
        fileName = url.toLocalFile();
        showTitle += url.toLocalFile() + "  ";
        if(fileName.isEmpty()){
            continue;
        }
        this->setWindowTitle(showTitle);
        qDebug() << "~~filename :" << fileName;

        QFileInfo fileinfo(fileName);
        if(fileinfo.isDir())
        {
            this->setWindowTitle(url.toLocalFile());
            qDebug() << "~~is dir!!,filename :" << fileName;
//            QString msgstr(str_china(不能读取文件夹 %1.).arg(fileName));
//            ShowTipsInfoWithShowTime(msgstr, 1000);
            //遍历文件夹
//            QDir dir(fileName);
            zeropathFileNums();
            zeroloopFileNums();
//            qDebug() << "文件夹["<< fileName << "]里文件数量为:" << iteratorDirectory_FileNums(fileName);
            zeropathFileNums();
            zeroloopFileNums();
            quint32 IteratorNums = iteratorDirectory_FileNums(fileName);
            ShowTipsInfoWithShowTime(QString("文件数量为:%2,文件夹[%1]").arg(fileName).arg(IteratorNums),
                                     3000);
            zeroloopFileNums();
            textprogress_init();
            key_escaple_pressed = false;
            if(IteratorNums > 500)
            {
                if(QMessageBox::Yes == QMessageBox::warning(NULL, "warning", "文件数量较大，是否继续？", QMessageBox::Yes , QMessageBox::No))
                {
                    iteratorDirectory_Saveui(fileName);
                }
            }
            else
            {
                iteratorDirectory_Saveui(fileName);
            }
//
            key_escaple_pressed = false;
            zeropathFileNums();
            zeroloopFileNums();

        }
        else
        {
            qDebug() << "~~ is file!!,filename :" << fileName;
            readTextFileAppend(fileName);
        }


    }
#endif
}

/*============================================
* FuncName    : autoCCode::dropEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::dropEvent(QDropEvent *event)
{
    urls = event->mimeData()->urls();
    isDropFileEnd = false;
    filedraged(urls);
    isDropFileEnd = true;
    SendParent2ChildUI(0);
}

/*============================================
* FuncName    : autoCCode::readTextFile
* Description :
* @fileName   :
* Author      :
* Time        : 2017-05-28
============================================*/
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
        QString text_china;
        if(ui_setup->checkBox_showpath->isChecked())
        {
            text_china += stream.readAll() + "\n\n\n======[end]========\n" + fileName + "\n";
        }
        else
        {
            text_china += stream.readAll();
        }
        //        ui->codeshow_textEdit->setText(stream.readAll());
        SWITCH_genshow_textEdit_SetText(text_china);
        //        qDebug() << "content:" << stream.readAll();
    }
    else
    {
        QMessageBox::warning(NULL, str_china("提示"), str_china("文件较大"),NULL,NULL);
    }
    file.close();
}


/*============================================
* FuncName    : autoCCode::readTextFileAppend
* Description :
* @fileName   :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::readTextFileAppend(const QString &fileName)
{
#define MAXREADFILESIZE 10*1024*1024  /* 最大读取文件10M */
    str_print(fileName);
    /*  读取文件 只读   */
    QFile file(fileName);

    QString text_china = ui->genshow_textEdit->toPlainText();

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(NULL, str_china(提示), QString::fromLocal8Bit("读取文件失败[%1]").arg(fileName),NULL,NULL);
        return;
    }

    if(file.size() < MAXREADFILESIZE)
    {
        //        qDebug() << "file opend!!";
        QTextStream stream(&file);
        if(ui->checkBox_codecutf8->isChecked())
        {
            stream.setCodec("utf-8");
        }

        if(ui_setup->checkBox_showpath->isChecked())
        {
            text_china += stream.readAll() + "\n\n\n======[end]========\n" + fileName + "\n\n\n\n";
        }
        else
        {
            text_china += stream.readAll();
        }
        //        ui->codeshow_textEdit->setText(stream.readAll());
        SWITCH_genshow_textEdit_SetText(text_china);
        //        qDebug() << "content:" << stream.readAll();
    }
    else
    {
        QMessageBox::warning(NULL, str_china("提示"), str_china("文件较大"),NULL,NULL);
    }
    file.close();
}

/*============================================
* FuncName    : autoCCode::SearchEnter
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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


/*============================================
* FuncName    : autoCCode::listWidget_note_with_currentRowChanged
* Description :
* @row        :
* Author      :
* Time        : 2017-05-28
============================================*/
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
        //内容前缀头 内容头
        if (ui_setup->checkBox_content_withheader->isChecked())
        {
            GenCode_str+="/*  ";
            GenCode_str+=selectresult.note_list.at(index);
            GenCode_str+="   */";
            GenCode_str+="\n";
        }
        GenCode_str+=selectresult.content_list.at(index);
        GenCode_str+="\n";
        GenCode_str+="\n";

        SWITCH_genshow_textEdit_SetText(GenCode_str);
        SearchTextResWithColor2(GenCode_str);
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

/*============================================
* FuncName    : autoCCode::SetlistWidget_codeview_row
* Description :
* @row        :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SetlistWidget_codeview_row(int row)
{
    self_print(SetlistWidget_codeview_row);
    if(row < 0)
        row = 0;
    listWidget_codeview_row = row;
}
/*============================================
* FuncName    : autoCCode::GetlistWidget_codeview_row
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
int autoCCode::GetlistWidget_codeview_row(void)
{
    self_print(GetlistWidget_codeview_row);
    return listWidget_codeview_row;
}

/*  QT:设置textedit文本框中某个字符的格式 */
/*============================================
* FuncName    : autoCCode::setCharColor
* Description :
* @pos        :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::setStringColor
* Description :
* @pos        :
* @len        :
* Author      :
* Time        : 2017-05-28
============================================*/
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
        qApp->processEvents();
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
    //    ui->genshow_textEdit->updateGeometry();
    //    ui->genshow_textEdit->updatesEnabled();
    //    ui->genshow_textEdit->update();


    //    //修改数据显示不全的问题，为啥更改下大小就可以了？
    //    int h = ui->genshow_textEdit->height();
    //    int w = ui->genshow_textEdit->width();
    //    //    QPoint p();
    //    ui->genshow_textEdit->resize(w/2,h/2);
    //    ui->genshow_textEdit->resize(w,h);
    //    ui->genshow_textEdit->setFocus();
    //    ui->genshow_textEdit->update();

}

enum
{
    CHAR_ENGLISH,
    CHAR_LOCA8Bit,
    CHAR_NONE
};
/*============================================
* FuncName    : autoCCode::SearchTextResWithColor
* Description :
* @resStr     :
* Author      :
* Time        : 2017-05-28
============================================*/
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

#define SHOWMAXCHARNUMS_ENG 10   /* 英文最多颜色显示的个数 */
#define SHOWMAXCHARNUMS_CHI 20   /* 汉语最多颜色显示的个数 */

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

/*============================================
* FuncName    : autoCCode::SearchTextResWithColor2
* Description :
* @resStr     :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SearchTextResWithColor2(QString &resStr)
{

    //颜色框是否选中
    if(!ui->checkBox_ResWithColor->isChecked())
    {
        //        //背景白色，前景黑色
        setStringColor(0, 0);
        return;
    }
    //    //背景浅绿色，前景黑色
    //    ui->genshow_textEdit->setStyleSheet("background-color: rgb(60, 243, 243);color: rgb(0, 0, 0);");
    QString searchText = ui->lineEdit_search->text().trimmed();
    if(searchText.isEmpty())
    {
        //        ShowTipsInfo(QString::fromLocal8Bit("search text null!"));
        return;
    }

    QTime time;
    time.restart();
    while(1)
    {
        qApp->processEvents();
        //超长时间跳出 2s
        if(time.elapsed()/1000 >= 2)
        {
            break;
        }

//        FindCaseSensitively
        //精确查找
        if (ui->checkBox_query_exact->isChecked())
        {
            if (!ui->genshow_textEdit->find(searchText,QTextDocument::FindCaseSensitively))
            {
                //            ShowTipsInfo(QString::fromLocal8Bit("找不到 \"%1\"").arg(searchText));
                break;
            }
            else
            {
                QTextCursor cursor = ui->genshow_textEdit->textCursor();
                setStringColor(cursor.position() - searchText.length() + 1,
                               searchText.length());
                ui->genshow_textEdit->update();
            }
        }
        else
        {

            if (!ui->genshow_textEdit->find(searchText))
            {
                //            ShowTipsInfo(QString::fromLocal8Bit("找不到 \"%1\"").arg(searchText));
                break;
            }
            else
            {
                QTextCursor cursor = ui->genshow_textEdit->textCursor();
                setStringColor(cursor.position() - searchText.length() + 1,
                               searchText.length());
                ui->genshow_textEdit->update();
            }
        }
    }



//    while(1)
//    {
//        if (!ui->genshow_textEdit->find(searchText))
//        {
//            //            ShowTipsInfo(QString::fromLocal8Bit("找不到 \"%1\"").arg(searchText));
//            break;
//        }
//        else
//        {
//            QTextCursor cursor = ui->genshow_textEdit->textCursor();
//            setStringColor(cursor.position() - searchText.length() + 1,
//                           searchText.length());
//            ui->genshow_textEdit->update();
//        }
//    }


//    QSet<quint32> posset = getAllMatchPosResults(ui->genshow_textEdit->toPlainText(), searchText);
////    qDebug() << "posset result size " << posset.size();
//    QSet<quint32>::iterator i;
//    for (i = posset.begin(); i != posset.end(); ++i) {
//        //      *i = (*i).toLower(); // 使用 * 运算符获取遍历器所指的元素
////        qDebug()<<*i;
//        setStringColor(*i + 1,
//                       searchText.length());
//    }
//    ui->genshow_textEdit->update();



    return;
}

/*============================================
* FuncName    : autoCCode::listWidget_note_with_enter
* Description :
* @modelindex  :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::listWidget_note_with_enter(const QModelIndex &modelindex)
{
    self_print(listWidget_note_with_enter);
    rightTextShowClear_oncheched();
    //    qDebug()<<"index:"<<modelindex.row();
    unsigned int index = GetlistWidget_codeview_row();


    //内容前缀头 内容头
    if (ui_setup->checkBox_content_withheader->isChecked())
    {
        GenCode_str+="/*  ";
        GenCode_str+=selectresult.note_list.at(index);
        GenCode_str+="   */";
        GenCode_str+="\n";
    }
    GenCode_str+=selectresult.content_list.at(index);
    GenCode_str+="\n";
    GenCode_str+="\n";

    //    QString resStrUnicode = G2U(GenCode_str.toLocal8Bit().data());
    //    ui->genshow_textEdit->setText(resStrUnicode);



    SWITCH_genshow_textEdit_SetText(GenCode_str);
    SearchTextResWithColor2(GenCode_str);

    //    setCharColor(10);
    //    ui->genshow_textEdit->setHtml(GenCode_str);
    //    ui->genshow_textEdit->moveCursor(QTextCursor::End);
    ui->listWidget_codeview->setFocus();

}

/*============================================
* FuncName    : autoCCode::contentSetFocus
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::contentSetFocus(void)
{
    ui->lineEdit_search->clear();
    ui->lineEdit_search->setFocus();
}

/*============================================
* FuncName    : autoCCode::modify_content
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
    if(!isMainServer())
    {
        InDb_Dialog->show();
    }

}


/*============================================
* FuncName    : autoCCode::get_CurrentIndex_comboBox_vartype
* Description :
* @vartype    :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::get_aspect_list_index
* Description :
* @index_str  :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::rightTextShowClear_oncheched
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::rightTextShowClear_oncheched()
{
    if(ui->checkBox_rightTextClean->isChecked())//右边清空内容
        rightClear_textedit();
}

/*============================================
* FuncName    : autoCCode::getText_FromRight
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::getText_FromRight(void)
{
    self_print(getText_FromRight);
    QString str;
    str.clear();
    str = ui->genshow_textEdit->toPlainText();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(str,QClipboard::Clipboard);
}

/*============================================
* FuncName    : autoCCode::on_pushbtn_autoindb_clicked_self
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushbtn_autoindb_clicked_self()
{
    self_print(on_pushbtn_autoindb_clicked_self);

    if(dialog_autoindb->isHidden()){
        dialog_autoindb->show();
        dialog_autoindb->update();
    }

}
/*============================================
* FuncName    : autoCCode::on_ui_autoindb_pushBtn_Open_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_ui_autoindb_pushBtn_Open_clicked()
{
    self_print(on_ui_autoindb_pushBtn_Open_clicked);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
/*============================================
* FuncName    : QDesktopServices::storageLocation
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::on_ui_autoindb_pushBtn_process_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::get_autoindb_textedit_cursor_postion
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::ok_btn_dia_clicked_self_autoindb
* Description :
* @begintext   :
* @combinetext  :
* @ret         :
* Author      :
* Time        : 2017-05-28
============================================*/
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
        ShowTipsInfo(QString::fromLocal8Bit("类型 不能为空"));
//        QMessageBox::information(NULL, str_china(类型), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
        *ret = 1;
        return;
    }
    if(content.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("内容 不能为空"));
//        QMessageBox::information(NULL, str_china(内容), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
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
        ShowTipsInfo(QString::fromLocal8Bit("变量类型 不能为空"));
//        QMessageBox::information(NULL, str_china(变量类型), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        *ret = 1;
        return;
    }

    if(index_keyword.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("检索 不能为空"));
//        QMessageBox::information(NULL, str_china(检索), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        *ret = 1;
        return;
    }
    if(note.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("注释 不能为空"));
//        QMessageBox::information(NULL, str_china(注释), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
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

/*============================================
* FuncName    : autoCCode::pushdb_checkbox_if_selected
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::pasteClicpTextToSearchEdit
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::isCheckBox_cliptext_checked
* Description :
* @checked    :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::ui_dialog_AutoGetCon
* Description :
* @checked    :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::pasteClicpTextToAutoGetCon_UiDialog
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::IsClipboardChanged
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::PopInDbUi
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
                        if(!isMainServer())
                        {
                            InDb_Dialog->show();
                        }
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

/*============================================
* FuncName    : autoCCode::getLimitNum
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
int autoCCode::getLimitNum()
{
    return ui_autoindb->spinBox_notenumber->text().toInt()?ui_autoindb->spinBox_notenumber->text().toInt():10;
}
//是否显示界面左侧的内容了
/*============================================
* FuncName    : autoCCode::showcode_textEdit_AtBotton
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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

//void autoCCode::watchObject(QObject *watched)
//{

//}


/*============================================
* FuncName    : autoCCode::wheelEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
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
    //    qDebug() << "isCTRLKeyPressed :" << isCTRLKeyPressed;
    //    qDebug() << "windowFlags :" << this->windowFlags();

    //    if(isCTRLKeyPressed
    //            && IsCursorInGenShowUi(ui->genshow_textEdit))

    qDebug() << "isCTRLKeyPressed         :" <<  isCTRLKeyPressed;
    qDebug() << "Isgenshow_textEdit_Enter :" <<  Isgenshow_textEdit_Enter;
    qDebug() << "isToolsContent_Enter     :" <<  isToolsContent_Enter;
    qDebug() << "isToolsSuffix_Enter      :" <<  isToolsSuffix_Enter;



    if(isCTRLKeyPressed
            && Isgenshow_textEdit_Enter)
    {
        if(numSteps > 0)
        {
            ZoomInFont(ui->genshow_textEdit);
        }
        else if(numSteps < 0)
        {
            ZoomOutFont(ui->genshow_textEdit);
        }
    }

    ////    qDebug() << "isCTRLKeyPressed 2222:" << isCTRLKeyPressed;
    ////    qDebug() << "isTopLevel 2222:" << ui_toolsets->textEdit_main_content->isTopLevel();
    ////    if(isCTRLKeyPressed
    ////            && IsCursorInGenShowUi(ui_toolsets->textEdit_main_content))
    //    if(isCTRLKeyPressed
    //            && isToolsContent_Enter)
    //    {
    //        qDebug() << "ui_toolsets->textEdit_main_content!!!!!!!";
    //        if(numSteps > 0)
    //        {
    //            ZoomInFont(ui_toolsets->textEdit_main_content);
    //        }
    //        else if(numSteps < 0)
    //        {
    //            ZoomOutFont(ui_toolsets->textEdit_main_content);
    //        }
    //    }


    ////    isCTRLKeyPressed_TOOLSsuffix
    ////    if(isCTRLKeyPressed_TOOLSsuffix
    ////            /*&& IsCursorInGenShowUi(ui_toolsets->textEdit_suffix) */ )

    //    if(isCTRLKeyPressed && isToolsSuffix_Enter )
    //    {
    //        qDebug() << "ui_toolsets->textEdit_suffix_content!!!!!!!";
    //        if(numSteps > 0)
    //        {
    //            ZoomInFont(ui_toolsets->textEdit_suffix);
    //        }
    //        else if(numSteps < 0)
    //        {
    //            ZoomOutFont(ui_toolsets->textEdit_suffix);
    //        }
    //    }


    event->accept();      //接收该事件
}
//入库界面里的数据库列表弹出
/*============================================
* FuncName    : autoCCode::eventFilter_ui_dialog_langtype_comboBox
* Description :
* @watched    :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
bool autoCCode::eventFilter_ui_dialog_langtype_comboBox(QObject *watched, QEvent *event)
{
    if(!ui_setup->checkBox_indb_shortkey->isChecked())
    {
        return false;
    }


    //入库按钮
    if(watched == ui_dialog->langtype_comboBox)
    {
        if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
        {
            //            qDebug() << "comboBox_selectdb,coming here!!";
            ui_dialog->langtype_comboBox->showPopup();//combox下拉事件
        }/*
       else if (event->type()==QEvent::Leave)    // mouse leaves widget
       {
//            qDebug() << "comboBox_selectdb,leave now!!";
//            ui_dia_selectdb->comboBox_selectdb->showNormal();
//            dialog_selectdb->hide();
       }
       else if (event->type()==QEvent::WindowDeactivate)    // window was deactivated
       {
//            qDebug() << "comboBox_selectdb,WindowDeactivate!!";
//            ui_dia_selectdb
//            dialog_selectdb->hide();
       }
//        qDebug() << "comboBox_selectdb, event type:" << event->type();*/
    }
}

/*============================================
* FuncName    : autoCCode::eventFilter_ui_setup
* Description :
* @watched    :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
bool autoCCode::eventFilter_ui_setup(QObject *watched, QEvent *event)
{
    //    if(!ui_setup->checkBox_SeverFlag->isChecked())
    //    {
    //        return false;
    //    }

    //    qDebug() << "sendpkg checkbox is checked!!";




}

//bool autoCCode::eventFilter_ui_toolsets(QObject *watched, QEvent *event)
//{

//}

/*============================================
* FuncName    : autoCCode::eventFilter_ui_dialog
* Description :
* @watched    :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
bool autoCCode::eventFilter_ui_dialog(QObject *watched, QEvent *event)
{
    if(!ui_setup->checkBox_indb_shortkey->isChecked())
    {
        return false;
    }

    //入库按钮
    if(watched == ui->indb_btn)
    {
        if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
        {
            //            qDebug() << "comboBox_selectdb,coming here!!";
            //            dialog_selectdb->show();
            if(!isMainServer())
            {
                InDb_Dialog->show();//入库界面弹出
            }

        }/*
        else if (event->type()==QEvent::Leave)    // mouse leaves widget
        {
//            qDebug() << "comboBox_selectdb,leave now!!";
//            ui_dia_selectdb->comboBox_selectdb->showNormal();
//            dialog_selectdb->hide();
        }
        else if (event->type()==QEvent::WindowDeactivate)    // window was deactivated
        {
//            qDebug() << "comboBox_selectdb,WindowDeactivate!!";
//            ui_dia_selectdb
//            dialog_selectdb->hide();
        }
//        qDebug() << "comboBox_selectdb, event type:" << event->type();*/
    }


    //入库界面ok按钮
    if(watched == ui_dialog->ok_btn_dia)
    {
        if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
        {
            //            qDebug() << "comboBox_selectdb,coming here!!";
            //            dialog_selectdb->show();
            //            InDb_Dialog->show();//入库界面弹出
            ok_btn_dia_clicked_self();
        }/*
        else if (event->type()==QEvent::Leave)    // mouse leaves widget
        {
//            qDebug() << "comboBox_selectdb,leave now!!";
//            ui_dia_selectdb->comboBox_selectdb->showNormal();
//            dialog_selectdb->hide();
        }
        else if (event->type()==QEvent::WindowDeactivate)    // window was deactivated
        {
//            qDebug() << "comboBox_selectdb,WindowDeactivate!!";
//            ui_dia_selectdb
//            dialog_selectdb->hide();
        }
//        qDebug() << "comboBox_selectdb, event type:" << event->type();*/
    }

    //入库界面cancle按钮
    if(watched == ui_dialog->cancel_btn_dia)
    {
        if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
        {
            //            qDebug() << "comboBox_selectdb,coming here!!";
            //            dialog_selectdb->show();
            InDb_Dialog->hide();//入库界面隐藏
        }/*
        else if (event->type()==QEvent::Leave)    // mouse leaves widget
        {
//            qDebug() << "comboBox_selectdb,leave now!!";
//            ui_dia_selectdb->comboBox_selectdb->showNormal();
//            dialog_selectdb->hide();
        }
        else if (event->type()==QEvent::WindowDeactivate)    // window was deactivated
        {
//            qDebug() << "comboBox_selectdb,WindowDeactivate!!";
//            ui_dia_selectdb
//            dialog_selectdb->hide();
        }
//        qDebug() << "comboBox_selectdb, event type:" << event->type();*/
    }

}

//combox产生下拉事件，弹出内容
/*============================================
* FuncName    : autoCCode::eventFilter_ui_dia_selectdb_comboBox_selectdb
* Description :
* @watched    :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
bool autoCCode::eventFilter_ui_dia_selectdb_comboBox_selectdb(QObject *watched, QEvent *event)
{
    if(watched == ui_dia_selectdb->comboBox_selectdb)
    {
        if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
        {
            //            qDebug() << "comboBox_selectdb,coming here!!";
            //            dialog_selectdb->show();
            ui_dia_selectdb->comboBox_selectdb->showPopup();//combox下拉事件
        }
        else if (event->type()==QEvent::Leave)    // mouse leaves widget
        {
            //            qDebug() << "comboBox_selectdb,leave now!!";
            //            ui_dia_selectdb->comboBox_selectdb->showNormal();
            //            dialog_selectdb->hide();
        }
        else if (event->type()==QEvent::WindowDeactivate)    // window was deactivated
        {
            //            qDebug() << "comboBox_selectdb,WindowDeactivate!!";
            //            ui_dia_selectdb
            dialog_selectdb->hide();
        }
        //        qDebug() << "comboBox_selectdb, event type:" << event->type();
    }

}

bool autoCCode::eventFilter_ui_tools(QObject *watched, QEvent *event)
{
    if(watched == ui_toolsets->pushButton_opendir)
    {
        if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
        {
            qDebug() << "pushButton_opendir,coming here!!";
            //            dialog_selectdb->show();
            ui_toolsets->pushButton_opendir->showMenu();
//            emit ui_toolsets->pushButton_opendir->clicked();
//            ui_dia_selectdb->comboBox_selectdb->showPopup();//combox下拉事件
        }
        else if (event->type()==QEvent::Leave)    // mouse leaves widget
        {
            ui_toolsets->pushButton_opendir->menu()->hide();
            qDebug() << "pushButton_opendir,leave now!!";
            //            ui_dia_selectdb->comboBox_selectdb->showNormal();
            //            dialog_selectdb->hide();
        }
        else if (event->type()==QEvent::WindowDeactivate)    // window was deactivated
        {
            //            qDebug() << "comboBox_selectdb,WindowDeactivate!!";
            //            ui_dia_selectdb
//            dialog_selectdb->hide();
        }
        //        qDebug() << "comboBox_selectdb, event type:" << event->type();
    }


    if(watched == ui_toolsets->comboBox_path)
    {
        if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
        {
            qDebug() << "comboBox_path,coming here!!";
            ui_toolsets->comboBox_path->showPopup();
        }
    }

}


/*============================================
* FuncName    : autoCCode::eventFilter_ui_choseCodeDB_btn
* Description :
* @watched    :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
bool autoCCode::eventFilter_ui_choseCodeDB_btn(QObject *watched, QEvent *event)
{
    if(watched == ui->choseCodeDB_btn)
    {
        if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
        {
            //            qDebug() << "chose codedb btn focus in,coming here!!";
            dialog_selectdb->move(ui->choseCodeDB_btn->x(), ui->choseCodeDB_btn->y());
            dialog_selectdb->show();
            update();
        }
        else if (event->type()==QEvent::Leave)    // mouse leaves widget
        {
            //            qDebug() << "chose codedb btn focus out,leave now!!";
            //            dialog_selectdb->hide();
        }
    }
}


/*============================================
* FuncName    : autoCCode::eventFilter
* Description :
* @obj        :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
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
        if(event->type() == QEvent::Enter)
        {
            Isgenshow_textEdit_Enter = TRUE;
        }
        else if(event->type() ==QEvent::Leave)
        {
            Isgenshow_textEdit_Enter = FALSE;
        }


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



        //        if(obj == ui_toolsets->textEdit_main_content)
        //        {
        //            //按键处理
        //            if(event->type() == QEvent::KeyRelease)
        //            {
        //                isCTRLKeyPressed = FALSE;
        //            }
        //            //按键处理
        //            if(event->type() == QEvent::KeyPress)
        //            {
        //                //qDebug()<<"KeyPress ed!!";
        //                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //                int key = keyEvent->key();
        //                if (Qt::Key_Control == key){
        //                    qDebug()<<"Key_Control   Pressed";
        //                    isCTRLKeyPressed = TRUE;
        //                }
        //                else {
        //                    //qDebug()<<"else Key !!";
        //                }
        //            }
        //        }


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


    //按键处理

    //按键处理
    if(event->type() == QEvent::KeyPress)
    {
        //qDebug()<<"KeyPress ed!!";
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        if (Qt::Key_Control == key){
            qDebug()<<"Key_Control   Pressed!!";
            isCTRLKeyPressed = TRUE;
        }
        else {
            //qDebug()<<"else Key !!";
        }
    }
    else if(event->type() == QEvent::KeyRelease)
    {
        isCTRLKeyPressed = FALSE;
    }


    eventFilter_ui_choseCodeDB_btn(obj, event);
    eventFilter_ui_dia_selectdb_comboBox_selectdb(obj, event);
    eventFilter_ui_dialog(obj, event);
    eventFilter_ui_dialog_langtype_comboBox(obj, event);
    eventFilter_ui_setup(obj, event);
    eventFilter_ui_tools(obj, event);
    //    eventFilter_ui_toolsets(obj, event);

    return QObject::eventFilter(obj, event);
}


/*============================================
* FuncName    : autoCCode::keyPressEvent
* Description :
* @k          :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::keyPressEvent(QKeyEvent *k)
{
    //    if(k->key() == Qt::Key_A)
    //    {
    //        qDebug() << "key A is pressed";
    //    }

    //    if (k->key() == Qt::Key_Control)
    //    {
    //        qDebug() << "key Ctrl is pressed";
    //    }
    //    (WM_RBUTTONCLKDOWN)

    if(k->key() == Qt::Key_Escape)
    {
        qDebug() << "key Escape is pressed";
        key_escaple_pressed = true;
    }
    else
    {
        key_escaple_pressed = false;
    }
    return;
}

/*============================================
* FuncName    :
* Description :
* @e          :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::mouseMoveEvent ( QMouseEvent * e )
{
    //    qDebug() << "mouseMoveEvent" << endl;
    //    setMouseTracking(false);
}
/*============================================
* FuncName    :
* Description :
* @e          :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::mousePressEvent ( QMouseEvent * e )
{
    qDebug() << "mousePressEvent" << endl;


}
/*============================================
* FuncName    :
* Description :
* @e          :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::mouseReleaseEvent ( QMouseEvent * e )
{
    qDebug() << "mouseReleaseEvent" << endl;

}
/*============================================
* FuncName    : autoCCode::mouseDoubleClickEvent
* Description :
* @e          :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::mouseDoubleClickEvent( QMouseEvent * e )
{
    qDebug() << "mouseDoubleClickEvent" << endl;

}
//void autoCCode::dragEnterEvent(QDragEnterEvent *event)
//{
//    qDebug() << "dragEnterEvent" << endl;

//}
//void autoCCode::dropEvent(QDropEvent *event)
//{
//    qDebug() << "dropEvent" << endl;

//}

/*============================================
* FuncName    : autoCCode::on_lineEdit_search_MouseButtonDblClick
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::on_lineEdit_search_Key_Escape
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_lineEdit_search_Key_Escape()
{
    if(listView->isVisible())
        listView->hide();
}

/*============================================
* FuncName    : autoCCode::completeText
* Description :
* @index      :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::currentDbHaved
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::listWidget_codeview_subShow
* Description :
* @strlist    :
* Author      :
* Time        : 2017-05-28
============================================*/
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


/*============================================
* FuncName    : autoCCode::CharFormat
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::CharFormat(void)
{
    defcharfmt = ui->genshow_textEdit->currentCharFormat();
}

/*============================================
* FuncName    : autoCCode::setWindowTitle_Main
* Description :
* @arg1       :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::getCurrentDateTimeTxt
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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

/*============================================
* FuncName    : autoCCode::MainSetUp
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::MainSetUp()
{
    self_print(MainSetUp);
    SetUp_Dialog->show();
}

/*============================================
* FuncName    : autoCCode::on_pushButton_foreColor_clicked2
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_foreColor_clicked2()
{
    self_print(on_pushButton_foreColor_clicked);
    setforegroudColor();
}

/*============================================
* FuncName    : autoCCode::on_pushButton_backColor_clicked2
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_backColor_clicked2()
{
    self_print(on_pushButton_backColor_clicked);
    setbackgroudColor();
}
//颜色对话框设置-前景色
/*============================================
* FuncName    : autoCCode::setforegroudColor
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
    palettebtn.color(QPalette::Button);
    update();
}
//颜色对话框设置-背景色
/*============================================
* FuncName    : autoCCode::setbackgroudColor
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::setDefaultColor
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::setFont
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::ZoomInFont
* Description :
* @watched    :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ZoomInFont(QObject *watched)
{
    QWidget *pwnd =(QWidget *)watched;
    //    qDebug() << "ZoomInFont";
    QFont font = pwnd->font();
    int fontsize = font.pointSize();
    //    qDebug() << "OldFontFamily:" << OldFontFamily;
    //    qDebug() << "fontsize:" << fontsize;
    fontsize += 1;
    if(fontsize > MAXFONTSIZE)
        fontsize = MAXFONTSIZE;

    pwnd->setFont(QFont(font.family(),fontsize,font.weight()));
}

/*============================================
* FuncName    : autoCCode::ZoomOutFont
* Description :
* @watched    :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ZoomOutFont(QObject *watched)
{
    QWidget *pwnd =(QWidget *)watched;
    //    qDebug() << "ZoomOutFont";
    QFont font = pwnd->font();
    int fontsize = font.pointSize();
    //    qDebug() << "OldFontFamily:" << OldFontFamily;
    //    qDebug() << "fontsize:" << fontsize;
    fontsize -= 1;
    if(fontsize < MINFONTSIZE)
        fontsize = MINFONTSIZE;

    pwnd->setFont(QFont(font.family(),fontsize,font.weight()));
}

/*============================================
* FuncName    : autoCCode::IsCursorInGenShowUi
* Description :
* @watched    :
* Author      :
* Time        : 2017-05-28
============================================*/
int autoCCode::IsCursorInGenShowUi(QObject *watched)
{
    QWidget *pwdn = (QWidget *)watched;

    int x = pwdn->x();
    int y = pwdn->y();
    int w = pwdn->width();
    int h = pwdn->height();
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
/*============================================
* FuncName    : autoCCode::contextMenuEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
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
/*============================================
* FuncName    : autoCCode::on_pushButton_rightTextSelectIndb_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_rightTextSelectIndb_clicked()
{
    QString str_firstline;
    str_firstline.clear();

    QString str_selected;
    QString cotext = ui->genshow_textEdit->toPlainText().trimmed();
    if(cotext.isEmpty())
    {
        qDebug() << "context. empty";
        return;
    }
    if(isMainServer())
    {
        str_firstline = ui->genshow_textEdit->document()->findBlockByLineNumber(0).text();
        if(str_firstline.isEmpty())
            str_firstline = "no key!";

        str_selected = str_firstline;
    }
    else
    {
        //光标选中的文本
        str_selected = ui->genshow_textEdit->textCursor().selectedText().trimmed();
    }

    if(str_selected.isEmpty())
    {
        qDebug() << "select text. empty";
        return;
    }
    ok_btn_dia_clicked_self_another(cotext, str_selected);
}
//根据定义的数据，直接入库并显示
/*============================================
* FuncName    : autoCCode::ok_btn_dia_clicked_self_another
* Description :
* @con        :
* @str_sel    :
* Author      :
* Time        : 2017-05-28
============================================*/
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
        ShowTipsInfo(QString::fromLocal8Bit("类型 不能为空"));
//        QMessageBox::information(NULL, str_china(类型), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
        return;
    }
    if(content.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("内容 不能为空"));
//        QMessageBox::information(NULL, str_china(内容), str_china(不能为空), QMessageBox::Yes , QMessageBox::Yes);
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
        if(isMainServer())
            return;
        ShowTipsInfo(QString::fromLocal8Bit("内容已经存在"));
//        QMessageBox::information(NULL, str_china(声明), str_china(内容已经存在), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(vartype.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("变量类型 不能为空"));
//        QMessageBox::information(NULL, str_china(变量类型), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(index_keyword.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("检索 不能为空"));
//        QMessageBox::information(NULL, str_china(检索), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(note.isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("注释 不能为空"));
//        QMessageBox::information(NULL, str_china(注释), str_china(不能为空), QMessageBox::Yes, QMessageBox::Yes);
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




/*============================================
* FuncName    : autoCCode::on_pushButton_toolsSets_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_toolsSets_clicked()
{
    if(toolsTabWidget->isHidden())
    {
        WidgetXYsetDesktop_center(toolsTabWidget);//居XY中间
        toolsTabWidget->raise(); //到上层
        toolsTabWidget->show();
    }
}

/*============================================
* FuncName    : autoCCode::on_pushButton_notepad_exe_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_notepad_exe_clicked()
{
    //调用记事本
    ShellExecuteA(NULL,"open","NOTEPAD.EXE",NULL,NULL,SW_SHOWNORMAL);

    //    toolsTabWidget->hide();
}

/*============================================
* FuncName    : autoCCode::on_pushButton_calc_exe_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_calc_exe_clicked()
{
    //调用计算器
    ShellExecuteA(NULL,"open","calc.exe",NULL,NULL,SW_SHOWNORMAL);
}

/*============================================
* FuncName    : autoCCode::on_pushButton_notepadplus_exe_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_notepadplus_exe_clicked()
{
    //notepad++.exe
    LPCSTR str2 = "notepad++.exe";
    ShellExecuteA(NULL,"open", str2,NULL,NULL,SW_SHOWNORMAL);

}


/*============================================
* FuncName    : autoCCode::on_pushButton_python_exe_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_python_exe_clicked()
{
    //python.exe
    LPCSTR exepath = "python.exe";
    ShellExecuteA(NULL,"open", exepath,NULL,NULL,SW_SHOWNORMAL);
}

/*============================================
* FuncName    : autoCCode::on_pushButton_python_Dpathtestfile_exe_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_python_Dpathtestfile_exe_clicked()
{
    QString headerPython =
            QString("#!/usr/bin/env python") + "\n" +
            "# -*- coding: utf-8 -*-" + "\n" +
            "import time" + "\n" +
            "from time import ctime,sleep;" + "\n"+ "\n" +
            "print \"good,good!\""  + "\n"+
            "print 62 *\'=\'  "+  "\n"+
            "#write code here begin" +  "\n"+
            + "\n"+
            + "\n"+
            + "\n"+
            + "\n"+
            "#write code here end" +  "\n"+
            "print 62 *\'=\'  "+  "\n"+
            "for i in range(2000000):"  + "\n"+
            "    sleep(10)"  + "\n"+
            "    print \"loopint at pos \" +	 str(i)"  + "\n"+
            + "\n"+
            "sleep(100)";

    //python.exe
    LPCSTR exepath = "python.exe";
    LPCSTR filepath = "D:\\test.py";
    QFile *pFile = NULL;
    pFile = new QFile(filepath);
    if(!pFile->exists())
    {
        pFile->open(QIODevice::WriteOnly);
        pFile->write(headerPython.toLocal8Bit().data());
    }
    pFile->close();
    //notepad++.exe打开此文件
    LPCSTR notepadexe = "notepad++.exe";
    ShellExecuteA(NULL,"open", notepadexe,filepath,NULL,SW_SHOWNORMAL);
    ShellExecuteA(NULL,"open", exepath,filepath,NULL,SW_SHOWNORMAL);

}


/*============================================
* FuncName    : autoCCode::on_pushButton_show_calender_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_show_calender_clicked()
{
    qDebug() << "on_pushButton_show_calender_clicked";

//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    window_calender = new Window();
    window_calender->show();
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    //    window.show();
    //    Sleep(2000);
}


/*============================================
* FuncName    : autoCCode::on_pushButton_sourceinsight_exe_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_sourceinsight_exe_clicked()
{
    //    Insight3.exe
    LPCSTR exepath = "Insight3.exe";
    ShellExecuteA(NULL,"open", exepath,NULL,NULL,SW_SHOWNORMAL);
    exepath = "C:\\Program Files (x86)\\Source Insight 3\\Insight3.exe";
    ShellExecuteA(NULL,"open", exepath,NULL,NULL,SW_SHOWNORMAL);

}


/*============================================
* FuncName    : autoCCode::on_pushButton_cmd_exe_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pushButton_cmd_exe_clicked()
{
    //cmd.exe
    LPCSTR exepath = "cmd.exe";
    ShellExecuteA(NULL,"open", exepath,NULL,NULL,SW_SHOWNORMAL);
    //    exepath = "C:\\Program Files (x86)\\Source Insight 3\\Insight3.exe";
    //    ShellExecuteA(NULL,"open", exepath,NULL,NULL,SW_SHOWNORMAL);

}

/*============================================
* FuncName    : autoCCode::on_selectionChanged
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_selectionChanged()
{
    qDebug()  << "on_selectionChanged " << endl;
}

/*============================================
* FuncName    : autoCCode::on_pUdpdialog_okBtn_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_pUdpdialog_okBtn_clicked()
{
    qDebug() << "on_pUdpdialog_okBtn_clicked" ;
    //    qDebug() << "Ip Addr:" << pUdp_ui->lineEdit->text();
}

/*============================================
* FuncName    : autoCCode::getLstIp
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
QStringList autoCCode::getLstIp()
{
    QStringList lstIP;
    lstIP.clear();
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)   //我们使用IPv4地址
        {
            lstIP.append(address.toString());/*
            if(address.toString().contains("127.0."))
                continue;
            if(address.toString().contains("192.168.1."))
            {
                qDebug()<<"本机Ip："<<address.toString();
                return address.toString();
            }
            else
            {
                continue;
            }*/
        }
    }
    return lstIP;
}


/*============================================
* FuncName    : QTabWidget::event
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
bool QTabWidget::event(QEvent *)
{

}


/*============================================
* FuncName    : QTabWidget::keyPressEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void QTabWidget::keyPressEvent(QKeyEvent *event)
{
    //按键处理

    //按键处理
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        qDebug()<<key <<"  KeyPress ed!!";
        if (Qt::Key_Control == key){
            qDebug()<<"Key_Control   Pressed!!!!!!!!!!";
            isCTRLKeyPressed_uitools = TRUE;
        }
        if (Qt::Key_F == key) {
            qDebug()<<"Key_F   Pressed!!!!!!!!!!";
            isFKeyPressed_uitools = TRUE;
        }
        if (Qt::Key_C == key) {
            qDebug()<<"Key_C   Pressed!!!!!!!!!!";
            isCKeyPressed_uitools = TRUE;
        }
        else {
            //qDebug()<<"else Key !!";
        }
    }
    else if(event->type() == QEvent::KeyRelease)
    {
//        isCTRLKeyPressed_uitools = FALSE;
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        if (Qt::Key_Control == key){
            qDebug()<<"Key_Control   KeyRelease!!!!!!!!!!";
            isCTRLKeyPressed_uitools = FALSE;
        }
        else if (Qt::Key_F == key) {
            qDebug()<<"Key_F   KeyRelease!!!!!!!!!!";
            isFKeyPressed_uitools = FALSE;
        }
        else if (Qt::Key_C == key) {
            qDebug()<<"Key_C   KeyRelease!!!!!!!!!!";
            isCKeyPressed_uitools = FALSE;
        }
        else {
            //qDebug()<<"else Key !!";
        }
    }

    return ;
}



//void QTabWidget::wheelEvent(QWheelEvent *event)
//{
//    int numDegrees = event->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
//    int numSteps = numDegrees / 15;//滚动的步数，*15就是鼠标滚动的角度
//    if (event->orientation() == Qt::Horizontal) {
//        //        scrollHorizontally(numSteps);       //水平滚动
//        //        qDebug() << "horizontal";
//    } else {
//        //        scrollVertically(numSteps);       //垂直滚动
//        //        qDebug() << "vectorial numSteps:" <<numSteps << ",numDegrees:" << numDegrees;
//    }
////    qDebug() << "isCTRLKeyPressed :" << isCTRLKeyPressed;
////    qDebug() << "windowFlags :" << this->windowFlags();

////    if(isCTRLKeyPressed
////            && IsCursorInGenShowUi(ui->genshow_textEdit))

//    qDebug() << "isCTRLKeyPressed         :" <<  isCTRLKeyPressed_uitools;
////    qDebug() << "Isgenshow_textEdit_Enter :" <<  Isgenshow_textEdit_Enter;
////    qDebug() << "isToolsContent_Enter     :" <<  isToolsContent_Enter;
////    qDebug() << "isToolsSuffix_Enter      :" <<  isToolsSuffix_Enter;



////    qDebug() << "isCTRLKeyPressed 2222:" << isCTRLKeyPressed;
////    qDebug() << "isTopLevel 2222:" << ui_toolsets->textEdit_main_content->isTopLevel();
////    if(isCTRLKeyPressed
////            && IsCursorInGenShowUi(ui_toolsets->textEdit_main_content))
//    if(isCTRLKeyPressed_uitools
//            && isToolsContent_Enter)
//    {
//        qDebug() << "ui_toolsets->textEdit_main_content!!!!!!!";
//        if(numSteps > 0)
//        {
//            ZoomInFont(ui_toolsets->textEdit_main_content);
//        }
//        else if(numSteps < 0)
//        {
//            ZoomOutFont(ui_toolsets->textEdit_main_content);
//        }
//    }


////    isCTRLKeyPressed_TOOLSsuffix
////    if(isCTRLKeyPressed_TOOLSsuffix
////            /*&& IsCursorInGenShowUi(ui_toolsets->textEdit_suffix) */ )

//    if(isCTRLKeyPressed_uitools && isToolsSuffix_Enter )
//    {
//        qDebug() << "ui_toolsets->textEdit_suffix_content!!!!!!!";
//        if(numSteps > 0)
//        {
//            ZoomInFont(ui_toolsets->textEdit_suffix);
//        }
//        else if(numSteps < 0)
//        {
//            ZoomOutFont(ui_toolsets->textEdit_suffix);
//        }
//    }
//    event->accept();      //接收该事件
//}




//bool QTabWidget::eventFilter(QObject *obj, QEvent *event)
//{
//    //按键处理

//    //按键处理
//    if(event->type() == QEvent::KeyPress)
//    {
//        //qDebug()<<"KeyPress ed!!";
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        int key = keyEvent->key();
//        if (Qt::Key_Control == key){
//            qDebug()<<"Key_Control   Pressed";
//            isCTRLKeyPressed = TRUE;
//        }
//        else {
//            //qDebug()<<"else Key !!";
//        }
//    }
//    else if(event->type() == QEvent::KeyRelease)
//    {
//        isCTRLKeyPressed = FALSE;
//    }



//    {
//        qDebug() << "=========eventFilter_ui_toolsets";
//        if(watched == ui_toolsets->textEdit_main_content)
//        {
//            if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
//            {
//                isToolsContent_Enter = TRUE;
//     //            qDebug() << "comboBox_selectdb,coming here!!";
//    //            ui_dialog->langtype_comboBox->showPopup();//combox下拉事件
//            }
//            else if (event->type()==QEvent::Leave)    // mouse leaves widget
//            {
//                isToolsContent_Enter = FALSE;
//     //            qDebug() << "comboBox_selectdb,leave now!!";
//     //            ui_dia_selectdb->comboBox_selectdb->showNormal();
//     //            dialog_selectdb->hide();
//            }
//            /*
//            else if (event->type()==QEvent::WindowDeactivate)    // window was deactivated
//            {
//     //            qDebug() << "comboBox_selectdb,WindowDeactivate!!";
//     //            ui_dia_selectdb
//     //            dialog_selectdb->hide();
//            }
//     //        qDebug() << "comboBox_selectdb, event type:" << event->type();*/
//        }


//        if(watched == ui_toolsets->textEdit_suffix)
//        {
//            if (event->type()==QEvent::Enter)     //Event:enter // mouse enters widget
//            {
//                isToolsSuffix_Enter = TRUE;
//     //            qDebug() << "comboBox_selectdb,coming here!!";
//    //            ui_dialog->langtype_comboBox->showPopup();//combox下拉事件
//            }
//            else if (event->type()==QEvent::Leave)    // mouse leaves widget
//            {
//                isToolsSuffix_Enter = FALSE;
//     //            qDebug() << "comboBox_selectdb,leave now!!";
//     //            ui_dia_selectdb->comboBox_selectdb->showNormal();
//     //            dialog_selectdb->hide();
//            }
//            /*
//            else if (event->type()==QEvent::WindowDeactivate)    // window was deactivated
//            {
//     //            qDebug() << "comboBox_selectdb,WindowDeactivate!!";
//     //            ui_dia_selectdb
//     //            dialog_selectdb->hide();
//            }
//     //        qDebug() << "comboBox_selectdb, event type:" << event->type();*/
//        }
//    }

//    return QObject::eventFilter(obj, event);
//}

/*============================================
* FuncName    : autoCCode::on_textEdit_main_content_textChanged
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_textEdit_main_content_textChanged()
{
    textEdit_main_uitools = ui_toolsets->textEdit_main_content->toPlainText();
    qDebug() <<"textEdit_main:" << textEdit_main_uitools;
}

/*============================================
* FuncName    : autoCCode::on_textEdit_suffix_textChanged
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_textEdit_suffix_textChanged()
{
    textEdit_suff_uitools = ui_toolsets->textEdit_suffix->toPlainText();
    qDebug() <<"textEdit_suff:" << textEdit_suff_uitools;

}

/************************************************/
/*函 数:ReadContent                               */
/*入 参:无                                        */
/*出 参:无                                        */
/*返 回:无                                        */
/*功 能:读取ip地址                                 */
/*author :wxj                                    */
/*version:1.0                                    */
/*时 间:2015.8.17                                 */
/*************************************************/
/*============================================
* FuncName    : autoCCode::ReadContent
* Description :
* @filename   :
* Author      :
* Time        : 2017-05-28
============================================*/
QString autoCCode::ReadContent(QString filename)
{
    QFile file(filename);

    if(file.exists())
    {
        QByteArray dataFromFile;
        QString ipaddr;
        file.open(QIODevice::ReadOnly);
        dataFromFile=file.readAll();
        file.close();
        qDebug() << "read ip:" << dataFromFile;
        ipaddr = QString(dataFromFile);
        qDebug() << "ipaddr :" << ipaddr;
        return ipaddr;
    }
    else
    {
        file.open(QIODevice::WriteOnly);
        file.write(DEFAULT_HOSTADDR);
        file.close();
        qDebug() << "read ip:" << DEFAULT_HOSTADDR;
        return QString(DEFAULT_HOSTADDR);
    }
}

/************************************************/
/*函 数:SaveContent                                */
/*入 参:无                                        */
/*出 参:无                                        */
/*返 回:无                                        */
/*功 能:保存ip地址                                 */
/*author :wxj                                    */
/*version:1.0                                    */
/*时 间:2015.8.17                                 */
/*************************************************/
/*============================================
* FuncName    : autoCCode::SaveContent
* Description :
* @filename   :
* @ipaddr     :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SaveContent(QString filename, QString ipaddr)
{
    QFile file(filename);

    if(file.exists())
    {
        file.open(QIODevice::WriteOnly);
        file.write(ipaddr.toLocal8Bit());
        file.close();
    }
}

/*============================================
* FuncName    : autoCCode::ReadLocalIpAddr
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
QString autoCCode::ReadLocalIpAddr()
{
    return ReadContent("./serverip.conf");
}

/*============================================
* FuncName    : autoCCode::SaveLocalIpaddr
* Description :
* @ipaddr     :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SaveLocalIpaddr(QString ipaddr)
{
    SaveContent("./serverip.conf", ipaddr);
}

/*============================================
* FuncName    : autoCCode::ReadRemoteIpAddr
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
QString autoCCode::ReadRemoteIpAddr()
{
    return ReadContent("./remoteip.conf");
}

/*============================================
* FuncName    : autoCCode::SaveRemoteIpaddr
* Description :
* @rip        :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SaveRemoteIpaddr(QString rip)
{
    SaveContent("./remoteip.conf", rip);
}

/*============================================
* FuncName    : autoCCode::saveRemoteIP
* Description :
* @rip        :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::saveRemoteIP(QString rip)
{
    remoteip = rip;
    qDebug() << "remote ip:" << remoteip;
    SaveRemoteIpaddr(remoteip);
}

/*============================================
* FuncName    : autoCCode::saveBindLocalIP
* Description :
* @lip        :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::saveBindLocalIP(QString lip)
{
    localip = lip;
    qDebug() << "local ip:" << localip;

    if(localip.isEmpty() ||  (RET_SUCESS != CheckIPAddr(localip)))
    {
        qDebug() << "local ip null or ip err!!";
        return;
    }

    if(remoteip.isEmpty() ||  (RET_SUCESS != CheckIPAddr(remoteip)))
    {
        qDebug() << "remote ip null or ip err!!";
        return;
    }

    //远端是否支持
    if(!ui_setup->checkBox_supportRemote->isChecked())
    {
        return;
    }

    if(hostaddr)
    {
        hostaddr->setAddress(localip);
    }
    else
    {
        hostaddr = new QHostAddress();
        hostaddr->setAddress(localip);
    }
    SaveRemoteIpaddr(remoteip);
    SaveLocalIpaddr(localip);


    if(tcpserver && tcpserver->isListening())
    {
        tcpserver->close();
        tcpserver = NULL;
        qDebug("stop listening！");
    }

    tcpserver = new QTcpServer();
    while (!tcpserver->isListening() &&
           !tcpserver->listen(*hostaddr, DEFAULT_PORT)) {
        qDebug() <<"tcp err:" << tcpserver->errorString();
        qDebug() <<"host addr:" << *hostaddr;
        QMessageBox::StandardButton ret;
        ret = QMessageBox::critical(this,
                                    str_china("回环"),
                                    str_china("无法开始测试: %1.")
                                    .arg(tcpserver->errorString()),
                                    QMessageBox::Retry
                                    | QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
            return;
    }

    QObject::connect(tcpserver,SIGNAL(newConnection()),this,
                     SLOT(recvClientMsg()));

    qDebug() <<"bind local ip ok," << localip;


    if(isMainServer())//接收服务器只接收数据，不发送数据
    {
//        QMessageBox::information(NULL, str_china(提示),
//                                 str_china(数据库服务器端！),NULL,NULL);
        ShowTipsInfo(QString::fromLocal8Bit("提示!! 数据库服务器端！"));
        ifSeverUi();
        return;
    }

    if(socket)
    {
        socket->disconnectFromHost();
        socket->abort();
        socket->close();
        socket = NULL;
    }
    socket = new QTcpSocket;
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);//优化为最低延迟，后面的1代码启用该优化。
    socket->connectToHost(remoteip, DEFAULT_PORT);
    qDebug() <<"socket err:" << socket->errorString();
    qDebug() <<"remote ip addr:" << remoteip;

    QObject::connect(socket,SIGNAL(connected()),this,
                     SLOT(hellosocket()));
    QObject::connect(socket,SIGNAL(bytesWritten(qint64)),this,
                     SLOT(updateClientProgress(qint64)));

    QTimer *sendtimer = new QTimer();
    QObject::connect(sendtimer,SIGNAL(timeout()),this,
                     SLOT(timerwritemsg()));
    sendtimer->start(3000);
}


/*============================================
* FuncName    : autoCCode::hellosocket
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::hellosocket()
{
    //    QString page = " you addr";
    //    socket->write("GET " + page.toLocal8Bit() + "\r\n");
//    QMessageBox::information(NULL, str_china(版本),
//                             str_china(连接远端成功！！),NULL,NULL);
    ShowTipsInfo(QString::fromLocal8Bit("版本!! 连接远端成功！！"));
}

/*============================================
* FuncName    : autoCCode::timerwritemsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::timerwritemsg()
{
    static QString oldmsg;
    QString bigmsg = ui->genshow_textEdit->toPlainText();
    if(bigmsg == oldmsg || bigmsg.isEmpty())
    {
        return;
    }


    outBlockFile = bigmsg.toLocal8Bit();
#ifdef DEBUG
    qDebug() <<"read file size:" << outBlockFile.size() ;
#endif

    TotalBytes = outBlockFile.size();

    //    bytesToWrite = TotalBytes - socket->write(outBlockFile);
    qDebug() << "write msg:" << bigmsg;


    outBlock.resize(0);
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.resetStatus();
    sendOut.setVersion(QDataStream::Qt_4_0);

    QString readFname("tag");
    //发送文件名称
    //    sendOut <<qint64(0) <<readFname;
    //占据文件大小位置
    sendOut << TotalBytes;
    //TotalBytes为总数据长度，即（数据量长度+文件名长度+文件名）
    TotalBytes += outBlock.size(); //加上图片名称长度
    sendOut.device()->seek(0);

    //    //总字节(文件大小 + 8字节 + 文件名) ，
    //    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*1));

    //    (文件大小 [8字节])
    //    sendOut << TotalBytes;
    bytesToWrite = TotalBytes - socket->write(outBlock);//将名称发出后，剩余图片大小
    ////    ui->clientStatusLabel->setText(str_china("已连接"));
    //#ifdef DEBUG
    //    qDebug() << "TotalBytes:" << TotalBytes;
    //#endif

    qDebug() << "TotalBytes:" << TotalBytes;
    qDebug() << "bytesToWrite:" << bytesToWrite;

    oldmsg = bigmsg;
}


//检测IP地址错误
/*============================================
* FuncName    : autoCCode::CheckIPAddr
* Description :
* @ipaddr     :
* Author      :
* Time        : 2017-05-28
============================================*/
int autoCCode::CheckIPAddr(QString ipaddr)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(!regExp.exactMatch(ipaddr))
    {
        QMessageBox::warning(NULL, str_china("提示"), str_china("ip地址错误"),NULL,NULL);
        return RET_FAIL;
    }

    return RET_SUCESS;
}


/*============================================
* FuncName    : autoCCode::readfromremote
* Description :
* @recvBigMsg  :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::readfromremote(QString recvBigMsg)
{
    //    static bool readdone = READ_DONE;
    //    if(READ_DONE == readdone)
    //    {
    //        TotalReadBytes = clientConnection->read(sizeof(qint64)).toLongLong();
    //        readdone = READING;
    //    }

    //    qDebug() << "recicve byte size:" << TotalReadBytes;
    //    QString readdata = clientConnection->readAll();




    //    qDebug() << "numBytes:--------->>"<<numBytes;
    //    byteWritten += (int)numBytes;
    //    if(bytesToWrite > 0)
    //    {
    //        qDebug() <<"-->:outBlockFile size:" << outBlockFile.size();

    //        bytesToRead -= (int)socket->write(outBlockFile);
    //        qDebug() <<"-->:bytesToWrite size:" << bytesToWrite;
    //    }
    //    else
    //    {
    //        qDebug() << "-->: send msg done!!";
    //        TotalReadBytes = 0;
    //        byteReadden = 0;
    //        readdone = READ_DONE;
    //    }

    //    if(READ_DONE != readdone)
    //    {
    //        return;
    //    }

    if(!isMainServer())//非主服务端时不存储数据
    {
        return;
    }

    //    bigmsg
    //讲到的数据入库

    qDebug() << "i can read somethig:\n" << recvBigMsg.toLocal8Bit();
    ui->genshow_textEdit->clear();
    SWITCH_genshow_textEdit_SetText(QString::fromLocal8Bit(recvBigMsg.toAscii()));


    on_pushButton_rightTextSelectIndb_clicked();
}


/*============================================
* FuncName    : autoCCode::recvClientMsg
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::recvClientMsg()
{
    clientConnection = tcpserver->nextPendingConnection();
    qDebug() << "-->>client socket:" << clientConnection;
    sockthread *pthreadsock = new sockthread(this);
    pthreadsock->setSocketConnect(clientConnection);
    QObject::connect(pthreadsock,SIGNAL(emitMsgDoneSignal(QString)),
                     this,SLOT(readfromremote(QString)));
    pthreadsock->start();
}

/*============================================
* FuncName    : autoCCode::checkSupportRemote
* Description :
* @flag       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::checkSupportRemote(bool flag)
{
    if(!flag)
        return;

    saveBindLocalIP(localip);
}

/*============================================
* FuncName    : autoCCode::isMainServer
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
bool autoCCode::isMainServer()
{
    return ui_setup->checkBox_SeverFlag->isChecked();
}

/*============================================
* FuncName    : autoCCode::ifSeverUi
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ifSeverUi()
{
    bool isserverflag = isMainServer();
    if(isserverflag)
    {
        //        ui->pushbtn_autoindb->setEnabled(false);
        ui->indb_btn->setEnabled(false);
    }
    else
    {
        //        ui->pushbtn_autoindb->setEnabled(true);
        ui->indb_btn->setEnabled(true);
    }
}
/*============================================
* FuncName    : autoCCode::displayErr
* Description :
* @socketError  :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::displayErr(QAbstractSocket::SocketError socketError)
{
    if(socketError == QTcpSocket::RemoteHostClosedError)
        return;
    QMessageBox::information(this,str_china("网络"),
                             str_china("产生如下错误： %1")
                             .arg(tcpserver->errorString()));

    tcpserver->close();
    tcpserver = NULL;

#ifdef SHOWCURSOR
    QApplication::restoreOverrideCursor();
#endif
}


/*============================================
* FuncName    : autoCCode::updateClientProgress
* Description :
* @numBytes   :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::updateClientProgress(qint64 numBytes)
{
    qDebug() << "numBytes:--------->>"<<numBytes;
    byteWritten += (int)numBytes;
    if(bytesToWrite > 0)
    {
        qDebug() <<"-->:outBlockFile size:" << outBlockFile.size();

        bytesToWrite -= (int)socket->write(outBlockFile);
        qDebug() <<"-->:bytesToWrite size:" << bytesToWrite;
    }
    else
    {
        qDebug() << "-->: send msg done!!";
        TotalBytes = 0;
        byteWritten = 0;
    }
}



//void autoCCode::updateReadMsgProgress()
//{
//    QDataStream in(clientConnection);
//    in.setVersion(QDataStream::Qt_4_0);

//    static bool recvdone = READ_DONE;

//    if(bytesReceived <= sizeof(qint64)*1  && (recvdone == READ_DONE)){
//        if((clientConnection->bytesAvailable() >= sizeof(qint64)*1)){
//            in>>TotalReadBytes;
////            bytesReceived += sizeof(qint64)*1;
//            inBlock.resize(0);
//            recvdone = READING;
//        }
//    }


//    if (bytesReceived < TotalReadBytes){
//        /* 实际需要接收的字节数 */
//        bytesNeedRecv = TotalReadBytes - bytesReceived;
//        bytesReceived += clientConnection->bytesAvailable();

//        if(bytesReceived >= TotalReadBytes){
//            inBlock.append(clientConnection->read(bytesNeedRecv));
//            bytesReceived = TotalReadBytes;
//        }else{
//            inBlock.append(clientConnection->readAll());
//        }

//        qDebug() << "bytesReceived:"<< bytesReceived;
//        qDebug() << "TotalReadBytes   :"<< TotalReadBytes;
//    }

//    if (bytesReceived == TotalReadBytes) {
//        QString  bigmsg = inBlock;
//        //入库
//        readfromremote(bigmsg);

//        TotalReadBytes = 0;
//        bytesReceived = 0;
////        fileNameSize = 0;
//        bytesNeedRecv = 0;
//        inBlock.resize(0);

//        recvdone = READ_DONE;
//    }


//}

QSet<QString> autoCCode::getAllMatchResults(const QString text, const QString regexp)
{
    QSet<QString> resultSet;
    QRegExp rx(regexp);
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1)
    {
//        qDebug() << "pos :" << pos;
        pos += rx.matchedLength();
        QString result = rx.cap(0);
        resultSet << result;
    }
    return resultSet;
}

QSet<quint32> autoCCode::getAllMatchPosResults(const QString text, const QString regexp)
{
    QSet<quint32> resultPosSet;
    int pos = 0;
    //精确查找
    if (ui->checkBox_query_exact->isChecked())
    {
        QRegExp rx(regexp);
        while ((pos = rx.indexIn(text, pos, QRegExp::CaretAtOffset)) != -1)
        {
    //        qDebug() << "pos :" << pos;
            resultPosSet << pos;
            pos += rx.matchedLength();
    //        QString result = rx.cap(0);
        }

    }
    else
    {
        QRegExp rx(regexp.toLower());
        QString lowertext = text.toLower();
        while ((pos = rx.indexIn(lowertext, pos, QRegExp::CaretAtOffset)) != -1)
        {
            //        qDebug() << "pos :" << pos;
            resultPosSet << pos;
            pos += rx.matchedLength();
            //        QString result = rx.cap(0);
        }

    }

    return resultPosSet;
}

/*============================================
* FuncName    : autoCCode::on_btn_find_down_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_btn_find_down_clicked()
{
    if (ui->lineEdit_search->text().isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("search text null!"));
        return;
    }
    QString searchText = ui->lineEdit_search->text().trimmed();
//    QSet<QString> tmpset = getAllMatchResults(ui->genshow_textEdit->toPlainText(), searchText);
//    qDebug() << "result size " << tmpset.size();
//    QSet<QString>::iterator i;
//    for (i = tmpset.begin(); i != tmpset.end(); ++i) {
////      *i = (*i).toLower(); // 使用 * 运算符获取遍历器所指的元素
//       qDebug()<<*i;
//    }

    //精确查找
    if (ui->checkBox_query_exact->isChecked())
    {
        if (!ui->genshow_textEdit->find(searchText,QTextDocument::FindCaseSensitively))
        {
            ShowTipsInfo(QString::fromLocal8Bit("找不到 \"%1\"").arg(searchText));
        }
        else
        {
            QTextCursor cursor = ui->genshow_textEdit->textCursor();
            setStringColor(cursor.position() - searchText.length() + 1,
                           searchText.length());
            ui->genshow_textEdit->update();
        }
    }
    else
    {

        if (!ui->genshow_textEdit->find(searchText))
        {
            ShowTipsInfo(QString::fromLocal8Bit("找不到 \"%1\"").arg(searchText));
        }
        else
        {
            QTextCursor cursor = ui->genshow_textEdit->textCursor();
            setStringColor(cursor.position() - searchText.length() + 1,
                           searchText.length());
            ui->genshow_textEdit->update();
        }
    }



//    if (!ui->genshow_textEdit->find(searchText))
//    {
//        ShowTipsInfo(QString::fromLocal8Bit("找不到 \"%1\"").arg(searchText));
//    }
//    else
//    {
//        QTextCursor cursor = ui->genshow_textEdit->textCursor();
//        setStringColor(cursor.position() - searchText.length() + 1,
//                       searchText.length());
//        ui->genshow_textEdit->update();
//    }
}

/*============================================
* FuncName    : autoCCode::on_btn_find_up_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_btn_find_up_clicked()
{
    if (ui->lineEdit_search->text().isEmpty())
    {
        ShowTipsInfo(QString::fromLocal8Bit("search text null!"));
        return;
    }
    QString searchText = ui->lineEdit_search->text().trimmed();
//    if (!ui->genshow_textEdit->find(searchText, QTextDocument::FindBackward))
//    {
//        ShowTipsInfo(QString::fromLocal8Bit("找不到 \"%1\"").arg(searchText));
//    }
//    else
//    {
//        //向后查找不正常
//        //        QTextCursor cursor = ui->genshow_textEdit->textCursor();
//        //        setStringColor(cursor.position()/* - searchText.length() + 1*/,
//        //                       searchText.length());
//        //        ui->genshow_textEdit->update();
//    }


    //精确查找
    if (ui->checkBox_query_exact->isChecked())
    {
        if (!ui->genshow_textEdit->find(searchText, QTextDocument::FindBackward | QTextDocument::FindCaseSensitively))
        {
            ShowTipsInfo(QString::fromLocal8Bit("找不到 \"%1\"").arg(searchText));
        }
        else
        {
            //向后查找不正常
            //        QTextCursor cursor = ui->genshow_textEdit->textCursor();
            //        setStringColor(cursor.position()/* - searchText.length() + 1*/,
            //                       searchText.length());
            //        ui->genshow_textEdit->update();
        }
    }
    else
    {
        if (!ui->genshow_textEdit->find(searchText, QTextDocument::FindBackward))
        {
            ShowTipsInfo(QString::fromLocal8Bit("找不到 \"%1\"").arg(searchText));
        }
        else
        {
            //向后查找不正常
            //        QTextCursor cursor = ui->genshow_textEdit->textCursor();
            //        setStringColor(cursor.position()/* - searchText.length() + 1*/,
            //                       searchText.length());
            //        ui->genshow_textEdit->update();
        }
    }

}

/*============================================
* FuncName    : autoCCode::ShowTipsInfo
* Description :
* @s          :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ShowTipsInfo(QString s)
{
    MsgTipsAutoShut *tipsinfo = new MsgTipsAutoShut(NULL);
    tipsinfo->SetTipsInfo(s);
    tipsinfo->show();
}

/*============================================
* FuncName    : autoCCode::ShowTipsInfoWithShowTime
* Description :
* @s          :
* @ultimeout  :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ShowTipsInfoWithShowTime(QString s, quint32 ultimeout)
{
    MsgTipsAutoShut *tipsinfo = new MsgTipsAutoShut(NULL, ultimeout);
    tipsinfo->SetTipsInfo(s);
    tipsinfo->show();
}

/*============================================
* FuncName    : autoCCode::on_showlarger_btn_clicked
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_showlarger_btn_clicked()
{
    static quint32 cout = 0;
    static quint32 orgwidth = ui->genshow_textEdit->width();
    static quint32 orgheight = ui->genshow_textEdit->height();
    static QPoint orgpt = ui->genshow_textEdit->pos();
    static quint32 resizeheight = height() - 30;
    if (0 == (cout++ % 2) )
    {
        orgwidth = ui->genshow_textEdit->width();
        orgheight = ui->genshow_textEdit->height();
        orgpt = ui->genshow_textEdit->pos();
        resizeheight = height() - 30;
        ui->verticalLayout_2->removeWidget(ui->genshow_textEdit);
        ui->genshow_textEdit->move(10,40);
        ui->genshow_textEdit->resize(1360,resizeheight);
    }
    else
    {
        ui->verticalLayout_2->addWidget(ui->genshow_textEdit);
        ui->genshow_textEdit->resize(orgwidth, orgheight);
        ui->genshow_textEdit->move(orgpt);
    }
    update();

}

/*============================================
* FuncName    : autoCCode::check_genshow_textEdit_is_append
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::check_genshow_textEdit_is_append()
{
    qDebug() << "check_genshow_textEdit_is_append";
    if(!ui_setup->checkBox_showcontent_autoappend->isChecked())
    {
        return;
    }
    QString text;
    text = ui->genshow_textEdit->toPlainText();
    text += get_clipboard_data();
    SWITCH_genshow_textEdit_SetText(text);
}

/*============================================
* FuncName    : autoCCode::get_clipboard_data
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
QString autoCCode::get_clipboard_data()
{
    QClipboard *clip = QApplication::clipboard();
    return clip->text();
}

/*============================================
* FuncName    : autoCCode::ReadHistorySettings
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::ReadHistorySettings()
{
    QSettings m_settings("weilaidb.com.cn", "autoccode");
    qDebug() << "read setting filename:" << m_settings.fileName();
    qDebug() << "ui_setup->checkBox_showcontent_autoappend->isChecked() setting:" << m_settings.value("GenShowAppend").toBool();
    ui_setup->checkBox_showcontent_autoappend->setChecked(m_settings.value("GenShowAppend").toBool());
    ui_setup->checkBox_rtQuery->setChecked(m_settings.value("RealTimeQuery").toBool());
    ui_setup->checkBox_showpath->setChecked(m_settings.value("ShowPath").toBool());
    ui_setup->checkBox_indb_shortkey->setChecked(m_settings.value("InDbShortKey").toBool());
    ui_setup->checkBox_rightTextSelectIndb->setChecked(m_settings.value("rightselectindb").toBool());
    ui_setup->checkBox_SeverFlag->setChecked(m_settings.value("ServerFlag").toBool());
    ui_setup->checkBox_supportRemote->setChecked(m_settings.value("SupportRemote").toBool());
    ui_dia_selectdb->comboBox_selectdb->setCurrentIndex(m_settings.value("CurrentDataBase").toInt());
    ui->checkBox_query_exact->setChecked(m_settings.value("QueryExact").toBool());
    ui_setup->checkBox_content_withheader->setChecked(m_settings.value("ContentWithHeader").toBool());
    ui_setup->checkBox_codeshortcut->setChecked(m_settings.value("codeshortcut").toBool());
    ui_setup->checkBox_contentbeauty->setChecked(m_settings.value("contentbeauty").toBool());
    ui->checkBox_codecutf8->setChecked(m_settings.value("codecutf8").toBool());
    ui->checkBox_senddata2subui->setChecked(m_settings.value("senddata2subui").toBool());
    ui_toolsets->comboBox_path->addItems(m_settings.value("comboBox_path").toStringList());
//    ui_toolsets->checkBox_isfile->setChecked(m_settings.value("checkBox_isfile").toBool());
    pathlist = m_settings.value("comboBox_path").toStringList();
    ui_toolsets->checkBox_autoopen->setChecked(m_settings.value("checkBox_autoopen").toBool());
    ui_toolsets->lineEdit_hostip->setText(m_settings.value("lineEdit_hostip").toByteArray());

//    QPalette palettebtn ;
//    QColor color= m_settings.value("ForeColor").Color;
//    palettebtn.setColor(QPalette::Button, color);
//    palettebtn.setColor(QPalette::Text, color);
////    QColor color = palette().background().color();
////    QVariant variant = color;
//    ui_setup->pushButton_foreColor->setPalette(palettebtn);


    this->restoreGeometry(m_settings.value("AutoCCode_Geometry").toByteArray());

//    localeCombo->setCurrentIndex(m_settings.value("Language").toInt());
//    firstDayCombo->setCurrentIndex(m_settings.value("WeekStart").toInt());
//    selectionModeCombo->setCurrentIndex(m_settings.value("Selection_Mode").toInt());
//    gridCheckBox->setChecked(m_settings.value("Show_Grid").toBool());
//    navigationCheckBox->setChecked(m_settings.value("Show_Navigation_Bar").toBool());
//    horizontalHeaderCombo->setCurrentIndex(m_settings.value("Horizontal_Header").toInt());
//    verticalHeaderCombo->setCurrentIndex(m_settings.value("Vertival_Header").toInt());
//    weekdayColorCombo->setCurrentIndex(m_settings.value("WeekdayColor").toInt());
//    weekendColorCombo->setCurrentIndex(m_settings.value("WeekendColor").toInt());
//    headerTextFormatCombo->setCurrentIndex(m_settings.value("Header_Font").toInt());
//    firstFridayCheckBox->setChecked(m_settings.value("First_Friday").toBool());
//    mayFirstCheckBox->setChecked(m_settings.value("May_First").toBool());
//    m_clock->restoreGeometry(m_settings.value("Clock_Geometry").toByteArray());
//    m_Birthday = QDate::fromString(m_settings.value("Birthday_Date").toString(), "yyyy-MM-dd");
//    this->restoreGeometry(m_settings.value("Calendar_Geometry").toByteArray());

//    if (m_Birthday.isNull())
//        goBirthday->setEnabled(false);
}

/*============================================
* FuncName    : autoCCode::WriteCurrentSettings
* Description :
* @           :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::WriteCurrentSettings()
{
    QSettings m_settings("weilaidb.com.cn", "autoccode");
    qDebug() << "ui_setup->checkBox_showcontent_autoappend->isChecked():" << ui_setup->checkBox_showcontent_autoappend->isChecked();
    m_settings.setValue("GenShowAppend", ui_setup->checkBox_showcontent_autoappend->isChecked());
    m_settings.setValue("RealTimeQuery",ui_setup->checkBox_rtQuery->isChecked());
    m_settings.setValue("ShowPath", ui_setup->checkBox_showpath->isChecked());
    m_settings.setValue("InDbShortKey",ui_setup->checkBox_indb_shortkey->isChecked());
    m_settings.setValue("rightselectindb", ui_setup->checkBox_rightTextSelectIndb->isChecked());
    m_settings.setValue("ServerFlag", ui_setup->checkBox_SeverFlag->isChecked());
    m_settings.setValue("SupportRemote", ui_setup->checkBox_supportRemote->isChecked());
    m_settings.setValue("CurrentDataBase", ui_dia_selectdb->comboBox_selectdb->currentIndex());
    m_settings.setValue("QueryExact", ui->checkBox_query_exact->isChecked());
    m_settings.setValue("ContentWithHeader", ui_setup->checkBox_content_withheader->isChecked());
    m_settings.setValue("codeshortcut", ui_setup->checkBox_codeshortcut->isChecked());
    m_settings.setValue("contentbeauty", ui_setup->checkBox_contentbeauty->isChecked());
    m_settings.setValue("codecutf8", ui->checkBox_codecutf8->isChecked());
    m_settings.setValue("senddata2subui", ui->checkBox_senddata2subui->isChecked());
    m_settings.setValue("comboBox_path", pathlist);
//    m_settings.setValue("checkBox_isfile", ui_toolsets->checkBox_isfile->isChecked());
    m_settings.setValue("checkBox_autoopen", ui_toolsets->checkBox_autoopen->isChecked());
    m_settings.setValue("lineEdit_hostip", ui_toolsets->lineEdit_hostip->text());

//    m_settings.setValue("ForeColor", ui_setup->pushButton_foreColor->palette());


//    m_settings.setValue("Language", localeCombo->currentIndex());
//    m_settings.setValue("WeekStart", firstDayCombo->currentIndex());
//    m_settings.setValue("Selection_Mode", selectionModeCombo->currentIndex());
//    m_settings.setValue("Show_Grid", gridCheckBox->isChecked());
//    m_settings.setValue("Show_Navigation_Bar", navigationCheckBox->isChecked());
//    m_settings.setValue("Horizontal_Header", horizontalHeaderCombo->currentIndex());
//    m_settings.setValue("Vertival_Header", verticalHeaderCombo->currentIndex());
//    m_settings.setValue("WeekdayColor", weekdayColorCombo->currentIndex());
//    m_settings.setValue("WeekendColor", weekendColorCombo->currentIndex());
//    m_settings.setValue("Header_Font", headerTextFormatCombo->currentIndex());
//    m_settings.setValue("First_Friday", firstFridayCheckBox->isChecked());
//    m_settings.setValue("May_First", mayFirstCheckBox->isChecked());
//    m_settings.setValue("Clock_Geometry", m_clock->saveGeometry());
//    m_settings.setValue("Birthday_Date", m_Birthday.toString ("yyyy-MM-dd"));
//    m_settings.setValue("Calendar_Geometry", this->saveGeometry());
    m_settings.setValue("AutoCCode_Geometry", this->saveGeometry());

    qDebug() << "setting filename:" << m_settings.fileName();

}

/*============================================
* FuncName    : autoCCode::closeEvent
* Description :
* @event      :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent";
    WriteCurrentSettings();
    event->accept();
}


/*============================================
* FuncName    : autoCCode::on_checkBox_query_exact_stateChanged
* Description :
* @arg1       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_checkBox_query_exact_stateChanged(int arg1)
{
    qDebug() << "on_checkBox_query_exact_stateChanged arg1 :" <<arg1;

    setStringColor(0, 0);
    SWITCH_genshow_textEdit_SetText(GenCode_str);
    SearchTextResWithColor2(GenCode_str);
}


/*============================================
* FuncName    : autoCCode::updateHelloMsg
* Description :
* @--         :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::updateHelloMsg(void)
{
    qDebug() << "update Hello Msg and updat!!!!!!!!!!!!!!" << endl;
//    helloworldNet->emitMsg(msg);
    helloworldNet->updateMsg(ui->genshow_textEdit->toPlainText());
    SendParent2ChildUI(0);
}

/*============================================
* FuncName    : autoCCode::on_checkBox_codeshortcut_stateChanged
* Description :
* @arg1       :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_checkBox_codeshortcut_stateChanged(int arg1)
{
    qDebug() << "CodeShortCut Ui:"<< arg1;
    if (arg1)
        woUi->show();
    else
        woUi->hide();
}

/*============================================
* FuncName    : autoCCode::on_checkBox_codecutf8_toggled
* Description :
* @checked    :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::on_checkBox_codecutf8_toggled(bool checked)
{
//    if(checked)
//    {
        filedraged(urls);
//    }
}

/*============================================
* FuncName    : autoCCode::SendParent2ChildUI
* Description :
* @state      :
* Author      :
* Time        : 2017-05-28
============================================*/
void autoCCode::SendParent2ChildUI(int state)
{
//    if(!state)
//        return;
    if(!isDropFileEnd)
        return;
    if(!ui->checkBox_senddata2subui->isChecked())
        return;
    woUi->setChildUI(ui->genshow_textEdit->toPlainText());
//    woUi->setChildUI(QString("abc"));
    isDropFileEnd = true;
}


void autoCCode::showbeautycontent(QString str, bool selfContained)
{
    if(ui_setup->checkBox_contentbeauty->isChecked())
    {
        if(ui->textEdit_right_beauty->isHidden())
            ui->textEdit_right_beauty->show();
        if(!ui->genshow_textEdit->isHidden())
            ui->genshow_textEdit->hide();

        QString html;
        if(selfContained)
            html += "<html>\n<body>\n";



        html += str;

        if(selfContained)
            html += "</body>\n</html>\n";

        if(!ui->genshow_textEdit->isHidden())
            ui->genshow_textEdit->hide();

        ui->textEdit_right_beauty->setText(html);
    }
    else
    {
        if(!ui->textEdit_right_beauty->isHidden())
            ui->textEdit_right_beauty->hide();
        if(ui->genshow_textEdit->isHidden())
            ui->genshow_textEdit->show();

        ui->genshow_textEdit->setPlainText(str);
    }

}

//添加路径
void autoCCode::on_tools_pushButton_addpath_clicked()
{
    qDebug() << "on_tools_pushButton_addpath_clicked";
    QString currentpath = ui_toolsets->comboBox_path->currentText();
//    toolsTabWidget->co
    qDebug() << "currentpath:" << currentpath;
    if(currentpath.simplified().length() == 0)
    {
        return;
    }
//    currentpath = currentpath.simplified();
    if(!currentpath.contains(":"))
    {
        currentpath += ":";
    }
    foreach (QString path, pathlist) {
        if(path == currentpath)
        {
            return;
        }
    }

    bool checkedstate = ui_toolsets->checkBox_autoopen->isChecked();
    ui_toolsets->checkBox_autoopen->setChecked(false);



    pathlist.append(currentpath);
    qDebug() << "pathlist size:" << pathlist.size();
//更新items
    ui_toolsets->comboBox_path->clear();
    pathlist.sort();
    ui_toolsets->comboBox_path->addItems(pathlist);

    ui_toolsets->checkBox_autoopen->setChecked(checkedstate);
}


void autoCCode::on_tools_pushButton_openpath_clicked()
{
    qDebug() << "on_tools_pushButton_openpath_clicked";
    QString currentpath = ui_toolsets->comboBox_path->currentText();
//    toolsTabWidget->co
//    currentpath = currentpath.simplified();
    qDebug() << "currentpath:" << currentpath;
    qDebug() << "pathlist size:" << pathlist.size();

    //python.exe
    LPCSTR exepath = "explorer.exe";
    LPCSTR filepath = currentpath.toAscii().data();

    ShellExecuteA(NULL,"open", exepath,filepath,NULL,SW_SHOWNORMAL);


}

void autoCCode::on_tools_pushButton_delpath_clicked()
{
    qDebug() << "on_tools_pushButton_delpath_clicked";
    QString currentpath = ui_toolsets->comboBox_path->currentText();
//    toolsTabWidget->co
    qDebug() << "currentpath:" << currentpath;
    if(currentpath.simplified().length() == 0)
    {
        return;
    }
//    currentpath = currentpath.simplified();
    if(!currentpath.contains(":"))
    {
        currentpath += ":";
    }
    pathlist.removeOne(currentpath);

    qDebug() << "pathlist size:" << pathlist.size();

    bool checkedstate = ui_toolsets->checkBox_autoopen->isChecked();
    ui_toolsets->checkBox_autoopen->setChecked(false);



//更新items
    ui_toolsets->comboBox_path->clear();
    pathlist.sort();
    ui_toolsets->comboBox_path->addItems(pathlist);
    ui_toolsets->checkBox_autoopen->setChecked(checkedstate);

}

void autoCCode::on_tools_pushButton_openfile_clicked()
{
    qDebug() << "on_tools_pushButton_openfile_clicked";
//    QString currentpath = ui_toolsets->comboBox_path->currentText();
    static QString orgdir = "";

    if(!ui_toolsets->comboBox_path->currentText().isEmpty())
    {
        QString usetext = ui_toolsets->comboBox_path->currentText();
        QFileInfo fileInfo(usetext);
        orgdir = fileInfo.path();
        if(fileInfo.isDir())
        {
            orgdir = usetext;
        }
        else
        {
            orgdir = fileInfo.path();
        }
        qDebug() << "comboBox_path current text:" << usetext;
        qDebug() << "orgdir:" << orgdir;
    }

    QString dir ;

    dir = QFileDialog::getOpenFileName(this,
            tr("Open File"),
            orgdir,
            "",
            0);
        if (!dir.isNull())
        {
            //fileName是文件名

        }
        else{
            //点的是取消
           return;
        }

    bool checkedstate = ui_toolsets->checkBox_autoopen->isChecked();
    ui_toolsets->checkBox_autoopen->setChecked(false);

    dir = dir.replace("\/","\\");
    ui_toolsets->comboBox_path->setEditText(dir);
    on_tools_pushButton_addpath_clicked();
    toolsTabWidget->raise(); //到上层
    toolsTabWidget->show();

    orgdir = dir;
    Sleep(200);

    ui_toolsets->checkBox_autoopen->setChecked(checkedstate);

//    qDebug("\033[32m123\033[33m456\n");
}

void autoCCode::on_tools_pushButton_opendir_clicked()
{
    qDebug() << "on_tools_pushButton_opendir_clicked";
//    QString currentpath = ui_toolsets->comboBox_path->currentText();
    static QString orgdir = "";

    if(!ui_toolsets->comboBox_path->currentText().isEmpty())
    {
        QString usetext = ui_toolsets->comboBox_path->currentText();
        QFileInfo fileInfo(usetext);
        orgdir = fileInfo.path();
        if(fileInfo.isDir())
        {
            orgdir = usetext;
        }
        else
        {
            orgdir = fileInfo.path();
        }
        qDebug() << "comboBox_path current text:" << usetext;
        qDebug() << "orgdir:" << orgdir;
    }

    QString dir ;

    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
    orgdir,
    QFileDialog::ShowDirsOnly
    | QFileDialog::DontResolveSymlinks);
    if (!dir.isNull())
    {
        //fileName是文件夹名

    }
    else{
        //点的是取消
       return;
    }

    bool checkedstate = ui_toolsets->checkBox_autoopen->isChecked();
    ui_toolsets->checkBox_autoopen->setChecked(false);

    dir = dir.replace("\/","\\");
    ui_toolsets->comboBox_path->setEditText(dir);
    on_tools_pushButton_addpath_clicked();
    toolsTabWidget->raise(); //到上层
    toolsTabWidget->show();

    orgdir = dir;
    Sleep(200);

    ui_toolsets->checkBox_autoopen->setChecked(checkedstate);

//    qDebug("\033[32m123\033[33m456\n");
}

void autoCCode::on_tools_pushButton_openpath_auto(int index)
{
    qDebug() << "path combox changed:" << index;
    if(!ui_toolsets->checkBox_autoopen->isChecked())
        return;
    static int saveindex = 0xffff;
    if(saveindex != index)
    {
        qDebug() << "path combox changed:" << index << ", saveindex:" << saveindex;
        on_tools_pushButton_openpath_clicked();
        saveindex = index;
    }

}



void autoCCode::on_tools_pushButton_connectssh_clicked()
{
    ui_toolsets->textEdit_showresult->clear();
    QString ipaddr = ui_toolsets->lineEdit_hostip->text().trimmed();
    ssh2_forut *sshtest = new ssh2_forut(ipaddr.toAscii().data(), ui_toolsets->textEdit_showresult);

    sshtest->whenconnected();
    delete sshtest;


}


void autoCCode::procFindShortCut()
{
    qDebug() << "procFindShortCut";
    ui->lineEdit_search->setFocus();
}
void autoCCode::procClearShortCut()
{
    qDebug() << "procClearShortCut";
    ui->lineEdit_search->setText("");
    ui->lineEdit_search->setFocus();
}

void autoCCode::ProcShowText(QString searchtext)
{
    if(ui->lineEdit_search->text().simplified().isEmpty())
    {
        ui->lineEdit_search->setPlaceholderText("请输入查找的数据");
    }
}
