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




#include "frmkasa.h"
#include "ui_frmkasa.h"
#include <QtGui>
#include <QtSql>
//#include "connection.h"
#include <QTextStream>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <Qt>
#include <QDebug>

#include "ui/frmpregart.h"
#include "frmkrajdana.h"
#include "mojiheaderi.h"
#include "Ispis/ispismali.h"
#include "ui/frmpregdokum.h"

#include "ui/frmlogin.h"
#include <QResource>
#include <QLibrary>
#include "ui/frmtartodabir.h"

//#include "ui/frmtqml.h"
#include "ui/frmopcije.h"
#include "ui/frmdodajstavku.h"
//#include "ui/classizracuni.h"
#include <QMutex>
#include "ui/frmispiskuhinja.h"
#include "ui/frmispisdelstolova.h"
//#include "ui/frmstoltool.h"
#include "ui/frmrnalog.h"
#include "ui/frmstolodvajanje.h"
#include <ui/frmispisodabir.h>
#include <ui/omeni.h>


#include <QMessageBox>
#include <QInputDialog>


#include <QSettings>

//qml
/*
#include <QApplication>
#include <QDeclarativeView>
#include <QUrl>
*/

//#include <frmpregled.h>
//#include "ui/baze.h"


frmKasa::frmKasa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmKasa)

{
    ui->setupUi(this);
    lblNarudzbaPoruka = new QLabel(this);
    UcitavanjeForme = true;
    LogRac = true;
    StoloviModel = new QStandardItemModel(0,4);
    ui->lineEdit->installEventFilter(this);
    ui->tableView->installEventFilter(this);
    ui->tableStolovi->installEventFilter(this);
    ui->txtPORUKA->installEventFilter(this);
    //ui->tableView->installEventFilter(this);

//    ui->listView->installEventFilter(this);
    //KreirajMeniGumbice();
    constStoPise = "STOL";
    brziRacunStoj = false;
    BrziCounter =0;
    //IZRAC = new clsIzracuni();
    //IZRAC->DefVrijednostVarijabli();
    this->setWindowTitle(QString(tr("Fiskal Kasa %1")).arg(qApp->property("App_Verzija").toString()));
//    this->setWindowTitle(QString("");
    connect(ui->btnF2,SIGNAL(pressed()),this,SLOT(slotF2()));
    connect(ui->btnF12,SIGNAL(pressed()),this,SLOT(slotF12()));
    connect(ui->btnF3,SIGNAL(pressed()),this,SLOT(slotPromijenaKonobara()));
    connect(ui->btnF4,SIGNAL(pressed()),this,SLOT(slotF4()));
    connect(ui->btnIzlaz,SIGNAL(pressed()),this,SLOT(slotIzlazIzPrograma()));
    connect(ui->btnKrajSmjeneDana,SIGNAL(pressed()),this,SLOT(slotKrajSmjeneDana()));
    connect(ui->btnPregRacuna,SIGNAL(pressed()),this,SLOT(slotPregledRacuna()));
    //connect(ui->btnTableStoloviDolje,SIGNAL(pressed()),this,SLOT(slotTStolDown()));
    connect(ui->btnOdaberiStol,SIGNAL(pressed()),this,SLOT(slotOdaberiStol()));
    connect(ui->btnDodajStol,SIGNAL(pressed()),this,SLOT(slotDodajStol()));
    connect(ui->btnBrisiStol,SIGNAL(pressed()),this,SLOT(slotBrisiStol()));
    connect(ui->btnArtikli,SIGNAL(pressed()),this,SLOT(slotArtikli()));
    connect(ui->btnBrziRacun,SIGNAL(pressed()),this,SLOT(slotBrziRacun()));
    connect(ui->btnF8,SIGNAL(pressed()),this,SLOT(slotF8()));
    connect(ui->btnART_Brisi,SIGNAL(pressed()),this,SLOT(slotArtBrisiStavku()));
    connect(ui->btnART_Izmijena,SIGNAL(pressed()),this,SLOT(slotArtEdit()));
    connect(ui->btnIspisStol,SIGNAL(pressed()),this,SLOT(slotUpitZaIspisStol()));
    connect(ui->btnCRM,SIGNAL(pressed()),this,SLOT(on_btnCRM_Pressed()));
    connect(ui->btnDokOdvajanja,SIGNAL(pressed()),this,SLOT(on_btnDokOdvajanje_Pressed()));

    //connect(ui->tableView->horizontalHeader(),SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),ui->tableView,SLOT();
    ui->tableView->setSortingEnabled(true);
    //connect(ui->tableView->horizontalHeader(),SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),this,SLOT(slotSortColumn(int,Qt::SortOrder)));
    //this->setFixedSize(1024,768);
    dbLocal = QSqlDatabase::database("local.sqlite");
    ui->pushButton->setVisible(false);
    ui->pushButton->setEnabled(false);
    SamoZakljucajTimer = new QTimer(this); //timer za samozakljucaj
    IspisKuhinja = false;
    ui->txtPORUKA->setVisible(false);
    //fcrm = new frmcrm(this);

    ui->btnDokOdvajanja->setEnabled(false);
    ui->btnDokOdvajanja->setVisible(false);
    ui->btnCRMMakniVezu->setVisible(false);
    _CRMID.clear();

    OdabraniStol = 0;
    OdabraniNarID = 0;


//    modelArtikli = new QSqlQueryModel();

    //ui->btnCRM->setEnabled(false);
//    IspisKuhinjaStol=0;


}
frmKasa::~frmKasa()
{
    delete ui;
}
bool frmKasa::ProvjeraPoruka()
{
    QSqlDatabase db = QSqlDatabase::database("baza");
    QString Qveri = "";
    QSqlQuery q("",db);

    Qveri = "select * from poruke where aktivna='1'";
    if (q.exec(Qveri))
    {
        QString PorukaMSG = "";
        int pPoruka = q.record().indexOf("poruka");
        int pDozvoljen_rad = q.record().indexOf("dozvoljen_rad");
        bool Dozvoljen_Rad = true;
        while (q.next())
        {
            if (q.value(pDozvoljen_rad).toString() == "0")
            {
                Dozvoljen_Rad = false;
            }
            PorukaMSG += QString("\r%1").arg(q.value(pPoruka).toString());
        }
        if (!Dozvoljen_Rad)
        {
            porukaKreni(QString(tr("Pricekajte dok se ne zavrsi obrada\r%1")).arg(PorukaMSG),10000);
            return false;
        }
    }
    return true;
}

void frmKasa::ProvjeraNePoslanihRacuna()
{
//    modelHeader->setQuery("select r1.*,rf.odgovor,rf.potvrden from rac1 r1 left join rac1fiskal rf on r1.id=rf.racid "
//                          " where length(r1.zki)=32 and rf.odgovor is null group by r1.id order by id desc",dbH);


    //return;
    QSqlDatabase db = QSqlDatabase::database("baza");
    QString Qveri = "";
    QSqlQuery q("",db);

    Qveri = "select * from poruke";
    if (q.exec(Qveri))
    {
        QString PorukaMSG = "";
        int pPoruka = q.record().indexOf("poruka");
        int pDozvoljen_rad = q.record().indexOf("dozvoljen_rad");
        bool Dozvoljen_Rad = true;
        while (q.next())
        {
            if (q.value(pDozvoljen_rad).toString() == "0")
            {
                Dozvoljen_Rad = false;
            }
            if (!Dozvoljen_Rad)
            {
                porukaKreni(QString(tr("Pricekajte dok se ne zavrsi obrada\r%1")).arg(q.value(pPoruka).toString()),5000);
            }
        }
    }

    Qveri = "select count(r1.id) imaih from rac1 r1 left join rac1fiskal rf on r1.id=rf.racid"
    " where length(r1.zki)=32 and rf.odgovor is null group by r1.id order by r1.id desc";
    if (!q.exec(Qveri))
    {
        qDebug() << "greska provjera neposlanih" << q.lastError() << q.lastQuery();
    }else
    {
        if (!q.next())
        {
            ui->txtPORUKA->setVisible(false);
            return;
        }
        if (q.value(0).toDouble() > 0)
        {
            ui->txtPORUKA->setVisible(true);
            QMessageBox::warning(this,tr("VAZNO"),tr("Imate racuna koji nisu poslani preko fiskalizacije\nPonovite slanje iz pregleda racuna"),"OK");
        }else
        {
            ui->txtPORUKA->setVisible(false);
        }
    }

}

void frmKasa::porukaKreni(QString PrikaziPoruku, int VrijemeTrajanja)
{
    PorukaInfo.setParent(this);
    PorukaInfo.setText(PrikaziPoruku);
    QFont  serifFont("Times", 18, QFont::Bold);
    PorukaInfo.setFont(serifFont);
    PorukaInfo.setWordWrap(true);
    //PorukaInfo.setFixedSize(300,100);
    PorukaInfo.show();
    PorukaInfo.move((this->width()/2)-(PorukaInfo.width()/2),(this->height()/2)-(PorukaInfo.height()/2));
    PorukaInfo.setStyleSheet("color: rgb(255, 0, 0);");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(porukaStop()));
    timer->start(VrijemeTrajanja);
}
void frmKasa::porukaStop()
{
    PorukaInfo.setText("");
    PorukaInfo.setVisible(false);
}

void frmKasa::slotUpitZaIspisStol()
{
/*
    bool ok;
    QString NNN = QInputDialog::getText(this,QString("Ispis %1 br ? ").arg(constStoPise),QString("Ispis %1 na cekanju").arg(constStoPise), QLineEdit::Normal,"1", &ok);
    if (ok && !NNN.isEmpty())
    {
        //slotIspisStol(NNN.toInt());
        frmIspisKuhinja *fis = new frmIspisKuhinja();
        fis->BrStola = NNN.toInt();
        fis->show();
    }*/
    frmIspisKuhinja *fis = new frmIspisKuhinja();
    fis->show();
    fis->BrStola = ui->txtOdabraniStol->text().toInt();
    fis->UcitajPostavke();



}
void frmKasa::slotF8()
{
    if (qApp->property("Printer-Ladica").toString() != "1")
        return;
    /*
    QFile file("/tmp/ispLadica.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);
    ispisMali *isp = new ispisMali();
    //isp->UcitajPostavkePrintera();



    ispRac << isp->printerLadicaPrefix << isp->printerLadicaSuffix;
    ispRac << isp->printerResetPrefix << isp->printerResetSuffix;
    file.close();
    //qDebug() << "Ladica:" << ispRac <<":";

    QString Komanda;
    QString PrinterPort;
    if (!qApp->property("Printer-Port").isNull())
    {
        PrinterPort = qApp->property("Printer-Port").toString();
    }else
    {

        QMessageBox::warning(this,"Printer","Nemate definiran port za ispis\nPokusavam ispis na /dev/lp0","OK");
        PrinterPort = "/dev/lp0";
    }
    if (!qApp->property("Printer-Konverzija").isNull())
    {
        Komanda = QString("cat /tmp/ispLadica.txt | iconv -f UTF8 -t %1 -o %2" ).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
    }else
    {
        Komanda= QString("cat /tmp/ispLadica.txt >> %1").arg(PrinterPort);
    }
//    Komanda = "lpr -o raw /opt/CashDrawer.txt";
*/
    QString PrinterPort;
    if (!qApp->property("Printer-Port").isNull())
    {
        PrinterPort = qApp->property("Printer-Port").toString();
    }else
    {

        QMessageBox::warning(this,tr("Printer"),tr("Nemate definiran port za ispis\nPokusavam ispis na /dev/lp0"),"OK");
        PrinterPort = "/dev/lp0";
    }
    QString Komanda = "./ladica.py";
    qDebug() << system(Komanda.toUtf8().constData());
    Komanda = QString("cat /tmp/ispLadica.txt >> %1").arg(PrinterPort);
    system(Komanda.toUtf8().constData());
}
void frmKasa::prikaziSkladCjenik()
{

    QString txtS=QString("KasaID: %1 ").arg(qApp->property("Firma_OznNapUr").toString());
    ui->lbl_Sklad_Cjenik->setText(txtS);
    QSqlDatabase dbB = QSqlDatabase::database("baza");
    if (!dbB.open())
    {
        qDebug() << "Greska otvaranje baze - PrikaziSkladCjenik";
        return;
    }
    QSqlQuery q("",dbB);
    if (!q.exec(QString("select naziv from matsklad where id=%1").arg(qApp->property("Prog-SID").toInt())))
    {
        qDebug() << "Greska dohvata prikaza sklad";
        return;
    }
    q.next();
    txtS += QString("Skladiste: %1").arg(q.value(0).toString());
    ui->lbl_Sklad_Cjenik->setText(txtS);
    if (!q.exec(QString("select naziv from cjenik where id=%1").arg(qApp->property("Prog-CID").toInt())))
    {
        qDebug() << "Greska dohvata cjeinka";
        return;
    }
    q.next();
//    QString txtS=ui->lbl_Sklad_Cjenik->text();
    txtS += QString(",Cjenik: %1").arg(q.value(0).toString());
    ui->lbl_Sklad_Cjenik->setText(txtS);
}

