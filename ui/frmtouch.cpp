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




#include "frmtouch.h"
#include "ui_frmtouch.h"
#include <QDebug>
//#include <QSint>
#include <QtSql>
#include "ui/frmlogin.h"
#include "ui/frmloginrftag.h"
#include "ui/frmdodajstavku.h"
#include "ui/frmispisodabir.h"
#include "ui/stoodabir.h"
#include "ui/frmkrajdana.h"
//#include <QMutex>

frmtouch::frmtouch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmtouch)
{
    ui->setupUi(this);
    ui->pushButton->setVisible(false);
    //frmTipk = new frmTipkovnica();
    SamoZakljucajTimer = new QTimer(this);
    SamoZakljucavanje(60);
    UcitajFormu();
    //ui->txtStol->installEventFilter(this);
    ui->txtStol->setText("");
//    signalMapper = new QSignalMapper(this);
    UcitajSankove();


    //ui->listRac->setMinimumSize(400,ui->listRac->height());
}

frmtouch::~frmtouch()
{
    delete ui;
}

void frmtouch::SamoZakljucavanje(int Vrijeme)
{
    qDebug() << "SamozakljucajT:" << qApp->property("Prog-SamoZakljucajTime").toInt();
    int TimerZakljucaj = qApp->property("Prog-SamoZakljucajTime").toInt(); //5;
    TimerZakljucaj=TimerZakljucaj*1000;
    if (Vrijeme>0)
        TimerZakljucaj = Vrijeme*1000;
    connect(SamoZakljucajTimer, SIGNAL(timeout()), this, SLOT(slotOdjava()));
    connect(SamoZakljucajTimer,SIGNAL(timeout()),SamoZakljucajTimer,SLOT(stop()));
    SamoZakljucajTimer->start(TimerZakljucaj);
}


void frmtouch::SamoZakljucajTimerReset()
{
    SamoZakljucajTimer->stop();
//    if (qApp->property("Prog-SamoZakljucaj").isNull())
//        return;
    SamoZakljucajTimer->start();
}


bool frmtouch::eventFilter(QObject *target, QEvent *event)
{
    QKeyEvent *keyEvent = (QKeyEvent *)event;
//qDebug() << target->objectName() << target->objectName().mid(0,7);
    if (target->objectName().mid(0,7) == "gumbic-")
    {
        //qDebug() << target->objectName() << target->objectName().mid(7,target->objectName().length()-1);
        if (event->type() == QEvent::MouseButtonRelease)
        {
            VratiTipku(target->objectName().mid(7,target->objectName().length()-1));
        }else if (event->type() == QEvent::KeyRelease)
        {
            if (keyEvent->key() == Qt::Key_Enter)
                VratiTipku(target->objectName().mid(7,target->objectName().length()-1));
        }
    }
    return QWidget::eventFilter(target,event);

}
void frmtouch::txtStol_Tipka(QString Tipka) //(Qt::Key Tipka)
{
    if (Tipka == "BACKSPACE")
    {
        ui->txtStol->setText(ui->txtStol->text().mid(0,ui->txtStol->text().length()-1));
    }else if (Tipka == "CLEAR")
    {
        ui->txtStol->setText("");
    }else if (Tipka == "ENTER")
    {
        UcitajListRac2(ui->txtStol->text().toInt());
        dw->close();
    }else
    {
        ui->txtStol->setText(ui->txtStol->text().append(Tipka));
        SamoZakljucajTimerReset();
//        UcitajListRac2(Tipka.toInt());
    }
}

void frmtouch::UcitajFormu()
{

    QSettings settings;
    ui->splitter->restoreState(settings.value("splitterSizes").toByteArray());

    UcitajOpcije();
    ui->lblStol->setText(constStoPise);
    this->setWindowTitle(QString("Fiskal Kasa v:%1").arg(qApp->property("App_Verzija").toString()));
    QLabel *kon = new QLabel(this);
    kon->clear();
    kon->setText(QString("Korisnik: %1").arg(qApp->property("KorisnikIme").toString()));
    kon->setMaximumHeight(20);
    kon->size().setHeight(25);
    kon->setStyleSheet("color: yellow;");

    QString txtS=QString("  KasaID: %1 ").arg(qApp->property("Firma_OznNapUr").toString());
    //ui->lbl_Sklad_Cjenik->setText(txtS);

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
    if (!q.exec(QString("select naziv from cjenik where id=%1").arg(qApp->property("Prog-CID").toInt())))
    {
        qDebug() << "Greska dohvata cjeinka";
        return;
    }
    q.next();
//    QString txtS=ui->lbl_Sklad_Cjenik->text();
    txtS += QString(",Cjenik: %1").arg(q.value(0).toString());
    kon->setText(kon->text().append(txtS));

    QLayoutItem* item;
    while ( ( item = ui->GornjiLay->layout()->takeAt(0) ) != NULL )
    {
        delete item->widget();
        delete item;
    }

    ui->GornjiLay->addWidget(kon);
    //qDebug() << QString("Sid=%1,Cid=%2").arg(qApp->property("Prog-SID").toInt())
//                .arg(qApp->property("Prog-CID").toInt());
    ui->scrollArea->setStyleSheet("background-image: url(firma_logo.jpg);");
    UcitajGrupe();

}

