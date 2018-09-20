/********************************************************************************
** Form generated from reading UI file 'toolstabwidget.ui'
**
** Created: Sun Sep 9 12:29:40 2018
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLSTABWIDGET_H
#define UI_TOOLSTABWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ToolsTabWidget
{
public:
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_calc;
    QPushButton *pushButton_cmd;
    QPushButton *pushButton_notepad;
    QPushButton *pushButton_notepadplus;
    QPushButton *pushButton_sourceinsight;
    QPushButton *pushButton_python;
    QPushButton *pushButton_python_Dtest;
    QWidget *tab_code;
    QGridLayout *gridLayout;
    QPushButton *pushButton_lineadd;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEdit_main_content;
    QTextEdit *textEdit_suffix;
    QWidget *tab_date;
    QGridLayout *gridLayout_4;
    QPushButton *pushButton_calender;
    QWidget *tab_3;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit_hostip;
    QPushButton *pushButton_connectssh;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_5;
    QTextEdit *textEdit_execcmd;
    QTextEdit *textEdit_showresult;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBox_path;
    QPushButton *pushButton_opendir;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_open;
    QCheckBox *checkBox_autoopen;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_addpath;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_delpath;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QTabWidget *ToolsTabWidget)
    {
        if (ToolsTabWidget->objectName().isEmpty())
            ToolsTabWidget->setObjectName(QString::fromUtf8("ToolsTabWidget"));
        ToolsTabWidget->resize(729, 382);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButton_calc = new QPushButton(tab);
        pushButton_calc->setObjectName(QString::fromUtf8("pushButton_calc"));
        pushButton_calc->setMinimumSize(QSize(0, 80));
        pushButton_calc->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(pushButton_calc, 0, 0, 1, 1);

        pushButton_cmd = new QPushButton(tab);
        pushButton_cmd->setObjectName(QString::fromUtf8("pushButton_cmd"));
        pushButton_cmd->setMinimumSize(QSize(0, 80));
        pushButton_cmd->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(pushButton_cmd, 0, 1, 1, 1);

        pushButton_notepad = new QPushButton(tab);
        pushButton_notepad->setObjectName(QString::fromUtf8("pushButton_notepad"));
        pushButton_notepad->setMinimumSize(QSize(0, 80));
        pushButton_notepad->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(pushButton_notepad, 1, 0, 1, 1);

        pushButton_notepadplus = new QPushButton(tab);
        pushButton_notepadplus->setObjectName(QString::fromUtf8("pushButton_notepadplus"));
        pushButton_notepadplus->setMinimumSize(QSize(0, 80));
        pushButton_notepadplus->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(pushButton_notepadplus, 2, 0, 1, 1);

        pushButton_sourceinsight = new QPushButton(tab);
        pushButton_sourceinsight->setObjectName(QString::fromUtf8("pushButton_sourceinsight"));
        pushButton_sourceinsight->setMinimumSize(QSize(0, 80));
        pushButton_sourceinsight->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(pushButton_sourceinsight, 2, 1, 1, 1);

        pushButton_python = new QPushButton(tab);
        pushButton_python->setObjectName(QString::fromUtf8("pushButton_python"));
        pushButton_python->setMinimumSize(QSize(0, 80));
        pushButton_python->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(pushButton_python, 3, 0, 1, 1);

        pushButton_python_Dtest = new QPushButton(tab);
        pushButton_python_Dtest->setObjectName(QString::fromUtf8("pushButton_python_Dtest"));
        pushButton_python_Dtest->setMinimumSize(QSize(0, 80));
        pushButton_python_Dtest->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(pushButton_python_Dtest, 3, 1, 1, 1);

        ToolsTabWidget->addTab(tab, QString());
        tab_code = new QWidget();
        tab_code->setObjectName(QString::fromUtf8("tab_code"));
        gridLayout = new QGridLayout(tab_code);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton_lineadd = new QPushButton(tab_code);
        pushButton_lineadd->setObjectName(QString::fromUtf8("pushButton_lineadd"));
        pushButton_lineadd->setMinimumSize(QSize(81, 51));
        pushButton_lineadd->setMaximumSize(QSize(81, 51));

        gridLayout->addWidget(pushButton_lineadd, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        textEdit_main_content = new QTextEdit(tab_code);
        textEdit_main_content->setObjectName(QString::fromUtf8("textEdit_main_content"));

        verticalLayout_2->addWidget(textEdit_main_content);

        textEdit_suffix = new QTextEdit(tab_code);
        textEdit_suffix->setObjectName(QString::fromUtf8("textEdit_suffix"));
        textEdit_suffix->setMinimumSize(QSize(0, 80));
        textEdit_suffix->setMaximumSize(QSize(16777215, 80));

        verticalLayout_2->addWidget(textEdit_suffix);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);

        ToolsTabWidget->addTab(tab_code, QString());
        tab_date = new QWidget();
        tab_date->setObjectName(QString::fromUtf8("tab_date"));
        gridLayout_4 = new QGridLayout(tab_date);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        pushButton_calender = new QPushButton(tab_date);
        pushButton_calender->setObjectName(QString::fromUtf8("pushButton_calender"));
        pushButton_calender->setMinimumSize(QSize(0, 100));

        gridLayout_4->addWidget(pushButton_calender, 0, 0, 1, 1);

        ToolsTabWidget->addTab(tab_date, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_5 = new QGridLayout(tab_3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lineEdit_hostip = new QLineEdit(tab_3);
        lineEdit_hostip->setObjectName(QString::fromUtf8("lineEdit_hostip"));
        lineEdit_hostip->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(lineEdit_hostip);

        pushButton_connectssh = new QPushButton(tab_3);
        pushButton_connectssh->setObjectName(QString::fromUtf8("pushButton_connectssh"));
        pushButton_connectssh->setMinimumSize(QSize(150, 30));

        horizontalLayout_3->addWidget(pushButton_connectssh);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(tab_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 20));

        horizontalLayout_4->addWidget(label);

        label_2 = new QLabel(tab_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 20));

        horizontalLayout_4->addWidget(label_2);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        textEdit_execcmd = new QTextEdit(tab_3);
        textEdit_execcmd->setObjectName(QString::fromUtf8("textEdit_execcmd"));

        horizontalLayout_5->addWidget(textEdit_execcmd);

        textEdit_showresult = new QTextEdit(tab_3);
        textEdit_showresult->setObjectName(QString::fromUtf8("textEdit_showresult"));

        horizontalLayout_5->addWidget(textEdit_showresult);


        verticalLayout->addLayout(horizontalLayout_5);


        gridLayout_5->addLayout(verticalLayout, 0, 0, 1, 1);

        ToolsTabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        comboBox_path = new QComboBox(tab_2);
        comboBox_path->setObjectName(QString::fromUtf8("comboBox_path"));
        comboBox_path->setMinimumSize(QSize(0, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(14);
        comboBox_path->setFont(font);
        comboBox_path->setStyleSheet(QString::fromUtf8("background-color: rgb(1, 147, 230);\n"
"color: rgb(255, 255, 255);"));
        comboBox_path->setEditable(true);
        comboBox_path->setMaxVisibleItems(50);

        horizontalLayout->addWidget(comboBox_path);

        pushButton_opendir = new QPushButton(tab_2);
        pushButton_opendir->setObjectName(QString::fromUtf8("pushButton_opendir"));
        pushButton_opendir->setMinimumSize(QSize(0, 50));
        pushButton_opendir->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(pushButton_opendir);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_open = new QPushButton(tab_2);
        pushButton_open->setObjectName(QString::fromUtf8("pushButton_open"));
        pushButton_open->setMinimumSize(QSize(150, 100));
        pushButton_open->setMaximumSize(QSize(150, 100));

        horizontalLayout_2->addWidget(pushButton_open);

        checkBox_autoopen = new QCheckBox(tab_2);
        checkBox_autoopen->setObjectName(QString::fromUtf8("checkBox_autoopen"));
        checkBox_autoopen->setMinimumSize(QSize(0, 50));
        checkBox_autoopen->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(checkBox_autoopen);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton_addpath = new QPushButton(tab_2);
        pushButton_addpath->setObjectName(QString::fromUtf8("pushButton_addpath"));
        pushButton_addpath->setMinimumSize(QSize(150, 100));
        pushButton_addpath->setMaximumSize(QSize(150, 100));

        horizontalLayout_2->addWidget(pushButton_addpath);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        pushButton_delpath = new QPushButton(tab_2);
        pushButton_delpath->setObjectName(QString::fromUtf8("pushButton_delpath"));
        pushButton_delpath->setMinimumSize(QSize(150, 100));
        pushButton_delpath->setMaximumSize(QSize(150, 100));

        horizontalLayout_2->addWidget(pushButton_delpath);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        gridLayout_3->addLayout(horizontalLayout_2, 1, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 2, 0, 1, 1);

        ToolsTabWidget->addTab(tab_2, QString());

        retranslateUi(ToolsTabWidget);

        ToolsTabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(ToolsTabWidget);
    } // setupUi

    void retranslateUi(QTabWidget *ToolsTabWidget)
    {
        ToolsTabWidget->setWindowTitle(QApplication::translate("ToolsTabWidget", "TabWidget", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_calc->setToolTip(QApplication::translate("ToolsTabWidget", "open calc", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_calc->setText(QApplication::translate("ToolsTabWidget", "\350\256\241\347\256\227\345\231\250", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_cmd->setToolTip(QApplication::translate("ToolsTabWidget", "open calc", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_cmd->setText(QApplication::translate("ToolsTabWidget", "\345\221\275\344\273\244CMD", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_notepad->setToolTip(QApplication::translate("ToolsTabWidget", "open notepad", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_notepad->setText(QApplication::translate("ToolsTabWidget", "\350\256\260\344\272\213\346\234\254", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_notepadplus->setToolTip(QApplication::translate("ToolsTabWidget", "open notepad++", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_notepadplus->setText(QApplication::translate("ToolsTabWidget", "NotePad++", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_sourceinsight->setToolTip(QApplication::translate("ToolsTabWidget", "open source insight", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_sourceinsight->setText(QApplication::translate("ToolsTabWidget", "SrcInsight", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_python->setToolTip(QApplication::translate("ToolsTabWidget", "open python", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_python->setText(QApplication::translate("ToolsTabWidget", "Python", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_python_Dtest->setToolTip(QApplication::translate("ToolsTabWidget", "open python D:test.py", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_python_Dtest->setText(QApplication::translate("ToolsTabWidget", "Python:DUt", 0, QApplication::UnicodeUTF8));
        ToolsTabWidget->setTabText(ToolsTabWidget->indexOf(tab), QApplication::translate("ToolsTabWidget", "EXE", 0, QApplication::UnicodeUTF8));
        pushButton_lineadd->setText(QApplication::translate("ToolsTabWidget", "\350\241\214\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        ToolsTabWidget->setTabText(ToolsTabWidget->indexOf(tab_code), QApplication::translate("ToolsTabWidget", "CODE", 0, QApplication::UnicodeUTF8));
        pushButton_calender->setText(QApplication::translate("ToolsTabWidget", "\346\227\245\345\216\206", 0, QApplication::UnicodeUTF8));
        ToolsTabWidget->setTabText(ToolsTabWidget->indexOf(tab_date), QApplication::translate("ToolsTabWidget", "\346\227\245\345\216\206", 0, QApplication::UnicodeUTF8));
        pushButton_connectssh->setText(QApplication::translate("ToolsTabWidget", "\350\277\236\346\216\245ssh", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ToolsTabWidget", "\346\211\247\350\241\214\345\221\275\344\273\244", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ToolsTabWidget", "\346\211\247\350\241\214\347\273\223\346\236\234", 0, QApplication::UnicodeUTF8));
        ToolsTabWidget->setTabText(ToolsTabWidget->indexOf(tab_3), QApplication::translate("ToolsTabWidget", "SSH", 0, QApplication::UnicodeUTF8));
        pushButton_opendir->setText(QApplication::translate("ToolsTabWidget", "...", 0, QApplication::UnicodeUTF8));
        pushButton_open->setText(QApplication::translate("ToolsTabWidget", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
        checkBox_autoopen->setText(QApplication::translate("ToolsTabWidget", "Auto", 0, QApplication::UnicodeUTF8));
        pushButton_addpath->setText(QApplication::translate("ToolsTabWidget", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        pushButton_delpath->setText(QApplication::translate("ToolsTabWidget", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        ToolsTabWidget->setTabText(ToolsTabWidget->indexOf(tab_2), QApplication::translate("ToolsTabWidget", "\346\211\223\345\274\200\350\267\257\345\276\204", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ToolsTabWidget: public Ui_ToolsTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLSTABWIDGET_H