void frmKasa::UcitajSveNakonForme()
{
    UcitajStolove();
    UcitajLocal();

    int i=0;

    while (UcitajOpcijeLocal() == false){
        if (i>4)
            break;
        i++;
    }
    ui->lblKorisnikIme->setText(qApp->property("KorisnikIme").toString());
    QPalette pl;
    if (qApp->property("KorisnikBoja").toString() != "")
    {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        pl.setColor(QPalette::Background,QColor(qApp->property("KorisnikBoja").toString()));
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
//        pl.setColor(QPalette::Background,QColor(qApp->property("KorisnikBoja").toString()));
        pl.setColor(QPalette::Window, QColor(qApp->property("KorisnikBoja").toString()));
#endif

        ui->lblKorisnikIme->setAutoFillBackground(true);
        ui->lblKorisnikIme->setPalette(pl);
    }

    //ui->lblKorisnikIme->
    //ui->statusbar->showMessage("Kasa:" + qApp->property("Firma_KasaID").toString()+ " " + qApp->property("Firma_Ime").toString());
    ui->lineEdit->setText("");
    //ui->lineEdit->setFocus();
    prikaziSkladCjenik();
    SamoZakljucavanje();
    ui->tableView->setFocus();
    ui->lineEdit->setFocus();
    //FormaZadnjiRacunPrikazi->setWindowFlags(Qt::WindowMinimized);
}

void frmKasa::PosloziGrid()
{
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(2,true);
    //ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(6,true);
    //ui->tableView->setColumnHidden(7,true);
    ui->tableView->setColumnWidth(4,500);
    ui->tableView->setColumnWidth(3,120);

    QSettings setTBvi;
    if (!setTBvi.value("kasa_tableView_postavke").isNull())
    {
        ui->tableView->horizontalHeader()->restoreState(setTBvi.value("kasa_tableView_postavke").toByteArray());
    }

}

void frmKasa::UcitajStolove()
{

    if (qApp->property("List-BrziRacun").toString() == "1")
    {
        ui->btnBrziRacun->setVisible(true);
        ui->btnBrziRacun->setEnabled(true);
    }else
    {
        ui->btnBrziRacun->setVisible(false);
        ui->btnBrziRacun->setEnabled(false);
    }
    if (UcitavanjeForme)
        return;

    //dbLocal = QSqlDatabase::addDatabase("QSQLITE","local.sqlite");
    //dbLocal.setDatabaseName("local.sqlite");
    if (qApp->property("List-Prikaz").toString() != "1")
    {
        ui->txtOdabraniStol->setText("1");
        OdabraniStol = 1;
        ui->verticalLayout->setEnabled(false);
        ui->btnBrisiStol->hide();
        ui->btnDodajStol->hide();
        ui->tableStolovi->hide();
        ui->btnOdaberiStol->hide();
        ui->btnF4->setEnabled(false);
        ui->btnF4->hide();
//        ui->btnArtikli->setEnabled(false);
        ui->btnIspisStol->setEnabled(false);
        ui->btnIspisStol->setVisible(false);
        ui->chkStolSamoMoji->setVisible(false);
        return;
    }else
    {
        //ui->txtOdabraniStol->setText("1");
        ui->verticalLayout->setEnabled(true);
        ui->btnBrisiStol->show();
        ui->btnDodajStol->show();
        ui->tableStolovi->show();
        ui->btnOdaberiStol->show();
        ui->btnF4->setEnabled(true);
        ui->btnF4->show();
//        ui->btnArtikli->setEnabled(true);
        ui->btnIspisStol->setEnabled(true);
        ui->btnIspisStol->setVisible(true);
        ui->chkStolSamoMoji->setVisible(false);
    }
    QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
    if (!dbL.open())
    {
        porukaKreni("UcitaStol-Nema pristup bazi\npokusajte za par sekundi",10000);
        return;
    }
    //QSqlQueryModel *model = new QSqlQueryModel();
    //model->setQuery("select dok,uid from rac2 group by dok",dbL);
    QSqlQuery qs("",dbL);
    QString qveri;
    if (ui->chkStolSamoMoji->isChecked())
    {
        qveri =QString("select dok,uid,narudzba,narucio_uid,narid from rac2 where uid=%1 group by dok,narid").arg(qApp->property("KonobarID").toString());
    }else
    {
        qveri = "select dok,uid,narudzba,narucio_uid,narid from rac2 group by dok,narid";
    }

    if (!qs.exec(qveri))
    {
        qDebug() << "UcitajStol:Greska " << qs.lastError() << qs.lastQuery();
        return;
    }
    int i=0;
    StoloviModel->clear();
    while (qs.next())
    {
        QStandardItem *red1 = new QStandardItem(QString("%1").arg(qs.value(qs.record().indexOf("dok")).toString()));
        QStandardItem *red2 = new QStandardItem(QString("%1").arg(qs.value(qs.record().indexOf("uid")).toString()));
        QStandardItem *red3;
        QStandardItem *red4 = new QStandardItem(QString("%1").arg(qs.value(qs.record().indexOf("narid")).toString()));
        QSqlQuery q("",QSqlDatabase::database("baza"));

        if (q.exec(QString("select k.puninaziv,kc.boja from korisnik k left join korisnik_boje kc on k.id=kc.uid where k.id=%1").arg(qs.value(qs.record().indexOf("uid")).toString())))
        {
            if (q.next())
            {
                //QStandardItem *red3 = new QStandardItem(QString("%1").arg(q.value(q.record().indexOf("PuniNaziv")).toString()));
                red3 = new QStandardItem(QString("%1").arg(q.value(q.record().indexOf("PuniNaziv")).toString()));
            }else
            {
                red3 = new QStandardItem("");
            }
        }else
        {
            red3 = new QStandardItem("");
        }

        //red1->setBackground(QColorDialog::getColor());
        //red1->setBackground(QColor(1, 1, 0, 0.498039));
        if (q.value(q.record().indexOf("boja")).toString() == "")
        {
           red1->setBackground(Qt::white);
        }else
        {
           red1->setBackground(QColor(q.value(q.record().indexOf("boja")).toString()));
        }
        qDebug() << "Stol nar: " << qs.value(qs.record().indexOf("narudzba")).toString();
        if (qs.value(qs.record().indexOf("narudzba")).toString() == "1")
            red1->setBackground(Qt::red);
        /*
        red1->setEditable(false);
        red2->setEditable(false);
        red3->setEditable(false);
        */
        //qDebug() << red1->background().color().toRgb();
        StoloviModel->setItem(i,0,red1);
        StoloviModel->setItem(i,1,red2);
        StoloviModel->setItem(i,2,red3);
        StoloviModel->setItem(i,3,red4);
        i++;
    }
    QStandardItem *H1 = new QStandardItem("DOK");
    QStandardItem *H2 = new QStandardItem("UID");
    QStandardItem *H3 = new QStandardItem("Korisnik");
    QStandardItem *H4 = new QStandardItem("NarID");
    StoloviModel->setHorizontalHeaderItem(0,H1);
    StoloviModel->setHorizontalHeaderItem(1,H2);
    StoloviModel->setHorizontalHeaderItem(2,H3);
    StoloviModel->setHorizontalHeaderItem(3,H4);

    //ui->tableStolovi->setModel(model);
    ui->tableStolovi->setModel(StoloviModel);
    ui->tableStolovi->setColumnHidden(1,true);
    ui->tableStolovi->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* fora za background
    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(QPointF(50, 50), QPointF(200, 200));
    linearGradient.setColorAt(0, Qt::white);
    linearGradient.setColorAt(1, Qt::green);
    palette->setBrush(QPalette::Base,*(new QBrush(linearGradient)));

    ui->tableStolovi->setPalette(*palette);
    */


    dbL.close();
    UcitajLocal();
    //ProvjeraNePoslanihRacuna();
    /*
    if (!ProvjeraKorisnikStol())
    {
        QMessageBox::warning(this,"DOK","Taj dokument je vec otvorio drugi korisnik","OK");
        ui->txtOdabraniStol->setText("");
        UcitajLocal();
        return;
    }
    */
}
void frmKasa::UcitajLocal()
{
    if (UcitavanjeForme)
        return;

    //dbLocal = QSqlDatabase::database("local.sqlite");
    QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
    if (!dbL.open())
    {
        porukaKreni("UcitaLoc-Nema pristup bazi\npokusajte za par sekundi",10000);
        return;
    }
    //josip
    QSqlQuery q("",dbL);
    if (qApp->property("KonobarPrava").toInt() != 99){
        if (!q.exec(QString("select count(*) from ispkuhinja where dok=%1 and narid=%2").arg(OdabraniStol).arg(OdabraniNarID)))
            qDebug() << "Greska " << q.lastError();
        q.next();
        if (q.value(0).toInt() > 0)
        {
            IspisKuhinja = true;
            ui->btnART_Brisi->setVisible(false);
            ui->btnART_Izmijena->setVisible(false);
        }else{
            IspisKuhinja = false;
            ui->btnART_Brisi->setVisible(true);
            ui->btnART_Izmijena->setVisible(true);
        }
    }else
    {
        ui->btnART_Brisi->setVisible(true);
        ui->btnART_Izmijena->setVisible(true);
    }
    if (qApp->property("KonobarPrava").toInt()<99 && !ProvjeriStolVlasnik(OdabraniStol,OdabraniNarID))
    {
        ui->txtOdabraniStol->setText("");
        OdabraniStol = NULL;
        OdabraniNarID = 0;
        return;
    }
//    QSqlQueryModel *model = new QSqlQueryModel();
    modelArtikli = new QSqlQueryModel();
    modelArtikli->setQuery(QString("select * from rac2 where dok=%1 and narid=%2 order by id desc")
                           .arg(OdabraniStol).arg(OdabraniNarID),dbL);
    ui->tableView->setModel(modelArtikli);

    int i;
    for (i=5;i<=modelArtikli->columnCount();i++)
    {
        ui->tableView->setItemDelegateForColumn(i,new mojiHeaderi(this));
    }
    QString qveri = QString("select * from rac2 where dok=%1 and kol=0").arg(OdabraniStol);
    if (!q.exec(qveri))
    {
        qDebug() << "Greska " << q.lastError();
    }else
    {
        while (q.next())
        {
            //QMessageBox::warning(this,"PAZNJA",QString("Imate kolicina 0 za artikal %1").arg(q.value(q.record().indexOf("naziv")).toString()),"OK");
//            PorukaInfo.setText(QString("Imate kolicina 0 za artikal %1").arg(q.value(q.record().indexOf("naziv")).toString()));
            porukaKreni(QString(tr("Na stolu %2 imate kolicina 0 za artikal %1")).arg(q.value(q.record().indexOf("naziv")).toString()).arg(OdabraniStol),20000);
        }
    }

    q.exec(QString("select sum(ukupno),sum(kol*povratnanaknada) from rac2 where dok=%1 and narid=%2")
           .arg(OdabraniStol).arg(OdabraniNarID));
    double sSUMA=0;
    double sPovratna=0;
    if (q.next())
    {
        sSUMA = q.value(0).toDouble();
        sPovratna = q.value(1).toDouble();
    }
    ui->lblSuma->setText(QString("%L1").arg(sSUMA,0,'f',2));

    q.exec(QString("select porez_sifra,sum(iznos_poreza) i from rac4 where dok=%1 and narid=%2 group by porez_sifra")
           .arg(OdabraniStol).arg(OdabraniNarID));
    double BPDV=0;
    double Porezi=0;
    QString ns;
    QString nsp;
    while (q.next())
    {
        Porezi += q.value(1).toDouble();
        nsp += QString("<tr><td>%1</td><td align=right>%2</td></tr>").arg(q.value(0).toString()).arg(q.value(1).toDouble(),0,'f',2);
    }
    BPDV = sSUMA - sPovratna - Porezi;
    ns= QString("<html><head><body><table><tr><td>BPDV</td><td align=\"right\">%1</td>").arg(BPDV,0,'f',2);
    ns += nsp;
    if (sPovratna != 0)
        ns += QString("<tr><td>Povratna</td><td align=\"right\">%1</td></tr>").arg(sPovratna,0,'f',2);
    ns += QString("</tr></table></body></head></html>");
    ui->lblSumaOST->setText(ns);
    dbL.close();
    //table->selectionModel()->select(index, QItemSelectionModel::Select);
    PosloziGrid();
    frmCRMLabelaPrikaz();
    ui->lineEdit->setText("");
}

