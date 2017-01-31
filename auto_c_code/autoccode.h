#ifndef AUTOCCODE_H
#define AUTOCCODE_H

#include <QtGui>
#include <QWidget>
#include "prefix_string.h"
#include <QDialog>
#include <QListWidgetItem>
#include <QListView>
#include "gencodedatabase.h"
#include "uithread.h"
#include "debugsets.h"
#include "version.h"
#include <QTcpSocket>
#include <QTcpServer>
#include "window_calender.h"

namespace Ui {
class autoCCode;
class Dialog;
class Dialog_select_database;
class AutoIndb;//自动入库界面
class SetUpDialog;
class ToolsTabWidget;//工具集
class UdpPkgDialog;//UDP
class WizardPage;
}

class codeEditor;

#define VARIABLE

class autoCCode : public QWidget,public GenCodeDatabase
{
    Q_OBJECT

public:

    explicit autoCCode(QWidget *parent = 0);
    ~autoCCode();

public:
    QDialog *InDb_Dialog;//手动入库界面
    QDialog *dialog_selectdb;//选择数据库界面
    QDialog *dialog_autoindb;//自动入库界面
    QDialog *SetUp_Dialog;//设置界面,操作函数的实体
//    QDialog *ToolSets_Dialog;
    QTabWidget *toolsTabWidget;//工具集界面,操作函数的实体
    QWizardPage *codetool;
    GenCodeDatabase *gb;

    QString selecttext;

    enum{
        STATE_CLIPBORD_CHAGED,
        STATE_CLIPBORD_NOCHAGED,
        STATE_CLIPBORD_UNKOWN
    };
    enum{
        FLAG_NO,
        FLAG_YES,
        FLAG_UNKNOWN
    };


protected://function declared
    void SetlistWidget_codeview_row(int row);
    int GetlistWidget_codeview_row(void);

    int get_CurrentIndex_comboBox_vartype(const QString &vartype);
    int get_aspect_list_index(const QString &index_str);

    void SearchTextResWithColor(QString &resStr);//带颜色的文本处理
    void SearchTextResWithColor2(QString &resStr);//带颜色的文本处理
    void setCharColor(unsigned int pos);/*  QT:设置textedit文本框中某个字符的格式 */
    void setStringColor(unsigned int pos,unsigned int len);/*  QT:设置textedit文本框中某个字符串的格式 */
    //安装事件过滤器
    virtual bool eventFilter ( QObject * watched, QEvent *event );
    virtual void keyPressEvent(QKeyEvent *k);
    void mouseMoveEvent ( QMouseEvent * e );
    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e );
    void mouseDoubleClickEvent( QMouseEvent * e );
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dropEvent(QDropEvent *event);
    bool eventFilter_ui_choseCodeDB_btn(QObject *watched, QEvent *event);
    bool eventFilter_ui_dia_selectdb_comboBox_selectdb(QObject *watched, QEvent *event);
    bool eventFilter_ui_dialog(QObject *watched, QEvent *event);
    bool eventFilter_ui_dialog_langtype_comboBox(QObject *watched, QEvent *event);
    bool eventFilter_ui_setup(QObject *watched, QEvent *event);
//    bool eventFilter_ui_toolsets(QObject *watched, QEvent *event);


    //安装鼠标滚动过滤器
    void wheelEvent(QWheelEvent *event);
    //鼠标是否在控件genshow_textEdit之中
    int IsCursorInGenShowUi(QObject *watched);
    int isCTRLKeyPressed;//Ctrl键是否按下
    int Isgenshow_textEdit_Enter;
    int isToolsContent_Enter;//Ctrl键是否按下 for tools widget
    int isToolsSuffix_Enter;//Ctrl键是否按下 for tools widget suffix


    //当前是否有选中库，如果没有则直接返回，可当作一个函数接口。
    bool currentDbHaved(void);

    //ui->listWidget_codeview->addItems(s1); 显示内容缩短优化
    QStringList listWidget_codeview_subShow(QStringList &strlist);

    void CharFormat(void);//文本显示格式

    void setWindowTitle_Main(const QString &arg1);

    QString getCurrentDateTimeTxt();//取名称为时间的文件字符串

    void SaveUiMove();
