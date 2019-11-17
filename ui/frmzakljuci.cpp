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




#include "frmzakljuci.h"
#include "ui_frmzakljuci.h"
#include <QtGui>
#include <QtSql>
#include "Ispis/ispismali.h"
#include "mojiheaderi.h"
#include "ui/frmispisodabir.h"
#include <QMessageBox>
#include <QInputDialog>


frmZakljuci::frmZakljuci(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmZakljuci)
{
    IDNacinPlacanja = 1;
    ui->setupUi(this);
    sBPDVc = 0;
    sBPDVp = 0;
    sSUMAc = 0;
    sSUMAp = 0;
    sRabatP = 0;
    sRabatK = 0;
    sPLACENO = 0;
    sZaVratiti = 0;
    mod = new QStandardItemModel(0,2);
    OpisPolje = "";
    _CRMID= 0;


    IzracRac = new clsIzracuni();
    IzracRac->DefVrijednostVarijabli();
    this->setWindowTitle("Fiskal Kasa");
    ui->frame_2->hide();
    ui->frame_Stavke->hide();
    ui->frame_3->hide();
    this->setFixedSize(ui->frame->width() + 20 ,ui->frame->height() + 15);
    //ui->frame->installEventFilter(this);
    ui->frame_2->installEventFilter(this);
    ui->frame_3->installEventFilter(this);

    nw = new QWidget();

    ntKU = new QTableView();
    ntKU->installEventFilter(this);
    ntKU->hide();
    ntNP = new QTableView();
    ntNP->installEventFilter(this);
    ntNP->hide();

    ui->btnPonuda->setHidden(true);
    connect(ui->btnM_ESC,SIGNAL(pressed()),this,SLOT(slotESC()));
    connect(ui->btnV_ESC,SIGNAL(pressed()),this,SLOT(slotESC()));
    connect(ui->btnM_F11,SIGNAL(pressed()),this,SLOT(slotF11()));
    connect(ui->btnV_F11,SIGNAL(pressed()),this,SLOT(slotF11()));
    connect(ui->btnM_F12,SIGNAL(pressed()),this,SLOT(slotF12()));
    connect(ui->btnV_F12,SIGNAL(pressed()),this,SLOT(slotF12()));
    connect(ui->btnM_F2,SIGNAL(pressed()),this,SLOT(slotF2_M()));
    connect(ui->btnV_F2,SIGNAL(pressed()),this,SLOT(slotF2_V()));
    connect(ui->btnM_F3,SIGNAL(pressed()),this,SLOT(slotF3()));
    connect(ui->btnV_F3,SIGNAL(clicked()),this,SLOT(slotF3()));
    connect(ui->btnM_F3,SIGNAL(clicked()),this,SLOT(slotF3()));
    connect(ui->btnM_F2,SIGNAL(clicked()),this,SLOT(slotF2_M()));
    connect(ui->btnV_F2,SIGNAL(clicked()),this,SLOT(slotF2_V()));

    ui->lblM_SSUMA->installEventFilter(this);
    ui->lblV_SSUMA->installEventFilter(this);
    ui->btnRacM->installEventFilter(this);
    ui->btnRacOtpremnica->installEventFilter(this);
    ui->btnRacV->installEventFilter(this);
    ui->btnIzlaz->installEventFilter(this);
    KupacID = 0;
    this->setWindowModality(Qt::WindowModal);
    //ui->btnRacOtpremnica->setVisible(false);
    //ui->btnRacOtpremnica->setEnabled(false);


}
frmZakljuci::~frmZakljuci()
{
    delete ui;
}

/*
  background-color: qlineargradient(spread:pad, x1:0.030303, y1:0.062, x2:0.944101, y2:0.562, stop:0 rgba(255, 255, 255, 255), stop:0.959596 rgba(48, 222, 37, 241));
*/

