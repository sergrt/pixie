/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Tue 26. Jun 11:16:15 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include "hled.h"

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *recvEdit;
    QPlainTextEdit *sendEdit;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *portBox;
    QLabel *label_2;
    QComboBox *baudRateBox;
    QLabel *label_3;
    QComboBox *dataBitsBox;
    QLabel *label_4;
    QComboBox *parityBox;
    QLabel *label_5;
    QComboBox *stopBitsBox;
    QLabel *label_6;
    QComboBox *queryModeBox;
    QLabel *label_7;
    QSpinBox *timeoutBox;
    QHBoxLayout *horizontalLayout;
    HLed *led;
    QPushButton *openCloseButton;
    QSpacerItem *verticalSpacer;
    QPushButton *sendButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(604, 485);
        horizontalLayout_2 = new QHBoxLayout(Dialog);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        recvEdit = new QPlainTextEdit(Dialog);
        recvEdit->setObjectName(QString::fromUtf8("recvEdit"));
        recvEdit->setMaximumBlockCount(800);

        verticalLayout->addWidget(recvEdit);

        sendEdit = new QPlainTextEdit(Dialog);
        sendEdit->setObjectName(QString::fromUtf8("sendEdit"));

        verticalLayout->addWidget(sendEdit);

        verticalLayout->setStretch(0, 3);
        verticalLayout->setStretch(1, 1);

        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        portBox = new QComboBox(Dialog);
        portBox->setObjectName(QString::fromUtf8("portBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, portBox);

        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        baudRateBox = new QComboBox(Dialog);
        baudRateBox->setObjectName(QString::fromUtf8("baudRateBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, baudRateBox);

        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        dataBitsBox = new QComboBox(Dialog);
        dataBitsBox->setObjectName(QString::fromUtf8("dataBitsBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, dataBitsBox);

        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        parityBox = new QComboBox(Dialog);
        parityBox->setObjectName(QString::fromUtf8("parityBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, parityBox);

        label_5 = new QLabel(Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        stopBitsBox = new QComboBox(Dialog);
        stopBitsBox->setObjectName(QString::fromUtf8("stopBitsBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, stopBitsBox);

        label_6 = new QLabel(Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_6);

        queryModeBox = new QComboBox(Dialog);
        queryModeBox->setObjectName(QString::fromUtf8("queryModeBox"));

        formLayout->setWidget(6, QFormLayout::FieldRole, queryModeBox);

        label_7 = new QLabel(Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_7);

        timeoutBox = new QSpinBox(Dialog);
        timeoutBox->setObjectName(QString::fromUtf8("timeoutBox"));
        timeoutBox->setMinimum(-1);
        timeoutBox->setMaximum(10000);
        timeoutBox->setSingleStep(10);
        timeoutBox->setValue(10);

        formLayout->setWidget(5, QFormLayout::FieldRole, timeoutBox);


        verticalLayout_2->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        led = new HLed(Dialog);
        led->setObjectName(QString::fromUtf8("led"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(led->sizePolicy().hasHeightForWidth());
        led->setSizePolicy(sizePolicy);
        led->setMinimumSize(QSize(20, 20));
        led->setMaximumSize(QSize(25, 25));

        horizontalLayout->addWidget(led);

        openCloseButton = new QPushButton(Dialog);
        openCloseButton->setObjectName(QString::fromUtf8("openCloseButton"));

        horizontalLayout->addWidget(openCloseButton);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        sendButton = new QPushButton(Dialog);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));

        verticalLayout_2->addWidget(sendButton);


        horizontalLayout_2->addLayout(verticalLayout_2);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "Port:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "BaudRate:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "DataBits:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "Parity:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Dialog", "StopBits:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Dialog", "QueryMode:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Dialog", "Timeout:", 0, QApplication::UnicodeUTF8));
        timeoutBox->setSuffix(QApplication::translate("Dialog", " ms", 0, QApplication::UnicodeUTF8));
        openCloseButton->setText(QApplication::translate("Dialog", "Open/Close", 0, QApplication::UnicodeUTF8));
        sendButton->setText(QApplication::translate("Dialog", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
