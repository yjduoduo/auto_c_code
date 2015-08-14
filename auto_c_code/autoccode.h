#ifndef AUTOCCODE_H
#define AUTOCCODE_H

#include <QtGui>
#include <QWidget>
#include "prefix_string.h"
#include <QDialog>
#include <QListWidgetItem>
#include <QListView>
#include "gencodedatabase.h"

namespace Ui {
class autoCCode;
class Dialog;
class Dialog_select_database;
class AutoIndb;//自动入库界面
}




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

    GenCodeDatabase b;

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
    void setCharColor(unsigned int pos);/*  QT:设置textedit文本框中某个字符的格式 */
    void setStringColor(unsigned int pos,unsigned int len);/*  QT:设置textedit文本框中某个字符串的格式 */
    //安装事件过滤器
    virtual bool eventFilter ( QObject * watched, QEvent *event );
    //当前是否有选中库，如果没有则直接返回，可当作一个函数接口。
    bool currentDbHaved(void);

    //ui->listWidget_codeview->addItems(s1); 显示内容缩短优化
    QStringList listWidget_codeview_subShow(QStringList &strlist);

    void CharFormat(void);//文本显示格式

    void setWindowTitle_Main(const QString &arg1);

    QString getCurrentDateTimeTxt();//取名称为时间的文件字符串
private slots:
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

    void setCliptext();

    void setCliptext_content();

    void note_clear();

    void note_clear_focus();

    void note_focus();

    void content_clear();

    void set_search_text();

    void search_text_clear();

    void minSize_ui_dialog();

    void widthSize_ui_dialog();

    void maxSize_ui_dialog();

    void setDesktop_center(QDialog *dialog);

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

    void pasteClicpTextToSearchEdit();

    void isCheckBox_cliptext_checked(bool checked);

    void ui_dialog_AutoGetCon(bool checked);

    void pasteClicpTextToAutoGetCon_UiDialog();

    int getLimitNum();//获取limit数据查询限制的大小

    int showcode_textEdit_AtBotton();//是否显示界面左侧的内容了
    void PopInDbUi();//弹出入库对话框
    quint8 IsClipboardChanged();//判断剪切板内容改变否

    void on_lineEdit_search_MouseButtonDblClick();
    void on_lineEdit_search_Key_Escape();

    void completeText(QModelIndex index);
public slots:

    void ok_btn_dia_clicked_self(void);

    void cancel_btn_dia_clicked_self(void);


private:
    void pushButtonSet(void);//按钮集合

    void keyPressEventSet();
    void textEditSet(void);
    void addstr_comboBox(void);

    void shortCutSet(void);
    void ProgressBarSet(void);
    //在创建了过滤器之后，下面要做的是安装这个过滤器。安装过滤器需要调用installEventFilter()函数。
    void InstallEventFilterSets(void);
    void ListViewSets();
    void ProgressBarSetValue(int value);
    void QTimerSet(void);
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
private:

    Ui::autoCCode *ui;
    Ui::Dialog *ui_dialog;//入库
    Ui::Dialog_select_database *ui_dia_selectdb;//选择数据库
    Ui::AutoIndb *ui_autoindb;//自动入库界面

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void readTextFile(const QString &fileName);


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

    QListView *listView; // lineEdit Search Text显示列表
    QStringListModel *model; // 完成列表的model

    QTextCharFormat defcharfmt;//当前显示字符格式

public:

    QTextCodec *codec;
};


#endif // AUTOCCODE_H