void frmZakljuci::Suma()
{
    //qDebug() << "SUMA";
    IzracRac->set_RacStol(BrojStola);
    IzracRac->set_RacRabatP(sRabatP);
    IzracRac->UcitajLocalRac();
    IzracRac->izracunRAC();
    ui->tablePorezi->setModel(IzracRac->stavkaListaPoreza);
    ui->lblM_RabatP->setText(QString("%1").arg(sRabatP));
    ui->lblV_RabatP->setText(QString("%1").arg(sRabatP));
    ui->lblM_RabatK->setText(QString("%1").arg(IzracRac->get_RacRabatK()));
    ui->lblM_SSUMA->setText(QString("%1").arg(IzracRac->get_RacSUM()));
    ui->lblM_SSUMAc->setText(QString("%1").arg(IzracRac->get_RacSumCisti()));
    ui->lblV_SSUMA->setText(QString("%1").arg(IzracRac->get_RacSUM()));
    ui->lblV_SSUMAc->setText(QString("%1").arg(IzracRac->get_RacSumCisti()));

}
void frmZakljuci::ucSuma()
{
    //qDebug() << "UCSUMA";
    IzracRac->set_RacStol(BrojStola);
    IzracRac->set_RacRabatP(sRabatP);
    IzracRac->UcitajLocalRac();
    IzracRac->izracunRAC();
    ui->tablePorezi->setModel(IzracRac->stavkaListaPoreza);
    ui->lblM_RabatK->setText(QString("%1").arg(IzracRac->get_RacRabatK()));
    ui->lblM_SSUMA->setText(QString("%1").arg(IzracRac->get_RacSUM()));
    ui->lblM_SSUMAc->setText(QString("%1").arg(IzracRac->get_RacSumCisti()));
    ui->lblV_SSUMA->setText(QString("%1").arg(IzracRac->get_RacSUM()));
    ui->lblV_SSUMAc->setText(QString("%1").arg(IzracRac->get_RacSumCisti()));
}

bool frmZakljuci::ProvjeraBrRacuna()
{
    if (BrojStola == 0)
    {
        QMessageBox::warning(this,"Kasa","Nije odabaran niti jedan racun","OK");
        this->close();
        return false;
    }else
    {
        return true;
    }
}

