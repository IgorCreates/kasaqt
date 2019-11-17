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




#include "frmlogin.h"
#include "ui_frmlogin.h"
#include "mojiheaderi.h"
#include <QMainWindow>
#include <QWidget>
#include <QtGui>
#include "frmodabirbaza.h"
#include <QtSql>
#include <Qt>
#include <ui/frmkasa.h>
#include <QApplication>
#include <QProcess>
#include <ui/frmtouch.h>
#include "ui/frmloginrftag.h"


frmLogin::frmLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmLogin)
{

    ui->setupUi(this);
    ui->frameLog->hide();
    ui->frame->show();
    this->setFixedSize(ui->frame->width(),ui->frame->height());
    ui->txtKorisnik->installEventFilter(this);
    ui->txtPassword->installEventFilter(this);
    ui->tableView->installEventFilter(this);
    Demo = false;

    ui->txtTpass->installEventFilter(this);

    connect(ui->btnTzaporka,SIGNAL(released()),this,SLOT(slotProvjeraLozinke()));
    this->setWindowTitle(QString(tr("Fiskal Kasa v:%1")).arg(qApp->property("App_Verzija").toString()));


    LocalDBHost = "localhost";
    LocalDBUser = "kasauser";
    LocalDBPass = "KasaPasSwd";
    LocalDBBaza = "local";


}

frmLogin::~frmLogin()
{
    delete ui;

}
void frmLogin::SetDemoPostavke()
{
    qApp->setProperty("Firma_Ime","KasaQT DEMO");
    qApp->setProperty("Firma_Adresa","DEMO_ADDRESS 1");
    qApp->setProperty("Firma_Postanski","10000");
    qApp->setProperty("Firma_OIB","10010001001");
    qApp->setProperty("Firma_USustavuPDV","true");
    qApp->setProperty("Firma_Grad","Zagreb");
    qApp->setProperty("Firma_OznPosPr","DEMOPROGRAM");
    qApp->setProperty("Firma_OznNapUr","1");
    qApp->setProperty("Firma_CB","Caffe Bar DEMO");
    qApp->setProperty("Certs_Path","mycert.pfx");
    qApp->setProperty("Certs_Sifra","1");
    qApp->setProperty("Firma_Rtip","R1");
    Demo = true;
}

bool frmLogin::UcitajCryptoBazu()
{

    QFile ProvFile("op.db");

    if (!ProvFile.exists()){
        SetDemoPostavke();
        return false;
    }

    QSqlDatabase dbLocal;
    //QSqlError err;
    bool JeNije=false;

    SetDemoPostavke();

    dbLocal = QSqlDatabase::addDatabase("QSQLITE","op.db");
    dbLocal.setDatabaseName("op.db");
    if (!dbLocal.open())
    {
        qDebug() << "Greska cry-baza";
        qDebug() << dbLocal.lastError();
        JeNije = false;
    }
    QSqlQuery qc("",dbLocal);
    if (!qc.exec("select * from firma"))
    {
        qDebug() << "Greska cry-q";
        qDebug() << qc.lastError();
        JeNije = false;
    }
    if (qc.next())
    {
        qApp->setProperty("Firma_Ime",qc.value(0).toString());
        qApp->setProperty("Firma_Adresa",qc.value(1).toString());
        qApp->setProperty("Firma_Grad",qc.value(2).toString());
        qApp->setProperty("Firma_Postanski",qc.value(3).toString());
        qApp->setProperty("Firma_OIB",qc.value(4).toString());
        qApp->setProperty("Firma_SER_PLOCA",qc.value(5).toString());
        qApp->setProperty("Firma_SER_DISK",qc.value(6).toString());
        qApp->setProperty("Firma_USustavuPDV",qc.value(7).toString());
        qApp->setProperty("Firma_OznPosPr",qc.value(8).toString());
        qApp->setProperty("Firma_OznNapUr",qc.value(9).toString());
        qApp->setProperty("Firma_CB",qc.value(10).toString());
        qApp->setProperty("Firma_Rtip",qc.value(11).toString());
        qDebug() << QString("Postavke-\rFirma:%1\rFirmaOib:%2\rFirmaUsustavuPDV:%3\rFirmaOznPosPR:%4\rFirmaOznNapUR:%5\rFirma_RTIP:%6").arg(qApp->property("Firma_Ime").toString())
                    .arg(qApp->property("Firma_OIB").toString()).arg(qApp->property("Firma_USustavuPDV").toString()).arg(qApp->property("Firma_OznPosPr").toString())
                    .arg(qApp->property("Firma_OznNapUr").toString()).arg(qApp->property("Firma_Rtip").toString());

        JeNije =true;
        Demo= false;
    }

    if (qc.exec("select cert,sifra from certs"))
    {
        if (qc.next())
        {
            qApp->setProperty("Certs_Path",qc.value(0).toString());
            qApp->setProperty("Certs_Sifra",qc.value(1).toString());
        }else
        {
            qApp->setProperty("Certs_Path","mycert.pfx");
            qApp->setProperty("Certs_Sifra","1");
        }
    }else
    {
        qDebug() << qc.lastError() << " " << qc.lastQuery();
        qApp->setProperty("Certs_Path","mycert.pfx");
        qApp->setProperty("Certs_Sifra","1");

    }
    qDebug() << qApp->property("Certs_Path").toString();
    qDebug() << "Cert-" << qApp->property("Certs_Path").toString() <<  ",Cert-sifra-" << qApp->property("Certs_Sifra").toString();
    qc.clear();
    dbLocal.close();

    //DeKriptBaze->deleteLater();

    //delete PokreniHAL;
    return JeNije;
}

