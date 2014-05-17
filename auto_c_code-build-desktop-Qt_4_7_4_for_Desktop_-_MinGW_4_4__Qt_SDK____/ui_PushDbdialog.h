/********************************************************************************
** Form generated from reading UI file 'PushDbdialog.ui'
**
** Created: Sat May 17 16:52:01 2014
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
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *content_textEdit_dia;
    QComboBox *langtype_comboBox;
    QTextEdit *index_textEdit_dia;
    QTextEdit *note_textEdit_dia;
    QPushButton *ok_btn_dia;
    QPushButton *cancel_btn_dia;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(764, 309);
        horizontalLayoutWidget = new QWidget(Dialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(50, -10, 691, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        label_3 = new QLabel(horizontalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        label_4 = new QLabel(horizontalLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        horizontalLayoutWidget_2 = new QWidget(Dialog);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(20, 40, 691, 101));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        content_textEdit_dia = new QTextEdit(horizontalLayoutWidget_2);
        content_textEdit_dia->setObjectName(QString::fromUtf8("content_textEdit_dia"));

        horizontalLayout_2->addWidget(content_textEdit_dia);

        langtype_comboBox = new QComboBox(horizontalLayoutWidget_2);
        langtype_comboBox->setObjectName(QString::fromUtf8("langtype_comboBox"));

        horizontalLayout_2->addWidget(langtype_comboBox);

        index_textEdit_dia = new QTextEdit(horizontalLayoutWidget_2);
        index_textEdit_dia->setObjectName(QString::fromUtf8("index_textEdit_dia"));

        horizontalLayout_2->addWidget(index_textEdit_dia);

        note_textEdit_dia = new QTextEdit(horizontalLayoutWidget_2);
        note_textEdit_dia->setObjectName(QString::fromUtf8("note_textEdit_dia"));

        horizontalLayout_2->addWidget(note_textEdit_dia);

        ok_btn_dia = new QPushButton(Dialog);
        ok_btn_dia->setObjectName(QString::fromUtf8("ok_btn_dia"));
        ok_btn_dia->setGeometry(QRect(500, 250, 91, 41));
        cancel_btn_dia = new QPushButton(Dialog);
        cancel_btn_dia->setObjectName(QString::fromUtf8("cancel_btn_dia"));
        cancel_btn_dia->setGeometry(QRect(620, 250, 91, 41));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "\345\206\205\345\256\271", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "\350\257\255\350\250\200\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "\346\243\200\347\264\242", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "\345\244\207\346\263\250", 0, QApplication::UnicodeUTF8));
        ok_btn_dia->setText(QApplication::translate("Dialog", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        cancel_btn_dia->setText(QApplication::translate("Dialog", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUSHDBDIALOG_H