bool frmKasa::eventFilter(QObject *target, QEvent *event)
{
    QString NewStyle;
    QString Pomoc = tr("F1-Pomoc\nEnter-potrvrda\nDel-brisanje");
    QKeyEvent *keyEvent = (QKeyEvent *)event;


    if (target == ui->txtPORUKA)
    {
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMessageBox::information(this,tr("VAZNO"),tr("Imate ne poslanih racuna prema poreznoj"),"OK");
            porukaKreni(tr("Imate ne poslanih racuna prema poreznoj"),10000);

        }
    }
    if (target == ui->lineEdit )
    {
        //SamoZakljucavanje();
            if (event->type() == QEvent::FocusIn)
            {
                UcitajStolove();
                UcitajLocal();
                ui->lineEdit->selectAll();
                ui->lineEdit->setFocus();
                ui->btnART_Brisi->setVisible(false);
                ui->btnART_Izmijena->setVisible(false);
                ui->btnKrajSmjeneDana->setVisible(true);
                ui->btnPregRacuna->setVisible(true);
                SamoZakljucajTimerReset();

            }
//#ifdef Q_OS_LINUX
            else if (event->type() == QEvent::KeyPress)
//#endif
//#ifdef Q_OS_WIN*/
//            else if (event->type() == QEvent::KeyRelease)
//#endif
            {
                SamoZakljucajTimerReset();
                switch (keyEvent->key())
                {
                    case Qt::Key_F9:
                        SamoZakljucajTimer->stop();
                        slotBrziRacun();
                        break;
                    case Qt::Key_F8:
                        if (keyEvent->modifiers() == Qt::ShiftModifier)
                        {
                            SamoZakljucajTimer->stop();
                            ispisDelStolova();
                        }else{
                            slotF8();
                        }
                        break;
                    case Qt::Key_F3:
                        slotPromijenaKonobara();
                        break;
                    case Qt::Key_F4:
                        slotF4();
                        break;
                    case Qt::Key_F12:
                        SamoZakljucajTimer->stop();
                        slotF12();
                        break;
                    //case Qt::Key_Insert:
                        //slotDodajStol();
                      //  break;
                    case Qt::Key_F1:
                        SamoZakljucajTimer->stop();
                        Pomoc += QString(tr("\n********\nF2-Menu\nF12-Zakljuci %1\nF4-%1 na cekanju\nInsert-Dodaj novi %1")).arg(constStoPise);
                        QMessageBox::information(this,tr("Pomoc ") ,Pomoc,"OK");
                        SamoZakljucavanje();
                        return true;
                        break;
                    case Qt::Key_F2:
                    //meni
                        slotF2();
                        break;
                    case Qt::Key_Enter:
                    case Qt::Key_Return:
                        //slotOdaberiStol();
                        //slotArtikli();
                        if (!ProvjeraPoruka())
                            return false;
                        if (provjeraCjenika())
                        {
                            if (ui->txtOdabraniStol->text().toInt() == 0)
                            {
                                OdabraniStol = ui->txtOdabraniStol->text().toInt();
                                QMessageBox::warning(this,tr("Vazno"),QString(tr("Niste odabrali niti jedan %1")).arg(constStoPise),QMessageBox::Ok);
                                ui->lineEdit->setFocus();
                                //return true;
                                //break;
                            }else
                            {
                                SamoZakljucajTimer->stop();
                                if (!mutex.tryLock())
                                    return false;
                                slotArtiklTrazi(ui->lineEdit->text());
                                mutex.unlock();

                            }
                        }
                        return true;
                        break;
                    case Qt::Key_Escape:
                        int ret = QMessageBox::warning(this,tr("Izlaz iz programa") ,tr("Da li zelite izaci iz programa?"),
                                 QMessageBox::Yes | QMessageBox::Default,
                                 QMessageBox::No | QMessageBox::Escape);
                        if (ret == QMessageBox::Yes)
                        {
                            qApp->exit();
                        }
                        break;
                }
            }
    } else if (target == ui->tableView )
    {
        if (event->type() == QEvent::FocusIn)
        {

            PamtiStyle = ui->tableView->styleSheet();
            NewStyle = ui->tableView->styleSheet();
            NewStyle += "\n";
            NewStyle += "border-color: rgb(255, 0, 0);"
                    "border-width: 2px;"
                    "border-style: solid;";
            ui->tableView->setStyleSheet(NewStyle);
            if (!IspisKuhinja)
            {
                ui->btnART_Brisi->setVisible(true);
                ui->btnART_Izmijena->setVisible(true);
            }else
            {
                ui->btnART_Brisi->setVisible(false);
                ui->btnART_Izmijena->setVisible(false);
            }

            ui->btnKrajSmjeneDana->setVisible(false);
            ui->btnPregRacuna->setVisible(false);
            UcitajLocal();
        }
        else if (event->type() == QEvent::FocusOut)
        {
            ui->tableView->setStyleSheet(PamtiStyle);
        }
//#ifdef Q_OS_LINUX
            else if (event->type() == QEvent::KeyPress)
/*#endif
#ifdef Q_OS_WIN
            else if (event->type() == QEvent::KeyRelease)
#endif*/
        {
            SamoZakljucajTimerReset();
            switch (keyEvent->key())
            {
                case Qt::Key_Return:
                case Qt::Key_Enter:
                    if (provjeraCjenika())
                    {
                        slotArtEdit();
                    }
                    break;
                case Qt::Key_F1:
                    QMessageBox::information(this,tr("Pomoc,"),Pomoc,QMessageBox::Ok);
                    break;
                case Qt::Key_Delete:
                    slotArtBrisiStavku();
                    return true;
                    break;
            }
        }
    } else if (target == ui->tableStolovi)
    {
        if (event->type() == QEvent::KeyRelease)
        {
            SamoZakljucajTimerReset();
            switch (keyEvent->key())
            {
                case Qt::Key_Return:
                case Qt::Key_Enter:
                    KliknutStol();
                    break;
                case Qt::Key_Delete:
                    slotBrisiStol();
                    //int ret = QMessageBox::warning(this,"PAZNJA","Da li zelite obrisati racun")
                    break;
            }
        }else if (event->type() == QEvent::FocusIn)
        {
            PamtiStyle = ui->tableStolovi->styleSheet();
            NewStyle= ui->tableStolovi->styleSheet();
            NewStyle += "\n";
            NewStyle += "border-color: rgb(255, 0, 0);"
                    "border-width: 2px;"
                    "border-style: solid;";
            ui->tableStolovi->setStyleSheet(NewStyle);
        }else if (event->type() == QEvent::FocusOut)
        {
            ui->tableStolovi->setStyleSheet(PamtiStyle);
        }
    }else
    {
        if (event->type() == QEvent::KeyRelease)
        {
            SamoZakljucajTimerReset();
            switch (keyEvent->key())
            {
                case Qt::Key_F1:
                    QMessageBox::information(this,tr("Pomoc"),Pomoc,"OK");
            }
        }else if (event->type() == QEvent::FocusIn)
        {
            if (target == ui->lineEdit)
            {
                //qDebug() << "SSS";
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

void frmKasa::slotArtBrisiStavku()
{
    if (!DozvolaBrisanjaArt())
        return;
    if (qApp->property("Prog-MakniPitajBrisanjeStavke").toString() != "1")
    {
        int ret = QMessageBox::warning(this,tr("Brisanje stavke"),tr("Da li zelite obrisati stavku ?"),QMessageBox::Yes|QMessageBox::No);
        if (ret != QMessageBox::Yes)
            return;
    }
    QSqlQuery qbr("",QSqlDatabase::database("local.sqlite"));
    int Rac2ID = modelArtikli->record(ui->tableView->currentIndex().row()).value("id").toInt();

    if (LogRac)
    {
        QString DatumBrisanja = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        if (!qbr.exec(QString("insert into log_rac2 select r.*,'%2' from rac2 r where r.id=%1").arg(Rac2ID).arg(DatumBrisanja)))
            qDebug() << "greska upis u log_rac2 " << qbr.lastError() << qbr.lastQuery();
        if (!qbr.exec(QString("insert into log_rac4 select r.*,'%2' from rac4 r where r2id=%1").arg(Rac2ID).arg(DatumBrisanja)))
            qDebug() << "greska upis u log_rac4 " << qbr.lastError() << qbr.lastQuery();
    }
    qbr.prepare("delete from rac2 where id=:ID");
//    int Rac2ID = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt();
    qbr.bindValue(":ID",Rac2ID);
    if (!qbr.exec())
    {
        QMessageBox::warning(0,"Greska u sql-u", "Greska kod brisanja stavke");
        qDebug() << qbr.lastError() << qbr.lastQuery();
        return;
    }
    if (!qbr.exec(QString("delete from rac4 where r2id=%1").arg(Rac2ID)))
        qDebug() << "Greska brisanja rac4" << qbr.lastError() << qbr.lastQuery();
    UcitajLocal();
    //ui->lineEdit->setFocus();
}


void frmKasa::exec()
{
    UcitajStolove();
    //ui->txtOdabraniStol->setText(model->record(ui->tableStolovi->currentIndex().row()).value("dok").toString());
    ui->tableStolovi->selectRow(0);
    ui->txtOdabraniStol->setText(ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toString());
    OdabraniStol = ui->txtOdabraniStol->text().toInt();
    UcitajLocal();
    ui->lineEdit->setFocus();
}


void frmKasa::frmCRMLabelaPrikaz()
{
    if (_CRMID.value(ui->txtOdabraniStol->text().toInt()))
    {
        ui->lblCRM->setVisible(true);
        ui->lblCRM->setText(QString(tr("Dodjeljen CRM-ID %1")).arg(_CRMID.value(ui->txtOdabraniStol->text().toInt())));
        ui->lblCRM->setStyleSheet("background-color: rgb(0, 255, 127);");
        ui->btnCRMMakniVezu->setVisible(true);
    }else
    {
        ui->lblCRM->setVisible(false);
        ui->btnCRMMakniVezu->setVisible(false);
    }
}

void frmKasa::slotIzlazIzPrograma()
{
    int ret = QMessageBox::warning(this,tr("Izlaz iz programa") ,tr("Da li zelite izaci iz programa?"),
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::No | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {
        qApp->exit();
    }
}
void frmKasa::slotPregledRacuna()
{
    //QRect qRect(QApplication::desktop()->screenGeometry());

    frmPregDokum *fPreg;
    fPreg = new frmPregDokum(this);
    //fPreg->show();
    fPreg->setWindowFlags(Qt::Window);
    fPreg->showMaximized();
    //int iXpos=qRect.width()/2-fPreg->width()/2;
    //int iYpos=qRect.height()/2-fPreg->height()/2;
    //fPreg->move(iXpos,iYpos);
}
void frmKasa::slotKrajSmjeneDana()
{
    frmKrajDana *wk;
    wk = new frmKrajDana();
    wk->show();
}
void frmKasa::slotF12()
{
    if (ui->tableView->model()->rowCount()<1)
        return;

//    QRect qRect = QApplication::desktop()->availableGeometry();




    fz = new frmZakljuci(this);

    fz->show();
    fz->BrojStola = ui->txtOdabraniStol->text().toInt();
    fz->ProvjeraBrRacuna();
//    int iXpos=qRect.width()/2-fz->width()/2;
//    int iYpos=qRect.height()/2-fz->height()/2;
    //fz->move(iXpos,iYpos);

}
void frmKasa::slotF2()
{

    QMenu *meniF2;
    meniF2 = new QMenu(ui->lineEdit);
    QAction *pAction1;
    QAction *pAction2;
    QAction *pAction3;
    QAction *pAction4;
    QAction *pAction5;
    QAction *pAction6;
    QAction *pAction7;

    pAction1 = new QAction(tr("Izlaz"), this);
    pAction2 = new QAction(tr("Pregled racuna"), this);
    pAction3 = new QAction(tr("Radni nalog"),this);
    pAction4 = new QAction(tr("Zaprimanje robe"),this);
    pAction5 = new QAction(tr("Opcije"),this);
    pAction6 = new QAction(tr("Ponovno ucitavanje opcija"),this);
    pAction7 = new QAction(tr("O programu"),this);

    meniF2->addAction(pAction1);
    meniF2->addAction(pAction2);
    meniF2->addAction(pAction3);
    meniF2->addAction(pAction4);
    meniF2->addAction(pAction5);
    meniF2->addAction(pAction6);
    meniF2->addAction(pAction7);

    pItem = meniF2->exec(ui->lineEdit->mapToGlobal(QPoint(0,0)));
    if(pItem == pAction1)
    {
        slotIzlazIzPrograma();
    }else if(pItem == pAction2)
    {
        slotPregledRacuna();
    }
    else if(pItem == pAction4)
    {
        //zaprimanje robe
    }else if (pItem == pAction3)
    {
        //radni nalog
        frmrnalog *frn = new frmrnalog();
        frn->showMaximized();

    }else if (pItem == pAction5)
    {
        frmOpcije *fOp = new frmOpcije();
        fOp->show();
        /*
        QString Omeni;
        Omeni = "Vlasnik programa:\n" + qApp->property("Firma_Ime").toString() + "\nOiB: " + qApp->property("Firma_OIB").toString();
        QMessageBox::about(this,"O meni",Omeni);
        //qDebug() << "Fime " << qApp->property("Firma_Ime").toString();
        */
    }else if (pItem == pAction6)
    {
        UcitajOpcijeLocal();
    }else if (pItem == pAction7)
    {
        oMeni *om = new oMeni(this);
        om->setWindowModality(Qt::WindowModal);
        om->show();
        om->Prikazi();
    }
    //return true;
}

void frmKasa::KreirajMeniGumbice()
{
    //QResource::registerResource("FIskalKasaResource.rcc");
    QPushButton *btnM0 = new QPushButton(QObject::tr("F2-Meni"),new QWidget);
    QPushButton *btnM1 = new QPushButton(QObject::tr("F4-Odabir stola"),new QWidget);
    QPushButton *btnM2 = new QPushButton(QObject::tr("F12-Napravi racun"),new QWidget);
    QPushButton *btnF3 = new QPushButton(QObject::tr("F3-Promijena korisnika"),new QWidget);
    QPushButton *btnM3 = new QPushButton(QObject::tr("&Pregled racuna"),new QWidget);
    QPushButton *btnM4 = new QPushButton(QObject::tr("&Kraj smjene/dana"),new QWidget);
    QPushButton *btnM5 = new QPushButton(QObject::tr("&Izlaz"),new QWidget);
    QPushButton *btnM6 = new QPushButton(QObject::tr("F6"),new QWidget);
    //btnM0->setIcon(QIcon(":/48x48/meni_48x48.png"));


    //QPixmap buttonImage("argazkiak/flecha.bmp");
    //QIcon Icon;
    //Icon.addPixmap ( p, QIcon::Normal, QIcon::Off );

    //btnM0->setIcon(Icon);
    //btnM0->setIconSize(QSize(22,22));
    //btnM0->setStyleSheet("image: url(:/16x16/icons/16x16/actions/ok.png);");
    //btnM0->setFixedWidth(200);
//    btnM0->setStyleSheet("background-image: url(:/16x16/icons/16x16/actions/ok.png"
//            "background-repeat: no-repeat;"
//            "background-position: center center");
    //btnM0->setIcon(QIcon(":/16x16/icons/16x16/actions/ok.png"));
    btnM0->setIcon(QIcon(":/48x48/icons/48x48/meni_48x48.png"));
    btnM2->setIcon(QIcon(":/48x48/icons/48x48/napravi_racun.png"));
    btnM3->setIcon(QIcon(":/48x48/icons/48x48/pregled_racuna.png"));
    btnM4->setIcon(QIcon(":/48x48/icons/48x48/kraj_smjene.png"));
    btnF3->setIcon(QIcon(":/48x48/icons/48x48/konobar_48x48.png"));
    btnM1->setIcon(QIcon(":/48x48/icons/48x48/stol.png"));

    btnM0->setIconSize(QSize(24,24));
    btnM1->setIconSize(QSize(24,24));
    btnM2->setIconSize(QSize(24,24));
    btnF3->setIconSize(QSize(24,24));
    btnM3->setIconSize(QSize(24,24));
    btnM4->setIconSize(QSize(24,24));
    btnM5->setIconSize(QSize(24,24));

    ui->layoutGumbi->addWidget(btnM0);
    ui->layoutGumbi->addWidget(btnM1);
    ui->layoutGumbi->addWidget(btnM2);
    ui->layoutGumbi->addWidget(btnF3);
    ui->layoutGumbi->addWidget(btnM3);
    ui->layoutGumbi->addWidget(btnM4);
    ui->layoutGumbi->addWidget(btnM5);
    ui->layoutGumbi->addWidget(btnM6);
    btnM0->setStyleSheet("image: url(:/16x16/icons/16x16/actions/ok.png);");
    //ui->layoutGumbi->SetMinimumSize(20,50);
    //connect(btnM1,SIGNAL(pressed()),this,slotF4());
    connect(btnM1,SIGNAL(pressed()),this,SLOT(slotF4()));
    connect(btnM2,SIGNAL(pressed()),this,SLOT(slotF12()));
    connect(btnM3,SIGNAL(pressed()),this,SLOT(slotPregledRacuna()));
    connect(btnM4,SIGNAL(pressed()),this,SLOT(slotKrajSmjeneDana()));
    connect(btnM5,SIGNAL(pressed()),this,SLOT(slotIzlazIzPrograma()));
    connect(btnF3,SIGNAL(pressed()),this,SLOT(slotPromijenaKonobara()));
    connect(btnM0,SIGNAL(pressed()),this,SLOT(slotF2()));
    connect(btnM6,SIGNAL(pressed()),this,SLOT(slotTest()));
}
void frmKasa::slotPromijenaKonobara()
{

    frmLogin *fl = new frmLogin();
    fl->PromjenaKorisnikaP(true);
    fl->show();
    //this->hide();
    this->close();
}
void frmKasa::slotTest()
{

}

bool frmKasa::ProvjeraKorisnikSifra()
{
    QString Sifra = QInputDialog::getText(this,tr("Zaporka korisnika"),tr("Upisite svoju zaporku"),QLineEdit::Password);
    //qDebug() << Sifra << " " << qApp->property("KorisnikSifra").toString();

    if (qApp->property("KorisnikSifra").toString() != Sifra)
    {
        QSqlQuery q("",QSqlDatabase::database("baza"));
        if (!q.exec(QString("select * from korisnik where active='A' and sifra='%1'").arg(Sifra)))
        {
            QMessageBox::warning(this,tr("Korisnik"),tr("Greska kod dohvata korisnika"),"OK");
            return false;
        }
        if (!q.next())
        {
            QMessageBox::warning(this,tr("Korisnik"),tr("Nepostojeci korisnik ili kriva zaporka"),"OK");
            return false;
        }
        qApp->setProperty("KorisnikSifra",q.value(q.record().indexOf("sifra")).toString());
        qApp->setProperty("KorisnikIme",q.value(q.record().indexOf("puninaziv")).toString());
        qApp->setProperty("KonobarOIB",q.value(q.record().indexOf("oib")).toString());
        qApp->setProperty("KonobarPrava",q.value(q.record().indexOf("dozvole")).toString());
        qApp->setProperty("KonobarID",q.value(q.record().indexOf("id")).toInt());
        ui->lblKorisnikIme->setText(qApp->property("KorisnikIme").toString());
    }
    return true;
}

void frmKasa::slotOdaberiStol()
{
    KliknutStol();
}
void frmKasa::slotBrisiStol()
{
    //if (!ui->tableStolovi->foc)
      //  return;

    if (qApp->property("KonobarPrava").toInt() != 99 && !ProvjeriStolVlasnik(ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toInt(),OdabraniNarID))
        return;
    if (!ProvjeraKorisnikStol())
        return;

    qDebug() << qApp->property("KonobarPrava").toInt();
    if (qApp->property("KonobarPrava").toInt() < 10)
    {
        QMessageBox::information(this,tr("Brisanje stola"),tr("Nemate prava za brisanja stola"),"OK");
        return;
    }
    QString BrojStola = ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toString();
    if (BrojStola == "")
        return;
    OdabraniNarID = ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),3)).toInt();
    int ret = QMessageBox::warning(this,tr("PAZNJA"),QString(tr("Da li zelite obrisati %1?\nBR: %2")).arg(constStoPise).arg(BrojStola),
                                       QMessageBox::Yes|QMessageBox::Default,
                                       QMessageBox::No|QMessageBox::Escape);

        if (ret == QMessageBox::Yes)
        {
            QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
            if (!dbL.open())
            {
                porukaKreni("BrisiStol-Nema Pristup bazi\npokusajte za par sekundi",10000);
                return;
            }

            QSqlQuery q("",dbL);
            //QString DatumBrisanja = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
            QString DatumBrisanja = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            if (!q.exec(QString("insert into del_stol "
                           " select r.*,'%2' from rac2 r where r.dok=%1 and r.narid=%3").arg(BrojStola).arg(DatumBrisanja).arg(OdabraniNarID)))
            {
                qDebug() << "Greska brisanja stola DEL_STOL : " << q.lastError() << q.lastQuery();
            }
            if (!q.exec(QString("insert into del_stol4 select r.*,'%2' from rac4 r where r.dok=%1 and r.narid=%3").arg(BrojStola).arg(DatumBrisanja).arg(OdabraniNarID)))
                qDebug() << "Greska brisanja stola DEL_STOL4 : " << q.lastError() << q.lastQuery();
            //.arg(ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toString())

            q.prepare("delete from rac2 where dok=:DOK and narid=:NARID");
            //int brStola = ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toString();
            q.bindValue(":DOK",BrojStola);
            q.bindValue(":NARID",OdabraniNarID);
            if (!q.exec())
            {
                QMessageBox::warning(0,"Greska u sql-u", "Greska kod brisanja dokumenta na cekanju");
                qDebug() << q.lastError();
                dbL.close();
            }

            if (!q.exec(QString("delete from rac4 where dok=%1 and narid=%2").arg(BrojStola).arg(OdabraniNarID)))
            {
                QMessageBox::warning(0,"Greska u sql-u", "Greska kod brisanja dokumenta na cekanju");
                qDebug() << q.lastError();
                dbL.close();
            }
            q.exec(QString("delete from ispKuhinja where dok=%1 and narid=%2").arg(BrojStola).arg(OdabraniNarID));
            dbL.close();

            QSqlQuery qn("",QSqlDatabase::database("baza"));
            if (!qn.exec(QString("update narudzba1 set status='brisana' where id=%1").arg(OdabraniNarID)))
                qDebug() << "Greska update nar statusa-" << qn.lastError() << qn.lastQuery();

            UcitajStolove();
            QModelIndex index = ui->tableStolovi->model()->index(0,0);
            ui->tableStolovi->setCurrentIndex(index);
            //_CRMID[ui->txtOdabraniStol->text().toInt()] = 0;
            _CRMID.remove(ui->txtOdabraniStol->text().toInt());

            OdabraniNarID =0 ;
            KliknutStol();
        }
}

void frmKasa::slotArtikli()
{

    // test
    /*
    frmPregart *ff = new frmPregart(this);
    ui->horizontalLayout_2->addWidget(ff);
    ff->StalnoUpaljen = true;
    ff->BrojStola = ui->txtOdabraniStol->text().toInt();
    connect(ff,SIGNAL(VratiAID(int,QString,QString,double,double,double,double,int,int)),this,SLOT(UpisArt(int,QString,QString,double,double,double,double,int,int)));
    return;
    */
    // kraj test

    if (!ProvjeraPoruka())
        return;
    if (provjeraCjenika())
    {
        if (ui->txtOdabraniStol->text().toInt() == 0)
        {
            QMessageBox::warning(this,tr("Vazno"),QString(tr("Niste odabrali niti jedan %1")).arg(constStoPise),QMessageBox::Ok);
            ui->lineEdit->setFocus();
            //return true;
            //break;
        }else
        {
            SamoZakljucajTimer->stop();
            if (!mutex.tryLock())
                return;
            slotArtiklTrazi(ui->lineEdit->text());
            mutex.unlock();
        }
    }
//    return true;
//    slotArtiklTrazi();
    return;
}
bool frmKasa::UcitajOpcijeLocal()
{
    if (UcitavanjeForme)
        return false;
    bool StoVracam = false;
    //qDebug() << "UcitajOpcije";
    //ucitavanje lokalnih opcija iz local.sqlite
    //i pucanje u qApp.setproperty
    QSqlDatabase dbO = QSqlDatabase::database("local.sqlite");
    QSqlQuery q("",dbO);
    if (!dbO.open())
    {
        QMessageBox::warning(this,tr("Opcije"),tr("Greska kod ucitavanja opcija\rPokrenite ponovno program"),"OK");
        qApp->exit();
        qDebug() << dbO.lastError();
        return StoVracam;
    }
    QString qveri;
    qveri = "select sto,sifra,value1 from opcije where sto in ('Printer','Certifikati','List','Polog','Prog','IspisStol','Mail')";
    if (!q.exec(qveri))
    {
        //QMessageBox::warning(this,"Opcije","Greska kod ucitavanja opcija\nPonovno pokrenite program","OK");

        qDebug() << "Greska kod ucitavanja opcija";
        qDebug() << q.lastError();
        qDebug() << q.lastQuery();
        return StoVracam;
    }
    while (q.next())
    {
        QString PropImeT = QString("%1-%2").arg(q.value(0).toString()).arg(q.value(1).toString());
        //PropIme += "-";
        //PropIme += q.value(1).toChar();
        //qDebug() << PropImeT;
        QByteArray ba = PropImeT.toUtf8();
        const char* PropIme = ba.data();
        QVariant PropVal = q.value(2).toString();
//        qDebug() << PropIme << PropVal.toString();
        qApp->setProperty(PropIme,PropVal.toString());
        StoVracam = true;
    }
    constStoPise = qApp->property("List-").toString();
    QString PorukaDolje;


    PorukaDolje = QString(tr("Kasa:%1 Firma ime:%2 SID: %3 CID: %4")).arg(qApp->property("Firma_KasaID").toString())
            .arg(qApp->property("Firma_Ime").toString()).arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt());
    //ui->statusbar->showMessage("Kasa:" + qApp->property("Firma_KasaID").toString()+ " " + qApp->property("Firma_Ime").toString() +
    //                           " SID:" + qApp->property("Prog-SID").toInt() + " CID:" + qApp->property("Prog-CID").toInt());
    ui->statusbar->showMessage(PorukaDolje);
    ui->btnF4->setText(QString(tr("F4-Odabir\n%1")).arg(qApp->property("List-").toString()));

    //lbl_poruka_sklad_cjenik.setText("SKLAD");
    //ui->statusbar->addWidget(lbl_poruka_sklad_cjenik);
    if (qApp->property("Printer-Ladica").toString() != "1")
    {
        ui->btnF8->setHidden(true);
    }else
    {
        ui->btnF8->setVisible(true);
        ui->btnF8->setHidden(false);
        ui->btnF8->show();
    }

    return StoVracam;
}
void frmKasa::slotArtEdit()
{
    if (!DozvolaBrisanjaArt())
        return;

    if (qApp->property("KonobarPrava").toInt() != 99)
    {
        if (IspisKuhinja)
        {
            QMessageBox::warning(this,tr("PAZNJA"),tr("Ovaj dok je ispisan za kuhinju\ri nemate prava vise brisat stavke"),"OK");
            return;
        }
    }
    if (ui->tableView->currentIndex().row() < 0)
        return;
    frmDodajStavku *fd = new frmDodajStavku(this);
    fd->setWindowModality(Qt::WindowModal);
    fd->show();
    fd->slotRadimSto("Update");
    fd->BrojStola = ui->txtOdabraniStol->text().toInt();
    fd->NarID = OdabraniNarID;
    fd->NarucioUID= 0;
    fd->artID = modelArtikli->record(ui->tableView->currentIndex().row()).value("aid").toInt();
    fd->Rac2ID = modelArtikli->record(ui->tableView->currentIndex().row()).value("id").toInt();
    fd->ui->txtArtJedCijena->setText(modelArtikli->record(ui->tableView->currentIndex().row()).value("pc").toString());
    fd->ui->txtArtKolicina->setText(modelArtikli->record(ui->tableView->currentIndex().row()).value("kol").toString());
               // ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),5)).toString());
    fd->ui->txtArtNazivOriginal->setText(modelArtikli->record(ui->tableView->currentIndex().row()).value("naziv").toString());
                //ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),4)).toString());
    fd->ui->txtArtRabatP->setText(modelArtikli->record(ui->tableView->currentIndex().row()).value("rabatp").toString());
                //ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),9)).toString());
    fd->ui->txtArtSifra->setText(modelArtikli->record(ui->tableView->currentIndex().row()).value("artsif").toString());
                //ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),3)).toString());
//    fd->ui->

    fd->artGrupaID = modelArtikli->record(ui->tableView->currentIndex().row()).value("grupaid").toInt();
            //ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),13)).toInt();
    fd->artIspisKuhinja = modelArtikli->record(ui->tableView->currentIndex().row()).value("ispis_kuhinja").toInt();
            //ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),14)).toInt();
    fd->ui->txtArtPovratnaNaknada->setText(modelArtikli->record(ui->tableView->currentIndex().row()).value("povratnanaknada").toString());
   qDebug() <<  modelArtikli->record(ui->tableView->currentIndex().row()).value("povratnanaknada").toInt();
    fd->ui->txtArtKolicina->selectAll();
    fd->ui->txtArtKolicina->setFocus();
    fd->Suma();
}
void frmKasa::UpisArt(int aid,QString artsif,QString artnaziv,double artKol,
                      double artPC,double artRabat,double artPovratnaNaknada,int artGrupaID,int artIspisKuhinja)
{
    if (aid == 0)
        return;

    QString LineArtSifra = artsif;

//    qDebug() << LineArtSifra.mid(0,6);

    clsIzracuni *IZRAC = new clsIzracuni();
    IZRAC->DefVrijednostVarijabli();

    //classIzracuni ci;// = new classIzracuni();
    int RacID;// = 0;
    int RacDok;
    int i=0;
    bool ImaNema = false;
    for (i;i<ui->tableView->model()->rowCount();i++)
    {
        //qDebug() << "Lart " << LineArtSifra << ","  << ui->tableView->model()->data(ui->tableView->model()->index(i,3)).toString();
        if (modelArtikli->record(ui->tableView->currentIndex().row()).value("aid").toInt() == aid)
        {
            ImaNema= true;
            //AID = ui->tableView->model()->data(ui->tableView->model()->index(i,2)).toInt();
            //LineArtSifra = ui->tableView->model()->data(ui->tableView->model()->index(i,3)).toString();
            RacID = modelArtikli->record(i).value("id").toInt();
                    //ui->tableView->model()->data(ui->tableView->model()->index(i,0)).toInt();
            artKol = modelArtikli->record(i).value("kol").toDouble();
                    //ui->tableView->model()->data(ui->tableView->model()->index(i,5)).toDouble()+1;
            artGrupaID = modelArtikli->record(i).value("grupaid").toInt();
                    //ui->tableView->model()->data(ui->tableView->model()->index(i,14)).toInt();
            artIspisKuhinja = modelArtikli->record(i).value("ispis_kuhinja").toInt();
                    //ui->tableView->model()->data(ui->tableView->model()->index(i,15)).toInt();
            RacDok = modelArtikli->record(i).value("dok").toInt();
                    //ui->tableView->model()->data(ui->tableView->model()->index(i,1)).toInt();
            artRabat = modelArtikli->record(i).value("rabatp").toDouble();
                    //ui->tableView->model()->data(ui->tableView->model()->index(i,9)).toDouble();
            artPC = modelArtikli->record(i).value("pc").toDouble();
                    //ui->tableView->model()->data(ui->tableView->model()->index(i,7)).toDouble();
            artnaziv = modelArtikli->record(i).value("naziv").toString();
                  //  ui->tableView->model()->data(ui->tableView->model()->index(i,4)).toString();
            artPovratnaNaknada = modelArtikli->record(i).value("povratnanaknada").toInt();
                    //ui->tableView->model()->data(ui->tableView->model()->index(i,17)).toDouble();
        }
    }
    /*
    if (ci.DodajStavku(LineArtSifra,AID,RacID,ui->txtOdabraniStol->text().toInt(),artKolicina))
    {
        UcitajLocal();
        ui->lineEdit->setText("");
        ui->lineEdit->setFocus();
        return;
    }
    */
    if (ImaNema)
    {
        //ima vec ta stavka na racunu
        IZRAC->set_StavkaAID(aid,LineArtSifra);
        IZRAC->set_StavkaArtNaziv(artnaziv);
        IZRAC->set_StavkaKol(artKol);
        IZRAC->set_StavkaPC(artPC);
        IZRAC->set_StavkaRabatP(artRabat);
        IZRAC->set_StavkaPovratnaNaknada(artPovratnaNaknada);
        IZRAC->set_StavkaPodaci(ui->txtOdabraniStol->text().toInt(),artGrupaID,artIspisKuhinja,RacID,OdabraniNarID);
        IZRAC->izracunMPC();
        IZRAC->UpisStavkeUBazu();
    }else
    {
        //nova stavka na racun
        IZRAC->set_StavkaAID(aid,LineArtSifra);
        IZRAC->set_StavkaArtNaziv(artnaziv);
        IZRAC->set_StavkaKol(artKol);
        IZRAC->set_StavkaPC(artPC);
        IZRAC->set_StavkaRabatP(artRabat);
        IZRAC->set_StavkaPovratnaNaknada(artPovratnaNaknada);
        IZRAC->set_StavkaPodaci(ui->txtOdabraniStol->text().toInt(),artGrupaID,artIspisKuhinja,0,OdabraniNarID);
        IZRAC->izracunMPC();
        IZRAC->UpisStavkeUBazu();
    }
    UcitajLocal();
    ui->lineEdit->setText("");
    return;
}