void frmLogin::UcitajBazu()
{

    QSqlDatabase dbLocal;
    QSqlError err;
    /*
    shell openssl des
    */
    qDebug() << QSqlDatabase::drivers();

    if (PromjenaKorisnika == false)
    {
        dbLocal = QSqlDatabase::addDatabase("QMYSQL","local.sqlite");
        dbLocal.setDatabaseName(LocalDBBaza);
        dbLocal.setHostName(LocalDBHost);
        dbLocal.setUserName(LocalDBUser);
        dbLocal.setPassword(LocalDBPass);
        qDebug() << "dbLocal postavke dbname:" << LocalDBBaza << " hostname:" << LocalDBHost << " username:" << LocalDBUser;
    }else
    {
        dbLocal = QSqlDatabase::database("local.sqlite");
    }

    //dbLocal.open();
    if (!dbLocal.open())
    {
        qDebug() << "\nGreska db-"+dbLocal.lastError().text()+"\n";
        qDebug() << err.ConnectionError;
        QMessageBox::warning(this, tr("Ne mogu otvoriti bazu"), tr("greska Otvaranje konekcije: ") + dbLocal.lastError().text());
    }
    QSqlQuery q("",dbLocal);
    q.exec("select * from baze where izabrana=1");
    if (q.next())
    {
        QString dbBazaOpis = "";
        dbBazaOpis = QString(tr("Izabrana baza:%1")).arg(q.value(5).toString());

        ui->lblInfo->setText(dbBazaOpis);
        QSqlDatabase dbBaza;
        if (PromjenaKorisnika == false)
        {
            dbBaza = QSqlDatabase::addDatabase("QMYSQL","baza");
            dbBaza.setDatabaseName(q.value(4).toString());
            dbBaza.setHostName(q.value(1).toString());
            dbBaza.setUserName(q.value(2).toString());
            dbBaza.setPassword(q.value(3).toString());
            qDebug() << "dbBaza postavke dbname:" << q.value(4).toString() << " dbhostname:" << q.value(1).toString();
            //qDebug() << q.value(4).toString() << q.value(1).toString() << q.value(2).toString() << q.value(3).toString();
        }else
        {
            dbBaza = QSqlDatabase::database("baza");
        }
        if (!dbBaza.open())
        {
            QMessageBox::warning(this,tr("BAZA"),tr("Odabrana baza nije dostupna\nProvjeritre mrezne veze ili konfiguracije"),"OK");
        }
        //id INTEGER PRIMARY KEY AUTOINCREMENT,host text,username text,password text,database text,opis text, tip text,
    }

    q.finish();
    dbLocal.close();
    dbLocal.database().close();
}

bool frmLogin::eventFilter(QObject *target, QEvent *event)
{
    QString Pomoc = tr("F1-Pomoc\nEnter-potrvrda\nDel-brisanje");
    QKeyEvent *keyEvent = (QKeyEvent *)event;

        if (event->type() == QEvent::KeyPress)
        {
            switch (keyEvent->key())
            {
                case Qt::Key_Enter:
                case Qt::Key_Return:
                    if (target == ui->tableView)
                    {
                        ui->txtTpass->setFocus();
                        return false;
                    }
                    slotProvjeraLozinke();
                    //this->hide();
                    break;
                case Qt::Key_F1:
                    Pomoc += "\nF12-odabir baze za rad\n";
                    QMessageBox::information(this,tr("Pomoc ") ,Pomoc,"OK");
                    return true;
                    break;
                case Qt::Key_F12:
                    PromjenaBaza();
                    break;
                case Qt::Key_Escape:
                    //this->close();
                    qApp->closeAllWindows();
                    qApp->setQuitOnLastWindowClosed(true);
                    qApp->closingDown();
                    //QApplication.exit();
                    this->close();
                    qApp->exit(0);
                    qApp->exit();
                    return false;
                    break;
            }
        }
    return QWidget::eventFilter(target,event);
}

void frmLogin::slotPokaziKorisnike()
{

    QSqlDatabase dbK;
    dbK = QSqlDatabase::database("baza");
    dbK.open();
    modK = new QSqlQueryModel();
    modK->setQuery("select puninaziv,id from korisnik where active='A'",dbK);
    ui->tableView->setModel(modK);
    ui->tableView->setStyleSheet("font: 75 italic 16pt Ubuntu;"
                      "color: rgb(85, 255, 0);"
                      "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));");


    ui->tableView->setObjectName("lw-korisnik");
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnWidth(0,500);
    ui->tableView->selectRow(0);
//    lw->move(iXpos,iYpos);
//    lw->show();
}

