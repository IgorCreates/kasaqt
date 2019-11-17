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




#include "frmpopup_preg.h"
#include "ui_frmpopup_preg.h"
#include <QtSql>
#include <QMessageBox>

frmPopUP_Preg::frmPopUP_Preg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPopUP_Preg)
{
    ui->setupUi(this);
    ui->tablePregled->installEventFilter(this);
    this->setWindowTitle("Fiskal Kasa");
}

frmPopUP_Preg::~frmPopUP_Preg()
{
    delete ui;
}
void frmPopUP_Preg::UcitajPreg(const QString &SQL, const QString &StoUcitava)
{

    QSqlDatabase dbp;
    dbp=QSqlDatabase::database("baza");
    if (!dbp.open())
    {
        qDebug() << "\nGreska db-"+dbp.lastError().text()+"\n";
        QMessageBox::warning(this, "Ne mogu otvoriti bazu", "greska Otvaranje konekcije: " + dbp.lastError().text());
    }
    QSqlQueryModel *model;
    model = new QSqlQueryModel();
    model->setQuery(SQL,dbp);
    ui->tablePregled->setModel(model);
    StoUcitano = StoUcitava;
}