void frmtouch::UcitajGrupe()
{
    while (QLayoutItem* item = ui->gridZaGrupe->takeAt(0)) //layout->takeAt(0))
    {
         if (QWidget* widget = item->widget())
             delete widget;
    }



    //ui->listBrowseWidget->clear();
    QSqlQuery q("",QSqlDatabase::database("baza"));
    //QString Qveri("select id,opis,glavna,ispis_kuhinja,statisticka from grupe");
    QString Qveri = QString("select g.id,g.opis,g.glavna,g.ispis_kuhinja,g.statisticka from viz_rac a "
                  " left join grupe g on a.grupaid=g.id where a.sid=%1 and a.cid=%2 group by a.grupaid order by g.opis")
            .arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt());
    if (!q.exec(Qveri))
        qDebug() << "greska:" << q.lastQuery() << q.lastError();

    //q.exec(QString("select * from viz_rac"));


    //desniGridLay->setContentsMargins(0,0,0,0);
    int row = 1;
    int col = 1;
    while (q.next())
    {
        QWidget *tt = KreirajGumbicGrupa(q.value(q.record().indexOf("id")).toInt(),q.value(q.record().indexOf("opis")).toString(),
                                         q.value(q.record().indexOf("glavna")).toInt(),q.value(q.record().indexOf("ispis_kuhinja")).toInt(),
                                         q.value(q.record().indexOf("statisticka")).toString(),this);

        ui->gridZaGrupe->addWidget(tt,row,col);
        itemArt = new QListWidgetItem();
        itemArt->setSizeHint(QSize(0,70));

        if (col >=5)
        {
            row++;
            ui->gridZaGrupe->setRowMinimumHeight(row,40);
            ui->gridZaGrupe->setColumnMinimumWidth(col,70);
            col=1;
        }else
        {
            col++;
        }
    }
    ui->scrollAreaGrupe->setMaximumHeight(300);
    ui->scrollAreaGrupe->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 40px; }");
    ui->scrollAreaGrupe->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { height: 40px; }");
   // ui->scrollAreaGrupe->setStyleSheet("background-color: \"#3E606F\";");

}
void frmtouch::UcitajArtikle(int grupaID)
{

    //brisanje svig gumbica
    while (QLayoutItem* item = ui->gridZaDesni->takeAt(0)) //layout->takeAt(0))
    {
         if (QWidget* widget = item->widget())
             delete widget;
    }
    //ui->listBrowseWidget->clear();
    QSqlQuery q("",QSqlDatabase::database("baza"));
    QString Qveri;
    Qveri = QString("select * from viz_rac where sid=%1 and cid=%2").arg(qApp->property("Prog-SID").toInt())
                  .arg(qApp->property("Prog-CID").toInt());
    if (grupaID != 0)
        Qveri.append(QString(" and grupaid=%1").arg(grupaID));

    Qveri.append(QString(" order by naziv"));
   // qDebug() << Qveri;

    if (!q.exec(Qveri))
        qDebug() << "greska:" << q.lastQuery() << q.lastError();

    //q.exec(QString("select * from viz_rac"));
/*
    desniGridLay = new QGridLayout();
    desniGridLay->setSpacing(5);
    desniGridLay->setMargin(10);
    desniGridLay->setHorizontalSpacing(5);
    desniGridLay->setVerticalSpacing(5);
*/
    //desniGridLay->setContentsMargins(0,0,0,0);
    int row = 1;
    int col = 1;
//    q.first();
    while (q.next())
    {
        QPushButton *tt = KreirajGumbicArt(q.value(q.record().indexOf("id")).toInt(),q.value(q.record().indexOf("artsif")).toString(),
                                       q.value(q.record().indexOf("naziv")).toString(),
                                   q.value(q.record().indexOf("pc")).toDouble(),q.value(q.record().indexOf("opis")).toString(),
                                   "KOM","",this);

//        tt->setStyleSheet(ui->btnTemplateArt->styleSheet());
        ui->gridZaDesni->addWidget(tt,row,col);
        itemArt = new QListWidgetItem();
        itemArt->setSizeHint(QSize(0,70));

        if (col >=5)
        {
            row++;
            ui->gridZaDesni->setRowMinimumHeight(row,80);
            ui->gridZaDesni->setColumnMinimumWidth(col,150);
            col=1;
        }else
        {
            col++;
        }
    }
    //ui->scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 20px; }");
//    ui->scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 40px; }");

    ui->scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 30px; } "
                                                       "QScrollBar::up-arrow:vertical { height: 50px; width: 30px } "
                                                       "QScrollBar::down-arrow:vertical { height: 50px; width: 30px }");


    //ui->scrollArea->setStyleSheet("background-color: \"#3E606F\""); //#D1DBBD #91AA9D
    SamoZakljucajTimerReset();
}

void frmtouch::on_btnBrowse_released()
{
    UcitajGrupe();
    SamoZakljucajTimerReset();
    //UcitajArtikle(0);
}

void frmtouch::VratiTipku(QString Tipka)
{
    //qDebug() << Tipka;
    //dodaje u rac2 +1 aid
    //i poziva
    if (ui->txtStol->text().toInt() < 1)
    {
        QMessageBox::warning(this,"Paznja",QString("Niste odabrali %1").arg(constStoPise),"OK");
        return;
    }


    clsIzracuni *IZRAC = new clsIzracuni();
    IZRAC->DefVrijednostVarijabli();
   // qDebug() << QTime::currentTime().toString("hh:mm:ss:zzzz") << "Start Izrac";
    IZRAC->set_StavkaAID(0,Tipka);
//    IZRAC->set_StavkaArtNaziv(ArtNaziv);
    IZRAC->set_StavkaKol(1);
//    IZRAC->set_StavkaPC(artPC);
    IZRAC->set_StavkaRabatP(0);
    IZRAC->set_StavkaPovratnaNaknada(0);
    IZRAC->set_StavkaPodaci(ui->txtStol->text().toInt(),0,0,0,0); //,artGrupaID,artIspKuhijna,0);
    //qDebug() << "Start Izrac Ucitaj";

    IZRAC->UcitajArt();
    //qDebug() << "Start Izrac Izracun";

    IZRAC->izracunMPC();
    //qDebug() << "Start Izrac Upisubazu";

    IZRAC->UpisStavkeUBazu();
    //qDebug() << "UCitajListu";

    UcitajListRac2(ui->txtStol->text().toInt());
    //qDebug() << QTime::currentTime().toString("hh:mm:ss:zzzz") << "Kraj";
    SamoZakljucajTimerReset();


}
QWidget *frmtouch::KreirajGumbicGrupa(int grpID, QString grpOpis, int grpGlavna, int grpIspisKuhinja, QString grpStatisticka, QWidget *parent)
{

    signalMapperGrupa = new QSignalMapper(this);
    //Button *Gumbic = new Button(grpOpis,parent);
    QPushButton *Gumbic = new QPushButton(parent);
    Gumbic->setMinimumSize(80,60);
    Gumbic->setMaximumSize(100,60);
    Gumbic->setStyleSheet( "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 white, stop: 1 blue);"
                          "border-style: solid;"
                          "border-color: beige;"
                          "border-width: 1px;"
                          "border-radius: 25px;"
                           "color: yellow;");


    Gumbic->setText(QString("%1").arg(grpOpis));
//    connect(signalMapper, SIGNAL(mapped(const QString &)),
//            this, SIGNAL(clicked(const QString &)));
    connect(signalMapperGrupa,SIGNAL(mapped(int)),
            this,SLOT(UcitajArtikle(int)));
    connect(Gumbic,SIGNAL(clicked(bool)),signalMapperGrupa,SLOT(map()));
    signalMapperGrupa->setMapping(Gumbic, grpID);


    return Gumbic;
}

