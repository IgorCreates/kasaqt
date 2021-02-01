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




#include "frmrazduzisklad.h"
#include "ui_frmrazduzisklad.h"
#include <QtGui>
#include <QtSql>
#include <QMessageBox>

frmrazduziSklad::frmrazduziSklad(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmrazduziSklad)
{
    ui->setupUi(this);
    this->setWindowTitle("Fiskal Kasa");

}

frmrazduziSklad::~frmrazduziSklad()
{
    delete ui;
}

void frmrazduziSklad::on_btnRazduziSklad_released()
{
    int ret = QMessageBox::information(this,"PAZNJA","Da li zelite sa skladista skinuti robu?",
                                       QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {
        if (DokTIP == "Rnalog")
        {
            SID = ui->cmbSklad->model()->index(ui->cmbSklad->currentIndex(),0).data().toInt();
            QSqlQuery q("",QSqlDatabase::database("baza"));
            QString qveri= QString("update rnalog3 set sid=%1,skinuto=1 where rid=%2").arg(SID).arg(DokID);
            if (!q.exec(qveri))
            {
                qDebug() << "Greska razduzi sklad " << q.lastError() << q.lastQuery();
                QMessageBox::warning(this,"GRESKA","Greska kod upisa u bazu","OK");
                this->close();
            }
        }
        QMessageBox::information(this,"UPIS","Roba skinuta sa skladista","OK");
        this->close();
    }
}
void frmrazduziSklad::UcitajFRM(const QString DokOpis, const int SkladID, const QString Doktip, const int Dokid)
{
    SID = SkladID;
    DokTIP = Doktip;
    DokID = Dokid;
    ui->txtDokument->setText(DokOpis);
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("select id,concat(sifra,'-',naziv) from matsklad order by id",QSqlDatabase::database("baza"));
    ui->cmbSklad->setModel(model);
    ui->cmbSklad->setModelColumn(1);
}
