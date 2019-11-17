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




#include "frmprinteropcije.h"
#include "ui_frmprinteropcije.h"
//#include <QtSql>
#include <QtGui>
#include <QMessageBox>


frmPrinterOpcije::frmPrinterOpcije(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPrinterOpcije)
{

    ui->setupUi(this);
    this->setWindowTitle("Fiskal Kasa");
    UcitajOpcije();

}

frmPrinterOpcije::~frmPrinterOpcije()
{
    delete ui;
}
void frmPrinterOpcije::UcitajOpcije()
{
    QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
    dbL.open();
    QSqlQuery q("",dbL);
    q.exec("select sto,value1 from isppostavke");
    while (q.next())
    {
        if (q.value(0).toString() == "header")
            ui->txtIspisPostavkeHeader->setPlainText(q.value(1).toString());
        if (q.value(0).toString() == "racun")
            ui->txtIspisPostavkeRac->setPlainText(q.value(1).toString());
        if (q.value(0).toString() == "redosljed")
        {
            if (q.value(1).toString() == "1")
            {
                ui->radioIspisPostavkeH1->setChecked(true);
            }
            if (q.value(1).toString() == "2")
            {
                ui->radioIspisPostavkeH2->setChecked(true);
            }
        }
    }
    QSqlDatabase dbB = QSqlDatabase::database("baza");
    QSqlQueryModel *modelO = new QSqlQueryModel();
    QSqlQueryModel *model2 = new QSqlQueryModel();
    dbB.open();
    modelO->setQuery("select id,naziv from printeri",dbB);
    ui->cmbPrinter->setModel(modelO);
    ui->cmbPrinter->setModelColumn(1);
    //qDebug() << ui->cmbPrinter->currentIndex();
    model2->setQuery(QString("select funkcija,prefix,prefix_nastavak,suffix,suffix_nastavak,id from printer_postavke where prnid=%1")
                                   .arg(ui->cmbPrinter->model()->index(ui->cmbPrinter->currentIndex(),0).data().toInt()),dbB);
//ui->cmbPrinter->model()->index(ui->cmbPrinter->currentIndex(),0).data().toInt()
    ui->tableView->setModel(model2);
}

void frmPrinterOpcije::on_pushButton_pressed()
{
    QString Poruka = "**Varijable: \n"
            "firma_ime - naziv firme, firma_oib - oib firme,firma_adresa - adresa firme\n"
            "firma_grad - grad firme, firma_zip - pos br firme, firma_cb - naziv lokala(caffe bar demo)\n"
            "rac_br - broj racuna, rac_datum - datum racuna, rac_za_platiti - polje za platiti,"
            "rac_zahvala - zahvala (obicno na kraju racuna),rac_prodao - djelatnik koji je napravio rac\n"
            ", rac_stol - stol (ako se koristi),rac_placeno - nacin placanja,rac_porezi - stavke poreza\n"
            ", artikli_zaglavlje - zaglavlje(naziv artikla,kol,cijena), artikli - artikli na racunu\n"
            "rac_napomena-napomena na racunu(kod zakljucivanja racuna)"
            "rac_pdvobveznik\n"
            "**koristi se samo za R1/R2 racune **\n"
            "kupac_ime - naziv kupca, kupac_adresa - adresa kupca,kupac_mjesto - grad sa post br,kupac_oib - oib kupca\n"
            "**Znakovi (-,=,_,*),znak se stavlja u <z>=</z>,stavite samo jedan znak,jer ispisuje taj znak onoliko\n"
            "puta koliko je sirina stranice\n"
            "<b> - pocetak bold, </b> - kraj bold";
    QMessageBox::information(this,"Pomoc",Poruka,"OK");
}

void frmPrinterOpcije::on_btnUpisIspisPostavke_2_pressed()
{
    ui->btnUpisIspisPostavke->setEnabled(false);
    QSqlDatabase dbO = QSqlDatabase::database("local.sqlite");
    QSqlQuery q("",dbO);
    q.exec("delete from isppostavke");
    int Redosljed;
    if (ui->radioIspisPostavkeH1->isChecked())
    {
        Redosljed = 1;
    }else
    {
        Redosljed = 2;
    }
    if (!q.exec(QString("insert into isppostavke select 'header','%1' union all select 'racun','%2' union all select 'redosljed','%3'")
                .arg(ui->txtIspisPostavkeHeader->toPlainText()).arg(ui->txtIspisPostavkeRac->toPlainText()).arg(Redosljed)))
    {
        qDebug() << "Greska upisa opcija" << q.lastError() << q.lastQuery();
    }
    ui->btnUpisIspisPostavke->setEnabled(true);
}