QPushButton *frmtouch::KreirajGumbicArt(int artID,QString artSif, QString artNaziv, double artPC, QString artGrupa, QString artJMJ, QString artSlikaPath,QWidget* parent)
{
    //qDebug() << artID << artSlikaPath;


    signalMapperArt = new QSignalMapper(this);
//    Button *Gumbic = new Button(artNaziv,parent);
    QPushButton *Gumbic = new QPushButton(parent);
    Gumbic->setMinimumSize(100,80);
    Gumbic->setMaximumSize(150,100);
    // " "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 white, stop: 1 blue);"
    /*
    Gumbic->setStyleSheet(" background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 white, stop: 1 blue);"
                          "border-width: 1px;"
                          "border-radius: 25px;"
                          "font: 75 11pt \"Trebuchet MS\";"
                          "color: yellow;"
                          "QPushButton:checked { background-color: red; }");
    */
    QString Naziv= "";
    //qDebug() << "**";

    if (artNaziv.length()>13)
    {
        Naziv = artNaziv.mid(0,16);
//        Naziv +=  "\n" + artNaziv.mid(12,artNaziv.length());
        for (int i = 0;i<=artNaziv.length();i=i+16)
        {
            //qDebug() << i << "," << Naziv;
            if (i>15 )
                Naziv+="\n";
            Naziv += artNaziv.mid(i,i);
        }

    }else
    {
        Naziv = artNaziv;
    }
    //qDebug() << "Naziv:" << Naziv;
    //qDebug() << "**";
    Gumbic->setText(QString("%1\n%2-%3 %4").arg(Naziv).arg(artJMJ).arg(artPC).arg(qApp->property("App_VALUTA").toString()));
    connect(signalMapperArt,SIGNAL(mapped(QString)),
            this,SLOT(VratiTipku(QString)));
    connect(Gumbic,SIGNAL(clicked(bool)),signalMapperArt,SLOT(map()));
    signalMapperArt->setMapping(Gumbic, artSif);

    Gumbic->setStyleSheet(" QPushButton {"
                          "color: #ffff00;"
                          "border: 2px solid #555;"
                          "font: 10pt \"Comic Sans MS\";"
                          "border-radius: 11px;"
                          "padding: 5px;"
                          "background: qradialgradient(cx: 0.3, cy: -0.4,"
                          "fx: 0.3, fy: -0.4,"
                          "radius: 1.35, stop: 0 #fff, stop: 1 #0000ff);"
                          "min-width: 80px;"
                          "}"
                          ""
                          "QPushButton:hover {"
                          "background: qradialgradient(cx: 0.3, cy: -0.4,"
                          "fx: 0.3, fy: -0.4,"
                          "radius: 1.35, stop: 0 #fff, stop: 1 #8080ff);"
                          "}"
                          ""
                          " QPushButton:pressed {"
                          "background: qradialgradient(cx: 0.4, cy: -0.1,"
                          "fx: 0.4, fy: -0.1,"
                          "radius: 1.35, stop: 0 #fff, stop: 1 #c0c0ff);"
                          "}");


    return Gumbic;
}


bool frmtouch::UcitajOpcije()
{
    bool StoVracam = false;
    //qDebug() << "UcitajOpcije";
    //ucitavanje lokalnih opcija iz local.sqlite
    //i pucanje u qApp.setproperty
    QSqlDatabase dbO = QSqlDatabase::database("local.sqlite");
    QSqlQuery q("",dbO);
    if (!dbO.open())
    {
        QMessageBox::warning(this,"Opcije","Greska kod ucitavanja opcija\rPokrenite ponovno program","OK");
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


    PorukaDolje = QString("Kasa:%1 Firma ime:%2 SID: %3 CID: %4").arg(qApp->property("Firma_KasaID").toString())
            .arg(qApp->property("Firma_Ime").toString()).arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt());
    //ui->statusbar->showMessage(PorukaDolje);
    //ui->btnF4->setText(QString("F4-Odabir\n%1").arg(qApp->property("List-").toString()));

    if (qApp->property("Printer-Ladica").toString() != "1")
    {
        //ui->btnF8->setHidden(true);
    }else
    {
        /*
        ui->btnF8->setVisible(true);
        ui->btnF8->setHidden(false);
        ui->btnF8->show();
        */
    }
    SamoZakljucajTimerReset();
    return StoVracam;

}

void frmtouch::UcitajLocal()
{

}



void frmtouch::on_btnStolClear_released()
{

   // ui->listRac->clear();
    /*
    if (qApp->property("KonobarPrava").toInt() != 99 && !ProvjeriStolVlasnik(ui->txtStol->text().toInt()))
        return;
    if (!ProvjeraKorisnikStol())
        return;
*/

    //qDebug() << qApp->property("KonobarPrava").toInt();
    if (qApp->property("KonobarPrava").toInt() < 10)
    {
        QMessageBox::information(this,"Brisanje stola","Nemate prava za brisanja stola","OK");
        return;
    }
    QString BrojStola = ui->txtStol->text();
    if (BrojStola == "")
        return;
    int ret = QMessageBox::warning(this,"PAZNJA",QString("Da li zelite obrisati %1?\nBR: %2").arg(constStoPise).arg(BrojStola),
                                       QMessageBox::Yes|QMessageBox::Default,
                                       QMessageBox::No|QMessageBox::Escape);

        if (ret == QMessageBox::Yes)
        {
            QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
            if (!dbL.open())
            {
                //porukaKreni("BrisiStol-Nema Pristup bazi\npokusajte za par sekundi",10000);
                QMessageBox::warning(this,"Greska","BrisiStol-Nema Pristup bazi\npokusajte za par sekundi","OK");
                return;
            }

            QSqlQuery q("",dbL);
            QString DatumBrisanja = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            if (!q.exec(QString("insert into del_stol "
                           " select r.*,'%2' from rac2 r where r.dok=%1").arg(BrojStola).arg(DatumBrisanja)))
            {
                qDebug() << "Greska brisanja stola DEL_STOL : " << q.lastError() << q.lastQuery();
            }
            if (!q.exec(QString("insert into del_stol4 select r.*,'%2' from rac4 r where r.dok=%1").arg(BrojStola).arg(DatumBrisanja)))
                qDebug() << "Greska brisanja stola DEL_STOL4 : " << q.lastError() << q.lastQuery();

            q.prepare("delete from rac2 where dok=:DOK");
            q.bindValue(":DOK",BrojStola);
            if (!q.exec())
            {
                QMessageBox::warning(0,"Greska u sql-u", "Greska kod brisanja dokumenta na cekanju");
                qDebug() << q.lastError();
                dbL.close();
            }

            if (!q.exec(QString("delete from rac4 where dok=%1").arg(BrojStola)))
            {
                QMessageBox::warning(0,"Greska u sql-u", "Greska kod brisanja dokumenta na cekanju");
                qDebug() << q.lastError();
                dbL.close();
            }
            q.exec(QString("delete from ispKuhinja where dok=%1").arg(BrojStola));
            dbL.close();
        }
    UcitajListRac2(ui->txtStol->text().toInt());
    SamoZakljucajTimerReset();
}

