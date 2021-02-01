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
** Form generated from reading UI file 'omeni.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OMENI_H
#define UI_OMENI_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_oMeni
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTextEdit *txtOmeni;
    QPushButton *btnGumbicOK;

    void setupUi(QDialog *oMeni)
    {
        if (oMeni->objectName().isEmpty())
            oMeni->setObjectName(QString::fromUtf8("oMeni"));
        oMeni->resize(520, 320);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(oMeni->sizePolicy().hasHeightForWidth());
        oMeni->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(oMeni);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        txtOmeni = new QTextEdit(oMeni);
        txtOmeni->setObjectName(QString::fromUtf8("txtOmeni"));

        verticalLayout->addWidget(txtOmeni);

        btnGumbicOK = new QPushButton(oMeni);
        btnGumbicOK->setObjectName(QString::fromUtf8("btnGumbicOK"));

        verticalLayout->addWidget(btnGumbicOK);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(oMeni);
        QObject::connect(btnGumbicOK, SIGNAL(released()), oMeni, SLOT(close()));

        QMetaObject::connectSlotsByName(oMeni);
    } // setupUi

    void retranslateUi(QDialog *oMeni)
    {
        oMeni->setWindowTitle(QApplication::translate("oMeni", "O-Meni", 0));
        txtOmeni->setHtml(QApplication::translate("oMeni", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600; font-style:italic; color:#3d74eb;\">Fiskal Kasa  Ver: 2.8.0</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">Program izrado : </span><span style=\" font-weight:600; font-style:italic;\">Juka-Net Informatika</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	web: <a href=\"http://www.juka-net.hr\"><span style=\" "
                        "text-decoration: underline; color:#0000ff;\">www.juka-net.hr</span></a></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	mail: <a href=\"mailto:info@juka-net.hr\"><span style=\" text-decoration: underline; color:#0000ff;\">info@juka-net.hr</span></a></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">Za potrebe dan na koristenje:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	<span style=\" font-weight:600; font-style:italic;\">Firma Ime</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	Firma adresa</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	Firma OIB</p>\n"
""
                        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; font-style:italic; text-decoration: underline;\">Compiled : ver ver</span></p></body></html>", 0));
        btnGumbicOK->setText(QApplication::translate("oMeni", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class oMeni: public Ui_oMeni {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OMENI_H
