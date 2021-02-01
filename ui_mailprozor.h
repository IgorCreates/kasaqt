/*
* This file is part of KasaQT, an open-source PointOfSale
*
* (C) Igor Jukic & Ante Biocic 2012-2016
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: Igor Jukic <igor.jukic.76@gmail.com>
*/




/********************************************************************************
** Form generated from reading UI file 'mailprozor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAILPROZOR_H
#define UI_MAILPROZOR_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_mailProzor
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *sendBtn;
    QPushButton *exitBtn;
    QGridLayout *gridLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *server;
    QLabel *label_2;
    QLineEdit *port;
    QLabel *label_3;
    QLineEdit *uname;
    QLabel *label_4;
    QLineEdit *paswd;
    QLabel *label_5;
    QLineEdit *rcpt;
    QLabel *label_6;
    QLineEdit *subject;
    QLabel *label_8;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *file;
    QPushButton *browseBtn;
    QPlainTextEdit *msg;

    void setupUi(QDialog *mailProzor)
    {
        if (mailProzor->objectName().isEmpty())
            mailProzor->setObjectName(QString::fromUtf8("mailProzor"));
        mailProzor->resize(662, 572);
        layoutWidget = new QWidget(mailProzor);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(50, 250, 224, 29));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        sendBtn = new QPushButton(layoutWidget);
        sendBtn->setObjectName(QString::fromUtf8("sendBtn"));

        horizontalLayout->addWidget(sendBtn);

        exitBtn = new QPushButton(layoutWidget);
        exitBtn->setObjectName(QString::fromUtf8("exitBtn"));

        horizontalLayout->addWidget(exitBtn);

        gridLayout = new QGridLayout(mailProzor);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
        formLayout->setVerticalSpacing(9);
        label = new QLabel(mailProzor);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        server = new QLineEdit(mailProzor);
        server->setObjectName(QString::fromUtf8("server"));

        formLayout->setWidget(0, QFormLayout::FieldRole, server);

        label_2 = new QLabel(mailProzor);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        port = new QLineEdit(mailProzor);
        port->setObjectName(QString::fromUtf8("port"));

        formLayout->setWidget(1, QFormLayout::FieldRole, port);

        label_3 = new QLabel(mailProzor);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        uname = new QLineEdit(mailProzor);
        uname->setObjectName(QString::fromUtf8("uname"));

        formLayout->setWidget(2, QFormLayout::FieldRole, uname);

        label_4 = new QLabel(mailProzor);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        paswd = new QLineEdit(mailProzor);
        paswd->setObjectName(QString::fromUtf8("paswd"));
        paswd->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(3, QFormLayout::FieldRole, paswd);

        label_5 = new QLabel(mailProzor);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        rcpt = new QLineEdit(mailProzor);
        rcpt->setObjectName(QString::fromUtf8("rcpt"));

        formLayout->setWidget(4, QFormLayout::FieldRole, rcpt);

        label_6 = new QLabel(mailProzor);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        subject = new QLineEdit(mailProzor);
        subject->setObjectName(QString::fromUtf8("subject"));

        formLayout->setWidget(5, QFormLayout::FieldRole, subject);

        label_8 = new QLabel(mailProzor);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_8);

        label_7 = new QLabel(mailProzor);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_7);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        file = new QLineEdit(mailProzor);
        file->setObjectName(QString::fromUtf8("file"));
        file->setReadOnly(true);

        horizontalLayout_3->addWidget(file);

        browseBtn = new QPushButton(mailProzor);
        browseBtn->setObjectName(QString::fromUtf8("browseBtn"));

        horizontalLayout_3->addWidget(browseBtn);


        formLayout->setLayout(6, QFormLayout::FieldRole, horizontalLayout_3);

        msg = new QPlainTextEdit(mailProzor);
        msg->setObjectName(QString::fromUtf8("msg"));

        formLayout->setWidget(7, QFormLayout::FieldRole, msg);


        gridLayout->addLayout(formLayout, 0, 0, 1, 1);


        retranslateUi(mailProzor);

        QMetaObject::connectSlotsByName(mailProzor);
    } // setupUi

    void retranslateUi(QDialog *mailProzor)
    {
        mailProzor->setWindowTitle(QApplication::translate("mailProzor", "Dialog", 0));
        sendBtn->setText(QApplication::translate("mailProzor", "Send", 0));
        exitBtn->setText(QApplication::translate("mailProzor", "Exit", 0));
        label->setText(QApplication::translate("mailProzor", "Smtp-server:", 0));
        server->setText(QApplication::translate("mailProzor", "smtp.gmail.com", 0));
        label_2->setText(QApplication::translate("mailProzor", "Server port:", 0));
        port->setText(QApplication::translate("mailProzor", "465", 0));
        label_3->setText(QApplication::translate("mailProzor", "Username:", 0));
        label_4->setText(QApplication::translate("mailProzor", "Password:", 0));
        paswd->setInputMask(QString());
        label_5->setText(QApplication::translate("mailProzor", "Recipant to:", 0));
        label_6->setText(QApplication::translate("mailProzor", "Subject:", 0));
        label_8->setText(QApplication::translate("mailProzor", "Attachment:", 0));
        label_7->setText(QApplication::translate("mailProzor", "Message:", 0));
        browseBtn->setText(QApplication::translate("mailProzor", "Browse...", 0));
    } // retranslateUi

};

namespace Ui {
    class mailProzor: public Ui_mailProzor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAILPROZOR_H