void frmtouch::slotOdjava()
{
    SamoZakljucajTimer->stop();
    /*
    frmloginrftag *fl = new frmloginrftag();
    fl->show();
    */
    this->close();
    this->deleteLater();
}

void frmtouch::on_btnLogout_released()
{
    slotOdjava();
}

void frmtouch::UcitajGumberNormalanGui()
{
    if (qApp->property("Prog-GUInarudzba").toString() == "1")
        return;
    QPushButton *btnBrziRac = new QPushButton();
    btnBrziRac->setText(tr("Racun"));
    btnBrziRac->setIcon(QIcon(":/ikone/ikone/napravi_racun.png"));
    btnBrziRac->setIconSize(QSize(30,30));
    btnBrziRac->setStyleSheet(" QPushButton {"
                              "color: #ffff00;"
                              "border: 2px solid #555;"
                              "font: 12pt \"Comic Sans MS\";"
                              "border-radius: 11px;"
                              "padding: 5px;"
                              "background: qradialgradient(cx: 0.3, cy: -0.4,"
                              "fx: 0.3, fy: -0.4,"
                              "radius: 1.35, stop: 0 #fff, stop: 1 #0000ff);"
                              "min-width: 80px;"
                              "}"
                              ""
                              "QPushButton:hover {"
                              "background: qradialgradient(cx: 0.3, cy: -0.4,"
                              "fx: 0.3, fy: -0.4,"
                              "radius: 1.35, stop: 0 #fff, stop: 1 #8080ff);"
                              "}"
                              ""
                              " QPushButton:pressed {"
                              "background: qradialgradient(cx: 0.4, cy: -0.1,"
                              "fx: 0.4, fy: -0.1,"
                              "radius: 1.35, stop: 0 #fff, stop: 1 #c0c0ff);"
                              "}");
    btnBrziRac->setMinimumSize(100,60);
    btnBrziRac->setMaximumSize(150,60);
    QPushButton *btnKrajSmjene = new QPushButton();
    btnKrajSmjene->setText(tr("Kraj smjene"));
    btnKrajSmjene->setIcon(QIcon(":/ikone/ikone/view_statistics.svg"));
    btnKrajSmjene->setIconSize(QSize(30,30));
    btnKrajSmjene->setStyleSheet(" QPushButton {"
                                 "color: #ffff00;"
                                 "border: 2px solid #555;"
                                 "font: 12pt \"Comic Sans MS\";"
                                 "border-radius: 11px;"
                                 "padding: 5px;"
                                 "background: qradialgradient(cx: 0.3, cy: -0.4,"
                                 "fx: 0.3, fy: -0.4,"
                                 "radius: 1.35, stop: 0 #fff, stop: 1 #0000ff);"
                                 "min-width: 80px;"
                                 "}"
                                 ""
                                 "QPushButton:hover {"
                                 "background: qradialgradient(cx: 0.3, cy: -0.4,"
                                 "fx: 0.3, fy: -0.4,"
                                 "radius: 1.35, stop: 0 #fff, stop: 1 #8080ff);"
                                 "}"
                                 ""
                                 " QPushButton:pressed {"
                                 "background: qradialgradient(cx: 0.4, cy: -0.1,"
                                 "fx: 0.4, fy: -0.1,"
                                 "radius: 1.35, stop: 0 #fff, stop: 1 #c0c0ff);"
                                 "}");
    btnKrajSmjene->setMinimumSize(100,60);
    btnKrajSmjene->setMaximumSize(150,60);

    ui->layNaruciGumbi->addWidget(btnKrajSmjene);
    ui->layNaruciGumbi->addWidget(btnBrziRac);

    connect(btnBrziRac,SIGNAL(clicked(bool)),this,SLOT(slotBtnBrziRacun()));
    connect(btnKrajSmjene,SIGNAL(clicked(bool)),this,SLOT(slotBtnKrajSmjene()));
    SamoZakljucajTimerReset();
}

void frmtouch::UcitajSankove()
{
    if (qApp->property("Prog-GUInarudzba").toString() != "1")
    {
        UcitajGumberNormalanGui();
        return;
    }
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    QString Qveri;
    Qveri = QString("select * from opcije_naruci order by id");

    if (!q.exec(Qveri))
    {
        qDebug() << "Greska ucitavanja - sankova -" << q.lastError() << q.lastQuery();
        QMessageBox::warning(this,"Greska","Greska ucitavanja sankova","OK");
        return;
    }

    Button *pregNar = new Button("",this);
    pregNar->setMinimumSize(60,40);
    pregNar->setMaximumSize(150,80);
    pregNar->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 white, stop: 1 blue);"
                           "border-style: solid;"
                           "border-color: beige;"
                           "border-width: 1px;"
                           "border-radius: 25px;"
                            "font: 75 19pt \"Trebuchet MS\";"
                            "color: yellow;");
    pregNar->setText("Pregled");
    connect(pregNar,SIGNAL(clicked(bool)),this,SLOT(UcitajNarudzbe()));

    ui->layNaruciGumbi->addWidget(pregNar);

    while (q.next())
    {
        signalMapperNaruciGumbic = new QSignalMapper(this);
        Button *Gumbic = new Button("ggg",this);
        Gumbic->setMinimumSize(60,40);
        Gumbic->setMaximumSize(150,80);
        Gumbic->setStyleSheet( "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 white, stop: 1 blue);"
                              "border-style: solid;"
                              "border-color: beige;"
                              "border-width: 1px;"
                              "border-radius: 25px;"
                               "font: 75 19pt \"Trebuchet MS\";"
                               "color: yellow;");


        Gumbic->setText(QString("Naruci\n%1").arg(q.value(q.record().indexOf("sank_opis")).toString()));
        connect(signalMapperNaruciGumbic,SIGNAL(mapped(int)),
                this,SLOT(NaruciSank(int)));
        connect(Gumbic,SIGNAL(clicked(bool)),signalMapperNaruciGumbic,SLOT(map()));

        signalMapperNaruciGumbic->setMapping(Gumbic, q.value(q.record().indexOf("id")).toInt());
        ui->layNaruciGumbi->addWidget(Gumbic);
    }
    SamoZakljucajTimerReset();

}