private slots:
    void ZoomInFont(QObject *watched);//放大字体
    void ZoomOutFont(QObject *watched);//缩小字体

    void MainSetUp();//主界面设置

    void on_save_btn_clicked();

    void db_comboBox_activated(const QString &arg1);

    void on_choseCodeDB_btn_clicked(void);

    void on_gencode_btn_clicked(void);

    void on_indb_btn_clicked(void);

    void on_indb_window_show_hide();

    //    void on_outdb_btn_clicked(void);

    void aboutVersion(void);

    void comboBox_selectdb_currentIndexChanged(const QString &arg1);

    //    void add_to_gen_code_textedit(QListWidgetItem* item);
    void add_to_gen_code_textedit_by_keyword(QListWidgetItem* item);

    void add_to_gen_code_textedit_by_note(QListWidgetItem* item);


    void rightClear_textedit(void);

    void ui_comboBox_vartype_currentIndexChanged(const QString &str);

    void comboBox_aspect_currentIndexChanged(const QString &str);

    void add_aspect_totable(void);

    void addstr_aspect_comboBox(void);

    void listWidget_note_scroll_sync(QListWidgetItem *item);

    void listWidget_codeview_scroll_sync(QListWidgetItem* item);

    void delete_btn_clicked_selfdefine(void);

    void SearchText(const QString &searchStr);

    void cleanLineTextEditSearch(void);


    void add_column_lowercase_keywords_content(void);

    void set_index_text();

    void set_note_textEdit_firstline();

    void on_checkBox_SEL_change(bool flag);

    void on_checkBox_rightTextSelectIndb_change(bool flag);

    void on_checkBox_rtQuery_change(bool flag);

    void on_checkBox_checkBox_SeverFlag_change(bool flag);

    quint8 get_rtQuery_enable();//判断是否支持实时查询

    void setCliptext();

    void setCliptext_content();

    void note_clear();

    void note_clear_focus();

    void note_focus();

    void content_clear();

    void content_focus();

    void set_search_text();

    void search_text_clear();

    void minSize_ui_dialog();

    void widthSize_ui_dialog();

    void maxSize_ui_dialog();

    void setDesktop_center(QDialog *dialog);

    void WidgetXsetDesktop_center(QWidget *widget);

    void WidgetYsetDesktop_center(QWidget *widget);

    void WidgetXYsetDesktop_center(QWidget *widget);

    void SearchEnter();

    void listWidget_note_with_currentRowChanged(int row);

    void listWidget_note_with_enter(const QModelIndex &modelindex);

    /*  隐藏和显示窗口   */
#if 1 /*for shotcut*/
    void toggle()
    {
#if 0

        if(isVisible() /*&& is_countshow()*/){
            setWindowFlags(Qt::Window);
            setWindowFlags(Qt::WindowSystemMenuHint);
            setVisible(FALSE);
        }
        else{
            setWindowFlags(Qt::WindowStaysOnTopHint);

            setVisible(TRUE);
//            setWindowFlags(Qt::WindowStaysOnBottomHint);
        }
#endif
        setVisible(!isVisible());

        //focusInEvent();
        //        setFocus();
        activateWindow();
        contentSetFocus();

        //this->ui.focus();
    }