void frmZakljuci::StoStisnuto(const QString &Sto)
{
    TipRacuna = Sto;
    if (Sto == "M")
    {
        ui->frame->hide();
        ui->frame_2->show();
        ui->frame_2->move(10,10);

        ui->frame_Stavke->show();
        ui->frame_Stavke->move((ui->frame_2->width()+12),10);
        this->setFixedSize((ui->frame_2->width()+20+ui->frame_Stavke->width()),ui->frame_2->height()+15);
        ui->frame_2->setFocus();
    }else if (Sto == "V" || Sto == "O")
    {
        if (Sto == "V" || Sto == "M"){
            StoSprema = " rac1 ";
        }else if (Sto == "O")
        {
            StoSprema = " vrac1 ";
            this->setWindowTitle("OTPREMNICA");
            ui->btnV_F12->setText("F12 - Napravi\notpremnicu");
            ui->label_22->setVisible(false);
            ui->lblV_nplacanja->setVisible(false);
            ui->btnV_F3->setVisible(false);
            ui->btnV_F3->setEnabled(false);
        }
        //ucitaj zemlje u cbZemlje
        QSqlDatabase dbO = QSqlDatabase::database("baza");
        QSqlQueryModel *modelO = new QSqlQueryModel();

        modelO->setQuery("select code,countrylocal from euvatincountry where code='HR' union all (select code,countrylocal from euvatincountry order by countrylocal)",dbO);
        ui->cbZemlja->setModel(modelO);
        ui->cbZemlja->setModelColumn(1);
        //*****
        ui->frame->hide();
        ui->frame_3->show();
        ui->frame_3->move(10,10);

        ui->frame_Stavke->show();
        ui->frame_Stavke->move((ui->frame_3->width()+12),10);
        this->setFixedSize((ui->frame_3->width()+20+ui->frame_Stavke->width()),ui->frame_3->height()+15);
        ui->frame_3->setFocus();
    }
    ucSuma();

}
void frmZakljuci::ucSumaNN()
{
    sBPDVc = 0;
    sBPDVp = 0;
    sSUMAc = 0;
    sSUMAp = 0;
    sRabatP = 0;
    sRabatK = 0;
    sPLACENO = 0;
    sZaVratiti = 0;

    QSqlDatabase db2;

    //db2 = QSqlDatabase::addDatabase("QSQLITE","local.sqlite");
    //db2.setDatabaseName("local.sqlite");
    db2 = QSqlDatabase::database("local.sqlite");
    if (!db2.open())
    {
        qDebug() << "\nGreska db-"+db2.lastError().text()+"\n";
        QMessageBox::warning(this, "Ne mogu otvoriti bazu", "greska neka Otvaranje konekcije: " + db2.lastError().text());
    }
    QSqlQuery q("",db2);
    q.exec(QString("select sum(ukupno) from rac2 where dok=%1").arg(BrojStola));
    if (q.next())
    {
        sSUMAc = q.value(0).toDouble();
        sSUMAp = sSUMAc;
    }
   //qDebug() << q.executedQuery();


    q.exec(QString("select porez_sifra,sum(iznos_poreza),pid from rac4 where dok=%1 group by porez_sifra").arg(BrojStola));
    int i=0;
    double PoreziSUMA = 0;
    while (q.next())
    {
        PoreziSUMA += q.value(1).toDouble();
        QStandardItem *red1 = new QStandardItem(QString("%1").arg(q.value(0).toString()));
        QStandardItem *red2 = new QStandardItem(QString("%1").arg(q.value(1).toDouble()));
        QStandardItem *red3 = new QStandardItem(QString("%1").arg(q.value(2).toInt()));
//        QStandardItem *red3 = new QStandardItem(QString("%1").arg(Porez));
//        QStandardItem *red4 = new QStandardItem(QString("%1").arg(q.value(3).toInt()));
        mod->setItem(i,0,red1);
        mod->setItem(i,1,red2);
        mod->setItem(i,2,red3);
        mod->item(i,0)->setBackground(Qt::red);
        i++;
    }
    ui->tablePorezi->setModel(mod);
    ui->tablePorezi->horizontalHeader()->setVisible(false);
//    for (i=1;i<=mod->columnCount();i++)
//    {
        ui->tablePorezi->setItemDelegateForColumn(1,new mojiHeaderi(this));
//    }
    ui->tablePorezi->setColumnWidth(0,45);
    ui->tablePorezi->setColumnWidth(1,100);
    ui->tablePorezi->setColumnWidth(2,30);

//    ui->tablePorezi->setColumnWidth(3,30);
//    sBpdv = sSSUMA - PoreziSUMA;
    sBPDVc = sSUMAc - PoreziSUMA;



    q.finish();
    db2.close();
    Suma();
}
void frmZakljuci::SumaNN()
{

    if (sRabatP != 0)
    {
        sRabatK = QString::number(sSUMAc * sRabatP/100,'f',2).toDouble();
        sSUMAp = QString::number(sSUMAc - sRabatK,'f',2).toDouble();
        sBPDVp = QString::number(sBPDVc - (sBPDVc * sRabatP/100),'f',2).toDouble();
        ui->lblM_RabatK->setText(QString("%L1").arg(sRabatK));// ,0,'f',2));
        ui->lblV_RabatK->setText(QString("%L1").arg(sRabatK));// ,0,'f',2));
    }else
    {
        sRabatK = 0;
        sSUMAp = QString::number(sSUMAc,'f',2).toDouble();
        sBPDVp = QString::number(sBPDVc,'f',2).toDouble();
        ui->lblM_RabatK->setText(QString("%L1").arg(sRabatK)); //,0,'f',2));
        ui->lblV_RabatK->setText(QString("%L1").arg(sRabatK)); //,0,'f',2));
    }
    //izracunati dobar bpdv i pdv
    //QString::number(sBpdv,'f',2).toDouble();
    sSUMAp =QString::number(sSUMAp,'f',2).toDouble();
    sSUMAc = QString::number(sSUMAc,'f',2).toDouble();
    sBPDVc = QString::number(sBPDVc,'f',2).toDouble();
    sBPDVp = QString::number(sBPDVp,'f',2).toDouble();

    ui->lblM_SSUMA->setText(QString("%L1").arg(sSUMAp,0,'f',2));
    ui->lblM_SSUMAc->setText(QString("%L1").arg(sSUMAc,0,'f',2));
    ui->lblV_SSUMA->setText(QString("%L1").arg(sSUMAp,0,'f',2));
    ui->lblV_SSUMAc->setText(QString("%L1").arg(sSUMAc,0,'f',2));

    double PoreziSUMA=0;
    //qDebug() << "modc" << mod->rowCount();
    for (int i=0;i<mod->rowCount();i++)
    {
        double Porez=0;
        Porez = ui->tablePorezi->model()->index(i,1).data().toDouble() - (ui->tablePorezi->model()->index(i,1).data().toDouble()* sRabatP/100);
        PoreziSUMA += Porez;
        PoreziSUMA = QString::number(PoreziSUMA,'2',2).toDouble();
        QStandardItem *red1 = new QStandardItem(QString("%1").arg(Porez));
        mod->setItem(i,3,red1);
        ui->tablePorezi->setItemDelegateForColumn(3,new mojiHeaderi(this));
    }
}