// pretraga artikla prema zadanim parametrima pretrage po sljedecem redoslijedu
// usporedba sa "grupa-" ako se ide na pregled grupe artikala
// - barcode (ako je zadano)
// - sifra (ako je zadano)
// - naziv od pocetka ako je zadano
void frmKasa::slotArtiklTrazi(QString artiklPart)
{
    // QString LineArtSifra = ui->lineEdit->text();
    QString ArtiklFindByThisString = artiklPart;

//    qDebug() << LineArtSifra.mid(0,6);
    if (ArtiklFindByThisString.mid(0,6) == "grupa-")
    {
        // odabrano pregled grupe artikala
        UcitajPregledArt(ArtiklFindByThisString,true);
    }
    else
    {
        QString artiklSifraThatWeFound = "";
        bool bArtiklFounded = false;

        // pretraga po barcode (ako je definirano)
        if (qApp->property("Prog-PretragaArtBcode").toString() == "1")
        {
            clsIzracuni *cIzracuni = new clsIzracuni();
            bArtiklFounded = cIzracuni->FindArtiklSifraByBarcode(ArtiklFindByThisString,artiklSifraThatWeFound);
        }

        if (!bArtiklFounded) //ako nije nadeno, trazi po sifri (ako je definirano)
            if (qApp->property("Prog-PretragaArtSif").toString() == "1")
            {
                clsIzracuni *cIzracuni = new clsIzracuni();
                bArtiklFounded = cIzracuni->FindArtiklSifraBySifra(ArtiklFindByThisString,artiklSifraThatWeFound);
            }

        if (!bArtiklFounded) //ako nije nadeno, trazi po nazivu (ako je definirano)
            if (qApp->property("Prog-PretragaArtLike").toString() == "1")
            {
                clsIzracuni *cIzracuni = new clsIzracuni();
                bArtiklFounded = cIzracuni->FindArtiklSifraByName(ArtiklFindByThisString,artiklSifraThatWeFound);
            }

        // nastaviti obradu dodavanja artikla (sukladno postavkama)
        if (bArtiklFounded) //ako je artikl pronaden kreni dalje obradivat
        {
            bool bArtiklZapisanDirektNaRacun=false; // (da se zna da li je obraden zahtijev za dodavanjem artikla)

            clsIzracuni *IZRAC = new clsIzracuni();
            IZRAC->DefVrijednostVarijabli();

            // pretraga artikla

            // if (qApp->property("Prog-AkoImaArt+1kol").toString() == "1" && ArtiklFindByThisString != "")
            if (qApp->property("Prog-AkoImaArt+1kol").toString() == "1" && artiklSifraThatWeFound != "")
            {
                // ako je definirano da poveca kolicinu ako je artikl vec na listi onda pretraziti listu

                //classIzracuni ci;// = new classIzracuni();
                int AID;//= 0;
                int RacID;// = 0;
                int RacDok;
                double artKolicina = 1;
                double artRabatP =0;
                double artPC = 0;
                double artPovratnaNaknada = 0;
                int artGrupaID = 0;
                int artIspKuhijna = 0;
                int stavkaNarucioUID=0;
                QString ArtNaziv = "";
                bool ImaArtiklaNaRacunu = false; //oznaka da je artikl vec na racunu
                int i=0;

                //pretraga liste artikala na racunu
                for (i;i<ui->tableView->model()->rowCount();i++)
                {
                    //qDebug() << "Lart " << LineArtSifra << ","  << ui->tableView->model()->data(ui->tableView->model()->index(i,3)).toString();
                    //if (modelArtikli->record(i).value("artsif").toString() == ArtiklFindByThisString)
                    if (modelArtikli->record(i).value("artsif").toString() == artiklSifraThatWeFound)
                    {
                        // Artikl sa tom sifrom pronaden na listi
                        ImaArtiklaNaRacunu= true;
                        AID = modelArtikli->record(i).value("aid").toInt();
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,2)).toInt();
                        RacID = modelArtikli->record(i).value("id").toInt();
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,0)).toInt();
                        artKolicina = modelArtikli->record(i).value("kol").toDouble()+1;
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,5)).toDouble()+1;
                        artGrupaID = modelArtikli->record(i).value("grupaid").toInt();
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,14)).toInt();
                        artIspKuhijna = modelArtikli->record(i).value("ispis_kuhinja").toInt();
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,15)).toInt();
                        RacDok = modelArtikli->record(i).value("dok").toInt();
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,1)).toInt();
                        artRabatP =  modelArtikli->record(i).value("rabatp").toDouble();
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,9)).toDouble();
                        artPC = modelArtikli->record(i).value("pc").toDouble();
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,7)).toDouble();
                        ArtNaziv = modelArtikli->record(i).value("naziv").toString();
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,4)).toString();
                        artPovratnaNaknada = modelArtikli->record(i).value("povratnanaknada").toDouble();
                                //ui->tableView->model()->data(ui->tableView->model()->index(i,17)).toDouble();
                        stavkaNarucioUID = modelArtikli->record(i).value("narucio_uid").toInt();
                    }
                }
                /*
                if (ci.DodajStavku(LineArtSifra,AID,RacID,ui->txtOdabraniStol->text().toInt(),artKolicina))
                {
                    UcitajLocal();
                    ui->lineEdit->setText("");
                    ui->lineEdit->setFocus();
                    return;
                }
                */
                if (ImaArtiklaNaRacunu)
                {
                    //ima vec ta stavka na racunu
                    IZRAC->set_StavkaAID(AID,artiklSifraThatWeFound);
                    IZRAC->set_StavkaArtNaziv(ArtNaziv);
                    IZRAC->set_StavkaKol(artKolicina);
                    IZRAC->set_StavkaPC(artPC);
                    IZRAC->set_StavkaRabatP(artRabatP);
                    IZRAC->set_StavkaPovratnaNaknada(artPovratnaNaknada);
                    IZRAC->set_StavkaNarucioUID(stavkaNarucioUID);
                    IZRAC->set_StavkaPodaci(ui->txtOdabraniStol->text().toInt(),artGrupaID,artIspKuhijna,RacID,OdabraniNarID);
                    IZRAC->izracunMPC();
                    IZRAC->UpisStavkeUBazu();
                    bArtiklZapisanDirektNaRacun = true;
                }
                else
                    bArtiklZapisanDirektNaRacun = false;
            }

            if (!bArtiklZapisanDirektNaRacun)
            {
                //artikl nije pronaden na racunu - nastaviti traziti na drugi nacin
                //IZRAC->set_StavkaAID(0,ArtiklFindByThisString);
                IZRAC->set_StavkaAID(0,artiklSifraThatWeFound);
                IZRAC->set_StavkaKol(1);
                IZRAC->set_StavkaNarucioUID(0);
                IZRAC->set_StavkaPodaci(ui->txtOdabraniStol->text().toInt(),0,0,0,OdabraniNarID);
                if (!IZRAC->UcitajArt())
                {   //ovdje se nebi smjelo doci jer je sifra vec pronadena, ali eto za svaki slucaj
                    UcitajPregledArt(ArtiklFindByThisString,false);
                    return;
                }
                else
                {
                    // pronaden artikl
                    if (qApp->property("Prog-ArtPitaKolicinu").toString() != "1")
                    {
                        // Nije ArtPitaKolicinu pa odmah  izracunati za kolicinu 1
                        IZRAC->izracunMPC();
                        IZRAC->UpisStavkeUBazu();
                    }
                    else
                    {
                        //ako je u opcijama ArtPitaKolicinu dize prozor frmdodajstavku sa podacima artikla
                        //nova stavka na racun
                        frmDodajStavku *fds = new frmDodajStavku(this);
                        fds->setWindowModality(Qt::WindowModal);
                        fds->show();
                        fds->ui->txtArtSifra->setText(ArtiklFindByThisString);
                        fds->ui->txtArtJedCijena->setText(QString("%1").arg(IZRAC->get_stavkaPC()));
                        fds->ui->txtArtNazivOriginal->setText(IZRAC->get_stavkaNaziv());
                        fds->ui->txtArtPovratnaNaknada->setText(QString("%1").arg(IZRAC->get_stavkaPovratnaNaknada()));
                        fds->ui->txtArtKolicina->selectAll();
                        fds->ui->txtArtKolicina->setFocus();
                        fds->BrojStola = ui->txtOdabraniStol->text().toInt();
                        fds->NarucioUID=0;
                        fds->NarID = OdabraniNarID;
                        fds->artID = IZRAC->get_stavkaAID();
                        fds->artGrupaID = IZRAC->get_stavkaArtGrupaID();  //model->record(ui->tableView->currentIndex().row()).value("grupaid").toInt();
                        fds->artIspisKuhinja = IZRAC->get_stavkaIspisKuhinja(); //model->record(ui->tableView->currentIndex().row()).value("ispis_kuhinja").toInt();
                        IZRAC->DefVrijednostVarijabli();
                        fds->Suma();
                    }
                }
            }
            UcitajLocal();
            ui->lineEdit->setText("");
            ui->lineEdit->setFocus();
            return;
        }
        else //artikl nije pronaden - ponudi izbornik artikala
        {
            if (ArtiklFindByThisString != "")
            {
                UcitajPregledArt(ArtiklFindByThisString,false);
            }
            else
            {
                UcitajPregledArt("",false);
            }
        }
    }
}
void frmKasa::UcitajPregledArt(QString StoTrazizm,bool grupa)
{
    if (ui->txtOdabraniStol->text() == "" || ui->txtOdabraniStol->text() == "0")
    {
        OdabraniStol = NULL;
        QMessageBox::warning(this,tr("RAC"),tr("Niste odabrali dokument"),"OK");
        return;
    }

    frmPregart *fp;
    fp = new frmPregart(this);
    fp->BrojStola = ui->txtOdabraniStol->text().toInt();
    fp->NarucioUID = 0;

    fp->NarID = OdabraniNarID;
    if (!grupa)
    {
        if (StoTrazizm != "")
        {
            fp->StoTrazim = StoTrazizm;//ui->lineEdit->text();
        }
    }else
    {
        fp->GrupaTrazim = StoTrazizm;
    }
    fp->setWindowModality(Qt::WindowModal);
    fp->setWindowFlags(Qt::Window);
    fp->showMaximized();
    fp->UcitajArtove();

}