void frmtouch::NaruciSank(int SankID)
{
    if (SankID == 0 )
        return;

    if (ui->listRac->count() < 1)
        return;
    int StolBr = ui->txtStol->text().toInt();
    /*
    if (!ProvjeriDaLiJeDokumentPrazan(ui->txtStol->text().toInt(),SankID))
        return;
*/

    //qDebug() << "Naruci Sank: " << SankID << " imaih:" << ui->listRac->count();

    QSqlQuery ql("",QSqlDatabase::database("local.sqlite"));
    if (!ql.exec(QString("select * from opcije_naruci where id=%1").arg(SankID)))
    {
        qDebug() << "Greska naruci sank " << ql.lastError() << ql.lastQuery();
    }
    if (ql.first())
    {
        SamoZakljucajTimer->stop();
        QSqlDatabase dbSank; // = new QSqlDatabase();
        dbSank = QSqlDatabase::addDatabase("QMYSQL","baza-sank");
        dbSank.setDatabaseName(ql.value(ql.record().indexOf("baza")).toString());
        dbSank.setHostName(ql.value(ql.record().indexOf("host")).toString());
        dbSank.setUserName(ql.value(ql.record().indexOf("user")).toString());
        dbSank.setPassword(ql.value(ql.record().indexOf("pass")).toString());
        if (!dbSank.open())
        {
            qDebug() << "Greska otvaranja konekcije sank baza " << dbSank.lastError();
            return;
        }

        QSqlQuery qdb("",dbSank);
        QString qveri;
        qveri = QString("select * from rac2 where narid=0 and dok=%1").arg(StolBr);
        ql.clear();
        if (!ql.exec(qveri))
        {
            qDebug() << ql.lastError() << ql.lastQuery();
        }

        QSqlQuery qnar("",QSqlDatabase::database("baza")); //upis u nar1 i dobivanje narid-a
        qveri = QString("insert into narudzba1 (br,datumn,narucio_uid,stol,status) select "
                "(select ifnull(max(br),0)+1 from narudzba1),now(),%1,%2,'otvorena'").arg(qApp->property("KonobarID").toString())
                .arg(ui->txtStol->text().toInt());

        int NarID = 0;
        if (!qnar.exec(qveri))
        {
            qDebug() << "Greska upisa u nar1 " << qnar.lastError() << qnar.lastQuery();
            QMessageBox::warning(this,"GRESKA","Greska kod upisa narudzbe\n Prekid unosa","OK");
            return;
        }else
        {
            NarID = qnar.lastInsertId().toInt();
        }


        QMap <qint32,qint32>NoviID; // = new QMap;
        QString NewDok = QString("%1").arg(StolBr);
        while (ql.next())
        {
            qint32 NewID;
            //NewDok = QString("%1%2").arg(qApp->property("KonobarID").toString()).arg(ui->txtStol->text().toInt());  //.arg(ql.value(ql.record().indexOf("dok")).toInt());

            qdb.prepare("insert into rac2 (dok,aid,artsif,naziv,kol,nc,pc,ukupno,rabatp,rabatk,ukpcisti,bpdvcisti,bpdv,grupaid"
                        ",ispis_kuhinja,uid,povratnanaknada,crmid,datedodano,narudzba,narucio_uid,narid)"
                        " values(:dok,:aid,:artsif,:naziv,:kol,:nc,:pc,:ukupno,:rabatp,:rabatk,:ukpcisti,:bpdvcisti,:bpdv,:grupaid"
                        ",:ispis_kuhinja,:uid,:povratnanaknada,:crmid,:datedodano,'1',:narucio_uid,:narid)");
            qdb.bindValue(":dok",StolBr);
            qdb.bindValue(":aid",ql.value(ql.record().indexOf("aid")).toInt());
            qdb.bindValue(":artsif",ql.value(ql.record().indexOf("artsif")).toString());
            qdb.bindValue(":naziv",ql.value(ql.record().indexOf("naziv")).toString());
            qdb.bindValue(":kol",ql.value(ql.record().indexOf("kol")).toDouble());
            qdb.bindValue(":nc",ql.value(ql.record().indexOf("nc")).toDouble());
            qdb.bindValue(":pc",ql.value(ql.record().indexOf("pc")).toDouble());
            qdb.bindValue(":ukupno",ql.value(ql.record().indexOf("ukupno")).toDouble());
            qdb.bindValue(":rabatp",ql.value(ql.record().indexOf("rabatp")).toDouble());
            qdb.bindValue(":rabatk",ql.value(ql.record().indexOf("rabatk")).toDouble());
            qdb.bindValue(":ukpcisti",ql.value(ql.record().indexOf("ukpcisti")).toDouble());
            qdb.bindValue(":bpdvcisti",ql.value(ql.record().indexOf("bpdvcisti")).toDouble());
            qdb.bindValue(":bpdv",ql.value(ql.record().indexOf("bpdv")).toDouble());
            qdb.bindValue(":grupaid",ql.value(ql.record().indexOf("grupaid")).toDouble());
            qdb.bindValue(":ispis_kuhinja",ql.value(ql.record().indexOf("ispis_kuhinja")).toDouble());
            qdb.bindValue(":uid",ql.value(ql.record().indexOf("uid")).toInt());
            qdb.bindValue(":povratnanaknada",ql.value(ql.record().indexOf("povratnanaknada")).toDouble());
            qdb.bindValue(":crmid",ql.value(ql.record().indexOf("crmid")).toInt());
            qdb.bindValue(":datedodano",ql.value(ql.record().indexOf("datedodano")).toString());
            qdb.bindValue(":narucio_uid",qApp->property("KonobarID").toInt());
            qdb.bindValue(":narid",NarID);
            if (!qdb.exec())
                qDebug() << "Greska upisa u bazu " << qdb.lastError() << qdb.lastQuery();

            NewID = qdb.lastInsertId().toInt();
            NoviID.insert(ql.value(ql.record().indexOf("id")).toInt(),NewID);

            qnar.prepare("insert into narudzba2 (narid,aid,artsif,naziv,kol,datedodano) values(:narid,:aid,:artsif,:naziv,:kol,now())");
            qnar.bindValue(":narid",NarID);
            qnar.bindValue(":aid",ql.value(ql.record().indexOf("aid")).toInt());
            qnar.bindValue(":artsif",ql.value(ql.record().indexOf("artsif")).toString());
            qnar.bindValue(":naziv",ql.value(ql.record().indexOf("naziv")).toString());
            qnar.bindValue(":kol",ql.value(ql.record().indexOf("kol")).toDouble());
            if (!qnar.exec())
                qDebug() << "Greska upisa u narudzba2 " << qnar.lastError() << qnar.lastQuery();
        }

        qveri = QString("select * from rac4 where dok=%1 and narid=0").arg(StolBr);
        if (!ql.exec(qveri))
        {
            qDebug() << ql.lastError() << ql.lastQuery();
        }
        qdb.clear();
        while (ql.next())
        {
            int NewId=0;
            qdb.prepare("insert into rac4 (dok,aid,pid,koeficijent_poreza,iznos_poreza,porez_sifra,porez_postotak,r2id,iznos_porezaz,osnovica,narid) "
                        " values(:dok,:aid,:pid,:koeficijent_poreza,:iznos_poreza,:porez_sifra,:porez_postotak,:r2id,:iznos_porezaz,:osnovica,:narid)");
            qdb.bindValue(":dok",StolBr);
            qdb.bindValue(":aid",ql.value(ql.record().indexOf("aid")).toInt());
            qdb.bindValue(":pid",ql.value(ql.record().indexOf("pid")).toInt());
            qdb.bindValue(":koeficijent_poreza",ql.value(ql.record().indexOf("koeficijent_poreza")).toDouble());
            qdb.bindValue(":iznos_poreza",ql.value(ql.record().indexOf("iznos_poreza")).toDouble());
            qdb.bindValue(":porez_sifra",ql.value(ql.record().indexOf("porez_sifra")).toString());
            qdb.bindValue(":porez_postotak",ql.value(ql.record().indexOf("porez_postotak")).toDouble());
            NewId = NoviID.value(ql.value(ql.record().indexOf("r2id")).toInt());
            qdb.bindValue(":r2id",NewId);
            qdb.bindValue(":iznos_porezaz",ql.value(ql.record().indexOf("iznos_porezaz")).toDouble());
            qdb.bindValue(":osnovica",ql.value(ql.record().indexOf("osnovica")).toDouble());
            qdb.bindValue(":narid",NarID);
            if (!qdb.exec())
                qDebug() << "Greska upisa rac2porez " << qdb.lastError() << qdb.lastQuery();
            qDebug() <<  qdb.lastQuery();
        }
        dbSank.close();

        frmIspisOdabir *fis = new frmIspisOdabir(this);
        fis->setWindowModality(Qt::WindowModal);
        fis->show();
        fis->RacunID = NarID;  //ui->txtStol->text().toInt();

        fis->NarID = NarID;
        fis->IspisMaliSto = "narudzba";
        fis->ProvjeraOpcija();
        if (!ql.exec(QString("delete from rac4 where narid=0 and dok=%1").arg(StolBr)))
            qDebug() << ql.lastError() << ql.lastQuery();
        if (!ql.exec(QString("delete from rac2 where narid=0 and dok=%1").arg(StolBr)))
            qDebug() << ql.lastError() << ql.lastQuery();
        UcitajListRac2(StolBr);

    }
}
void frmtouch::UcitajSankBazu()
{
}