void frmLogin::on_btnIzlaz_pressed()
{
    //this->destroy();
    qApp->exit();
}
void frmLogin::slotProvjeraLozinke()
{
    if(Demo)
    {
        QMessageBox::warning(this,tr("KASA"),tr("nemoguc ulaz bez licence"),"OK");
        return;
    }

    bool MozeDalje = false;
    if (ui->tableView->currentIndex().row() == -1)
    {
        QMessageBox::warning(this,tr("Login"),tr("Prvo izaberite korsnika"),"OK");
        ui->tableView->setFocus();
        return;
    }

    QSqlDatabase dbP = QSqlDatabase::database("baza");
    QSqlQuery q("",dbP);
    dbP.open();
    QString sif= QString("select k.puninaziv,k.oib,k.dozvole,kc.boja from korisnik k left join korisnik_boje kc on k.id=kc.uid where k.active='A' and  k.id=%1 and k.sifra='%2'").arg(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),1)).toString()).arg(ui->txtTpass->text());
    qDebug() << sif;
    if (q.exec(sif))
    {
        while (q.next())
        {
            qApp->setProperty("KorisnikIme",q.value(q.record().indexOf("puninaziv")).toString());
            qApp->setProperty("KonobarOIB",q.value(q.record().indexOf("oib")).toString());
            qApp->setProperty("KonobarPrava",q.value(q.record().indexOf("dozvole")).toInt());
            qApp->setProperty("KorisnikSifra",ui->txtTpass->text());
            if (q.value(q.record().indexOf("boja")).toString() != "")
            {
                qApp->setProperty("KorisnikBoja",q.value(q.record().indexOf("boja")).toString());
            }else
            {
                qApp->setProperty("KorniskBoja","#FFFFFF");
            }

            MozeDalje = true;
        }
    }
    if (MozeDalje == true)
    {
        qApp->setProperty("KonobarID",ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),1)).toString());
        PustiDalje();
    }else{
        QMessageBox::warning(this,tr("Login"),tr("Kriva lozinka"),"OK");
        ui->txtTpass->setFocus();
    }
}
void frmLogin::PustiDalje()
{

    if (qApp->property("Prog-GUIizgled").toString() == "touch")
    {

        frmtouch *fp = new frmtouch();
        fp->setWindowModality(Qt::WindowModal);
        fp->setWindowFlags(Qt::WindowStaysOnTopHint);
        //fp->show();
        fp->showMaximized();
        return;
    }else
    {
        frmKasa *ww;
        ww = new frmKasa();
        ww->UcitavanjeForme =false;
        ww->showMaximized();
        ww->UcitajSveNakonForme();
        if (qApp->property("Prog-LogRac").toString() != "1")
        {
            ww->SetLogRac(false);
        }else
        {
            ww->SetLogRac(true);
        }
        ww->ProvjeraNePoslanihRacuna();
    }
    this->close();
}
void frmLogin::PromjenaBaza()
{
    frmOdabirBaza *w;
    w = new frmOdabirBaza();
    w->show();
    connect(w,SIGNAL(UpdateBazeSignal()),this,SLOT(slotPokaziKorisnike()));

}
void frmLogin::PromjenaKorisnikaP(const bool Promjena)
{

    QSqlDatabase dbL;
    //QSqlError err;
    /*
    shell openssl des
    */
    dbL = QSqlDatabase::addDatabase("QSQLITE","op.db");
    dbL.setDatabaseName("op.db");
    if (!dbL.open())
    {
        qDebug() << dbL.lastError();
    }
    QSqlQuery q("",dbL);
    if (q.exec("select host,baza from local"))
    {
        q.next();
        LocalDBBaza = q.value(1).toString();
        LocalDBHost = q.value(0).toString();
        qDebug() << "NB0" << LocalDBBaza << " " << LocalDBHost;
    }else
    {
        qDebug() << "D" << q.lastError() << q.lastQuery();
    }
    q.clear();
    dbL.close();
    dbL.removeDatabase("op.db");
    qDebug() << "NB1" << LocalDBBaza << " " << LocalDBHost;
    PromjenaKorisnika = Promjena;
    if (PromjenaKorisnika == true)
    {
        UcitajBazu();
        slotPokaziKorisnike();
    }else
    {
        if (!UcitajCryptoBazu())
        {
            qDebug() << "Lic-DEMO RAD";
            QMessageBox::warning(this,tr("Kasa"),tr("Nije dozvoljen rad bez licence\rIZLAZ"),"OK");
            ui->btnUlaz->setEnabled(false);
            ui->txtPassword->setEnabled(false);
            ui->txtKorisnik->setEnabled(false);
            Demo= true;
        }else
        {
            UcitajBazu();
            if (qApp->property("Prog-LOGIN").toString() == "RFTAG")
            {
                frmloginrftag *fl = new frmloginrftag();
                fl->showMaximized();
                this->close();
            }else
            {
                slotPokaziKorisnike();
            }
        }
    }
}