void frmKasa::slotBrziRacun()
{
    if (!ui->btnBrziRacun->isEnabled())
        return;

    /*
    int ret = QMessageBox::warning(this,"Brzi racun GOTOVINA" ,"Da li zelite zakljuciti racun?",
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::No | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {*/
    BrziCounter ++;
    //qDebug() << "Sbrzistart" << BrziCounter;
    if (!mutex.tryLock())
    {
        //qDebug() << "Izlaz brzi mutex-tryl:" << BrziCounter;
        return;
    }
    else
    {
        //qDebug() << "els:" << BrziCounter;
    }
   // mutex.lock();


    if (ui->tableView->model()->rowCount()<1)
    {
        UcitajLocal();
        //qDebug() << "nema row";
        mutex.unlock();
        return;
    }
    ui->btnBrziRacun->setEnabled(false);

    if (qApp->property("Prog-KasaZadnjiRacunPrikaz").toString() == "1")
    {
        FormaZadnjiRacunPrikazi->show();
        FormaZadnjiRacunPrikazi->NovaSuma(0);
    }

    slotF8();

    //qDebug() << "brzi -prosao -" << ui->tableView->model()->rowCount();
/*
    QDialog *ProzorS = new QDialog(this);
    QLabel *LBL = new QLabel();
    LBL->setText("Pricekajte dok ne upise racun");
    QFont  serifFont("Times", 18, QFont::Bold);
    LBL->setFont(serifFont);
    LBL->setStyleSheet("color: rgb(255, 0, 0);");
    QVBoxLayout *VBX = new QVBoxLayout();
    VBX->addWidget(LBL);
    ProzorS->setWindowTitle("Upis racuna");
    ProzorS->thread()->setParent(new QThread);
    ProzorS->thread()->start();
    ProzorS->setFixedSize(700,400);
    ProzorS->setLayout(VBX);
    ProzorS->show();
    ProzorS->setFocus();
    */
    clsIzracuni *IZRAC = new clsIzracuni();
    IZRAC->DefVrijednostVarijabli();
    porukaKreni("Pricekajte dok ne upise racun",5000);
    IZRAC->set_RacStol(ui->txtOdabraniStol->text().toInt());
    IZRAC->set_RacNarID(OdabraniNarID);
    //IZRAC->set_RacLocalDOK(ui->txtOdabraniStol->text().toInt());
    IZRAC->set_RacNP(1);
    IZRAC->set_RacVrstaRac("M");
    IZRAC->set_RacRabatP(0);

    //IZRAC->set_CRMID(_CRMID);
    //IZRAC->set_CRMID(_CRMID[ui->txtOdabraniStol->text().toInt()]);
    //_CRMID.isEmpty() << _CRMID.value(ui->txtOdabraniStol->text().toInt());
    if (_CRMID.value(ui->txtOdabraniStol->text().toInt()))
        IZRAC->set_CRMID(_CRMID.value(ui->txtOdabraniStol->text().toInt()));
    IZRAC->UcitajLocalSUMrac();
    IZRAC->izracunRAC();

    FormaZadnjiRacunPrikazi->NovaSuma(IZRAC->get_RacSUM());

    if (!IZRAC->UpisiRac())
    {
        QMessageBox::warning(this,tr("Racun-upis"),tr("Greska kod upisa racuna\rProvjerite mrezne veze i pristup bazi\rili nazovite odrzavanje"),"OK");
    }else
    {
        _CRMID.remove(ui->txtOdabraniStol->text().toInt());
        ispisMali *isp = new ispisMali();
        //frmIspisOdabir *ispOd = new frmIspisOdabir(this);
        //ispOd->show();
        //ispOd->RacunID = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt();
        //ispOd->ProvjeraOpcija();
        frmIspisOdabir *fisp = new frmIspisOdabir();
        fisp->show();
        fisp->RacunID = IZRAC->get_Rac1ID(); // IzracRac->get_Rac1ID();
        fisp->ProvjeraOpcija();
/*
        if (qApp->property("Printer-DEFprinter-Veliki").toString() == "1") {
            isp->ispisVeliki("ispRacun",QString("\"rid=int:%1\"").arg(IZRAC->get_Rac1ID()));
        }else
        {
            isp->IspisMaliPos(QString("%1").arg(IZRAC->get_Rac1ID()));
        }
*/
    }
    OdabraniNarID = 0 ;


    UcitajLocal();
    //qDebug() << "brzi - kraj -" << ui->tableView->model()->rowCount();
//    ProzorS->close();
    porukaStop();
    mutex.unlock();
    ui->btnBrziRacun->setEnabled(true);
//    FormaZadnjiRacunPrikazi->activateWindow();
//    FormaZadnjiRacunPrikazi->raise();
    if (qApp->property("Prog-KasaZadnjiRacunPrikaz").toString() == "1")
    {
        FormaZadnjiRacunPrikazi->setFocus();
    }
}
void frmKasa::slotIspisStol(int StolBR)
{

    if (StolBR == NULL || StolBR == 0)
        return;
    QFile file("/tmp/ispStol.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);
    QSqlDatabase dbO = QSqlDatabase::database("baza");
    dbO.open();
    QSqlQuery q("",dbO);
    ispisMali *isp = new ispisMali();

    dbLocal.open();
    QSqlQuery ql("",dbLocal);
    if (!ql.exec(QString("select naziv,sum(kol) kolicina from rac2  where dok=%1 and narid=%2 group by aid order by naziv")
                 .arg(StolBR).arg(OdabraniNarID)))
        return;
    ispRac << QString(tr("Vrijeme ispisa: %1")).arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm")) << "\n";
    ispRac << QString(tr("Konobar: %1")).arg(qApp->property("KorisnikIme").toString()) << "\n";
    ispRac << QString(tr("Ispis Stol broj: %1")).arg(StolBR);
    ispRac << "\n";
    ispRac << QString("=").repeated(40);
    ispRac << "\n";
    double UkpSUM = 0;
    while (ql.next())
    {
//         QString::number(q.value(1).toDouble(),"f",2);
  //QString(" ").repeated(VM(QString("%L1").arg(q.value(4).toDouble(),0,'f',2),9))  + QString("%L1").arg(q.value(4).toDouble(),0,'f',2)
        ispRac << QString(" ").repeated(isp->VM(QString("%1").arg(ql.value(1).toString()),5));
        ispRac << ql.value(1).toString().mid(0,5);
        ispRac << " - ";
        ispRac << ql.value(0).toString().mid(0,30);
        ispRac << "\n";
        UkpSUM += ql.value(1).toDouble();
    }
    ispRac << QString("=").repeated(40);
    ispRac << QString(tr("Ukp suma: %1")).arg(UkpSUM);
    ispRac << "\n\n\n\n\n\n\n\n";
    dbLocal.close();
    dbO.close();
    ispRac << isp->printerRezacPrefix;
    ispRac << isp->printerResetPrefix;

    file.close();
    QString Komanda;
    QString PrinterPort;
    if (!qApp->property("Printer-Port").isNull())
    {
        PrinterPort = qApp->property("Printer-Port").toString();
    }else
    {
        QMessageBox::warning(this,tr("Printer"),tr("Nemate definiran port za ispis\nPokusavam ispis na /dev/lp0"),"OK");
        PrinterPort = "/dev/lp0";
    }
    if (!qApp->property("Printer-Konverzija").isNull())
    {
        Komanda = QString("cat /tmp/ispStol.txt | iconv -f UTF8 -t %1 -o %2" ).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
    }else
    {
        Komanda= QString("cat /tmp/ispStol.txt >> %1").arg(PrinterPort);
    }
    system(Komanda.toUtf8().constData());
    q.clear();
    dbO.close();

}
bool frmKasa::provjeraCjenika()
{
    QSqlDatabase dbB = QSqlDatabase::database("baza");
    dbB.open();
    QSqlQuery q("",dbB);
    q.exec(QString("call proc_Cjenik_GetIDActualNow(%1,@cid)").arg(qApp->property("Prog-SID").toInt()));
    if (!q.exec("select @cid"))
        qDebug() << q.lastError() << q.lastQuery();
    if (q.next())
    {
        //qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt()
        if (qApp->property("Prog-CID").toInt() == q.value(0).toInt())
        {
            return true;
        }else
        {
            QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
            QSqlQuery ql("",dbL);
            ql.exec("select count(*) from rac2");
            ql.next();
            if (ql.value(0).toInt() > 0)
            {
                porukaKreni("Cjenik se promijenio\nZatvorite otvorene racune",10000);
                return false;
            }
            qApp->setProperty("Prog-CID",q.value(0).toInt());
            prikaziSkladCjenik();
            return true;
        }
    }else
    {
        qDebug() << q.lastError() << q.lastQuery();
    }
    dbB.close();
    return true;
}

void frmKasa::ispisDelStolova()
{
    qDebug() << qApp->property("KonobarPrava").toInt();
    if (qApp->property("KonobarPrava").toInt() < 99)
        return;
    frmIspisDelStolova *ispDS = new frmIspisDelStolova();
    ispDS->show();
}



void frmKasa::on_pushButton_pressed()
{
    /*
    frmStolTool *st = new frmStolTool();
    st->showMaximized();
    st->BrStola = ui->txtOdabraniStol->text().toInt();
    st->UcitajForm();
    */

}
bool frmKasa::ProvjeraKorisnikStol()
{
    if (qApp->property("List-Prikaz").toString() != "1")
        return true;

    if (qApp->property("KonobarPrava").toInt() > 5 )
        return true;
    bool ImaStol = false;
    QSqlQuery ql("",QSqlDatabase::database("local.sqlite"));
    ql.exec(QString("select count(dok) from rac2 where dok=%1 and narid=%2").arg(OdabraniStol).arg(OdabraniNarID));
    ql.next();
    if (ql.value(0).toString() =="0")
    {
        ImaStol = true;
        return true;
    }
    if (!ImaStol)
    {
        ql.exec(QString("select count(*) from rac2 where dok=%1 and uid=%2 and narid=%3")
                .arg(OdabraniStol).arg(qApp->property("KonobarID").toString()).arg(OdabraniNarID));
        ql.next();
        if (ql.value(0).toString() != "0")
        {
            qDebug() << "ProvKorStol izasa - true";
            return true;
        }
    }
    qDebug() << "ProvKorStol izasa - false";
    return false;
}
void frmKasa::SamoZakljucavanje()
{/*
    QSqlQuery ql("",QSqlDatabase::database("local.sqlite"));
    if (!ql.exec("select value1 from opcije where sto='Prog' and sifra='SamoZakljucaj'"))
    {
        qDebug() << ql.lastError() << ql.lastQuery();
        return;
    }
    ql.next();
    */
    int TimerZakljucaj = 0;
//    TimerZakljucaj = ql.value(0).toInt();
    if (qApp->property("Prog-SamoZakljucaj").isNull())
        return;
    TimerZakljucaj = qApp->property("Prog-SamoZakljucajTime").toInt();
    if (TimerZakljucaj == 0)
        return;
    TimerZakljucaj=TimerZakljucaj*1000;
    //qDebug() << "SamoZakljucaj za " << TimerZakljucaj;
    //QTimer *timer = new QTimer(this);
    connect(SamoZakljucajTimer, SIGNAL(timeout()), this, SLOT(slotPromijenaKonobara()));
    connect(SamoZakljucajTimer,SIGNAL(timeout()),SamoZakljucajTimer,SLOT(stop()));
    SamoZakljucajTimer->start(TimerZakljucaj);
}
void frmKasa::SamoZakljucajTimerReset()
{
    SamoZakljucajTimer->stop();
    if (qApp->property("Prog-SamoZakljucaj").isNull())
        return;
    SamoZakljucajTimer->start();
}
void frmKasa::on_btnCRM_Pressed()
{
    fcrm = new frmcrm(this);
    fcrm->setWindowModality(Qt::WindowModal);
    fcrm->showMaximized();
    connect(fcrm,SIGNAL(VratiCRMID(int)),this,SLOT(CrmID(int)));
}
void frmKasa::on_btnDokOdvajanje_Pressed()
{
    frmstolodvajanje *frms = new frmstolodvajanje(this);
    frms->DokBR=ui->txtOdabraniStol->text().toInt();
    frms->show();
    frms->UcitajFormu();
}
void frmKasa::CrmID(int crmid)
{
    //_CRMID = crmid;
    //qDebug() << "CRMID-D1 < " << ui->txtOdabraniStol->text().toInt() << "," << crmid;
    //_CRMID.append(ui->txtOdabraniStol->text().toInt(),crmid);  //[ui->txtOdabraniStol->text().toInt()][crmid];
    _CRMID[ui->txtOdabraniStol->text().toInt()] = crmid;
    //_CRMID.resize();
    //qDebug() << "CRMID-D : " << _CRMID.isEmpty() << _CRMID.value(ui->txtOdabraniStol->text().toInt());
}

void frmKasa::on_btnCRMMakniVezu_pressed()
{
    int ret = QMessageBox::information(this,tr("CRM"),tr("Da li zelite maknuti vezu prema CRM-u?"),QMessageBox::Yes|QMessageBox::Default,
                                       QMessageBox::No|QMessageBox::Escape);
    if (ret != QMessageBox::Yes)
        return;
    _CRMID.remove(ui->txtOdabraniStol->text().toInt());
    UcitajLocal();
}


void frmKasa::slotF4()
{
    //ui->tableStolovi->setFocus();
    slotDodajStol();
}
void frmKasa::slotDodajStol()
{

    if (qApp->property("KonobarPrava").toInt() < 10)
    {
        QString Sifra = QInputDialog::getText(this,tr("Zaporka korisnika"),tr("Upisite svoju zaporku"),QLineEdit::Password);
        //qDebug() << Sifra << " " << qApp->property("KorisnikSifra").toString();

        if (Sifra != qApp->property("KorisnikSifra").toString())
        {
            QMessageBox::warning(this,tr("PAZNJA"),QString(tr("Kriva zaporka za korisnika %1")).arg(qApp->property("KorisnikIme").toString()));
            return;
        }
    }
    bool ok;
    QString NNN = QInputDialog::getText(this,QString(tr("Upisite br %1")).arg(constStoPise),QString(tr("Br %1 :\n(u slucaju da %1 nije otvoren\n otvara se novi)")).arg(constStoPise), QLineEdit::Normal,"1", &ok);
    if (ok && !NNN.isEmpty())
        {
//        qDebug() << "tableStoloviModelRowCount: " << StoloviModel->rowCount();  // << ui->tableStolovi->model()->rowCount();
        for (int i=0;i<ui->tableStolovi->model()->rowCount();i++)
        {
            if (ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(i,0)).toString() == NNN &&
                    ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(i,3)).toInt() == 0)
            {
                qDebug() << "narid** " << ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(i,3)).toInt();
                qDebug() << "narid** " << OdabraniNarID;

                if (qApp->property("KonobarPrava").toInt() != 99 && ProvjeriStolVlasnik(NNN.toInt(),OdabraniNarID))
                {
                    QModelIndex tblIndex = ui->tableStolovi->model()->index(i,0);
                    //ui->tableStolovi->selectionModel()->select(tblIndex,QItemSelectionModel::Select);
                    ui->tableStolovi->setCurrentIndex(tblIndex);
                    KliknutStol();
                    return;
                }else if (qApp->property("KonobarPrava").toInt() == 99)
                {
                    QModelIndex tblIndex = ui->tableStolovi->model()->index(i,0);
                    //ui->tableStolovi->selectionModel()->select(tblIndex,QItemSelectionModel::Select);
                    ui->tableStolovi->setCurrentIndex(tblIndex);
                    KliknutStol();
                    return;
                }
            }
        }
        if (qApp->property("KonobarPrava").toInt() != 99 && ProvjeriStolVlasnik(NNN.toInt(),OdabraniNarID))
        {
            ui->txtOdabraniStol->setText(NNN);
            OdabraniStol = NNN.toInt();
            OdabraniNarID=0;
            UcitajLocal();
            ui->lineEdit->setFocus();
        }else if (qApp->property("KonobarPrava").toInt() == 99)
        {
            ui->txtOdabraniStol->setText(NNN);
            OdabraniStol = NNN.toInt();
            OdabraniNarID=0;
            UcitajLocal();
            ui->lineEdit->setFocus();
        }
        }
}