#endif

    void alert(){
        qApp->alert(this,2000); //窗口非激活状态下8秒内通知用户
    }

    void contentSetFocus(void);

    void modify_content();

    void rightTextShowClear_oncheched();//cheched selected 右清空

    void getText_FromRight(void);

    void on_pushbtn_autoindb_clicked_self();

    void on_ui_autoindb_pushBtn_Open_clicked();

    void on_ui_autoindb_pushBtn_process_clicked();

    void get_autoindb_textedit_cursor_postion();

    void ok_btn_dia_clicked_self_autoindb(QString begintext,QString combinetext,int *ret);//自动入库函数

    void pushdb_checkbox_if_selected();

    void SearchText_WithTimer(void);

    void SearchText_WithTimer_Enter(void);

    void pasteClicpTextToSearchEdit();

    void isCheckBox_cliptext_checked(bool checked);

    void ui_dialog_AutoGetCon(bool checked);

    void pasteClicpTextToAutoGetCon_UiDialog();

    int getLimitNum();//获取limit数据查询限制的大小

    int showcode_textEdit_AtBotton();//是否显示界面左侧的内容了
    void PopInDbUi();//弹出入库对话框
    void check_genshow_textEdit_is_append();
    quint8 IsClipboardChanged();//判断剪切板内容改变否

    void on_lineEdit_search_MouseButtonDblClick();
    void on_lineEdit_search_Key_Escape();

    void completeText(QModelIndex index);
    void on_pushButton_foreColor_clicked2();

    void on_pushButton_backColor_clicked2();
    //颜色对话框设置-前景色
    void setforegroudColor();
    //颜色对话框设置
    void setbackgroudColor();
    //默认对话框颜色
    void setDefaultColor();
    //设置字体
    void setFont();
    //选择数据变化
    void on_selectionChanged();
    //UDP界面ok按钮按下
    void on_pUdpdialog_okBtn_clicked();
public slots:

    void ok_btn_dia_clicked_self(void);

    void cancel_btn_dia_clicked_self(void);

    QString get_clipboard_data();

    void ReadHistorySettings();
    void WriteCurrentSettings();


private:
    void pushButtonSet(void);//按钮集合
    void pushButton_setup();

    void keyPressEventSet();
    void textEditSet(void);
    void addstr_comboBox(void);

    void shortCutSet(void);
    void PopMenu(void);/* 右键菜单 */
    void ProgressBarSet(void);
    //在创建了过滤器之后，下面要做的是安装这个过滤器。安装过滤器需要调用installEventFilter()函数。
    void InstallEventFilterSets(void);
    void ListViewSets();
    void ProgressBarSetValue(int value);
    void QTimerSet(void);
    void ThreadSets();
    void DebugSets();
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

    void clr_selectresult(SelectResult &result);

    void clr_looktexthisresult(LookTextHistoryResult &result);

    void update_show_after_insert(void);

    void lineTextEditSet(void);

    void save_before_ops(void);

    void restore_before_ops(void);

    void clear_listWidget_beforecall(void);

    void judge_color_index(void);

    void listWidget_scrollToBottom(void);

    void listWidget_scrollToTop(void);

    void dragDropSet(void);

    void checkboxSet();

    QStringList getLstIp();

    void ShowTipsInfo(QString s);

    void ShowTipsInfoWithShowTime(QString s, quint32 ultimeout);
private:

    Ui::autoCCode *ui;
    Ui::Dialog *ui_dialog;//入库
    Ui::Dialog_select_database *ui_dia_selectdb;//选择数据库
    Ui::AutoIndb *ui_autoindb;//自动入库界面
    Ui::SetUpDialog *ui_setup;//设置界面
    Ui::ToolsTabWidget *ui_toolsets;//工具集
    Ui::UdpPkgDialog *pUdp_ui; //UDP
    Ui::WizardPage  *ui_codetool;


    /* pthread线程，防止界面假死 */
    UiThread *pthread_event;

    ClsDebugSets qCDebug;

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void readTextFile(const QString &fileName);
    void readTextFileAppend(const QString &fileName);
    void closeEvent(QCloseEvent *event);

protected:
    QString GenCode_str;
