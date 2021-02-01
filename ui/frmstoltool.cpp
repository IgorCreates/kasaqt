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




#include "frmstoltool.h"
#include "ui_frmstoltool.h"
#include "QtSql"
#include "mojiheaderi.h"
#include "QStandardItemModel"
//#include "QStandardItem"

frmStolTool::frmStolTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmStolTool)
{

    mod = new QStandardItemModel();
    ui->setupUi(this);
    this->setWindowTitle("Prebacivanje artikala/Spajanje racuna/Naplata izabranih artikala");
    //this->setWindowState(qt_check_for_QOBJECT_macro:);
}

frmStolTool::~frmStolTool()
{
    delete ui;
}
void frmStolTool::UcitajForm()
{
    UcitajRacunNaCekanju();
}

bool frmStolTool::UcitajRacunNaCekanju()
{

    /*
    QSqlQueryModel *mod = new QSqlQueryModel();
    mod->setQuery(QString("select aid,naziv,kol,pc,ukupno from rac2 where dok=%1").arg(BrStola),QSqlDatabase::database("local.sqlite"));
    ui->tableView->setModel(mod);
    int i;
    for (i=2;i<=mod->columnCount();i++)
    {
        ui->tableView->setItemDelegateForColumn(i,new mojiHeaderi(this));
    }
    ui->tableView->setColumnWidth(1,250);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(2,60);
    */
    QSqlQuery q(QString("select aid,naziv,kol,pc,ukupno from rac2 where dok=%1").arg(BrStola),QSqlDatabase::database("local.slqite"));
    //QPushButton *red6 = new QPushButton("+");
//        QStandardItem *red3 = new QStandardItem(QString("%1").arg(Porez));
//        QStandardItem *red4 = new QStandardItem(QString("%1").arg(q.value(3).toInt()));
    int i =0;
    while (q.next())
    {
        QStandardItem *red1 = new QStandardItem(QString("%1").arg(q.value(0).toInt()));
        QStandardItem *red2 = new QStandardItem(QString("%1").arg(q.value(1).toString()));
        QStandardItem *red3 = new QStandardItem(QString("%1").arg(q.value(2).toDouble()));
        QStandardItem *red4 = new QStandardItem(QString("%1").arg(q.value(3).toDouble()));
        QStandardItem *red5 = new QStandardItem(QString("%1").arg(q.value(4).toDouble()));
        mod->setItem(i,0,red1);
        mod->setItem(i,1,red2);
        mod->setItem(i,2,red3);
        mod->setItem(i,3,red4);
        mod->setItem(i,4,red5);
        //ui->tableView->setIndexWidget(new QPushButton("+"));

        //mod->setItem(i,5,red6);
        i++;
    }
    ui->tableView->setModel(mod);

}
