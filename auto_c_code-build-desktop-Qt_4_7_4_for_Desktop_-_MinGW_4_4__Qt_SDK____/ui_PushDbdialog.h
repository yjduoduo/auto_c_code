/********************************************************************************
** Form generated from reading UI file 'PushDbdialog.ui'
**
** Created: Sat Mar 12 23:27:21 2016
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUSHDBDIALOG_H
#define UI_PUSHDBDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QPushButton *pushButton_content_paste;
    QPushButton *pushButton_content_clear;
    QPushButton *pushButton_minsize;
    QPushButton *pushButton_widthsize;
    QPushButton *pushButton_maxsize;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QPushButton *pushButton_note;
    QPushButton *pushButton_note_clear;
    QCheckBox *checkBox_EOR;
    QCheckBox *checkBox_SEL;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *content_textEdit_dia;
    QVBoxLayout *verticalLayout;
    QComboBox *langtype_comboBox;
    QLabel *label_5;
    QComboBox *comboBox_vartype;
    QLabel *label_6;
    QComboBox *comboBox_aspect;
    QCheckBox *checkBox_AutoGet_Con;
    QSpacerItem *verticalSpacer_2;
    QTextEdit *index_textEdit_dia;
    QTextEdit *note_textEdit_dia;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *lineEdit_add_aspect;
    QPushButton *pushButton_add;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *ok_btn_dia;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel_btn_dia;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->setWindowModality(Qt::NonModal);
        Dialog->setEnabled(true);
        Dialog->resize(815, 426);
        Dialog->setWindowOpacity(0.8);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        pushButton_content_paste = new QPushButton(Dialog);
        pushButton_content_paste->setObjectName(QString::fromUtf8("pushButton_content_paste"));
        pushButton_content_paste->setMinimumSize(QSize(71, 31));

        horizontalLayout_3->addWidget(pushButton_content_paste);

        pushButton_content_clear = new QPushButton(Dialog);
        pushButton_content_clear->setObjectName(QString::fromUtf8("pushButton_content_clear"));
        pushButton_content_clear->setMinimumSize(QSize(71, 31));

        horizontalLayout_3->addWidget(pushButton_content_clear);

        pushButton_minsize = new QPushButton(Dialog);
        pushButton_minsize->setObjectName(QString::fromUtf8("pushButton_minsize"));
        pushButton_minsize->setMinimumSize(QSize(71, 31));

        horizontalLayout_3->addWidget(pushButton_minsize);

        pushButton_widthsize = new QPushButton(Dialog);
        pushButton_widthsize->setObjectName(QString::fromUtf8("pushButton_widthsize"));
        pushButton_widthsize->setMinimumSize(QSize(71, 31));

        horizontalLayout_3->addWidget(pushButton_widthsize);

        pushButton_maxsize = new QPushButton(Dialog);
        pushButton_maxsize->setObjectName(QString::fromUtf8("pushButton_maxsize"));
        pushButton_maxsize->setMinimumSize(QSize(71, 31));

        horizontalLayout_3->addWidget(pushButton_maxsize);

        horizontalSpacer_3 = new QSpacerItem(178, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        horizontalSpacer_4 = new QSpacerItem(48, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        horizontalSpacer_5 = new QSpacerItem(168, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        pushButton_note = new QPushButton(Dialog);
        pushButton_note->setObjectName(QString::fromUtf8("pushButton_note"));
        pushButton_note->setMinimumSize(QSize(71, 31));

        horizontalLayout->addWidget(pushButton_note);

        pushButton_note_clear = new QPushButton(Dialog);
        pushButton_note_clear->setObjectName(QString::fromUtf8("pushButton_note_clear"));
        pushButton_note_clear->setMinimumSize(QSize(71, 31));

        horizontalLayout->addWidget(pushButton_note_clear);

        checkBox_EOR = new QCheckBox(Dialog);
        checkBox_EOR->setObjectName(QString::fromUtf8("checkBox_EOR"));
        checkBox_EOR->setChecked(true);

        horizontalLayout->addWidget(checkBox_EOR);

        checkBox_SEL = new QCheckBox(Dialog);
        checkBox_SEL->setObjectName(QString::fromUtf8("checkBox_SEL"));
        checkBox_SEL->setEnabled(true);
        checkBox_SEL->setChecked(false);

        horizontalLayout->addWidget(checkBox_SEL);


        horizontalLayout_3->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        content_textEdit_dia = new QTextEdit(Dialog);
        content_textEdit_dia->setObjectName(QString::fromUtf8("content_textEdit_dia"));

        horizontalLayout_2->addWidget(content_textEdit_dia);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        langtype_comboBox = new QComboBox(Dialog);
        langtype_comboBox->setObjectName(QString::fromUtf8("langtype_comboBox"));

        verticalLayout->addWidget(langtype_comboBox);

        label_5 = new QLabel(Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout->addWidget(label_5);

        comboBox_vartype = new QComboBox(Dialog);
        comboBox_vartype->setObjectName(QString::fromUtf8("comboBox_vartype"));

        verticalLayout->addWidget(comboBox_vartype);

        label_6 = new QLabel(Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout->addWidget(label_6);

        comboBox_aspect = new QComboBox(Dialog);
        comboBox_aspect->setObjectName(QString::fromUtf8("comboBox_aspect"));

        verticalLayout->addWidget(comboBox_aspect);

        checkBox_AutoGet_Con = new QCheckBox(Dialog);
        checkBox_AutoGet_Con->setObjectName(QString::fromUtf8("checkBox_AutoGet_Con"));
        checkBox_AutoGet_Con->setChecked(true);

        verticalLayout->addWidget(checkBox_AutoGet_Con);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(verticalLayout);

        index_textEdit_dia = new QTextEdit(Dialog);
        index_textEdit_dia->setObjectName(QString::fromUtf8("index_textEdit_dia"));

        horizontalLayout_2->addWidget(index_textEdit_dia);

        note_textEdit_dia = new QTextEdit(Dialog);
        note_textEdit_dia->setObjectName(QString::fromUtf8("note_textEdit_dia"));

        horizontalLayout_2->addWidget(note_textEdit_dia);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lineEdit_add_aspect = new QLineEdit(Dialog);
        lineEdit_add_aspect->setObjectName(QString::fromUtf8("lineEdit_add_aspect"));
        lineEdit_add_aspect->setMinimumSize(QSize(141, 41));
        lineEdit_add_aspect->setMaximumSize(QSize(141, 41));

        horizontalLayout_5->addWidget(lineEdit_add_aspect);

        pushButton_add = new QPushButton(Dialog);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        pushButton_add->setMinimumSize(QSize(81, 41));
        pushButton_add->setMaximumSize(QSize(81, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font.setPointSize(14);
        pushButton_add->setFont(font);

        horizontalLayout_5->addWidget(pushButton_add);


        horizontalLayout_6->addLayout(horizontalLayout_5);

        horizontalSpacer_2 = new QSpacerItem(118, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        ok_btn_dia = new QPushButton(Dialog);
        ok_btn_dia->setObjectName(QString::fromUtf8("ok_btn_dia"));
        ok_btn_dia->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ok_btn_dia->sizePolicy().hasHeightForWidth());
        ok_btn_dia->setSizePolicy(sizePolicy);
        ok_btn_dia->setMinimumSize(QSize(10, 60));

        horizontalLayout_4->addWidget(ok_btn_dia);

        horizontalSpacer = new QSpacerItem(48, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        cancel_btn_dia = new QPushButton(Dialog);
        cancel_btn_dia->setObjectName(QString::fromUtf8("cancel_btn_dia"));
        cancel_btn_dia->setMinimumSize(QSize(111, 41));

        horizontalLayout_4->addWidget(cancel_btn_dia);


        horizontalLayout_6->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(horizontalLayout_6);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "\345\205\245\345\272\223", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "\345\206\205\345\256\271", 0, QApplication::UnicodeUTF8));
        pushButton_content_paste->setText(QApplication::translate("Dialog", "Paste", 0, QApplication::UnicodeUTF8));
        pushButton_content_clear->setText(QApplication::translate("Dialog", "clear", 0, QApplication::UnicodeUTF8));
        pushButton_minsize->setText(QApplication::translate("Dialog", "min size", 0, QApplication::UnicodeUTF8));
        pushButton_widthsize->setText(QApplication::translate("Dialog", "width", 0, QApplication::UnicodeUTF8));
        pushButton_maxsize->setText(QApplication::translate("Dialog", "max size", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "\350\257\255\350\250\200\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "\346\243\200\347\264\242", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "\345\244\207\346\263\250", 0, QApplication::UnicodeUTF8));
        pushButton_note->setText(QApplication::translate("Dialog", "Paste", 0, QApplication::UnicodeUTF8));
        pushButton_note_clear->setText(QApplication::translate("Dialog", "clear", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_EOR->setToolTip(QApplication::translate("Dialog", "\345\205\245\345\272\223\346\230\257\345\220\246\346\270\205\347\251\272\357\274\214\351\200\211\344\270\255\346\270\205\347\251\272", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_EOR->setText(QApplication::translate("Dialog", "EOR", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_SEL->setToolTip(QApplication::translate("Dialog", "\351\200\211\344\270\255\357\274\214\344\273\216\345\267\246\350\276\271\351\200\211\346\213\251\345\206\205\345\256\271\350\207\252\345\212\250\346\267\273\345\212\240\345\210\260\345\217\263\350\276\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_SEL->setText(QApplication::translate("Dialog", "SEL", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Dialog", "\345\217\230\351\207\217\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Dialog", "\350\214\203\347\225\264", 0, QApplication::UnicodeUTF8));
        checkBox_AutoGet_Con->setText(QApplication::translate("Dialog", "\350\207\252\345\212\250\350\216\267\345\217\226", 0, QApplication::UnicodeUTF8));
        pushButton_add->setText(QApplication::translate("Dialog", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        ok_btn_dia->setText(QApplication::translate("Dialog", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        cancel_btn_dia->setText(QApplication::translate("Dialog", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUSHDBDIALOG_H