void frmZakljuci::on_btnRacM_clicked()
{
    StoStisnuto("M");
}

bool frmZakljuci::eventFilter(QObject *target, QEvent *event)
{

    QKeyEvent *keyEvent = (QKeyEvent *)event;
    //qDebug() << "Ev=" << event->type();

    if (target == ui->lblM_SSUMA)
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            //QMessageBox::warning(this,"TT","DBLClick","OK");
            RabatUKP_Suma();
        }
    }
    if (event->type() == QEvent::FocusIn)
    {
        if (target == ui->btnRacM)
        {
            ui->btnRacM->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.030303, y1:0.062, x2:0.944101, y2:0.562, stop:0 rgba(255, 255, 255, 255), stop:0.959596 rgba(48, 222, 37, 241));");
            ui->btnRacOtpremnica->setStyleSheet("");
            ui->btnRacV->setStyleSheet("");
            ui->btnIzlaz->setStyleSheet("");
        }
        if (target == ui->btnRacOtpremnica)
        {
            ui->btnRacOtpremnica->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.030303, y1:0.062, x2:0.944101, y2:0.562, stop:0 rgba(255, 255, 255, 255), stop:0.959596 rgba(48, 222, 37, 241));");
            ui->btnRacM->setStyleSheet("");
            ui->btnRacV->setStyleSheet("");
            ui->btnIzlaz->setStyleSheet("");
        }
        if (target == ui->btnRacV)
        {
            ui->btnRacV->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.030303, y1:0.062, x2:0.944101, y2:0.562, stop:0 rgba(255, 255, 255, 255), stop:0.959596 rgba(48, 222, 37, 241));");
            ui->btnRacOtpremnica->setStyleSheet("");
            ui->btnRacM->setStyleSheet("");
            ui->btnIzlaz->setStyleSheet("");
        }
        if (target == ui->btnIzlaz)
        {
            ui->btnIzlaz->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.030303, y1:0.062, x2:0.944101, y2:0.562, stop:0 rgba(255, 255, 255, 255), stop:0.959596 rgba(48, 222, 37, 241));");
            ui->btnRacOtpremnica->setStyleSheet("");
            ui->btnRacM->setStyleSheet("");
            ui->btnRacV->setStyleSheet("");
        }

    }
    if ((target->objectName() == "NT1") || (target->objectName() == "NT_Kupci"))
    {
        if (keyEvent->key() == Qt::Key_Escape)
        {
            ntKU->hide();
            ntNP->hide();
            if (ui->frame_2->isVisible())
            {
                ui->frame_2->setFocus();
            }
            else
            {
                ui->frame_3->setFocus();
            }
            return true;
        }
        if ((keyEvent->key() == Qt::Key_Return) || (keyEvent->key() == Qt::Key_Enter))
        {
            if (target->objectName() == "NT1")
            {
                slotOdabranNP();
            }else if (target->objectName() == "NT_Kupci")
            {
                slotOdabranKupac();
            }
        }


        return QWidget::eventFilter(target,event);
    }
    if (event->type() == QEvent::KeyPress)
    {
        switch (keyEvent->key())
        {
            case Qt::Key_F1:
            {
                QString pomoc;
                pomoc="F1-pomoc";
                QMessageBox::information(this,"Pomoc",pomoc,QMessageBox::Ok);
                break;
            }
            case Qt::Key_F11:
                slotF11();
                break;
            case Qt::Key_F3:
            {
                slotF3();
                break;
            }
            case Qt::Key_F2:
            if (target->objectName() == "frame_2")
                {
                    slotF2_M();
                    break;
                }
            else
            {
                slotF2_V();
            }
                break;
            case Qt::Key_F12:
                slotF12();
                break;
        }

    }
    return QWidget::eventFilter(target,event);
}
void frmZakljuci::slotOdabranNP()
{
    ui->lblM_nplacanja->setText(nmodNP->data(ntNP->currentIndex()).toString());
    ui->lblV_nplacanja->setText(nmodNP->data(ntNP->currentIndex()).toString());
    IDNacinPlacanja = nmodNP->record(ntNP->currentIndex().row()).value("id").toInt();
    IzracRac->set_RacNP(IDNacinPlacanja);
    ntNP->close();
}

