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




#include "frmstolodvajanje.h"
#include "ui_frmstolodvajanje.h"
#include "clsizracuni.h"


frmstolodvajanje::frmstolodvajanje(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmstolodvajanje)
{
    ui->setupUi(this);
    this->setWindowTitle("Odvajanje stavka na druge dokumente");

    //signali
    connect(ui->btnIzlaz,SIGNAL(pressed()),this,SLOT(on_btnIzlaz()));
    connect(ui->btnMinus,SIGNAL(pressed()),this,SLOT(on_btnMinus()));
    connect(ui->btnPlus,SIGNAL(pressed()),this,SLOT(on_btnPlus()));
    connect(ui->btnPrebaci,SIGNAL(pressed()),this,SLOT(on_btnPrebaciStavku()));
    connect(ui->btnVrati,SIGNAL(pressed()),this,SLOT(on_btnVratiStavku()));


}
frmstolodvajanje::~frmstolodvajanje()
{
    delete ui;
}

void frmstolodvajanje::UcitajFormu()
{
    UcitajStol();
}

void frmstolodvajanje::UcitajStol()
{
    qDebug() << "Dokbr:" << DokBR;
    QSqlQueryModel *dg1 = new QSqlQueryModel();
    dg1->setQuery(QString("select artsif,naziv,kol,pc,ukupno,id from rac2 where dok=%1").arg(DokBR),QSqlDatabase::database("local.sqlite"));
    clsIzracuni *cIzrac = new clsIzracuni();
    cIzrac->set_RacStol(DokBR);
    cIzrac->UcitajLocalRac();

    ui->dgPorezi->setModel(cIzrac->stavkaListaPoreza);
    ui->dgStol->setModel(dg1);
}
void frmstolodvajanje::UcitajStolNovi()
{

}
void frmstolodvajanje::on_btnIzlaz()
{
    this->close();
}
void frmstolodvajanje::on_btnVratiStavku()
{

}
void frmstolodvajanje::on_btnPrebaciStavku()
{

}
void frmstolodvajanje::on_btnPlus()
{

}
void frmstolodvajanje::on_btnMinus()
{

}