bool frmKasa::ProvjeriStolVlasnik(int BrDokumenta, int NarID)
{
    if (qApp->property("List-Prikaz").toString() != "1")
        return true;
    if (qApp->property("Prog-SamoSvojDok").toString() != "1")
        return true;

    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    if (!q.exec(QString("select * from rac2 where dok=%1 and narid=%2").arg(BrDokumenta).arg(OdabraniNarID)))
    {
        qDebug() << "Greska Provjere vlasnika stola" << q.lastError() << q.lastQuery();
        return false;
    }
    if (!q.next())
    {
        return true;
    }else {
        //qDebug() << q.value(q.record().indexOf("uid")).toString();
        if (q.value(q.record().indexOf("uid")).toString() != qApp->property("KonobarID").toString())
        {
            QSqlQuery qv("",QSqlDatabase::database("baza"));
            if (!qv.exec(QString("select * from korisnik where id=%1").arg(q.value(q.record().indexOf("UID")).toInt())))
            {
                qDebug() << "ProvjeriStolVlasnik: Greska dohvata korisnika " << q.lastError() << q.lastQuery();
            }
            if (!qv.next())
            {
                //qDebug() << qApp->property("KonobarID").toString();
                QMessageBox::warning(this,tr("Vlasnik dokumenta"),tr("Netko drugi je otvorio taj dokument,pozovite voditelja da ga otvori/zatvori"),"OK");
            }else {
                //qDebug() << qv.value(0) << qv.value(1);
                QMessageBox::warning(this,tr("Vlasnik dokumenta"),QString(tr("%1 je vlasnik ovog dokumenta,nemate prava raditi po ovome dokumentu)"))
                                     .arg(qv.value(qv.record().indexOf("PuniNaziv")).toString()));
            }
            return false;
        }else
        {
            qDebug() << "PROVSTOL-VL-Izasao";
            return true;
        }
    }
}
void frmKasa::KliknutStol()
{

    if (!ui->tableStolovi->currentIndex().isValid())
        return;
    try
    {
        TableStoloviIndexGdjeJe = ui->tableStolovi->selectionModel()->currentIndex();
        ui->txtOdabraniStol->setText( ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toString());
        // NOVO ui->txtOdabraniStol->setText(StoloviModel->index(ui->tableStolovi->currentIndex().row(),0).data().toString());
        OdabraniStol = ui->txtOdabraniStol->text().toInt();
        OdabraniNarID = ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),3)).toInt();
        //ui->lineEdit->setFocus();
        ui->tableStolovi->selectionModel()->select(TableStoloviIndexGdjeJe,QItemSelectionModel::Select);
        UcitajLocal();
        frmCRMLabelaPrikaz();
        if (OdabraniNarID > 0)
        {
            QSqlQuery qu("",QSqlDatabase::database("baza"));
            int NarBR = 0;
            if (!qu.exec(QString("select n.br,k.puninaziv from narudzba1 n left join korisnik k on n.narucio_uid=k.id where n.id=%1")
                    .arg(OdabraniNarID)))
            {
                NarBR=OdabraniNarID;

            }else
            {
                qu.first();
                NarBR = qu.value(0).toInt();
            }

            lblNarudzbaPoruka->setText(QString(tr("Narudzba br: %1 Narucio: %2")).arg(NarBR)
                    .arg(ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),2)).toString()));
            lblNarudzbaPoruka->setMaximumWidth(5000);
            QFont  serifFont("TakaoPGothic", 12, QFont::Bold);
            lblNarudzbaPoruka->setFont(serifFont);
            ui->horizontalLayout_5->addWidget(lblNarudzbaPoruka);

        }else {
            lblNarudzbaPoruka->setText("");
        }

    }
    catch(...){}
    finally:
    {
        ui->lineEdit->setFocus();
    }

}