private://QString
    QString selected_langtype;

    SelectResult selectresult;

    SelectResult selectresult_tmp;
    unsigned int index_key_color_tmp;
    unsigned int index_note_color_tmp;


    codestructSets* sets;

    int index_key_color;//记录上次点击的位置，并注意再次点击时取消
    int index_note_color;

    int flag_selectLeft;

    int listWidget_codeview_row;

    int CurrentIndex_comboBox_aspect;
    int CurrentIndex_comboBox_vartype;
    LanguageType CurrentIndex_comboBox_langtype;


    QStringList aspect_list_mem;

    QTimer *timer;
    QTimer *timer_checkbox_sel;//入库checkbox如果选中，则将选择的内容自动添加到note
    int textline_total;
    QString cursor_left_text;//光标左侧文本
//    QString text_tmp;//提示信息框
    QStringList textlist_indb_content;

    QTimer *lineEdit_search_timer;
    QTimer *checkbox_getcliptext_timer;
    QTimer *checkbox_AutoGetCon_timer;
    QTimer *timer_datachangedpopui;//数据发生变化，弹出入库框
    QTimer *timer_genshow_textEdit_checkContent_append;//检测有内容变化时，自动追加到后面

    QListView *listView; // lineEdit Search Text显示列表
    QStringListModel *model; // 完成列表的model

    QTextCharFormat defcharfmt;//当前显示字符格式

    int is_selected; //推送内容是否选择

public:

    QTextCodec *codec;


    /* 右键菜单 */
private:
    void contextMenuEvent(QContextMenuEvent *event);
    QAction* Act_Maxsize;
    QAction* Act_Normal;

private slots:
    void ADDChange()
    {
        qDebug()<<"XML Item Pressed.";
    }
    void XMLChange()
    {
        qDebug()<<"SHOW Item Pressed.";
    }
    void on_pushButton_rightTextSelectIndb_clicked();
    void ok_btn_dia_clicked_self_another(QString con,QString str_sel);//根据定义的数据，直接入库并显示
    void on_pushButton_toolsSets_clicked();
    void on_pushButton_notepad_exe_clicked();
    void on_pushButton_calc_exe_clicked();
    void on_pushButton_notepadplus_exe_clicked();
    void on_pushButton_python_exe_clicked();
    void on_pushButton_python_Dpathtestfile_exe_clicked();
    void on_pushButton_sourceinsight_exe_clicked();
    void on_pushButton_cmd_exe_clicked();
    void on_pushButton_show_calender_clicked();


private slots://ui tools
    void on_textEdit_main_content_textChanged();
    void on_textEdit_suffix_textChanged();



private VARIABLE://ui tools
    QString textEdit_main_uitools;
    QString textEdit_suff_uitools;


private://network
    int CheckIPAddr(QString ipaddr);
    void SaveContent(QString filename, QString ipaddr);
    QString ReadContent(QString filename);
    void SaveLocalIpaddr(QString ipaddr);
    QString ReadLocalIpAddr();
    void SaveRemoteIpaddr(QString ipaddr);
    QString ReadRemoteIpAddr();
private slots://network
    void saveRemoteIP(QString rip);
    void saveBindLocalIP(QString lip);
    void hellosocket();
    void readfromremote(QString recvBigMsg);
    void timerwritemsg();
    void recvClientMsg();
    void checkSupportRemote(bool flag);
    bool isMainServer();
    void ifSeverUi();

    void displayErr(QAbstractSocket::SocketError socketError);
    void updateClientProgress(qint64 numBytes); //write data
//    void updateReadMsgProgress();

    void on_btn_find_down_clicked();

    void on_btn_find_up_clicked();

    void on_showlarger_btn_clicked();

private:
    QString remoteip;
    QString localip;
    QTcpSocket *socket;
    QHostAddress *hostaddr;
    QTcpServer *tcpserver;

    QTcpSocket *clientConnection;

    //写数据统计
    qint64 TotalBytes;
    qint64 byteWritten;
    qint64 bytesToWrite;
//    QString fileName;
//    QFile *localFile;
    QByteArray outBlock;
    QByteArray outBlockFile;//文件字节序列

private:
    Window *window_calender;
};


#endif // AUTOCCODE_H