void frmZakljuci::slotOdabranKupac()
{
    ui->txtKupacAdresa->setText(nmodKU->record(ntKU->currentIndex().row()).value("adresa").toString());
    ui->txtKupacNaziv->setText(nmodKU->record(ntKU->currentIndex().row()).value("naziv").toString());
    ui->txtKupacMB->setText(nmodKU->record(ntKU->currentIndex().row()).value("mb").toString());
    ui->txtKupacOIB->setText(nmodKU->record(ntKU->currentIndex().row()).value("oib").toString());
    ui->txtKupacSifra->setText(nmodKU->record(ntKU->currentIndex().row()).value("sifra").toString());
    ui->txtKupacMjesto->setText(nmodKU->record(ntKU->currentIndex().row()).value("mjesto").toString());

    for (int i=0; i <= ui->cbZemlja->model()->rowCount();i++)
    {
        if (ui->cbZemlja->model()->index(i,0).data().toString() == nmodKU->record(ntKU->currentIndex().row()).value("countrycode").toString() )
        {
            ui->cbZemlja->setCurrentIndex(i);
        }
    }


    IzracRac->set_RacKupac(0,ui->txtKupacNaziv->text(),ui->txtKupacAdresa->text(),ui->txtKupacOIB->text()
                           ,ui->txtKupacMjesto->text(),ui->txtKupacSifra->text(),ui->txtKupacMB->text(),
                           ui->cbZemlja->model()->index(ui->cbZemlja->currentIndex(),0).data().toString());
    ntKU->close();
}

void frmZakljuci::NT_Nplacanja(const QModelIndex &StoDeb)
{
    slotOdabranNP();
    /*
        ui->lblM_nplacanja->setText(StoDeb.data().toString());
        ui->lblV_nplacanja->setText(StoDeb.data().toString());
        IDNacinPlacanja = StoDeb.model()->index(StoDeb.row(),0).data().toInt();
        IzracRac->set_RacNP(IDNacinPlacanja);
        nt->close();
//        return;
*/
}
void frmZakljuci::NT_Kupci(const QModelIndex &StoDeb)
{
    slotOdabranKupac();
    /*
    ui->txtKupacAdresa->setText(nmodKU->record(ntKU->currentIndex().row()).value("adresa").toString());
    ui->txtKupacNaziv->setText(nmodKU->record(ntKU->currentIndex().row()).value("naziv").toString());
    ui->txtKupacMB->setText(nmodKU->record(ntKU->currentIndex().row()).value("mb").toString());
    ui->txtKupacOIB->setText(nmodKU->record(ntKU->currentIndex().row()).value("oib").toString());
    ui->txtKupacSifra->setText(nmodKU->record(ntKU->currentIndex().row()).value("sifra").toString());
    ui->txtKupacMjesto->setText(nmodKU->record(ntKU->currentIndex().row()).value("mjesto").toString());
    KupacID = nmodKU->record(ntKU->currentIndex().row()).value("id").toInt();
    IzracRac->set_RacKupac(KupacID,ui->txtKupacNaziv->text(),ui->txtKupacAdresa->text(),ui->txtKupacOIB->text()
                           ,ui->txtKupacMjesto->text(),ui->txtKupacSifra->text(),ui->txtKupacMB->text());
    ntKU->close();
    */

}

void frmZakljuci::on_btnRacV_clicked()
{
    StoStisnuto("V");
}

void frmZakljuci::on_btnIzlaz_pressed()
{
    this->close();
}

void frmZakljuci::on_btnPonuda_pressed()
{
    QMessageBox::information(this,"Demo","U DEMO radu programa nije moguce napraviti ponudu","OK");
}
void frmZakljuci::BrisanjeDokNaCekanju()
{
    QSqlDatabase db2 = QSqlDatabase::database("local.sqlite");
    if (!db2.open())
    {
        qDebug() << "\nGreska db-"+db2.lastError().text()+"\n";
        QMessageBox::warning(this, "Ne mogu otvorit bazu", "greska neka Otvaranje konekcije: " + db2.lastError().text());
    }
    QSqlQuery q("",db2);
    if (!q.exec(QString("delete from rac2 where dok=%1").arg(BrojStola)))
    {
        db2.close();
        QMessageBox::warning(0,"Greska u sql-u", "Greska kod brisanja dok na cekanju");
        return;
    }
    q.exec(QString("delete from rac4 where dok=%1").arg(BrojStola));
    db2.close();
    db2.database().close();
}

