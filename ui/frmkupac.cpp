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




#include "frmkupac.h"
#include "ui_frmkupac.h"
#include "QtGui"
#include "QtSql"
#include <QMessageBox>


frmkupac::frmkupac(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmkupac)
{
    ui->setupUi(this);
    this->setWindowTitle("Fiskal Kasa");
    NoviKupac = true;
    NovoVozilo = true;
    ui->txtFirmaNaziv->setFocus();
    _TabEnabledVozila = false;
    _PozvanaFormaIzvana = false;

}

frmkupac::~frmkupac()
{
    delete ui;
}
void frmkupac::ChekirajForumIzgled()
{
    if (_TabEnabledVozila)
    {
        ui->tabVozilo->setVisible(true);
    }else
    {
        ui->tabVozilo->setVisible(false);
    }
}

void frmkupac::on_btnUpis_released()
{
    QString Poruka = "Upisati izmijene?";
    if (NoviKupac)
    {
        Poruka = "Upisati novog kupca?";
    }
    int ret = QMessageBox::warning(this,"Baze" ,Poruka,
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::No | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {
        QSqlQuery q("",QSqlDatabase::database("baza"));
        QString sql;
        QString Naziv = ui->txtFirmaNaziv->text().replace("'","");
        QString Adresa = ui->txtFirmaAdresa->text().replace("'","");
        if (NoviKupac)
        {
            sql =  "insert into company (naziv,adresa,dobavljac,oib,email,telefon)";
            sql += QString(" values('%1','%2','N','%4','%5','%6')").arg(Naziv).arg(Adresa)
                    .arg(ui->txtFirmaOIB->text()).arg(ui->txtFirmaEmail->text()).arg(ui->txtFirmaTelefon->text());
        }else{
            sql = QString("update company set naziv='%1',adresa='%2',oib='%3',email='%4',telefon='%5',opis='%6' where id=%7").arg(Naziv)
                    .arg(Adresa).arg(ui->txtFirmaOIB->text()).arg(ui->txtFirmaEmail->text()).arg(ui->txtFirmaTelefon->text())
                    .arg(ui->txtFirmaOpis->toPlainText()).arg(KupacID);
            //NoviKupac = false;
        }

        if (!q.exec(sql))
        {
            QMessageBox::warning(this,"Greska kod upisa","Kupac nije unesen\nProvjerite da li vec ne postoji u bazi\nili da nije problem sa konekcijom prema bazi","OK");
            qDebug() << q.lastError();
            return;
        }else{
            QMessageBox::information(this,"Upis Kupca","Upisano","OK");
            ui->lblFirmaID->setText(QString("%1").arg(KupacID));
            if (NoviKupac)
                KupacID = q.lastInsertId().toInt();
            NoviKupac = false;
        }
        //if (_PozvanaFormaIzvana)
        emit ReloadGrida(KupacID);
    }
}

void frmkupac::on_btnVoziloUpis_released()
{
    QString Poruka = "Upisati izmijene?";
    if (NovoVozilo)
    {
        Poruka = "Upisati novo vozilo?";
    }
    int ret =  QMessageBox::warning(this,"Vozila",Poruka,
                                    QMessageBox::Yes|QMessageBox::Default,
                                    QMessageBox::No|QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {
        QSqlQuery q("",QSqlDatabase::database("baza"));
        QString sql;
        if (NovoVozilo)
        {
            sql = "insert into company_auto (kid,marka,model,godina,registracija,km,napomena)";
            sql += QString(" values('%1','%2','%3','%4','%5','%6','%7')").arg(KupacID).arg(ui->txtVoziloMarka->text().replace("'",""))
                    .arg(ui->txtVoziloModel->text().replace("'","")).arg(ui->txtVoziloGodina->text()).arg(ui->txtVoziloRegistracija->text())
                    .arg(ui->txtVoziloKM->text()).arg(ui->txtVoziloNapomena->toPlainText().replace("'",""));
//            NovoVozilo = false;
            if (!q.exec(sql))
            {
                QMessageBox::warning(this,"Greska kod upisa","Vozilo nije uneseno\nProvjerite pristup bazi","OK");
                qDebug() << q.lastError();
                return;
            }else{
                QMessageBox::information(this,"Upis vozila","Upisano","OK");
//                if (NovoVozilo)
//                    VoziloID = q.lastInsertId().toInt();
                CistiPoljaVozilo();
                UcitajVozilaTBLView();
                NovoVozilo = false;
            }
        }else
        {
            sql = QString("update company_auto set marka='%1',model='%2',godina='%3',registracija='%4',km='%5',napomena='%6' where id=%7")
                    .arg(ui->txtVoziloMarka->text()).arg(ui->txtVoziloModel->text()).arg(ui->txtVoziloGodina->text())
                    .arg(ui->txtVoziloRegistracija->text()).arg(ui->txtVoziloKM->text()).arg(ui->txtVoziloNapomena->toPlainText())
                    .arg(VoziloID);
            if (!q.exec(sql))
            {
                QMessageBox::warning(this,"Greska kod upisa","Izmijene nisu unesene\rProvjerite konekciju prema bazi","OK");
                qDebug() << q.lastError();
                return;
            }else
            {
                QMessageBox::information(this,"Upis vozila","Upisano","OK");
                CistiPoljaVozilo();
                UcitajVozilaTBLView();
            }
        }
    }
}

void frmkupac::on_tabVozilo_selected(const QString &arg1)
{
    UcitajVozilaTBLView();
}
void frmkupac::UcitajVozilaTBLView()
{
    QSqlQueryModel *mod = new QSqlQueryModel();
    mod->setQuery(QString("select id,marka,model,godina,registracija,km,napomena from company_auto where kid=%1").arg(KupacID),QSqlDatabase::database("baza"));
    ui->tableView->setModel(mod);

}

void frmkupac::UcitajKupca(int Kupac_ID)
{
    QSqlQuery q("",QSqlDatabase::database("baza"));
    if (!q.exec(QString("select id,naziv,oib,adresa,telefon,email,opis from company where id=%1").arg(Kupac_ID)))
        return;

    if (q.next())
    {
        ui->txtFirmaAdresa->setText(q.value(3).toString());
        ui->txtFirmaEmail->setText(q.value(5).toString());
        ui->txtFirmaNaziv->setText(q.value(1).toString());
        ui->txtFirmaOIB->setText(q.value(2).toString());
        ui->txtFirmaOpis->setPlainText(q.value(6).toString());
        ui->txtFirmaTelefon->setText(q.value(4).toString());
        NoviKupac= false;

    }
}

void frmkupac::on_btnVoziloIzmijena_released()
{
//    QModelIndex voz =  ui->tableView->currentIndex();
//    voz.data(0).toString()
//    ui->txtVoziloGodina->setText(ui->tableView->model()->index(ui->tableView->currentIndex(),0).data().toString());
//id,marka,model,godina,registracija,km,napomena
    if (NovoVozilo)
    {
        int I = ui->tableView->currentIndex().row();

        QString vozGodina = ui->tableView->model()->index(I,3).data().toString();
        QString vozMarka = ui->tableView->model()->index(I,1).data().toString();
        QString vozModel = ui->tableView->model()->index(I,2).data().toString();
        QString vozRegis = ui->tableView->model()->index(I,4).data().toString();
        QString vozKM = ui->tableView->model()->index(I,5).data().toString();
        QString vozNapo = ui->tableView->model()->index(I,6).data().toString();
        VoziloID = ui->tableView->model()->index(I,0).data().toInt();

        ui->txtVoziloGodina->setText(vozGodina);
        ui->txtVoziloMarka->setText(vozMarka);
        ui->txtVoziloModel->setText(vozModel);
        ui->txtVoziloRegistracija->setText(vozRegis);
        ui->txtVoziloKM->setText(vozKM);
        ui->txtVoziloNapomena->setPlainText(vozNapo);
        ui->btnVoziloIzmijena->setText("Izlaz izmijene");
        NovoVozilo=false;
        ui->txtVoziloMarka->setFocus();
    }else
    {
        CistiPoljaVozilo();
        NovoVozilo=true;
        ui->txtVoziloMarka->setFocus();
    }
}
void frmkupac::CistiPoljaVozilo()
{
    ui->txtVoziloGodina->setText("");
    ui->txtVoziloMarka->setText("");
    ui->txtVoziloModel->setText("");
    ui->txtVoziloRegistracija->setText("");
    ui->txtVoziloKM->setText("");
    ui->txtVoziloNapomena->setPlainText("");
    ui->btnVoziloIzmijena->setText("Izmijena");
    NovoVozilo=true;
}
/*
void frmkupac::ReloadGrida(int StoID)
{

}
*/