void frmtouch::on_listRac_itemClicked(QListWidgetItem *item)
{
    //qDebug() << "Stisnut";
    if (ui->txtStol->text() == "")
        return;
    if (ui->txtStol->text().toInt()<1)
        return;
    //NaruciSank(ui->txtStol->text().toInt());
}

void frmtouch::UcitajListRac2(int dok)
{
    if (ui->txtStol->text() == "")
        return;
    if (ui->txtStol->text().toInt()<1)
        return;

   // qDebug() << QTime::currentTime().toString("hh:mm:ss:zzzz") << "ListRac1";

    ui->listRac->clear();
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    QString Qveri;
    Qveri = QString("select * from rac2 where narid=0 and dok=%1 order by id desc").arg(dok);

    if (!q.exec(Qveri))
    {
        qDebug() << "Greska ucitavanje ListRac2-" << q.lastError() << q.lastQuery();
        QMessageBox::warning(this,"Greska","Greska ucitavanja locrac","OK");
        return;
    }
//    q.first();
    double Suma = 0;


    signalMapperArtEdit = new QSignalMapper(this);
    signalMapperArtBrisi = new QSignalMapper(this);

    connect(signalMapperArtEdit,SIGNAL(mapped(int)),
            this,SLOT(EditStavke(int)));

    connect(signalMapperArtBrisi,SIGNAL(mapped(int)),
            this,SLOT(BrisiStavku(int)));

    while (q.next())
    {
        QWidget *ab = new QWidget();
        QGridLayout *gl = new QGridLayout();
        QLabel* lblNaziv = new QLabel(QString("%1").arg(q.value(q.record().indexOf("naziv")).toString()));
        lblNaziv->setStyleSheet("color: lightgreen;"
                    "font: 75 11pt \"Trebuchet MS\";");
                                //"background-color: rgb(0, 216, 213);");

        QLabel* lblCijena = new QLabel(QString("%L1 %2").arg(q.value(q.record().indexOf("pc")).toDouble(),0,'f',2).arg(qApp->property("App_VALUTA").toString()));
        QLabel* lblKolicina = new QLabel(QString("%1 kom").arg(q.value(q.record().indexOf("kol")).toString()));
        QLabel* lblUkupno = new QLabel(QString("%L1 %2").arg(q.value(q.record().indexOf("ukupno")).toDouble(),0,'f',2).arg(qApp->property("App_VALUTA").toString()));
        if (q.value(q.record().indexOf("rabatp")).toDouble()>0)
            lblNaziv->setText(lblNaziv->text().append(QString(" Popust %1 \%").arg(q.value(q.record().indexOf("rabatp")).toDouble())));
        Button *btnEdit = new Button("Edit",this);
        btnEdit->setIcon(QIcon(":ikone/ikone/edit.png"));
        btnEdit->setMaximumSize(60,35);

        Button *btnDel = new Button("Delete",this);
        btnDel->setIcon(QIcon(":ikone/ikone/edit_delete.svg"));
        btnDel->setMaximumSize(60,35);

        btnEdit->setStyleSheet("background-color: rgb(0, 216, 213);");
        btnDel->setStyleSheet("background-color: rgb(0, 216, 213);");
        lblCijena->setStyleSheet("color: yellow;");
        lblUkupno->setStyleSheet("color: yellow;");
        lblKolicina->setStyleSheet("color: yellow;");

        connect(btnEdit,SIGNAL(clicked(bool)),signalMapperArtEdit,SLOT(map()));
        signalMapperArtEdit->setMapping(btnEdit, q.value(q.record().indexOf("id")).toInt());
        connect(btnDel,SIGNAL(clicked(bool)),signalMapperArtBrisi,SLOT(map()));
        signalMapperArtBrisi->setMapping(btnDel, q.value(q.record().indexOf("id")).toInt());


        gl->addWidget(lblNaziv,1,1,1,4); //Qt::AlignCenter);
        gl->addWidget(lblCijena,2,1,Qt::AlignRight); //,0,0,Qt::AlignRight);
        gl->addWidget(lblKolicina,2,2,Qt::AlignRight); //,0,0,Qt::AlignRight);
        gl->addWidget(lblUkupno,2,3,Qt::AlignRight);//0,0,Qt::AlignRight);
        gl->addWidget(btnEdit,1,5);
        gl->addWidget(btnDel,2,5);
        gl->setHorizontalSpacing(2);
        gl->setVerticalSpacing(2);

        ab->setLayout(gl);
        ab->setMinimumHeight(50);

        QListWidgetItem *it2 = new QListWidgetItem();
    //    it2->setSizeHint(QSize(0,60));
        it2->setSizeHint(ab->minimumSizeHint());

        ui->listRac->addItem(it2);
        ui->listRac->setItemWidget(it2,ab);


        Suma=Suma+q.value(q.record().indexOf("ukupno")).toDouble();
//        ui->listRac->addItem(q.value(q.record().indexOf("naziv")).toString());
    }
    //qDebug() << QTime::currentTime().toString("hh:mm:ss:zzzz") << "ListRac1-END";
    ui->lblSuma->setText(QString("%L1").arg(Suma,0,'f',2));

    ui->listRac->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 30px; }");
    ui->listRac->setStyleSheet("selection-background-color: lightblue; ");
