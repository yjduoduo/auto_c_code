#include "autoccode.h"
#include "ui_autoccode.h"
#include "ui_PushDbdialog.h"
#include "ui_dialog_select_database.h"
#include <QtGui>
#include "prefix_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "version.h"
#include "gencodedatabase.h"
#include <iostream>
using namespace std;





//编码汉字
#define str_china(A)     QString::fromLocal8Bit(#A)




autoCCode::autoCCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoCCode),
    ui_dialog(new Ui::Dialog),
    ui_dia_selectdb(new Ui::Dialog_select_database),
    sets(NULL),
    index_key_color(0),
    index_note_color(0),
    flag_selectLeft(1)
{
    ui->setupUi(this);
    InDb_Dialog = new QDialog(this);
    ui_dialog->setupUi(InDb_Dialog);

    dialog_selectdb =  new QDialog(this);;
    ui_dia_selectdb->setupUi(dialog_selectdb);

    pushButtonSet();
    textEditSet();
    comboBoxSet();
    listWidgetSet();
    addstr_comboBox();
    lineTextEditSet();
    dragDropSet();
    checkboxSet();

}
void autoCCode::checkboxSet()
{
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
    QObject::connect(ui->lineEdit_search,SIGNAL(textChanged(QString)),
                     this,SLOT(SearchText(QString)));
}

void autoCCode::dragDropSet(void)
{
    //允许拖放的文字添加到编辑框中
    ui->codeshow_textEdit->setAcceptDrops(true);
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
}
void autoCCode::set_search_text()
{
    QClipboard *clipboard = QApplication::clipboard();
    ui->lineEdit_search->clear();
    ui->lineEdit_search->setText(clipboard->text());
}
void autoCCode::note_clear()
{
    ui_dialog->note_textEdit_dia->clear();
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
    int wd=desk->width();
    int ht=desk->height();
    InDb_Dialog->move(0,(ht-height()/2)/2);
}
void autoCCode::maxSize_ui_dialog()
{

    InDb_Dialog->setMaximumSize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()));
//    InDb_Dialog->setWindowFlags(InDb_Dialog->windowFlags()& Qt::WindowMaximizeButtonHint & Qt::WindowMinimizeButtonHint);

    //    InDb_Dialog->resize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()));
    InDb_Dialog->resize(QSize(QApplication::desktop()->width(),QApplication::desktop()->height()));
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

    //左滚动，对应 右滚动
    QObject::connect(this->ui->listWidget_codeview,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(listWidget_note_scroll_sync(QListWidgetItem*)));


    QObject::connect(this->ui->listWidget_note,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(listWidget_codeview_scroll_sync(QListWidgetItem*)));
}


void autoCCode::comboBox_selectdb_currentIndexChanged(const QString &arg1)
{
    self_print(comboBox_selectdb_currentIndexChanged);
    str_print(arg1);

    selected_langtype = arg1;
    //str_print(selected_langtype);
    LanguageType langtype = getLanguageType(selected_langtype);
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
        select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2' and delflag=0 ")
                .arg(sets->talbename)
                .arg(selected_langtype);
    }else{
        select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2' and aspect_field='%3' and delflag=0")
                .arg(sets->talbename)
                .arg(selected_langtype)
                .arg(aspect);
    }
    clr_selectresult(selectresult);
    str_print(select_express);
    //gencode str clear
    GenCode_str.clear();

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult,
                     ASPECT_NONE);

    ui->codeshow_textEdit->setText(selectresult.contentstr);
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
          <<str_china(C)
         <<str_china(C++)
        <<str_china(Qt)
       <<str_china(Python)
      <<str_china(shell)
     <<str_china(Jave)
    <<str_china(Oracle)
    <<str_china(Qtquick);

    ui_dialog->langtype_comboBox->addItems(strlist);

    //select db dialog add strlist;
    ui_dia_selectdb->comboBox_selectdb->addItems(strlist);

    //范畴
    addstr_aspect_comboBox();

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
                                                    "../",
                                                    tr("Ctype (*.c *.C *.cc *.h)"
                                                       ";;Cpptype(*.cpp *.CPP *.h)"
                                                       ";;QTtype(*.c *.C *.cpp *.CPP *.ui *.rc *.pro *.h)"
                                                       ";;Pythontype(*.py *.PY)"
                                                       ";;JavaType(*.java)"
                                                       ";;All Files(*.*)"));
    qDebug()<<"fileName:"<<fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }
    //    QString text_file = file.readAll();
    QString text_china = QString::fromLocal8Bit(file.readAll().data());

    ui->codeshow_textEdit->setText(text_china);

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
//入库函数
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

