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
** Form generated from reading UI file 'stoodabir.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STOODABIR_H
#define UI_STOODABIR_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGraphicsView>
#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_stoodabir
{
public:
    QGridLayout *gridLayout;
    QGraphicsView *grPregled;
    QPushButton *btnSnimiPostavke;
    QWidget *widget;

    void setupUi(QDialog *stoodabir)
    {
        if (stoodabir->objectName().isEmpty())
            stoodabir->setObjectName(QString::fromUtf8("stoodabir"));
        stoodabir->resize(701, 537);
        gridLayout = new QGridLayout(stoodabir);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        grPregled = new QGraphicsView(stoodabir);
        grPregled->setObjectName(QString::fromUtf8("grPregled"));

        gridLayout->addWidget(grPregled, 1, 0, 1, 1);

        btnSnimiPostavke = new QPushButton(stoodabir);
        btnSnimiPostavke->setObjectName(QString::fromUtf8("btnSnimiPostavke"));

        gridLayout->addWidget(btnSnimiPostavke, 0, 0, 1, 1);

        widget = new QWidget(stoodabir);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 200));

        gridLayout->addWidget(widget, 2, 0, 1, 1);


        retranslateUi(stoodabir);

        QMetaObject::connectSlotsByName(stoodabir);
    } // setupUi

    void retranslateUi(QDialog *stoodabir)
    {
        stoodabir->setWindowTitle(QApplication::translate("stoodabir", "Stolovi", 0));
        btnSnimiPostavke->setText(QApplication::translate("stoodabir", "Postavke", 0));
    } // retranslateUi

};

namespace Ui {
    class stoodabir: public Ui_stoodabir {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STOODABIR_H
