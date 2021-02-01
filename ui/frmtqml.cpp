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




#include "frmtqml.h"
#include "ui_frmtqml.h"
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QDeclarativeView>
#include <QUrl>


frmTqml::frmTqml(QDesktopWidget *parent) :
    QMainWindow(parent)//,
    //ui(new Ui::frmTqml)
{

    //ui->setupUi(this);
    this->setWindowTitle("FiskalKasa");
    this->ui = new QDeclarativeView;
    this->ui->setSource(QUrl::fromLocalFile("touch/touch.qml"));
    this->ui->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    showMaximized();
    setCentralWidget(this->ui);

    QObject::connect((QObject*)this->ui->rootContext()->engine(),SIGNAL(quit()),this,SLOT(close()));


    //QObject::connect(ui->rootContext()->engine(),SIGNAL(quit()),this,SLOT(close()));
    //connect(this->ui->rootContext()->engine(),SIGNAL(quit()),this,SLOT(close()));
   // QWidget::connect()


    //QObject::connect((QObject*)view.engine(), SIGNAL(quit()), &a, SLOT(quit()));
/*
    QObject::connect((QObject*)this->ui->rootContext()->engine(),SIGNAL(quit()),this,SLOT(close()));

    QDeclarativeEngine *engine = new QDeclarativeEngine;
    QDeclarativeComponent component(engine, QUrl::fromLocalFile("qml/TestQML/TestQML.qml"));
    QObject *myObject = component.create();
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(myObject);
    qDebug()<<QObject::connect(item, SIGNAL(titleBarClicked()), &viewer, SLOT(showMaximized()));
*/
}

frmTqml::~frmTqml()
{
    //delete ui;
    delete this->ui;
}