//void autoCCode::on_outdb_btn_clicked(void)
//{
//    self_print(on_outdb_btn_clicked);
//}

LanguageType autoCCode::getLanguageType(QString &type)
{
#if 1
    if(type == "C"){
        return languagetype_C_;
    }else if(type == "Qt"){
        return languagetype_Qt_;
    }
    else if(type == "Python"){
        return languagetype_Python_;
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
        return languagetypeCpp_;
    }
    else if(type == "Oracle")
    {
        return languagetype_Oracle_;
    }
    else{
        return languagetype_Err_;
    }
#else //type must be integer
    switch(type)
    {
    case "C":
        return languagetype_C_;
    case "Qt":
        return languagetype_Qt_;
    case "Python":
        return languagetype_Python_;
    case "Jave":
        return languagetype_Jave_;
    case "C++":
        return languagetypeCpp_;
    default:
        return languagetype_Err_;
    }
#endif
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
    QString content = ui_dialog->content_textEdit_dia->toPlainText().trimmed();
    QString lanaugetype = ui_dialog->langtype_comboBox->currentText();
    QString index_keyword   = ui_dialog->index_textEdit_dia->toPlainText().trimmed();
    index_keyword = index_keyword.replace("\n"," ");
//    index_keyword.trimmed();
    QString note   = ui_dialog->note_textEdit_dia->toPlainText().trimmed();
    note = note.replace("\n"," ");
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

    QString select_express = QString("select content from %1 where lantype='%2' and content='%3' and vartype='%4' and delflag=0")
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


    b.creatable(&insertcontent);
    b.inserttable(&insertcontent);

#ifndef DEBUG_V

    if(ui->checkBox_inbox->isChecked())
    {
        //对话框不关闭
        ui_dialog->content_textEdit_dia->clear();
    }else{
        InDb_Dialog->close();
    }


#else
    //对话框不关闭
    ui_dialog->content_textEdit_dia->clear();
#endif

    //内容添加后，更新控件中内容的相关显示
    update_show_after_insert();
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
#endif


}
//添加到右边的内容中
void autoCCode::add_to_gen_code_textedit_by_keyword(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit);
    QString str = item->text();
    unsigned int index = 0;
    for(int i=0;i<selectresult.content_list.size();i++){
        if(str == selectresult.keyword_list.at(i))
            index = i;
        qDebug()<<"note list:"<<selectresult.note_list.at(i);
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

}
//添加到右边的内容中
void autoCCode::add_to_gen_code_textedit_by_note(QListWidgetItem* item)
{
    self_print(add_to_gen_code_textedit);
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

}


void autoCCode::rightClear_textedit(void)
{
    GenCode_str.clear();
    ui->genshow_textEdit->clear();
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

    //每次重新开始记录，否则滚动时点击会死机
    index_key_color = 0;
    index_note_color = 0;


}
void autoCCode::clear_listWidget_beforecall(void)
{
    ui->listWidget_codeview->clear();
    ui->listWidget_note->clear();
}
void autoCCode::listWidget_scrollToBottom(void)
{
    ui->listWidget_codeview->scrollToBottom();
    ui->listWidget_note->scrollToBottom();
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



    ui->codeshow_textEdit->setText(selectresult.contentstr);

    clear_listWidget_beforecall();
    ui->listWidget_codeview->addItems(selectresult.keyword_list);
    ui->listWidget_note->addItems(selectresult.note_list);

    listWidget_scrollToBottom();
}