void frmZakljuci::slotUpisiRaucn()
{

}
void frmZakljuci::slotESC()
{
    this->close();
}
void frmZakljuci::slotF11()
{
    bool ok;
    QString NNN = QInputDialog::getText(this,"Popust","Upisite popust u postocima", QLineEdit::Normal,"", &ok);
    if (ok && !NNN.isEmpty())
    {
        sRabatP = NNN.toDouble();
        ui->lblM_RabatP->setText(NNN);
        ui->lblV_RabatP->setText(NNN);
        Suma();
    }

}
void frmZakljuci::slotF12()
{
    if (TipRacuna == "V" || TipRacuna == "O")
    {
        if (ui->txtKupacNaziv->text()=="" || ui->txtKupacOIB->text() == "" || ui->txtKupacAdresa->text() == "")
        {
            QMessageBox::warning(this,"Upis", "Niste upisali potrebne podatke za kupca\nPotrebni podaci Naziv,Adresa,Oib","OK");
            return;
        }
    }
    QString PorZaklj=" racun?";
    if (TipRacuna == "O")
        PorZaklj=" otpremnicu?";
    int ret = QMessageBox::warning(this,"Napravi racun" ,QString("Da li zelite napraviti ").append(PorZaklj),
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::No | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {
        ui->btnM_F12->setVisible(false);
        QLabel *PorukaInfo = new QLabel();
        PorukaInfo->setParent(this);
        PorukaInfo->setText("Pricekajte do zavrsetka upisa racuna");
        QFont  serifFont("Times", 24, QFont::Bold);
        PorukaInfo->setFont(serifFont);
        //PorukaInfo.setFixedSize(300,100);
        PorukaInfo->show();
        PorukaInfo->move((this->width()/2)-(PorukaInfo->width()/2),(this->height()/2)-(PorukaInfo->height()/2));
        PorukaInfo->setStyleSheet("color: rgb(255, 0, 0);");

        //slotUpisiRaucn();
        IzracRac->set_RacStol(BrojStola);
        IzracRac->set_RacNP(IDNacinPlacanja);
        if (TipRacuna == "V" || TipRacuna == "O")
        {
            IzracRac->set_RacVrstaRac("V");
        }else
        {
            IzracRac->set_RacVrstaRac(TipRacuna);
        }
        IzracRac->set_RacTiprac(TipRacuna);
        IzracRac->set_RacKupac(KupacID,ui->txtKupacNaziv->text(),ui->txtKupacAdresa->text(),ui->txtKupacOIB->text(),ui->txtKupacMjesto->text()
                               ,ui->txtKupacSifra->text(),ui->txtKupacMB->text(),
                               ui->cbZemlja->model()->index(ui->cbZemlja->currentIndex(),0).data().toString());
        IzracRac->set_RacOpisPolje(OpisPolje);
        if (IzracRac->UpisiRac())
        {
            BrisanjeDokNaCekanju();


            ispisMali *isp = new ispisMali();


            //ladica
            if (qApp->property("Printer-Ladica").toString() == "1")
            {
                QString PrinterPort;
                if (!qApp->property("Printer-Port").isNull())
                {
                    PrinterPort = qApp->property("Printer-Port").toString();
                }else
                {
                    QMessageBox::warning(this,"Printer","Nemate definiran port za ispis\nPokusavam ispis na /dev/lp0","OK");
                    PrinterPort = "/dev/lp0";
                }
                QString Komanda = "./ladica.py";
                qDebug() << system(Komanda.toUtf8().constData());
                Komanda = QString("cat /tmp/ispLadica.txt >> %1").arg(PrinterPort);
                system(Komanda.toUtf8().constData());
            }

           //kraj ladica




//            mojiHeaderi *t = new mojiHeaderi();
            if (TipRacuna != "O")
            {
                //t->xmlRacunZahtjev(IzracRac->get_Rac1ID(),false);
            //    isp->IspisMaliPos(QString("%1").arg(IzracRac->get_Rac1ID()));
            }else {
            //    isp->IspisMaliPosVrac(QString("%1").arg(IzracRac->get_Rac1ID()));
            }
            //QMessageBox::information(this,"Racun","Racun je upisan","OK");

            int bb = IzracRac->get_Rac1ID();

            //QFuture<void> ispR = QtConcurrent::run(IspisRac,bb);
            this->hide();
            QThread* thread = new QThread;
            frmIspisOdabir* fisp = new frmIspisOdabir();
            fisp->moveToThread(thread);
            //connect(fisp,SIGNAL(accepted()),fisp,SLOT(show()));
            connect(fisp,SIGNAL(destroyed()),thread,SLOT(deleteLater()));
            connect(fisp,SIGNAL(rejected()),thread,SLOT(deleteLater()));
//            connect(fisp,SIGNAL(finished(int)),thread,SLOT(quit()));
            connect(fisp,SIGNAL(finished(int)),fisp,SLOT(deleteLater()));
            connect(fisp,SIGNAL(finished(int)),thread,SLOT(deleteLater()));

            fisp->show();
            fisp->RacunID = bb;
            if (TipRacuna == "O")
                fisp->RacTipRacuna = "vrac1";
            fisp->ProvjeraOpcija();
            thread->start();
            connect(thread,SIGNAL(finished()),this,SLOT(close()));
            connect(thread,SIGNAL(destroyed()),this,SLOT(close()));
            connect(thread,SIGNAL(terminated()),this,SLOT(close()));
            this->hide();
            this->close();
        }else
        {
            PorukaInfo->setText("Greska kod upisa racuna\rProvjerite pristup bazi\rili nazovite odrzavanje");
        }
    }
}
void frmZakljuci::IspisRac(int racID)
{
    frmIspisOdabir *fisp = new frmIspisOdabir();
    fisp->show();
    fisp->RacunID = racID; // IzracRac->get_Rac1ID();
}

void frmZakljuci::slotF2_M()
{
    bool ok;
    QString NNN = QInputDialog::getText(this,"Placanje","Upisite uplatu", QLineEdit::Normal,"", &ok);
    if (ok && !NNN.isEmpty())
    {
        double mPlaceno;
        double mZaVratit;
        mPlaceno = NNN.toDouble();
        mZaVratit = mPlaceno - IzracRac->get_RacSUM();
        if (mPlaceno == 0)
            mZaVratit = 0;
        ui->lblM_Placeno->setText(QString("%L1").arg(mPlaceno,0,'f',2));
        ui->lblMza_vratiti->setText(QString("%L1").arg(mZaVratit,0,'f',2));
    }

}
void frmZakljuci::slotF2_V()
{
    QSqlDatabase dbp = QSqlDatabase::database("baza");
    if (!dbp.open())
        QMessageBox::warning(this,"GRESKA","BAZA",QMessageBox::Ok);

    nmodKU = new QSqlQueryModel();
    QString sqlC = "select * from company ";
    bool sqlCand = false;
    if (ui->txtKupacNaziv->text().length() >0 )
    {
       sqlC +=  QString(" where naziv like '%%1%' ").arg(ui->txtKupacNaziv->text());
       sqlCand = true;
    }
    if (ui->txtKupacOIB->text().length()>0)
    {
        if (!sqlCand)
        {
            sqlC += " where ";
        }else
        {
            sqlC += " and ";
        }
        sqlC += QString(" oib like '%1%' ").arg(ui->txtKupacOIB->text());
        sqlCand = true;
    }
    if (ui->txtKupacSifra->text().length() > 0)
    {
        if (!sqlCand)
        {
            sqlC += " where ";
        }else
        {
            sqlC += " and ";
        }
        sqlC += QString(" sifra like '%1%' ").arg(ui->txtKupacSifra->text());
    }
    //qDebug() << sqlC;
    nmodKU->setQuery(sqlC,dbp);
    ntKU->setModel(nmodKU);
    ntKU->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    ntKU->setMinimumSize(this->width(),this->height());
    ntKU->setColumnHidden(0,true);
    ntKU->setColumnWidth(1,500);
    //nt->horizontalHeader()->hide();
    ntKU->verticalHeader()->hide();
    ntKU->setObjectName("NT_Kupci");
    ntKU->setWindowFlags(Qt::WindowStaysOnTopHint);
    ntKU->showMaximized();
    ntKU->show();
    ntKU->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ntKU->setFocus();

    connect(ntKU,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(NT_Kupci(QModelIndex)));
    connect(ntKU,SIGNAL(doubleClicked(QModelIndex)),ntKU,SLOT(close()));
    connect(ntKU,SIGNAL(doubleClicked(QModelIndex)),ui->frame_3,SLOT(setFocus()));

}

void frmZakljuci::slotF3()
{
        QSqlDatabase dbp = QSqlDatabase::database("baza");
        if (!dbp.open())
            QMessageBox::warning(this,"GRESKA","BAZA",QMessageBox::Ok);

        nmodNP = new QSqlQueryModel();
        nmodNP->setQuery("select id,naziv from kartice where fiskal_znak in ('G','K','O')",dbp);
        ntNP->setModel(nmodNP);
        ntNP->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        ntNP->setMinimumSize(this->width(),this->height());
        ntNP->setColumnHidden(0,true);
        ntNP->setColumnWidth(1,500);
        //nt->horizontalHeader()->hide();
        ntNP->verticalHeader()->hide();
        ntNP->setObjectName("NT1");
        ntNP->setWindowFlags(Qt::WindowStaysOnTopHint);
        ntNP->installEventFilter(this);
        ntNP->show();
        ntNP->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        ntNP->setFocus();

        connect(ntNP,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(NT_Nplacanja(QModelIndex)));
        connect(ntNP,SIGNAL(doubleClicked(QModelIndex)),ntNP,SLOT(close()));
        connect(ntNP,SIGNAL(doubleClicked(QModelIndex)),ui->frame_2,SLOT(setFocus()));

        /*
        connect(nt,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(NT_Nplacanja(QModelIndex)));
        connect(nt,SIGNAL(doubleClicked(QModelIndex)),nt,SLOT(close()));
        connect(nt,SIGNAL(doubleClicked(QModelIndex)),ui->frame_2,SLOT(setFocus()));
        /*
        connect(nt,SIGNAL(pressed(QModelIndex &)),this,SLOT(Debug(QModelIndex)));
        connect(nt,SIGNAL(pressed(QModelIndex &)),nt,SLOT(close()));
        connect(nt,SIGNAL(pressed(QModelIndex &)),ui->frame_2,SLOT(setFocus()));
        */
        //nw->show();
        dbp.close();

}

void frmZakljuci::on_btnRacOtpremnica_pressed()
{
    StoStisnuto("O");
}
void frmZakljuci::Funkcija_Napomena()
{
    txtOpis = new QPlainTextEdit;
    //NoviNazivProzor = new QWidget();
    OpisProzor = new QDialog(this);
    OpisUpis = new QPushButton("UPIS");
    OpisIzlaz = new QPushButton("IZLAZ");
    vboxLayout = new QVBoxLayout;


    txtOpis->setObjectName("NoviNaziv");
    OpisUpis->setObjectName("NoviNazivG1");
    vboxLayout->addWidget(txtOpis);
    vboxLayout->addWidget(OpisUpis);
    vboxLayout->addWidget(OpisIzlaz);
    OpisProzor->setLayout(vboxLayout);

    OpisProzor->setWindowTitle("Upis napomene");
    OpisProzor->show();

    //txtOpis->setPlainText(ui->txtArtNazivOriginal->text());
//    txtNoviNaziv->installEventFilter(this);

    connect(OpisIzlaz,SIGNAL(released()),OpisProzor,SLOT(close()));
    connect(OpisUpis,SIGNAL(released()),this,SLOT(DodajOpis()));
    connect(OpisUpis,SIGNAL(released()),OpisProzor,SLOT(close()));

}
void frmZakljuci::DodajOpis()
{
    OpisPolje = txtOpis->toPlainText();
}

void frmZakljuci::on_btnNapomena_released()
{
    Funkcija_Napomena();
}

void frmZakljuci::on_btnNapomena_2_released()
{
    Funkcija_Napomena();
}
void frmZakljuci::RabatUKP_Suma()
{
    double UKPSUMA = QInputDialog::getDouble(this,"Za platiti","Upisite novu sumu za platiti\nMozete upisati samo manju sumu od ponudene\nAko zelite vratiti na staru sumu vratite je sa:\nESC(da se vratite iz prozora izmijene) F11(popust)-i upisite 0",
                                             ui->lblM_SSUMA->text().toDouble(),0,ui->lblM_SSUMAc->text().toDouble());
    sRabatP = ((ui->lblM_SSUMA->text().toDouble()-UKPSUMA)*100)/ui->lblM_SSUMA->text().toDouble();
    Suma();
}
