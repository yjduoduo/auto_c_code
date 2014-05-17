/********************************************************************************
** Form generated from reading UI file 'autoccode.ui'
**
** Created: Sat May 17 11:48:06 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOCCODE_H
#define UI_AUTOCCODE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_autoCCode
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *get_listlabel;
    QComboBox *db_comboBox;
    QTextEdit *codeshow_textEdit;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *tocode_btn;
    QPushButton *save_btn;
    QPushButton *close_btn;
    QTextEdit *genshow_textEdit;

    void setupUi(QWidget *autoCCode)
    {
        if (autoCCode->objectName().isEmpty())
            autoCCode->setObjectName(QString::fromUtf8("autoCCode"));
        autoCCode->resize(792, 481);
        gridLayout = new QGridLayout(autoCCode);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        get_listlabel = new QLabel(autoCCode);
        get_listlabel->setObjectName(QString::fromUtf8("get_listlabel"));
        get_listlabel->setEnabled(true);

        horizontalLayout_2->addWidget(get_listlabel);

        db_comboBox = new QComboBox(autoCCode);
        db_comboBox->setObjectName(QString::fromUtf8("db_comboBox"));

        horizontalLayout_2->addWidget(db_comboBox);


        verticalLayout->addLayout(horizontalLayout_2);

        codeshow_textEdit = new QTextEdit(autoCCode);
        codeshow_textEdit->setObjectName(QString::fromUtf8("codeshow_textEdit"));

        verticalLayout->addWidget(codeshow_textEdit);


        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        tocode_btn = new QPushButton(autoCCode);
        tocode_btn->setObjectName(QString::fromUtf8("tocode_btn"));

        horizontalLayout_5->addWidget(tocode_btn);

        save_btn = new QPushButton(autoCCode);
        save_btn->setObjectName(QString::fromUtf8("save_btn"));

        horizontalLayout_5->addWidget(save_btn);

        close_btn = new QPushButton(autoCCode);
        close_btn->setObjectName(QString::fromUtf8("close_btn"));

        horizontalLayout_5->addWidget(close_btn);


        verticalLayout_2->addLayout(horizontalLayout_5);

        genshow_textEdit = new QTextEdit(autoCCode);
        genshow_textEdit->setObjectName(QString::fromUtf8("genshow_textEdit"));

        verticalLayout_2->addWidget(genshow_textEdit);


        horizontalLayout_4->addLayout(verticalLayout_2);


        horizontalLayout_3->addLayout(horizontalLayout_4);


        gridLayout->addLayout(horizontalLayout_3, 0, 0, 1, 1);


        retranslateUi(autoCCode);

        QMetaObject::connectSlotsByName(autoCCode);
    } // setupUi

    void retranslateUi(QWidget *autoCCode)
    {
        autoCCode->setWindowTitle(QApplication::translate("autoCCode", "autoCCode", 0, QApplication::UnicodeUTF8));
        get_listlabel->setText(QApplication::translate("autoCCode", "GetList", 0, QApplication::UnicodeUTF8));
        tocode_btn->setText(QApplication::translate("autoCCode", "To\344\273\243\347\240\201\345\272\223", 0, QApplication::UnicodeUTF8));
        save_btn->setText(QApplication::translate("autoCCode", "save", 0, QApplication::UnicodeUTF8));
        close_btn->setText(QApplication::translate("autoCCode", "close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class autoCCode: public Ui_autoCCode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOCCODE_H
