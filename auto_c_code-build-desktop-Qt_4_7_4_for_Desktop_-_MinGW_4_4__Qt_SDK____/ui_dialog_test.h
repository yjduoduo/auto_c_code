/********************************************************************************
** Form generated from reading UI file 'dialog_test.ui'
**
** Created: Sun May 18 07:58:54 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_TEST_H
#define UI_DIALOG_TEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Dialog_test
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Dialog_test)
    {
        if (Dialog_test->objectName().isEmpty())
            Dialog_test->setObjectName(QString::fromUtf8("Dialog_test"));
        Dialog_test->resize(400, 300);
        buttonBox = new QDialogButtonBox(Dialog_test);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(Dialog_test);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog_test, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog_test, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog_test);
    } // setupUi

    void retranslateUi(QDialog *Dialog_test)
    {
        Dialog_test->setWindowTitle(QApplication::translate("Dialog_test", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog_test: public Ui_Dialog_test {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_TEST_H