//                               "background-color: rgb(0, 216, 213);");

    SamoZakljucajTimerReset();


}

void frmtouch::on_btnStolOdabir_released()
{
    ui->txtStol->setText("");
    dw = new QDockWidget(this);
    dw->setStyleSheet("background-color: lightblue;");
    dw->setObjectName("DW-TIPK");
   // dw->setWidget(frmTipk);
    dw->setMinimumSize(300,300);
    dw->setMaximumSize(300,300);
    frmTipkovnica *fp = new frmTipkovnica();
    fp->ResetSvega();
    fp->set_IzgledTipkovnice("Tipkovnica");
//    fp->show();
//    fp->setWindowModality(Qt::WindowModal);
    dw->setWidget(fp);
//    ui->DesniLay->addWidget(dw,Qt::AlignRight);
    ui->ljeviLayTipkovnica->addWidget(dw,Qt::AlignTop);
    connect(fp,SIGNAL(VratiTipku(QString)),this,SLOT(txtStol_Tipka(QString)));
    connect(fp,SIGNAL(Ubijena()),dw,SLOT(close()));
    connect(fp,SIGNAL(Ubijena()),this,SLOT(SamoZakljucajTimerReset()));

    SamoZakljucajTimerReset();

}

void frmtouch::EditStavke(int r2id)
{
    if (r2id == 0 )
        return;
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    QString Qveri = QString("select * from rac2 where id=%1").arg(r2id);
    if (!q.exec(Qveri))
    {
        qDebug() << "Greska edit stavke" << q.lastError() << q.lastQuery();
        return;
    }
    if (q.next())
    {
        //SamoZakljucajTimer->stop();
        SamoZakljucajTimer->stop();
        SamoZakljucavanje(30);
        q.value(q.record().indexOf("aid")).toInt();
        frmDodajStavku *fd = new frmDodajStavku(this);
        fd->setWindowModality(Qt::WindowModal);
        fd->show();
        fd->slotRadimSto("Update");
        fd->BrojStola = ui->txtStol->text().toInt();

        fd->artID = q.value(q.record().indexOf("aid")).toInt();
        fd->Rac2ID = r2id;
        fd->ui->txtArtJedCijena->setText(q.value(q.record().indexOf("pc")).toString());
        fd->ui->txtArtKolicina->setText(q.value(q.record().indexOf("kol")).toString());
        fd->ui->txtArtNazivOriginal->setText(q.value(q.record().indexOf("naziv")).toString());
        fd->ui->txtArtRabatP->setText(q.value(q.record().indexOf("rabatp")).toString());
        fd->ui->txtArtSifra->setText(q.value(q.record().indexOf("artsif")).toString());
        fd->artGrupaID = q.value(q.record().indexOf("grupaid")).toInt();
        fd->artIspisKuhinja = q.value(q.record().indexOf("ispis_kuhinja")).toInt();
        fd->ui->txtArtPovratnaNaknada->setText(q.value(q.record().indexOf("povratnanaknada")).toString());
        fd->ui->txtArtKolicina->selectAll();
        fd->ui->txtArtKolicina->setFocus();
        fd->Suma();
        connect(fd,SIGNAL(StisnutOK(int)),this,SLOT(UcitajListRac2(int)));
        connect(fd,SIGNAL(StisnutOK(int)),this,SLOT(SamoZakljucavanje()));
        //connect(fd,SIGNAL(StisnutCancel()),this,SLOT(SamoZakljucavanje()));
    }
}