void frmKasa::on_tableStolovi_doubleClicked()
{
//    if (index < 0)
//        return;
    try
    {
    qDebug() << "Stol-" << ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toInt();
    if (ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toInt() < 0)
        return;
    if (qApp->property("KonobarPrava").toInt() < 10)
    {
        if (!ProvjeriStolVlasnik(ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toInt(),OdabraniNarID))
            return;
        if (!ProvjeraKorisnikSifra())
            return;
    }
    if (qApp->property("KonobarPrava").toInt() !=99 && !ProvjeriStolVlasnik(ui->tableStolovi->model()->data(ui->tableStolovi->model()->index(ui->tableStolovi->currentIndex().row(),0)).toInt(),OdabraniNarID))
    {
        //KliknutStol();
        return;
    }else
    {
        KliknutStol();
    }
    }
    catch (...){
        qDebug() << err.text();

    }

}

void frmKasa::on_chkStolSamoMoji_stateChanged(int arg1)
{
    UcitajStolove();
}
bool frmKasa::DozvolaBrisanjaArt()
{
    if (qApp->property("Prog-ZabranaBrisanjaArt").toString() == "1")
    {
        if (qApp->property("KonobarPrava").toInt() < 15)
        {
                QMessageBox::warning(this,tr("Brisanje/Izmijena stavke"),tr("Nemate dozvolu za brisanje ili izmijenu stavke,\nzovite voditelja"),"OK");
                return false;
        }
    }
    return true;
}

void frmKasa::on_btnSnimiPostavke_released()
{
    QByteArray MyArray = ui->tableView->horizontalHeader()->saveState(); //  ui->tbvTest->horizontalHeader()->saveState();
//    QSettings MySetting(QSettings::IniFormat, QSettings::UserScope, "Test");
//       MySetting.setValue("column_width", MyArray);
    QSettings setTBview;
    setTBview.setValue("kasa_tableView_postavke",MyArray);
}


