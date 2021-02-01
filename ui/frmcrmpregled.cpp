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




#include "frmcrmpregled.h"
#include "ui_frmcrmpregled.h"
#include <QtSql>

frmcrmpregled::frmcrmpregled(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmcrmpregled)
{
    ui->setupUi(this);
    _CRM_ID = 0;
    ui->btnIzmijena->setEnabled(false);
    ui->btnIzmijena->setVisible(false);

}

frmcrmpregled::~frmcrmpregled()
{
    delete ui;
}


void frmcrmpregled::UcitaFormu(int crmid)
{
    ui->lblRacunVeza->setEnabled(false);
    ui->lblRacunVeza->setVisible(false);
    if (crmid == 0)
        return;
    QSqlQuery q("",QSqlDatabase::database("baza"));
    QString qveri = QString("select c.*,co.opis from crm c left join crmopis co on c.id=co.crmid where c.id=%1").arg(crmid);

    if (!q.exec(qveri))
        return;
    if (!q.next())
        return;
    ui->txtKupac->setText(q.value(q.record().indexOf("kupac_naziv")).toString());
    ui->txtProdavac->setText(q.value(q.record().indexOf("prodavac_naziv")).toString());
    ui->txtUnio->setText(q.value(q.record().indexOf("unio_naziv")).toString());
    ui->txtOpis->setText(q.value(q.record().indexOf("opis")).toString());
    ui->txtDatumDolaska->setText(q.value(q.record().indexOf("datum_unosa")).toString());
    qveri = QString("select c.crmid,c.rid,r.br from crmrac c left join rac1 r on c.rid=r.id where c.crmid=%1").arg(crmid);
    bool Dalje = true;
    if (!q.exec(qveri))
        Dalje = false;
    if (!q.next())
        Dalje = false;
    if (!Dalje)
        return;
    ui->lblRacunVeza->setEnabled(true);
    ui->lblRacunVeza->setVisible(true);
    ui->lblRacunVeza->setText(QString(tr("Racun veza - br racuna: %1")).arg(q.value(2).toString()));
}
