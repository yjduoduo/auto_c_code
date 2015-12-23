/********************************************************************************
** Form generated from reading UI file 'setup1.ui'
**
** Created: Wed Dec 23 23:19:01 2015
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUP1_H
#define UI_SETUP1_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SetUpDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton_foreColor;
    QPushButton *pushButton_backColor;
    QLabel *label_3;
    QPushButton *pushButton_font;
    QCheckBox *checkBox_rightTextSelectIndb;
    QCheckBox *checkBox_rtQuery;
    QCheckBox *checkBox_showpath;
    QCheckBox *checkBox_email;

    void setupUi(QDialog *SetUpDialog)
    {
        if (SetUpDialog->objectName().isEmpty())
            SetUpDialog->setObjectName(QString::fromUtf8("SetUpDialog"));
        SetUpDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(SetUpDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(SetUpDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 41, 31));
        label_2 = new QLabel(SetUpDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 70, 41, 31));
        pushButton_foreColor = new QPushButton(SetUpDialog);
        pushButton_foreColor->setObjectName(QString::fromUtf8("pushButton_foreColor"));
        pushButton_foreColor->setGeometry(QRect(60, 36, 75, 23));
        pushButton_foreColor->setAutoFillBackground(true);
        pushButton_foreColor->setDefault(true);
        pushButton_foreColor->setFlat(true);
        pushButton_backColor = new QPushButton(SetUpDialog);
        pushButton_backColor->setObjectName(QString::fromUtf8("pushButton_backColor"));
        pushButton_backColor->setGeometry(QRect(60, 75, 75, 23));
        pushButton_backColor->setAutoFillBackground(true);
        pushButton_backColor->setDefault(true);
        pushButton_backColor->setFlat(true);
        label_3 = new QLabel(SetUpDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 110, 41, 31));
        pushButton_font = new QPushButton(SetUpDialog);
        pushButton_font->setObjectName(QString::fromUtf8("pushButton_font"));
        pushButton_font->setGeometry(QRect(60, 112, 75, 23));
        pushButton_font->setAutoFillBackground(true);
        pushButton_font->setDefault(true);
        pushButton_font->setFlat(true);
        checkBox_rightTextSelectIndb = new QCheckBox(SetUpDialog);
        checkBox_rightTextSelectIndb->setObjectName(QString::fromUtf8("checkBox_rightTextSelectIndb"));
        checkBox_rightTextSelectIndb->setGeometry(QRect(60, 150, 72, 17));
        checkBox_rightTextSelectIndb->setChecked(true);
        checkBox_rtQuery = new QCheckBox(SetUpDialog);
        checkBox_rtQuery->setObjectName(QString::fromUtf8("checkBox_rtQuery"));
        checkBox_rtQuery->setGeometry(QRect(60, 170, 72, 17));
        checkBox_rtQuery->setChecked(true);
        checkBox_showpath = new QCheckBox(SetUpDialog);
        checkBox_showpath->setObjectName(QString::fromUtf8("checkBox_showpath"));
        checkBox_showpath->setGeometry(QRect(60, 191, 91, 17));
        checkBox_showpath->setChecked(false);
        checkBox_email = new QCheckBox(SetUpDialog);
        checkBox_email->setObjectName(QString::fromUtf8("checkBox_email"));
        checkBox_email->setGeometry(QRect(60, 210, 91, 17));
        checkBox_email->setChecked(false);

        retranslateUi(SetUpDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SetUpDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SetUpDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SetUpDialog);
    } // setupUi

    void retranslateUi(QDialog *SetUpDialog)
    {
        SetUpDialog->setWindowTitle(QApplication::translate("SetUpDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SetUpDialog", "\345\211\215\346\231\257\350\211\262", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SetUpDialog", "\350\203\214\346\231\257\350\211\262", 0, QApplication::UnicodeUTF8));
        pushButton_foreColor->setText(QString());
        pushButton_backColor->setText(QString());
        label_3->setText(QApplication::translate("SetUpDialog", "\345\255\227  \344\275\223", 0, QApplication::UnicodeUTF8));
        pushButton_font->setText(QString());
#ifndef QT_NO_TOOLTIP
        checkBox_rightTextSelectIndb->setToolTip(QApplication::translate("SetUpDialog", "\345\234\250\345\217\263\344\276\247\347\225\214\351\235\242\351\200\211\344\270\255\345\205\263\351\224\256\345\255\227\345\220\216\357\274\214\347\202\271\345\207\273\345\215\263\345\217\257\345\205\245\345\272\223", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_rightTextSelectIndb->setText(QApplication::translate("SetUpDialog", "\345\217\263\351\200\211\345\205\245\345\272\223", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_rtQuery->setToolTip(QApplication::translate("SetUpDialog", "\345\234\250\346\220\234\347\264\242\346\240\217\346\224\257\346\214\201\345\256\236\346\227\266\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_rtQuery->setText(QApplication::translate("SetUpDialog", "\345\256\236\346\227\266\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_showpath->setToolTip(QApplication::translate("SetUpDialog", "\347\224\237\346\210\220\344\273\243\347\240\201\345\272\223\346\267\273\345\212\240\346\226\207\344\273\266\346\227\266\345\206\205\345\256\271\346\234\253\345\260\276\346\234\211\346\226\207\344\273\266\350\267\257\345\276\204\346\230\276\347\244\272", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_showpath->setText(QApplication::translate("SetUpDialog", "\346\230\276\347\244\272\346\226\207\344\273\266\350\267\257\345\276\204", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_email->setToolTip(QApplication::translate("SetUpDialog", "\347\224\237\346\210\220\344\273\243\347\240\201\345\272\223\346\267\273\345\212\240\346\226\207\344\273\266\346\227\266\345\206\205\345\256\271\346\234\253\345\260\276\346\234\211\346\226\207\344\273\266\350\267\257\345\276\204\346\230\276\347\244\272", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_email->setText(QApplication::translate("SetUpDialog", "@Email", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SetUpDialog: public Ui_SetUpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUP1_H
