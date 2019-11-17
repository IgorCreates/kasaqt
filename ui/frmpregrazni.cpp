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




#include "frmpregrazni.h"
#include "ui_frmpregrazni.h"
#include <QMessageBox>


frmpregrazni::frmpregrazni(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmpregrazni)
{
    ui->setupUi(this);
    qveri = "";
    _frmPregledSto = "";
    connect(ui->btnOK,SIGNAL(pressed()),this,SLOT(Vrati()));
    connect(ui->btnNovi,SIGNAL(pressed()),this,SLOT(GumbicNovi()));
    connect(ui->dgPregled,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(Vrati()));
    ui->dgPregled->installEventFilter(this);
}

frmpregrazni::~frmpregrazni()
{
    delete ui;
}

void frmpregrazni::PonovnoUcitajGrid(int CurrentID)
{
 //   QSqlQuery q("",QSqlDatabase::database("baza"));
    model = new QSqlQueryModel();
    model->setQuery(qveri,QSqlDatabase::database("baza"));
    ui->dgPregled->setModel(model);

}
void frmpregrazni::UcitajFormu()
{

    if (_frmPregledSto ==  "crm-kupac")
    {
        //qveri =  "select id,naziv,adresa,mjesto from company order by naziv";
        this->setWindowTitle("CRM-Kupac");
    }else if (_frmPregledSto == "crm-prodavac")
    {
        //qveri = "select id,naziv,puninaziv from korisnik order by naziv";
        this->setWindowTitle("CRM-Prodavac");
        ui->btnNovi->setVisible(false);
        ui->btnNovi->setEnabled(false);
    }else
    {
        QMessageBox::warning(this,"pregledi-razni","Nije odabran niti jedan pregled","ok");
        this->close();
    }
    PonovnoUcitajGrid(0);
}

void frmpregrazni::Vrati()
{
    QList<QString> Lista;
    if (_frmPregledSto == "crm-kupac")
    {
        Lista << model->record(ui->dgPregled->currentIndex().row()).value("naziv").toString();
        Lista << model->record(ui->dgPregled->currentIndex().row()).value("adresa").toString();
        Lista << model->record(ui->dgPregled->currentIndex().row()).value("mjesto").toString();
    }else if (_frmPregledSto == "crm-prodavac")
    {
        Lista << model->record(ui->dgPregled->currentIndex().row()).value("naziv").toString();
        Lista << model->record(ui->dgPregled->currentIndex().row()).value("puninaziv").toString();
    }

    emit VratiStoTrazi(_frmPregledSto,
                       model->record(ui->dgPregled->currentIndex().row()).value("id").toInt(),Lista);
                       //ui->dgPregled->model()->data(ui->dgPregled->model()->index(ui->dgPregled->currentIndex().row(),0)).toInt());
    this->close();
}

void frmpregrazni::GumbicNovi()
{
    PozoviFormuKupac(true);
}

void frmpregrazni::PozoviFormuKupac(bool Novi)
{
    frmkupac *fk = new frmkupac(this);
    fk->show();
    fk->NoviKupac = Novi;
    connect(fk,SIGNAL(ReloadGrida(int)),this,SLOT(PonovnoUcitajGrid(int)));
}


bool frmpregrazni::eventFilter(QObject *target, QEvent *event)
{
    QKeyEvent *keyEvent = (QKeyEvent *)event;

    if (target == ui->dgPregled)
    {
        if (event->type() == QEvent::KeyRelease)
        {
            if (!(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return))
            {
                return false;
            }
            Vrati();
        }
    }
    return QWidget::eventFilter(target,event);
}
