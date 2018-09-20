/********************************************************************************
** Form generated from reading UI file 'codesophia.ui'
**
** Created: Thu Sep 20 21:43:22 2018
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CODESOPHIA_H
#define UI_CODESOPHIA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CodeSophia
{
public:
    QAction *action_declare;
    QAction *action_fucntion;
    QAction *action_define;
    QAction *action_struct;
    QAction *action_structprint;
    QAction *actionCPP;
    QAction *actionJAVA;
    QAction *actionSHELL;
    QAction *action;
    QAction *action_2;
    QAction *action_3;
    QAction *action_6;
    QAction *action_7;
    QAction *action_save;
    QAction *actionC_2;
    QAction *action_header;
    QAction *action_ifcondition;
    QAction *action_loop;
    QAction *action_note;
    QAction *action_common_print;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QTextEdit *textEdit_key;
    QTextEdit *textEdit_result;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_leftclear;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton_rightclear;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButton_gen;
    QPushButton *pushButton_load;
    QCheckBox *checkBox_showFunc;
    QCheckBox *checkBox_showAllText;
    QPushButton *pushButton_paster2left;
    QPushButton *pushButton_fetchright;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox_keytips;
    QHBoxLayout *horizontalLayout;
    QLabel *label_print;
    QLineEdit *lineEdit_print;
    QComboBox *comboBox_print;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_dataprint;
    QLineEdit *lineEdit_dataprint;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CodeSophia)
    {
        if (CodeSophia->objectName().isEmpty())
            CodeSophia->setObjectName(QString::fromUtf8("CodeSophia"));
        CodeSophia->resize(800, 600);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        CodeSophia->setFont(font);
        action_declare = new QAction(CodeSophia);
        action_declare->setObjectName(QString::fromUtf8("action_declare"));
        action_fucntion = new QAction(CodeSophia);
        action_fucntion->setObjectName(QString::fromUtf8("action_fucntion"));
        action_define = new QAction(CodeSophia);
        action_define->setObjectName(QString::fromUtf8("action_define"));
        action_struct = new QAction(CodeSophia);
        action_struct->setObjectName(QString::fromUtf8("action_struct"));
        action_structprint = new QAction(CodeSophia);
        action_structprint->setObjectName(QString::fromUtf8("action_structprint"));
        actionCPP = new QAction(CodeSophia);
        actionCPP->setObjectName(QString::fromUtf8("actionCPP"));
        actionJAVA = new QAction(CodeSophia);
        actionJAVA->setObjectName(QString::fromUtf8("actionJAVA"));
        actionSHELL = new QAction(CodeSophia);
        actionSHELL->setObjectName(QString::fromUtf8("actionSHELL"));
        action = new QAction(CodeSophia);
        action->setObjectName(QString::fromUtf8("action"));
        action_2 = new QAction(CodeSophia);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        action_3 = new QAction(CodeSophia);
        action_3->setObjectName(QString::fromUtf8("action_3"));
        action_6 = new QAction(CodeSophia);
        action_6->setObjectName(QString::fromUtf8("action_6"));
        action_7 = new QAction(CodeSophia);
        action_7->setObjectName(QString::fromUtf8("action_7"));
        action_save = new QAction(CodeSophia);
        action_save->setObjectName(QString::fromUtf8("action_save"));
        actionC_2 = new QAction(CodeSophia);
        actionC_2->setObjectName(QString::fromUtf8("actionC_2"));
        action_header = new QAction(CodeSophia);
        action_header->setObjectName(QString::fromUtf8("action_header"));
        action_ifcondition = new QAction(CodeSophia);
        action_ifcondition->setObjectName(QString::fromUtf8("action_ifcondition"));
        action_loop = new QAction(CodeSophia);
        action_loop->setObjectName(QString::fromUtf8("action_loop"));
        action_note = new QAction(CodeSophia);
        action_note->setObjectName(QString::fromUtf8("action_note"));
        action_common_print = new QAction(CodeSophia);
        action_common_print->setObjectName(QString::fromUtf8("action_common_print"));
        centralwidget = new QWidget(CodeSophia);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        textEdit_key = new QTextEdit(frame);
        textEdit_key->setObjectName(QString::fromUtf8("textEdit_key"));

        gridLayout_2->addWidget(textEdit_key, 1, 0, 1, 1);

        textEdit_result = new QTextEdit(frame);
        textEdit_result->setObjectName(QString::fromUtf8("textEdit_result"));

        gridLayout_2->addWidget(textEdit_result, 1, 4, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton_leftclear = new QPushButton(frame);
        pushButton_leftclear->setObjectName(QString::fromUtf8("pushButton_leftclear"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_leftclear->sizePolicy().hasHeightForWidth());
        pushButton_leftclear->setSizePolicy(sizePolicy);
        pushButton_leftclear->setMinimumSize(QSize(50, 0));

        verticalLayout->addWidget(pushButton_leftclear);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        pushButton_rightclear = new QPushButton(frame);
        pushButton_rightclear->setObjectName(QString::fromUtf8("pushButton_rightclear"));
        sizePolicy.setHeightForWidth(pushButton_rightclear->sizePolicy().hasHeightForWidth());
        pushButton_rightclear->setSizePolicy(sizePolicy);
        pushButton_rightclear->setMinimumSize(QSize(50, 0));

        verticalLayout->addWidget(pushButton_rightclear);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        pushButton_gen = new QPushButton(frame);
        pushButton_gen->setObjectName(QString::fromUtf8("pushButton_gen"));
        sizePolicy.setHeightForWidth(pushButton_gen->sizePolicy().hasHeightForWidth());
        pushButton_gen->setSizePolicy(sizePolicy);
        pushButton_gen->setMinimumSize(QSize(50, 0));

        verticalLayout->addWidget(pushButton_gen);

        pushButton_load = new QPushButton(frame);
        pushButton_load->setObjectName(QString::fromUtf8("pushButton_load"));
        sizePolicy.setHeightForWidth(pushButton_load->sizePolicy().hasHeightForWidth());
        pushButton_load->setSizePolicy(sizePolicy);
        pushButton_load->setMinimumSize(QSize(50, 0));

        verticalLayout->addWidget(pushButton_load);

        checkBox_showFunc = new QCheckBox(frame);
        checkBox_showFunc->setObjectName(QString::fromUtf8("checkBox_showFunc"));

        verticalLayout->addWidget(checkBox_showFunc);

        checkBox_showAllText = new QCheckBox(frame);
        checkBox_showAllText->setObjectName(QString::fromUtf8("checkBox_showAllText"));

        verticalLayout->addWidget(checkBox_showAllText);

        pushButton_paster2left = new QPushButton(frame);
        pushButton_paster2left->setObjectName(QString::fromUtf8("pushButton_paster2left"));
        sizePolicy.setHeightForWidth(pushButton_paster2left->sizePolicy().hasHeightForWidth());
        pushButton_paster2left->setSizePolicy(sizePolicy);
        pushButton_paster2left->setMinimumSize(QSize(50, 0));

        verticalLayout->addWidget(pushButton_paster2left);

        pushButton_fetchright = new QPushButton(frame);
        pushButton_fetchright->setObjectName(QString::fromUtf8("pushButton_fetchright"));
        sizePolicy.setHeightForWidth(pushButton_fetchright->sizePolicy().hasHeightForWidth());
        pushButton_fetchright->setSizePolicy(sizePolicy);
        pushButton_fetchright->setMinimumSize(QSize(50, 0));

        verticalLayout->addWidget(pushButton_fetchright);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalSpacer = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);


        gridLayout_2->addLayout(verticalLayout, 1, 1, 1, 1);

        comboBox_keytips = new QComboBox(frame);
        comboBox_keytips->setObjectName(QString::fromUtf8("comboBox_keytips"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font1.setPointSize(14);
        font1.setBold(false);
        font1.setWeight(50);
        comboBox_keytips->setFont(font1);
        comboBox_keytips->setMaxVisibleItems(50);

        gridLayout_2->addWidget(comboBox_keytips, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_print = new QLabel(frame);
        label_print->setObjectName(QString::fromUtf8("label_print"));

        horizontalLayout->addWidget(label_print);

        lineEdit_print = new QLineEdit(frame);
        lineEdit_print->setObjectName(QString::fromUtf8("lineEdit_print"));

        horizontalLayout->addWidget(lineEdit_print);

        comboBox_print = new QComboBox(frame);
        comboBox_print->setObjectName(QString::fromUtf8("comboBox_print"));
        comboBox_print->setEditable(true);

        horizontalLayout->addWidget(comboBox_print);

        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_dataprint = new QLabel(frame);
        label_dataprint->setObjectName(QString::fromUtf8("label_dataprint"));

        horizontalLayout->addWidget(label_dataprint);

        lineEdit_dataprint = new QLineEdit(frame);
        lineEdit_dataprint->setObjectName(QString::fromUtf8("lineEdit_dataprint"));

        horizontalLayout->addWidget(lineEdit_dataprint);


        gridLayout_2->addLayout(horizontalLayout, 0, 4, 1, 1);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        CodeSophia->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CodeSophia);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 19));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        CodeSophia->setMenuBar(menubar);
        statusbar = new QStatusBar(CodeSophia);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CodeSophia->setStatusBar(statusbar);

        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addAction(action_header);
        menu->addAction(action_declare);
        menu->addAction(action_fucntion);
        menu->addAction(action_define);
        menu->addAction(action_struct);
        menu->addAction(action_structprint);
        menu->addAction(action_ifcondition);
        menu->addAction(action_loop);
        menu->addAction(action_note);
        menu->addSeparator();
        menu->addAction(action_common_print);
        menu_2->addAction(actionC_2);
        menu_2->addAction(actionCPP);
        menu_2->addAction(actionJAVA);
        menu_2->addAction(actionSHELL);
        menu_3->addAction(action_save);

        retranslateUi(CodeSophia);

        QMetaObject::connectSlotsByName(CodeSophia);
    } // setupUi

    void retranslateUi(QMainWindow *CodeSophia)
    {
        CodeSophia->setWindowTitle(QApplication::translate("CodeSophia", "CodeSophia", 0, QApplication::UnicodeUTF8));
        action_declare->setText(QApplication::translate("CodeSophia", "\345\243\260\346\230\216", 0, QApplication::UnicodeUTF8));
        action_fucntion->setText(QApplication::translate("CodeSophia", "\345\207\275\346\225\260", 0, QApplication::UnicodeUTF8));
        action_define->setText(QApplication::translate("CodeSophia", "\345\256\217", 0, QApplication::UnicodeUTF8));
        action_struct->setText(QApplication::translate("CodeSophia", "\347\273\223\346\236\204\344\275\223", 0, QApplication::UnicodeUTF8));
        action_structprint->setText(QApplication::translate("CodeSophia", "\347\273\223\346\236\204\344\275\223\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
        actionCPP->setText(QApplication::translate("CodeSophia", "C++", 0, QApplication::UnicodeUTF8));
        actionJAVA->setText(QApplication::translate("CodeSophia", "JAVA", 0, QApplication::UnicodeUTF8));
        actionSHELL->setText(QApplication::translate("CodeSophia", "SHELL", 0, QApplication::UnicodeUTF8));
        action->setText(QApplication::translate("CodeSophia", "\345\244\264\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        action_2->setText(QApplication::translate("CodeSophia", "\346\263\250\351\207\212", 0, QApplication::UnicodeUTF8));
        action_3->setText(QApplication::translate("CodeSophia", "\345\243\260\346\230\216", 0, QApplication::UnicodeUTF8));
        action_6->setText(QApplication::translate("CodeSophia", "\345\256\217", 0, QApplication::UnicodeUTF8));
        action_7->setText(QApplication::translate("CodeSophia", "\345\207\275\346\225\260", 0, QApplication::UnicodeUTF8));
        action_save->setText(QApplication::translate("CodeSophia", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
        actionC_2->setText(QApplication::translate("CodeSophia", "C", 0, QApplication::UnicodeUTF8));
        action_header->setText(QApplication::translate("CodeSophia", "\345\244\264\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        action_ifcondition->setText(QApplication::translate("CodeSophia", "\346\235\241\344\273\266", 0, QApplication::UnicodeUTF8));
        action_loop->setText(QApplication::translate("CodeSophia", "\345\276\252\347\216\257", 0, QApplication::UnicodeUTF8));
        action_note->setText(QApplication::translate("CodeSophia", "\346\263\250\351\207\212", 0, QApplication::UnicodeUTF8));
        action_common_print->setText(QApplication::translate("CodeSophia", "\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
        pushButton_leftclear->setText(QApplication::translate("CodeSophia", "\345\267\246\346\270\205\347\251\272", 0, QApplication::UnicodeUTF8));
        pushButton_rightclear->setText(QApplication::translate("CodeSophia", "\345\217\263\346\270\205\347\251\272", 0, QApplication::UnicodeUTF8));
        pushButton_gen->setText(QApplication::translate("CodeSophia", "\347\224\237\346\210\220", 0, QApplication::UnicodeUTF8));
        pushButton_load->setText(QApplication::translate("CodeSophia", "\345\212\240\350\275\275\347\273\223\346\236\234", 0, QApplication::UnicodeUTF8));
        checkBox_showFunc->setText(QApplication::translate("CodeSophia", "\345\207\275\346\225\260\345\220\215", 0, QApplication::UnicodeUTF8));
        checkBox_showAllText->setText(QApplication::translate("CodeSophia", "\345\205\250\346\226\207\346\234\254", 0, QApplication::UnicodeUTF8));
        pushButton_paster2left->setText(QApplication::translate("CodeSophia", "\350\264\264\345\267\246<-", 0, QApplication::UnicodeUTF8));
        pushButton_fetchright->setText(QApplication::translate("CodeSophia", "\345\217\226\345\217\263->", 0, QApplication::UnicodeUTF8));
        label_print->setText(QApplication::translate("CodeSophia", "\346\211\223\345\215\260\345\207\275\346\225\260", 0, QApplication::UnicodeUTF8));
        label_dataprint->setText(QApplication::translate("CodeSophia", "\346\211\223\345\215\260\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("CodeSophia", "\345\212\237\350\203\275", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("CodeSophia", "\350\257\255\350\250\200", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("CodeSophia", "\347\273\223\346\236\234", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CodeSophia: public Ui_CodeSophia {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CODESOPHIA_H