void frmPrinterOpcije::on_cmbPrinter_currentIndexChanged(int index)
{
    UcitajTable();
    //qDebug() << ui->cmbPrinter->model()->rowCount();
}

void frmPrinterOpcije::UcitajTable()
{
    QSqlDatabase dbB = QSqlDatabase::database("baza");
    dbB.open();
    QSqlQueryModel *model2 = new QSqlQueryModel();
    QString qveri = QString("select funkcija,prefix,prefix_nastavak,suffix,suffix_nastavak,id from printer_postavke where prnid=%1").arg(ui->cmbPrinter->model()->index(ui->cmbPrinter->currentIndex(),0).data().toInt());
    model2->setQuery(qveri,dbB);
    ui->tableView->setModel(model2);
}

void frmPrinterOpcije::on_btnUpis_pressed()
{
    QString qveri;
    if (ui->lblPID->text() != "")
    {
        qveri = QString("update printer_postavke set prefix='%1',suffix='%2',prefix_nastavak='%3',suffix_nastavak='%4' where id=%5")
                .arg(ui->txtPPrefix->text()).arg(ui->txtPSuffix->text()).arg(ui->txtPPNastavak->text()).arg(ui->txtPSNastavak->text()).arg(ui->lblPID->text());
    }else{
        qveri = QString("insert into printer_postavke (prefix,suffix,prefix_nastavak,suffix_nastavak,funkcija,prnid) values('%1','%2','%3','%4','%5','%6')")
                .arg(ui->txtPPrefix->text()).arg(ui->txtPSuffix->text()).arg(ui->txtPPNastavak->text()).arg(ui->txtPSNastavak->text())
                .arg(ui->cmbPrnFunkcija->currentText()).arg(ui->cmbPrinter->model()->index(ui->cmbPrinter->currentIndex(),0).data().toString());
    }
    qDebug() << qveri;
    QSqlDatabase dbB = QSqlDatabase::database("baza");
    dbB.open();
    QSqlQuery q("",dbB);
    if (!q.exec(qveri))
        qDebug() << "Greska upisa opcija-" << q.lastError() << q.lastQuery();
    UcitajTable();
    CleanPolja();

}

void frmPrinterOpcije::on_tableView_doubleClicked(const QModelIndex &index)
{
    //ui->tableView->currentIndex().row()).value("pg_cifra").toString()
    ui->txtPPNastavak->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),2)).toString());
    ui->txtPPrefix->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),1)).toString());
    ui->txtPSuffix->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),3)).toString());
    ui->txtPSNastavak->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),4)).toString());
    //ui->cmbPrnFunkcija->itemText(1);
    ui->lblPID->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),5)).toString());
    int id = ui->cmbPrnFunkcija->findText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toString());
    qDebug() << ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toString();
    ui->cmbPrnFunkcija->setCurrentIndex(id);
    ui->btnBrisi_2->setEnabled(true);
}

void frmPrinterOpcije::on_btnBrisi_2_pressed()
{
    CleanPolja();
}
void frmPrinterOpcije::CleanPolja()
{
     ui->txtPPNastavak->setText("");
     ui->txtPSNastavak->setText("");
     ui->txtPPrefix->setText("");
     ui->txtPSuffix->setText("");
     ui->lblPID->setText("");
     ui->btnBrisi_2->setEnabled(false);
}

void frmPrinterOpcije::on_btnBrisi_pressed()
{
    QSqlDatabase dbB = QSqlDatabase::database("baza");
    dbB.open();
    QSqlQuery q("",dbB);
    QString qveri = QString("delete from printer_postavke where id=%1").arg(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),5)).toString());
    if (!q.exec(qveri))
        qDebug() << "Greska brisanja opcija " << q.lastError() << q.lastQuery();
    UcitajTable();
    CleanPolja();
}
