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
** Form generated from reading UI file 'zakljuci.ui'
**
** Created: Wed Sep 5 15:12:14 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZAKLJUCI_H
#define UI_ZAKLJUCI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QFrame *frame;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QFrame *frame_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QFrame *frame_3;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QLabel *label_4;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(754, 732);
        frame = new QFrame(Dialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 381, 191));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(40, 20, 301, 41));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
        pushButton_2 = new QPushButton(frame);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(40, 70, 301, 41));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        pushButton_3 = new QPushButton(frame);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(40, 120, 301, 41));
        pushButton_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);"));
        frame_2 = new QFrame(Dialog);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setEnabled(true);
        frame_2->setGeometry(QRect(10, 210, 381, 211));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 62, 18));
        lineEdit = new QLineEdit(frame_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(50, 10, 113, 26));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(70, 100, 191, 18));
        frame_3 = new QFrame(Dialog);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setEnabled(true);
        frame_3->setGeometry(QRect(200, 450, 381, 211));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(frame_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 10, 62, 18));
        lineEdit_2 = new QLineEdit(frame_3);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(50, 10, 113, 26));
        label_4 = new QLabel(frame_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(70, 100, 191, 18));

        retranslateUi(Dialog);
        QObject::connect(pushButton, SIGNAL(clicked()), frame_2, SLOT(show()));
        QObject::connect(pushButton, SIGNAL(clicked()), frame, SLOT(hide()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), frame_3, SLOT(show()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), frame, SLOT(hide()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), Dialog, SLOT(close()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Dialog", "Maloprodajni ra\304\215un", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Dialog", "R1 ra\304\215un", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("Dialog", "Natrag u kasu", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "Sum", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "Maloprodaja", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "Sum", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "R1 ra\304\215un", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZAKLJUCI_H