void frmtouch::BrisiStavku(int r2id)
{
    int ret = QMessageBox::warning(this,"Brisanje stavke","Da li zelite obrisati stavku",QMessageBox::Yes | QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (ret == QMessageBox::No)
        return;
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    if (q.exec(QString("delete from rac2 where id=%1").arg(r2id)))
    {
        if(!q.exec(QString("delete from rac4 where r2id=%1").arg(r2id)))
        {
            qDebug() << "Greska brisanja iz rac4" << q.lastError() << q.lastQuery();
        }

        UcitajListRac2(ui->txtStol->text().toInt());
    }
}

void frmtouch::on_splitter_splitterMoved(int pos, int index)
{
    //ui->lblStol->setText(QString("w %1").arg(ui->listRac->width()));
}


void  frmtouch::SplitterSetSizeKorisnik()
{
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    QString Qveri = QString("insert into opcije (sto,sifra,opis,value) select 'TOUCH','SIZE','Touch splitter size','%1'");
//            .arg(ui->splitter->);
    if (!q.exec(Qveri))
    {
        qDebug() << "Greska spremanja splitter-size" << q.lastError() << q.lastQuery();
        return;
    }
}

void frmtouch::on_btnMeni_released()
{
    QSettings settings;
    settings.setValue("splitterSizes", ui->splitter->saveState());
}
void frmtouch::prikaziSkladCjenik()
{

}

void frmtouch::on_pushButton_pressed()
{
    stoodabir *sod = new stoodabir();
    sod->show();
}

void frmtouch::slotBtnBrziRacun()
{
    if (ui->txtStol->text().toInt() < 1)
        return;
    if (ui->listRac->count()<1)
        return;
    if (!mutex.tryLock())
    {
        return;
    }
    else
    {
        //qDebug() << "els:" << BrziCounter;
    }
    /*
    if (ui->tableView->model()->rowCount()<1)
    {
        UcitajLocal();
        //qDebug() << "nema row";
        mutex.unlock();
        return;
    }
    ui->btnBrziRacun->setEnabled(false);
    */
    SamoZakljucajTimer->stop();
    qDebug() << "Start brzi racun" << QTime::currentTime();
    clsIzracuni *IZRAC = new clsIzracuni();
    IZRAC->DefVrijednostVarijabli();
    IZRAC->set_RacStol(ui->txtStol->text().toInt());
    IZRAC->set_RacNP(1);
    IZRAC->set_RacVrstaRac("M");
    IZRAC->set_RacRabatP(0);
    /*
    if (_CRMID.value(ui->txtOdabraniStol->text().toInt()))
        IZRAC->set_CRMID(_CRMID.value(ui->txtOdabraniStol->text().toInt()));
        */
    IZRAC->UcitajLocalSUMrac();
    IZRAC->izracunRAC();
    if (!IZRAC->UpisiRac())
    {
        QMessageBox::warning(this,"Racun-upis","Greska kod upisa racuna\rProvjerite mrezne veze i pristup bazi\rili nazovite odrzavanje","OK");
    }else
    {


        frmIspisOdabir *fisp = new frmIspisOdabir(this);
        fisp->setWindowModality(Qt::WindowModal);
        fisp->show();
        fisp->RacunID = IZRAC->get_Rac1ID(); // IzracRac->get_Rac1ID();
        fisp->ProvjeraOpcija();
        connect(fisp,SIGNAL(SignalOdjava()),this,SLOT(slotOdjava()));
/*
        if (qApp->property("Printer-DEFprinter-Veliki").toString() == "1") {
            isp->ispisVeliki("ispRacun",QString("\"rid=int:%1\"").arg(IZRAC->get_Rac1ID()));
        }else
        {
            isp->IspisMaliPos(QString("%1").arg(IZRAC->get_Rac1ID()));
        }
*/
    }

    //UcitajLocal();
    UcitajListRac2(ui->txtStol->text().toInt());
    mutex.unlock();
    qDebug() << "Brzi racun gotov " << QTime::currentTime();
    SamoZakljucavanje();
    //SamoZakljucajTimerReset();
//    slotOdjava();
}

void frmtouch::slotBtnKrajSmjene()
{
    frmKrajDana *wk;
    wk = new frmKrajDana();
    wk->show();
}

bool frmtouch::ProvjeraKorisnikStol()
{

}

bool frmtouch::ProvjeriStolVlasnik(int BrDokumenta)
{

}

bool frmtouch::ProvjeriDaLiJeDokumentPrazan(int BrojDokumenta,int SankID)
{
    if (qApp->property("Prog-GUInarudzba").toString() != "1")
        return false;
    QSqlQuery ql("",QSqlDatabase::database("local.sqlite"));
    if (!ql.exec(QString("select * from opcije_naruci where id=%1").arg(SankID)))
    {
        qDebug() << "Greska naruci sank " << ql.lastError() << ql.lastQuery();
    }
    if (ql.first())
    {
        SamoZakljucajTimer->stop();
        QSqlDatabase dbSank; // = new QSqlDatabase();
        dbSank = QSqlDatabase::addDatabase("QMYSQL","baza-sank");
        dbSank.setDatabaseName(ql.value(ql.record().indexOf("baza")).toString());
        dbSank.setHostName(ql.value(ql.record().indexOf("host")).toString());
        dbSank.setUserName(ql.value(ql.record().indexOf("user")).toString());
        dbSank.setPassword(ql.value(ql.record().indexOf("pass")).toString());
        if (!dbSank.open())
        {
            qDebug() << "Greska otvaranja konekcije sank baza " << dbSank.lastError();
            return false;
        }
        QSqlQuery q("",dbSank);
        QString Qveri = QString("select count(*) from rac2 where dok=%1%2")
                .arg(qApp->property("KonobarID").toString()).arg(ui->txtStol->text().toInt());
        if (!q.exec(Qveri))
        {   qDebug() << "Greska provjere otvorenog stola " << q.lastError() << q.lastQuery();
            return false;
        }
        q.first();
        if (q.value(0).toInt() > 0)
        {
            QMessageBox::warning(this,"PAZNJA","Ovaj stol je jos otvoren na sanku\nili je narudzba u procesu izrade\nOtvaram prvi sljedeci","OK");
            q.exec(QString("select max(dok) from rac2 where dok like '%1%'").arg(qApp->property("KonobarID").toString()));
            q.first();
            QString NoviBr = QString("%1").arg(QString(q.value(0).toString()
                                                       .replace(qApp->property("KonobarID").toString(),"")).toInt()+1);
            ui->txtStol->setText(NoviBr.replace(qApp->property("KonobarID").toString(),""));

            QMessageBox::information(this,"Paznja",QString("NOVI BR narudzbe: %1%2")
                                     .arg(qApp->property("KonobarID").toString()).arg(NoviBr),"OK");
            return true;
        }else
        {
            return true;
        }
        dbSank.close();
    }
    return false;
}

void frmtouch::UcitajNarudzbe()
{
    QDialog *dw = new QDialog(this);
    QVBoxLayout *vbl = new QVBoxLayout();
    dw->setLayout(vbl);
    QTableView *tbv = new QTableView();
    QSqlQueryModel *qm = new QSqlQueryModel();
    qm->setQuery(QString("select br,datumn,stol,status from narudzba1 where narucio_uid=%1 and status='otvorena' order by id desc")
                 .arg(qApp->property("KonobarID").toString()),QSqlDatabase::database("baza"));
    tbv->setModel(qm);
    QPushButton *btnI = new QPushButton();
    btnI->setText("Izlaz");
    btnI->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 white, stop: 1 blue);"
                        "border-style: solid;"
                        "border-color: beige;"
                        "border-width: 1px;"
                        "border-radius: 25px;"
                         "font: 75 19pt \"Trebuchet MS\";"
                         "color: yellow;");
    btnI->setIcon(QIcon(":/ikone/ikone/window_close.svg"));
    vbl->addWidget(tbv);
    vbl->addWidget(btnI);
    dw->setWindowTitle("Pregled otvorenih narudzba");
    dw->showFullScreen();
    connect(btnI,SIGNAL(clicked(bool)),dw,SLOT(close()));
    SamoZakljucajTimerReset();
}