void autoCCode::ui_comboBox_vartype_currentIndexChanged(const QString &str)
{
    self_print(ui_comboBox_vartype_currentIndexChanged);
    str_print(str);

    if(!sets)
        return;

    if(str.contains("header")){
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where vartype='%2' and delflag=0")
                .arg(sets->talbename)
                .arg("header");
        select_db_by_vartype(select_express);
    }else if(str.contains("function")){
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where vartype='%2' and delflag=0")
                .arg(sets->talbename)
                .arg("function");
        select_db_by_vartype(select_express);
    }else if(str.contains("struct")){
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where vartype='%2' and delflag=0")
                .arg(sets->talbename)
                .arg("struct");
        select_db_by_vartype(select_express);
    }
    else if(str.contains("variable")){
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where vartype='%2' and delflag=0")
                .arg(sets->talbename)
                .arg("variable");
        select_db_by_vartype(select_express);
    }
    else{
        //        str_print(sets->langtype);
        QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2' and delflag=0")
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
    ui->listWidget_note->item(index_key_color)->setBackground(QBrush(Qt::white,Qt::SolidPattern));
    ui->listWidget_note->item(index)->setBackground(QBrush(Qt::green,Qt::SolidPattern));
    index_key_color = index; //
    str_print(index_key_color);

    ui->listWidget_codeview->item(index_note_color)->setBackground(QBrush(Qt::white,Qt::SolidPattern));
    flag_selectLeft = 0 ;
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
    ui->listWidget_codeview->item(index_note_color)->setBackground(QBrush(Qt::white,Qt::SolidPattern));
    ui->listWidget_codeview->item(index)->setBackground(QBrush(Qt::red,Qt::SolidPattern));
    index_note_color = index; //


    ui->listWidget_note->item(index_key_color)->setBackground(QBrush(Qt::white,Qt::SolidPattern));


    flag_selectLeft = 1 ;
    str_print( flag_selectLeft );
}

void autoCCode::update_show_after_insert(void)
{
    if(!sets)
        return;
    QString select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2' and delflag=0")
            .arg(sets->talbename)
            .arg(getLanguageStr(sets->langtype));
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
                             str_china(请选择左侧进行删除),
                             QMessageBox::Yes,QMessageBox::Yes);

        return;
    }

    /*  输入对话框   */
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

    QString select_express;
    select_express.clear();
    select_express = QString("select lowercase_keyworks,keywords,content,lantype,note,vartype from %1 where lantype='%2' and delflag=0")
            .arg(sets->talbename)
            .arg(getLanguageStr(sets->langtype));


    clr_selectresult(selectresult);

    str_print(select_express);

    b.searchdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult,
                     searchStr.toLower());

    ui->codeshow_textEdit->setText(selectresult.contentstr);

    clear_listWidget_beforecall();
    ui->listWidget_codeview->addItems(selectresult.keyword_list);
    ui->listWidget_note->addItems(selectresult.note_list);

}
void autoCCode::cleanLineTextEditSearch(void)
{
    ui->lineEdit_search->setFocus();
    if(ui->lineEdit_search->text().isEmpty())
        return;
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
    select_express = QString("select content,lantype,keywords,note,vartype from %1 where lantype='%2'")
            .arg(sets->talbename)
            .arg(getLanguageStr(sets->langtype));
    clr_selectresult(selectresult);
    str_print(select_express);
    GenCode_str.clear();

    b.selectdatabase(sets->databasename,select_express.toLocal8Bit().data(),
                     selectresult,
                     ASPECT_NONE);

    ui->codeshow_textEdit->setText(selectresult.contentstr);

    clear_listWidget_beforecall();
    ui->listWidget_codeview->addItems(selectresult.keyword_list);
    ui->listWidget_note->addItems(selectresult.note_list);

    //    return;

    QStringList::const_iterator iterator = selectresult.keyword_list.begin();

    while( iterator != selectresult.keyword_list.end()){
        cout << (*iterator).toAscii().data() << endl;
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
    /*  读取文件 只读   */
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        ui->codeshow_textEdit->setText(stream.readAll());
    }
    file.close();
}
