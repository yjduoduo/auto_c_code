/********************************************************************************
** Form generated from reading UI file 'dialog_select_database.ui'
**
** Created: Wed Jan 17 01:05:47 2018
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_SELECT_DATABASE_H
#define UI_DIALOG_SELECT_DATABASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Dialog_select_database
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *comboBox_selectdb;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBox_aspect;

    void setupUi(QDialog *Dialog_select_database)
    {
        if (Dialog_select_database->objectName().isEmpty())
            Dialog_select_database->setObjectName(QString::fromUtf8("Dialog_select_database"));
        Dialog_select_database->resize(307, 120);
        buttonBox = new QDialogButtonBox(Dialog_select_database);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(-80, 70, 371, 61));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        comboBox_selectdb = new QComboBox(Dialog_select_database);
        comboBox_selectdb->setObjectName(QString::fromUtf8("comboBox_selectdb"));
        comboBox_selectdb->setGeometry(QRect(30, 40, 261, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(26);
        comboBox_selectdb->setFont(font);
        comboBox_selectdb->setMaxVisibleItems(50);
        label = new QLabel(Dialog_select_database);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 301, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\271\274\345\234\206"));
        font1.setPointSize(18);
        label->setFont(font1);
        label_2 = new QLabel(Dialog_select_database);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 30, 151, 61));
        label_2->setFont(font1);
        comboBox_aspect = new QComboBox(Dialog_select_database);
        comboBox_aspect->setObjectName(QString::fromUtf8("comboBox_aspect"));
        comboBox_aspect->setGeometry(QRect(70, 40, 16, 31));
        comboBox_aspect->setFont(font);
        comboBox_aspect->raise();
        label_2->raise();
        buttonBox->raise();
        comboBox_selectdb->raise();
        label->raise();

        retranslateUi(Dialog_select_database);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog_select_database, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog_select_database, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog_select_database);
    } // setupUi

    void retranslateUi(QDialog *Dialog_select_database)
    {
        Dialog_select_database->setWindowTitle(QApplication::translate("Dialog_select_database", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog_select_database", "\350\257\267\351\200\211\346\213\251\345\257\271\345\272\224\347\232\204\346\225\260\346\215\256\345\272\223\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog_select_database: public Ui_Dialog_select_database {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_SELECT_DATABASE_H
