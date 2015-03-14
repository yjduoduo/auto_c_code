/********************************************************************************
** Form generated from reading UI file 'autoindb.ui'
**
** Created: Sun Nov 30 00:31:09 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOINDB_H
#define UI_AUTOINDB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AutoIndb
{
public:
    QPushButton *pushBtn_Open;
    QTextEdit *textEdit_show;
    QCheckBox *checkBox_nullornot;
    QSpinBox *spinBox_notenumber;
    QLabel *label;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushBtn_process;
    QProgressBar *progressBar;

    void setupUi(QWidget *AutoIndb)
    {
        if (AutoIndb->objectName().isEmpty())
            AutoIndb->setObjectName(QString::fromUtf8("AutoIndb"));
        AutoIndb->setWindowModality(Qt::WindowModal);
        AutoIndb->resize(754, 497);
        pushBtn_Open = new QPushButton(AutoIndb);
        pushBtn_Open->setObjectName(QString::fromUtf8("pushBtn_Open"));
        pushBtn_Open->setGeometry(QRect(20, 70, 91, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(18);
        pushBtn_Open->setFont(font);
        textEdit_show = new QTextEdit(AutoIndb);
        textEdit_show->setObjectName(QString::fromUtf8("textEdit_show"));
        textEdit_show->setGeometry(QRect(120, 30, 621, 451));
        checkBox_nullornot = new QCheckBox(AutoIndb);
        checkBox_nullornot->setObjectName(QString::fromUtf8("checkBox_nullornot"));
        checkBox_nullornot->setGeometry(QRect(20, 270, 71, 16));
        spinBox_notenumber = new QSpinBox(AutoIndb);
        spinBox_notenumber->setObjectName(QString::fromUtf8("spinBox_notenumber"));
        spinBox_notenumber->setGeometry(QRect(20, 330, 71, 21));
        spinBox_notenumber->setMaximum(200);
        spinBox_notenumber->setValue(50);
        label = new QLabel(AutoIndb);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 300, 91, 21));
        layoutWidget = new QWidget(AutoIndb);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 180, 89, 63));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushBtn_process = new QPushButton(layoutWidget);
        pushBtn_process->setObjectName(QString::fromUtf8("pushBtn_process"));
        pushBtn_process->setFont(font);

        verticalLayout->addWidget(pushBtn_process);

        progressBar = new QProgressBar(layoutWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);


        retranslateUi(AutoIndb);

        QMetaObject::connectSlotsByName(AutoIndb);
    } // setupUi

    void retranslateUi(QWidget *AutoIndb)
    {
        AutoIndb->setWindowTitle(QApplication::translate("AutoIndb", "Form", 0, QApplication::UnicodeUTF8));
        pushBtn_Open->setText(QApplication::translate("AutoIndb", "\346\211\223\345\274\200 ", 0, QApplication::UnicodeUTF8));
        checkBox_nullornot->setText(QApplication::translate("AutoIndb", "\346\263\250\351\207\212\347\251\272", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AutoIndb", "\346\263\250\351\207\212\345\211\215\350\213\245\345\271\262\345\255\227\347\254\246", 0, QApplication::UnicodeUTF8));
        pushBtn_process->setText(QApplication::translate("AutoIndb", "\345\244\204\347\220\206", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AutoIndb: public Ui_AutoIndb {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOINDB_H
