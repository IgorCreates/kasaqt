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




#include "frmrnalog.h"
#include "ui_frmrnalog.h"
#include "ui/frmkupac.h"
#include "QtSql"
#include "QtGui"
#include "Ispis/ispismali.h"
#include "ui/frmrazduzisklad.h"
#if QT_VERSION > 0x050000
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#endif

frmrnalog::frmrnalog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmrnalog)
{
    ui->setupUi(this);
    this->setWindowTitle("Radni nalog");
//    mod = new QStandardItemModel(0,2);

//    nw = new QWidget();
    ntK = new QTableView();
    ntK->installEventFilter(this);
    ntK->close();
    ntV = new QTableView();
    ntV->installEventFilter(this);
    ntV->close();
    ntS = new QTableView();
    ntS->installEventFilter(this);
    ntS->close();
    ntR = new QTableView();
    ntR->installEventFilter(this);
    ntR->close();
    ntRZ = new QTableView();
    ntRZ->installEventFilter(this);
    ntRZ->close();
    PntK = new QDialog(this);
    PntK->installEventFilter(this);
    PntK->close();
    PntV = new QDialog(this);
    PntV->installEventFilter(this);
    PntV->close();
    PntS = new QDialog(this);
    PntS->installEventFilter(this);
    PntS->close();
    PntR = new QDialog(this);
    PntR->installEventFilter(this);
    PntR->close();
    PntRZ = new QDialog(this);
    PntRZ->installEventFilter(this);
    PntRZ->close();
    PntK->setObjectName("PntK");
    vbLntK = new QVBoxLayout;
    vbLntK->addWidget(ntK);
    PntK->setLayout(vbLntK);
    PntV->setObjectName("PntV");
    vbLntV = new QVBoxLayout;
    vbLntV->addWidget(ntV);
    PntV->setLayout(vbLntV);
    PntS->setObjectName("PntS");
    vbLntS = new QVBoxLayout;
    vbLntS->addWidget(ntS);
    PntS->setLayout(vbLntS);
    PntK->setObjectName("PntR");
    vbLntR = new QVBoxLayout;
    vbLntR->addWidget(ntR);
    PntR->setLayout(vbLntR);
    PntRZ->setObjectName("PntRZ");
    vbLntRZ = new QVBoxLayout;
    vbLntRZ->addWidget(ntRZ);
    PntRZ->setLayout(vbLntRZ);
    PntK->setWindowTitle("Pretraga po kupcu");
    PntV->setWindowTitle("Pretraga po vozilu");
    PntS->setWindowTitle("Pretraga po stavkama/roba/materijal/usluga");
    PntR->setWindowTitle("Pretraga po otvorenim radnim nalozima");
    PntRZ->setWindowTitle("Pretraga po zatvorenim radnim nalozima");

    ui->txtKupacInfo->installEventFilter(this);
    ui->txtVoziloInfo->installEventFilter(this);
    ui->txtStavkaNaziv->installEventFilter(this);
    ui->txtTrazeni->installEventFilter(this);
    ui->txtNapomena->installEventFilter(this);
    ui->tableStavke->installEventFilter(this);
    ui->txtStavkaKol->installEventFilter(this);
    RadniNalogID =0;
    RadniNalogBR =0;
    RadniNalogNovi=true;
    RadniNalogEdit=false;
    UcitajStavke();
    //ui->btnNapraviRac->setEnabled(false);
    ui->btnNapraviRac->setEnabled(true);

}

frmrnalog::~frmrnalog()
{
    delete ui;
}

void frmrnalog::on_pushButton_4_pressed()
{
    frmkupac *fk = new frmkupac();
    fk->show();
    if (KupacID >0 )
    {
        fk->NoviKupac = false;
        fk->KupacID = KupacID;
        fk->UcitajKupca(KupacID);
    }
}



void frmrnalog::on_pushButton_5_pressed()
{

}


bool frmrnalog::eventFilter(QObject *target, QEvent *event)
{
    //qDebug() << target->objectName();
    QKeyEvent *keyEvent = (QKeyEvent *)event;
    if (target->objectName() == "NTVozila" || target->objectName() == "NTKupci" || target->objectName() == "NTStavke"
            || target->objectName() == "NTRnalozi" || target->objectName() == "NTRnaloziZ"
            || target->objectName() == "PntK" || target->objectName() == "PntV" || target->objectName() == "PntS"
            || target->objectName() == "PntR" || target->objectName() == "PntRZ")
    {
        if (keyEvent->key() == Qt::Key_Escape)
        {

            if (target == ntK)
            {
//                ntK->close();
                PntK->close();
            }
            if (target == ntV)
            {
//                ntV->close();
                PntV->close();
            }
            if (target == ntS)
            {
//                ntS->close();
                PntS->close();
            }
            if (target == ntR)
            {
//                ntR->close();
                PntR->close();
            }
            if (target == ntRZ)
            {
//                ntRZ->close();
                PntRZ->close();
            }
        }
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
        {
            if (!RadniNalogEdit)
                return false;
        //    ui->lblM_nplacanja->setText(nmod->data(nt->currentIndex()).toString());
        //    ui->lblV_nplacanja->setText(nmod->data(nt->currentIndex()).toString());
            if (target->objectName() == "NTVozila")
            {
                //ui->txtVoziloInfo->setText(nmod->data(nt->currentIndex()).toString());
                NT_Vozila(ntV->currentIndex());
//                ntV->close();
                PntV->close();
                return true;
            }
            if (target->objectName() == "NTKupci")
            {
//                ui->txtKupacInfo->setText(nmod->data(nt->currentIndex()).toString());
                NT_Kupci(ntK->currentIndex());
//                ntK->close();
                PntK->close();
                return true;
            }
            if (target->objectName() == "NTStavke")
            {
                NT_Stavke(ntS->currentIndex());
//                ntS->close();
                PntS->close();
                return true;
            }
            if (target->objectName() == "NTRnalozi")
            {
                NT_Rnalozi(ntR->currentIndex());
//                ntR->close();
                PntR->close();
                return true;
            }
            if (target->objectName() == "NTRnaloziZ")
            {
                NT_RnaloziZ(ntRZ->currentIndex());
//                ntRZ->close();
                PntRZ->close();
                return true;
            }
        }
    }
    if (target == ui->txtKupacInfo)
    {
        if (keyEvent->key() == Qt::Key_F2)
        {
            TraziKupca();
        }
    }
    if (target == ui->txtVoziloInfo)
    {
        if (keyEvent->key() == Qt::Key_F2)
        {
            TraziVozilo();
        }
    }
    if (target == ui->txtTrazeni)
    {
        if (event->type() == QEvent::FocusOut)
        {
            if (!RadniNalogEdit)
                return false;
            if (RadniNalogID == 0)
                return false;
            QSqlQuery q("",QSqlDatabase::database("baza"));
            q.exec(QString("select * from rnalog2 where rid=%1").arg(RadniNalogID));
            if (q.next())
            {
                q.exec(QString("update rnalog2 set opis_narucenih='%1' where rid=%2").arg(ui->txtTrazeni->toPlainText()).arg(RadniNalogID));
            }else
            {
                q.exec(QString("insert into rnalog2 (rid,opis_narucenih) values(%1,'%2')").arg(RadniNalogID).arg(ui->txtTrazeni->toPlainText()));
            }
        }
    }
    if (target == ui->txtStavkaNaziv)
    {
        if (keyEvent->key() == Qt::Key_F2)
        {
            TraziStavku();
        }
        if (event->type() == QEvent::KeyRelease)
        {
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                //ui->txtStavkaKol->setFocus();
            }
        }
    }
    if (target == ui->txtNapomena)
    {
        if (event->type() == QEvent::FocusOut)
        {
            if (!RadniNalogEdit)
                return false;
            if (RadniNalogID == 0)
                return false;
            QSqlQuery q("",QSqlDatabase::database("baza"));
            q.exec(QString("select * from rnalog2 where rid=%1").arg(RadniNalogID));
            if (q.next())
            {
                q.exec(QString("update rnalog2 set opis_napomena='%1' where rid=%2").arg(ui->txtNapomena->toPlainText()).arg(RadniNalogID));
            }else
            {
                q.exec(QString("insert into rnalog2 (rid,opis_napomena) values(%1,'%2')").arg(RadniNalogID).arg(ui->txtNapomena->toPlainText()));
            }
        }
    }
    if (target == ui->txtStavkaKol)
    {
        if (event->type() == QEvent::KeyRelease)
        {
            switch (keyEvent->key())
            {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                ui->btnStavkaDodaj->setFocus();
                break;
            }
        }
    }
    if (target == ui->tableStavke)
    {
        if (event->type() == QEvent::KeyRelease)
        {
            switch (keyEvent->key())
            {
            case Qt::Key_Delete:
                BrisiStavku();
                break;
            case Qt::Key_Enter:
            case Qt::Key_Return:
                EditStavke();
                break;
            }
        }
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            EditStavke();
        }
    }
    return QWidget::eventFilter(target,event);
}
void frmrnalog::TraziVozilo()
{
    if (!RadniNalogEdit)
        return;
    if (!ProvjeraRnalog())
        return;

    QString sql;
    if (KupacID != 0)
    {
        sql = QString("select id,concat(registracija,',',marka,',',model,',',godina,',',',',km) opis from company_auto where kid=%1 and registracija='%2'")
                .arg(KupacID).arg(ui->txtVoziloInfo->text());
        sql += QString(" union (select id,concat(registracija,',',marka,',',model,',',godina,',',',',km) opis from company_auto where kid=%1 and registracija like").arg(KupacID);
        sql += QString(" '%%1%')").arg(ui->txtVoziloInfo->text());
    }else
    {
        sql = QString("select id,concat(registracija,',',marka,',',model,',',godina,',',',',km) opis from company_auto where kid=").arg(KupacID);
    }
   // qDebug() << sql;
    nmod = new QSqlQueryModel();
    nmod->setQuery(sql,QSqlDatabase::database("baza"));
    ntV->setModel(nmod);
//    ntV->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    ntV->setMinimumSize(this->width(),this->height());
    ntV->setColumnHidden(0,true);
    ntV->setColumnWidth(1,600);
    //nt->horizontalHeader()->hide();
    ntV->verticalHeader()->hide();
    ntV->setObjectName("NTVozila");
//    ntV->setWindowFlags(Qt::WindowStaysOnTopHint);
    ntV->setWindowTitle("Odabir vozila po kupcu");
//    ntV->show();
//    ntV->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //PntV = new QDialog(this);
   // ProzorIskoci->installEventFilter(this);
    PntV->setMinimumSize(this->width()/2,this->height()/2);
    PntV->show();

    ntV->setFocus();
    connect(ntV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(NT_Vozila(QModelIndex)));
    connect(ntV,SIGNAL(doubleClicked(QModelIndex)),PntV,SLOT(close()));
//    connect(ntV,SIGNAL(doubleClicked(QModelIndex)),ntV,SLOT(close()));
    //connect(nt,SIGNAL(doubleClicked(QModelIndex)),ui->txtTrazeni,SLOT(setFocus()));
}
void frmrnalog::TraziKupca()
{
    if (!RadniNalogEdit)
        return;

    if (!ProvjeraRnalog())
        return;

    QString sql;
    if (ui->txtKupacInfo->text().length() >0)
    {
        sql = QString("select id,naziv,oib,adresa,telefon,email from company where oib='%1'").arg(ui->txtKupacInfo->text());
        sql += QString(" union (select id,naziv,oib,adresa,telefon,email from company where naziv like '%%1%')").arg(ui->txtKupacInfo->text());
    }else
    {
        sql = "select id,naziv,oib,adresa,telefon,email from company";
    }
   // qDebug() << sql;

    nmod = new QSqlQueryModel();
    nmod->setQuery(sql,QSqlDatabase::database("baza"));
    ntK->setModel(nmod);
//    ntK->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    ntK->setMinimumSize(this->width(),this->height());
    ntK->setColumnHidden(0,true);
    ntK->setColumnWidth(1,500);
    //nt->horizontalHeader()->hide();
    ntK->verticalHeader()->hide();
    ntK->setObjectName("NTKupci");
    ntK->setWindowTitle("Kupci");
//    ntK->setWindowFlags(Qt::WindowStaysOnTopHint);
//    ntK->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //ProzorIskoci = new QDialog(this);

    //PntK = new QDialog(this);
    //ProzorIskoci->installEventFilter(this);
    /*
    PntK->setObjectName("PntK");
    vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(ntK);
    PntK->setLayout(vboxLayout);
    */
    PntK->setMinimumSize(this->width()/2,this->height()/2);
    PntK->show();
    ntK->setFocus();

    connect(ntK,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(NT_Kupci(QModelIndex)));
//    connect(ntK,SIGNAL(doubleClicked(QModelIndex)),ntK,SLOT(close()));
    connect(ntK,SIGNAL(doubleClicked(QModelIndex)),PntK,SLOT(close()));
    //connect(nt,SIGNAL(doubleClicked(QModelIndex)),ui->txtVoziloInfo,SLOT(setFocus()));

}
void frmrnalog::TraziStavku()
{
    if (!RadniNalogEdit)
        return;

    if (!ProvjeraRnalog())
        return;
    QString sql;
    if (ui->txtStavkaNaziv->text().length() >0)
    {
        sql = QString("select id,artsif,naziv from art where artsif like '%1%'").arg(ui->txtStavkaNaziv->text());
        sql += QString(" union (select id,artsif,naziv from art where naziv like '%%1%')").arg(ui->txtStavkaNaziv->text());
    }else
    {
        sql = "select id,artsif,naziv from art";
    }
    //qDebug() << sql;

    nmod = new QSqlQueryModel();
    nmod->setQuery(sql,QSqlDatabase::database("baza"));
    ntS->setModel(nmod);
//    ntS->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    ntS->setMinimumSize(this->width(),this->height());
    ntS->setColumnHidden(0,true);
    ntS->setColumnWidth(2,500);
    //nt->horizontalHeader()->hide();
    ntS->verticalHeader()->hide();
    ntS->setObjectName("NTStavke");
    ntS->setWindowTitle("Stavke");
//    ntS->setWindowFlags(Qt::WindowStaysOnTopHint);
//    ntS->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //ProzorIskoci = new QDialog(this);

   // PntS = new QDialog(this);
    //ProzorIskoci->installEventFilter(this);
    PntS->setObjectName("PntS");
    PntS->setWindowTitle("Pretraga po artiklima/uslugama");
    /*
    vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(ntS);
    PntS->setLayout(vboxLayout);
    */
    PntS->setMinimumSize(this->width()/2,this->height()/2);
    PntS->show();
    ntS->setFocus();



    connect(ntS,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(NT_Stavke(QModelIndex)));
   // connect(ntS,SIGNAL(doubleClicked(QModelIndex)),ntS,SLOT(close()));
    connect(ntS,SIGNAL(doubleClicked(QModelIndex)),PntS,SLOT(close()));
    //connect(nt,SIGNAL(doubleClicked(QModelIndex)),ui->txtVoziloInfo,SLOT(setFocus()));

}
void frmrnalog::TraziRnalozi()
{

    QString sql;
    sql = "select r.id,r.dok,r.datumn,r.kupac_opis,r.vozilo_opis,r.kid,r.auto_id from rnalog1 r  "
            "  where r.datumz is null or r.zatvoren is null";
//    qDebug() << sql;
    //ProzorIskoci = new QDialog(this);
    nmod = new QSqlQueryModel();
    nmod->setQuery(sql,QSqlDatabase::database("baza"));
    ntR->setModel(nmod);
    //ntR->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    //ntR->setMinimumSize(this->width(),this->height());
    ntR->setColumnHidden(0,true);
    ntR->setColumnWidth(2,500);
    //nt->horizontalHeader()->hide();
    ntR->verticalHeader()->hide();
    ntR->setObjectName("NTRnalozi");
    ntR->setWindowTitle("Otvoreni radni nalozi");
    PntR->setObjectName("PntR");
/*
    vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(ntR);
    PntR->setLayout(vboxLayout);
    */
    PntR->setMinimumSize(this->width()/2,this->height()/2);
    PntR->show();
    connect(ntR,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(NT_Rnalozi(QModelIndex)));
//    connect(ntR,SIGNAL(doubleClicked(QModelIndex)),ntR,SLOT(close()));
    connect(ntR,SIGNAL(doubleClicked(QModelIndex)),PntR,SLOT(close()));
    //connect(nt,SIGNAL(doubleClicked(QModelIndex)),ui->txtVoziloInfo,SLOT(setFocus()));

}
void frmrnalog::TraziRnalozZatvoreni()
{

    QString sql;
    sql = "select r.id,r.dok,r.datumn,r.kupac_opis,r.vozilo_opis,r.kid,r.auto_id from rnalog1 r "
            " where r.datumz is not null or r.zatvoren is not null";
//    qDebug() << sql;
    nmod = new QSqlQueryModel();
    nmod->setQuery(sql,QSqlDatabase::database("baza"));

    ntRZ->setModel(nmod);
//    ntRZ->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    ntRZ->setMinimumSize(this->width(),this->height());
    ntRZ->setColumnHidden(0,true);
    ntRZ->setColumnWidth(2,500);
    //nt->horizontalHeader()->hide();
    ntRZ->verticalHeader()->hide();
    ntRZ->setObjectName("NTRnaloziZ");
    ntRZ->setWindowTitle("Zatvoreni radni nalozi");
//    ntRZ->setWindowFlags(Qt::WindowStaysOnTopHint);
//    ntRZ->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //PntRZ = new QDialog(this);
   //ProzorIskoci->installEventFilter(this);
    PntRZ->setObjectName("PntRZ");
/*
    vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(ntRZ);
    PntRZ->setLayout(vboxLayout);
    */
    PntRZ->setMinimumSize(this->width()/2,this->height()/2);
    PntRZ->show();
    ntRZ->setFocus();

    connect(ntRZ,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(NT_RnaloziZ(QModelIndex)));
//    connect(ntRZ,SIGNAL(doubleClicked(QModelIndex)),ntRZ,SLOT(close()));
    connect(ntRZ,SIGNAL(doubleClicked(QModelIndex)),PntRZ,SLOT(close()));
    //connect(nt,SIGNAL(doubleClicked(QModelIndex)),ui->txtVoziloInfo,SLOT(setFocus()));

}

void frmrnalog::NT_Vozila(const QModelIndex &StoDeb)
{
    if (!RadniNalogEdit)
        return;

    ui->txtVoziloInfo->setText(StoDeb.data().toString());
    VoziloID = StoDeb.model()->index(StoDeb.row(),0).data().toInt();
    QSqlQuery q("",QSqlDatabase::database("baza"));
    q.exec(QString("update rnalog1 set auto_id=%1,vozilo_opis='%3' where id=%2").arg(VoziloID).arg(RadniNalogID).arg(ui->txtVoziloInfo->text()));
    ui->txtTrazeni->setFocus();
}
void frmrnalog::NT_Kupci(const QModelIndex &StoDeb)
{
    if (!RadniNalogEdit)
        return;
    KupacID = StoDeb.model()->index(StoDeb.row(),0).data().toInt();
    ui->txtKupacInfo->setText(StoDeb.model()->index(StoDeb.row(),1).data().toString());
    QSqlQuery q("",QSqlDatabase::database("baza"));
    if (!q.exec(QString("update rnalog1 set kid=%1,kupac_opis='%3' where id=%2").arg(KupacID).arg(RadniNalogID).arg(ui->txtKupacInfo->text())))
        qDebug() << q.lastError();
    ui->txtVoziloInfo->setFocus();
}
void frmrnalog::NT_Stavke(const QModelIndex &StoDeb)
{
    if (!RadniNalogEdit)
        return;

    //ui->txtStavkaNaziv->setText(StoDeb.data().toString());
    ui->txtStavkaNaziv->setText(StoDeb.model()->index(StoDeb.row(),2).data().toString());
    StavkaAID = StoDeb.model()->index(StoDeb.row(),0).data().toInt();
    StavkaArtSIF = StoDeb.model()->index(StoDeb.row(),1).data().toString();
    ui->txtStavkaKol->selectAll();
    //ui->txtStavkaKol->setFocus();
}

void frmrnalog::EditStavke()
{
    //ui->txtStavkaNaziv->setText(ui->tableStavke->model()->data();
    ui->txtStavkaNaziv->setText(modelStavke->record(ui->tableStavke->currentIndex().row()).value("naziv").toString());
    StavkaAID = modelStavke->record(ui->tableStavke->currentIndex().row()).value("aid").toInt();
    StavkaArtSIF = modelStavke->record(ui->tableStavke->currentIndex().row()).value("artsif").toString();
    ui->txtStavkaKol->setText(modelStavke->record(ui->tableStavke->currentIndex().row()).value("kol").toString());
    QSqlQuery q("",QSqlDatabase::database("baza"));
    //ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt()
    q.exec(QString("delete from rnalog3 where id=%1").arg(ui->tableStavke->model()->data(ui->tableStavke->model()->index(ui->tableStavke->currentIndex().row(),0)).toInt()));
    UcitajStavke();
    //CistiPoljaStavka();
    ui->txtStavkaKol->setFocus();
}


void frmrnalog::NT_Rnalozi(const QModelIndex &StoDeb)
{
    RadniNalogBR = StoDeb.model()->index(StoDeb.row(),1).data().toInt();
    RadniNalogID = StoDeb.model()->index(StoDeb.row(),0).data().toInt();
    qDebug() << "NT_RNalozi" << RadniNalogID;
    UcitajRnalog(RadniNalogID);
    ui->txtKupacInfo->setFocus();
//    RadniNalogNovi=true;
    RadniNalogEdit=true;
}
void frmrnalog::NT_RnaloziZ(const QModelIndex &StoDeb)
{
    RadniNalogBR = StoDeb.model()->index(StoDeb.row(),1).data().toInt();
    RadniNalogID = StoDeb.model()->index(StoDeb.row(),0).data().toInt();
    qDebug() << "NT_RNaloziZ" << RadniNalogID;
    UcitajRnalog(RadniNalogID);
    ui->txtKupacInfo->setFocus();
    RadniNalogNovi=false;
    RadniNalogEdit=false;
}

void frmrnalog::UcitajStavke()
{
    //QSqlQueryModel *mod= new QSqlQueryModel();
    //mod->setQuery(QString("select id,aid,artsif,naziv,kol from rnalog3 where rid=%1").arg(RadniNalogID),QSqlDatabase::database("baza"));
    modelStavke = new QSqlQueryModel();
    modelStavke->setQuery(QString("select id,aid,artsif,naziv,kol from rnalog3 where rid=%1").arg(RadniNalogID),QSqlDatabase::database("baza"));
    ui->tableStavke->setModel(modelStavke);
    ui->tableStavke->setColumnWidth(3,400);
    ui->tableStavke->setColumnHidden(0,true);
    ui->tableStavke->setColumnHidden(1,true);
}

void frmrnalog::on_btnStavkaDodaj_released()
{
    if (!RadniNalogEdit)
        return;
    if (!ProvjeraRnalog())
        return;
    QSqlQuery q("",QSqlDatabase::database("baza"));
    if (!q.exec(QString("insert into rnalog3 (rid,aid,artsif,naziv,kol) values(%1,%2,'%3','%4','%5')").arg(RadniNalogID).arg(StavkaAID)
                .arg(StavkaArtSIF).arg(ui->txtStavkaNaziv->text()).arg(ui->txtStavkaKol->text().toDouble())))
    {
        qDebug() << q.lastError();
    }
    UcitajStavke();
    CistiPoljaStavka();
    ui->txtStavkaNaziv->setFocus();
}
void frmrnalog::CistiPoljaStavka()
{
    if (!RadniNalogNovi)
        return;
    StavkaAID = 0;
    StavkaArtSIF = "";
    ui->txtStavkaKol->setText("1");
    ui->txtStavkaNaziv->setText("");
    ui->txtStavkaNaziv->setFocus();
}

void frmrnalog::on_btnStavkaBrisi_released()
{
    BrisiStavku();
}
void frmrnalog::UcitajRnalog(const int RnalogID)
{
    CistiSvaPolja();
    QSqlQuery q("",QSqlDatabase::database("baza"));
    q.exec(QString("select * from rnalog1 where id=%1").arg(RadniNalogID));
    QSqlQuery qc("",QSqlDatabase::database("baza"));
    if (q.next())
    {
        KupacID = q.value(q.record().indexOf("kid")).toInt();
        ui->txtKupacInfo->setText(q.value(q.record().indexOf("kupac_opis")).toString());
        VoziloID = q.value(q.record().indexOf("auto_id")).toInt();
        ui->txtVoziloInfo->setText(q.value(q.record().indexOf("vozilo_opis")).toString());
        RadniNalogBR = q.value(q.record().indexOf("dok")).toInt();
        if (q.value(q.record().indexOf("zatvoren")).toInt() == 1)
        {
            RadniNalogNovi = false;
            RadniNalogEdit = false;
            ui->btnNapraviRac->setEnabled(true);
        }else{
            RadniNalogEdit = true;
           // ui->btnNapraviRac->setEnabled(false);
        }
    }else{
        return;
    }
    q.exec(QString("select * from rnalog2 where rid=%1").arg(RadniNalogID));
    if (q.next())
    {
        ui->txtTrazeni->setPlainText(q.value(q.record().indexOf("opis_narucenih")).toString());
        ui->txtNapomena->setPlainText(q.value(q.record().indexOf("opis_napomena")).toString());
    }
    UcitajStavke();
}
bool frmrnalog::ProvjeraRnalog()
{

    if (RadniNalogID > 0 )
    {
        return true;
    }else
    {
        QMessageBox::warning(this,"Radni nalog","Prvo otvorite novi ili nastavite raditi na vec otvorenom nalogu","OK");
        return false;
    }
}

void frmrnalog::on_pushButton_8_released()
{
    QSqlQuery q("",QSqlDatabase::database("baza"));
    if (q.exec("insert into rnalog1 (datumn) values(now())"))
    {
        RadniNalogID = q.lastInsertId().toInt();
        RadniNalogNovi=false;
        RadniNalogEdit=true;
        CistiSvaPolja();
        CistiPoljaStavka();
//        ui->btnStavkaBrisi->setText(RadniNalogID);
        QMessageBox::information(this,"Radni nalog","Otvoren novi radni nalog","OK");
    }else{
        QMessageBox::warning(this,"Radni nalog","Greska kod otvaranja radnog naloga\nProvjerite vezu prema bazi","OK");
        qDebug() << q.lastError();
    }

}
void frmrnalog::CistiSvaPolja()
{
    ui->txtKupacInfo->setText("");
    ui->txtNapomena->setPlainText("");
    ui->txtStavkaKol->setText("1");
    ui->txtStavkaNaziv->setText("");
    ui->txtTrazeni->setPlainText("");
    ui->txtVoziloInfo->setText("");
    KupacID =0;
    VoziloID = 0;
    StavkaAID = 0;
    StavkaArtSIF = "";
}

void frmrnalog::on_pushButton_9_released()
{
    TraziRnalozi();
}

void frmrnalog::on_txtKupacInfo_lostFocus()
{
}

void frmrnalog::on_txtVoziloInfo_lostFocus()
{
}

void frmrnalog::on_pushButton_6_released()
{
    if (!RadniNalogNovi)
        return;
    int ret = QMessageBox::warning(this,"Zatvaranje RN","Da li zelite zatvoriti radni nalog? ",
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::No | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {
        QSqlQuery q("",QSqlDatabase::database("baza"));
        //if (!q.exec(QString("set @BR=(select ifnull(max(dok),0)+1 from rnalog1);update rnalog1 set dok=@BR,zatvoren=1,datumz=now() where id=%1").arg(RadniNalogID)))
        if (!q.exec(QString("call proc_DohvatBrojaDokumenta('RN','%1','%2','%3','%4',@BR);update rnalog1 set dok=@BR,zatvoren=1,datumz=now() where id=%4")
                    .arg(qApp->property("Firma_OznNapUr").toString()).arg(qApp->property("Firma_OznPosPr").toString()).arg("N").arg(RadniNalogID)))
        {
            qDebug() << q.lastError();
            QMessageBox::warning(this,"Radni nalog","Greska kod upisa radnong naloga\rProvjerite konekciju prema bazi","OK");
            return;
        }else{
            QMessageBox::information(this,"Radni nalog","Radni nalog zakljucen","OK");
            q.exec(QString("select dok from rnalog1 where id=%1").arg(RadniNalogID));
            if (q.next())
            {
                RadniNalogBR = q.value(0).toInt();
                ui->lblRNalog->setText(QString("Radni nalog br:%1").arg(RadniNalogBR));
                RadniNalogNovi = false;
                RadniNalogEdit = false;
                ui->btnNapraviRac->setEnabled(true);
            }
            if (qApp->property("Prog-RnalogRazSklad").toString() == "1")
            {
                frmrazduziSklad *rzs = new frmrazduziSklad();
                rzs->show();
                rzs->setWindowTitle("Razduzivanje sklad");
                rzs->UcitajFRM(QString("Radni nalog br %1").arg(RadniNalogBR),0,"Rnalog",RadniNalogID);
            }
        }
    }
}

void frmrnalog::on_pushButton_11_released()
{
    TraziRnalozZatvoreni();
}

void frmrnalog::on_pushButton_10_released()
{
    //ispis radnog nlg
    if (qApp->property("Prog-IspisRnalogCustom").toString() == "1")
    {
        ispisMali *isp = new ispisMali();
        isp->IspisRNalog(RadniNalogID);
    }else
    {
        IspisA4(RadniNalogID);
    }
}

void frmrnalog::on_pushButton_5_released()
{
    if (qApp->property("Prog-IspisRnalogCustom").toString() == "1")
    {
        ispisMali *isp = new ispisMali();
        isp->IspisRNalog(0);
    }else
    {
        IspisA4(0);
    }
}



void frmrnalog::on_btnNapraviRac_released()
{
    btnNapraviRacunKasa = new QPushButton("Izrada racuna u KASA\n(maloprodaja,R1/R2-kartice,gotovina)");
    btnNapraviRacunBOF = new QPushButton("Izrada racuna u BOF-u\nvirmanski racuni");
    btnNapraviRacunIZLAZ = new QPushButton("Izlaz");
    btnNapraviRacunKasa->setFixedHeight(50);
    btnNapraviRacunBOF->setFixedHeight(50);
    btnNapraviRacunIZLAZ->setFixedHeight(50);
    NapraviRacunVboxLay = new QVBoxLayout;
    NapraviRacunVboxLay->addWidget(btnNapraviRacunKasa);
    NapraviRacunVboxLay->addWidget(btnNapraviRacunBOF);
    NapraviRacunVboxLay->addWidget(btnNapraviRacunIZLAZ);
    NapraviRacunProzor = new QDialog(this);
    NapraviRacunProzor->setLayout(NapraviRacunVboxLay);
    NapraviRacunProzor->setObjectName("NapraviRacun");
    NapraviRacunProzor->setWindowTitle("Napravi racun iz rnaloga");


    NapraviRacunProzor->show();
    NapraviRacunProzor->setFixedWidth(400);
    connect(btnNapraviRacunKasa,SIGNAL(released()),this,SLOT(NapraviRacunKasa()));
    connect(btnNapraviRacunBOF,SIGNAL(released()),this,SLOT(NapraviRacunBOF()));
    connect(btnNapraviRacunIZLAZ,SIGNAL(released()),NapraviRacunProzor,SLOT(close()));
}
void frmrnalog::NapraviRacunKasa()
{
    int ret = QMessageBox::information(this,"Izrada racuna iz rnaloga","Da li zelite napravit racun na cekanju u kasi\riz radnog naloga?",
                                       QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {

        QSqlQuery q(QString("select r3.aid,r3.artsif,r3.naziv,r3.kol,c.punacijena,(r3.kol*c.punacijena),a.grupaid,1 "
                            " from rnalog3 r3 left join art a on r3.aid=a.id left join cjenikstavke c on r3.aid=c.aid  "
                            " where r3.rid=%1 and c.cid=%2").arg(RadniNalogID).arg(qApp->property("Prog-CID").toString()),QSqlDatabase::database("baza"));
        if (q.exec())
        {

            qDebug() << "Proso1";
            int LocalDOK=1;
            int Upisanih=0;
            QSqlQuery ql("",QSqlDatabase::database("local.sqlite"));
            ql.exec("select max(dok) from rac2");
            if (ql.next())
            {
                qDebug() << "Proso2";
                LocalDOK = ql.value(0).toInt()+1;
            }
            while (q.next())
            {

                
                qDebug() << "Proso3";
                ql.prepare("insert into rac2 (dok,aid,artsif,naziv,kol,pc,ukupno,rabatp,rabatk,ukpcisti,bpdv,grupaid,ispis_kuhinja) "
                           " values(:DOK,:AID,:ARTSIF,:NAZIV,:KOL,:PC,:UKUPNO,0,0,:UKPCISTI,0,:GRUPAID,1)");
                ql.bindValue(":DOK",LocalDOK);
                ql.bindValue(":AID",q.value(0).toInt());
                ql.bindValue(":ARTSIF",q.value(1).toString());
                ql.bindValue(":NAZIV",q.value(2).toString());
                ql.bindValue(":KOL",q.value(3).toDouble());
                ql.bindValue(":PC",q.value(4).toDouble());
                ql.bindValue(":UKUPNO",q.value(5).toDouble());
                ql.bindValue(":UKPCISTI",q.value(5).toDouble());
                ql.bindValue(":GRUPAID",q.value(6).toInt());
                if (!ql.exec())
                {
                    qDebug() << "Greska upisa " << ql.lastError() << ql.lastQuery();
                }else
                {

                    qDebug() << "Proso1iu " << Upisanih;
                    Upisanih++;
                }
            }
            if (Upisanih>0)
            {
                bool UspjesniUpdate = true;
                if (!q.exec(QString("insert into local.rac4 (dok,aid,pid,koeficijent_poreza,iznos_poreza,porez_sifra,r2id) "
                       " select r2.dok,r2.aid,p.id,p.faktorPunaCijenaPorez koeficijent_poreza,r2.ukupno*p.faktorPunaCijenaPorez iznos_poreza,"
                       "p.sifra porez_sifra,r2.id from local.rac2 r2 left join artiklporez ap on r2.aid=ap.aid left join porez p on ap.porezid=p.id where r2.dok=%1").arg(LocalDOK)))
                {
                    UspjesniUpdate = false;
                }
                if (!q.exec(QString("update local.rac2 r2,(select r2.aid,r2.ukupno,r2.ukupno*p.faktorPunaCijenaPorez porez,"
                       " r2.ukupno*p.faktorPunaCijenaOsnovica bpdv,p.faktorPunaCijenaPorez koeficijent_poreza,"
                       " p.sifra porez_sifra,p.id pid from local.rac2 r2 left join artiklporez ap on r2.aid=ap.aid "
                       " left join porez p on ap.porezid=p.id where r2.dok=%1) as tr1 set r2.bpdv=tr1.bpdv where r2.aid=tr1.aid").arg(LocalDOK)))
                {
                    UspjesniUpdate = false;
                }
                QString Poruka ="";
                if (!UspjesniUpdate)
                {
                    Poruka = "Program nije mogao izvrsiti azuriranje poreza po stavkama\r"
                            "Svaku stavku u racunu posebno otvorite i zatvorite da se upisu porezi\rInace Vam nece racun ispasti kako treba";
                }
                QMessageBox::information(this,"Racun iz rnaloga",QString("Napravljen dok na cekanju br: %1 %2").arg(LocalDOK).arg(Poruka),"OK");
            }
        }
        NapraviRacunProzor->close();
        this->close();
    }
}
void frmrnalog::NapraviRacunBOF()
{
    int ret = QMessageBox::information(this,"Izrada racuna iz rnaloga","Da li zelite napravit racun na cekanju u BOF-u\riz radnog naloga?",
                                       QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {
        NapraviRacunProzor->close();
        this->close();
    }
    QSqlQuery q("",QSqlDatabase::database("baza"));
    QString qveri = "set @NBR=(select ifnull(max(br),0)+1 from locrac1);insert into locrac1 (br,vrsta,idk,storno,kasaid,datumv,datumr,sum,bpdv,uid,oib,obracun,stol,np,"
            "rabatp,rabatk,rsum,rbpdv,cid,sid,oznpp) values(@NBR,'V',";
    qveri += QString("%1,'1',1,now(),now(),0,0,%2,'%3','N',1,1,0,0,0,0,%4,%5,'%6')").arg(KupacID)
            .arg(qApp->property("KonobarID").toString()).arg(qApp->property("KonobarOIB").toString()).arg(qApp->property("Prog-SID").toString())
            .arg(qApp->property("Prog-CID").toString()).arg(qApp->property("Firma_OznPosPr").toString());
    if (!q.exec(qveri))
    {
        qDebug() << q.lastError() << q.lastQuery();
        QMessageBox::warning(this,"Greska","Greska kod upisa polja","OK");
        return;
    }
    q.exec("set @RID=(select LAST_INSERT_ID())");
    q.exec("select @RID");
    int RID=0;
    if (q.next())
        RID = q.value(0).toInt();
    QSqlQuery qs("",QSqlDatabase::database("baza"));
    qs.exec(QString("select r3.aid,r3.artsif,r3.naziv,r3.kol,c.veleprodajnacijena bpdv,(r3.kol*c.veleprodajnacijena) ukupno,a.grupaid,1 "
            " from rnalog3 r3 left join art a on r3.aid=a.id left join cjenikstavke c on r3.aid=c.aid  "
            " where r3.rid=%1 and c.cid=%2").arg(RadniNalogID).arg(qApp->property("Prog-CID").toString()));
    while (qs.next())
    {
        q.prepare("insert into locrac2(rid,aid,artsif,jmjsifra,kol,nc,pcbpdv,pc,ukupno,bpdv,rabatp,rabatk,ukpcisti,naziv) "
                  " values(@RID,:AID,:ARTSIF,:JMJSIFRA,:KOL,:NC,:PCBPDV,:PC,:UKUPNO,:BPDV,0,0,:UKPCISTI,:NAZIV)");
        q.bindValue(":AID",qs.value(qs.record().indexOf("aid")).toString());
        q.bindValue(":ARTSIF",qs.value(qs.record().indexOf("artsif")).toString());
        q.bindValue(":JMJSIFRA","1");
        q.bindValue(":KOL",qs.value(qs.record().indexOf("kol")).toDouble());
        q.bindValue(":NC",0);
        q.bindValue(":PCBPDV",qs.value(qs.record().indexOf("bpdv")).toDouble());
        q.bindValue(":PC",qs.value(qs.record().indexOf("bpdv")).toDouble());
        q.bindValue(":UKUPNO",qs.value(qs.record().indexOf("ukupno")).toDouble());
        q.bindValue(":BPDV",qs.value(qs.record().indexOf("bpdv")).toDouble());
        q.bindValue(":UKPCISTI",qs.value(qs.record().indexOf("ukupno")).toDouble());
        q.bindValue(":NAZIV",qs.value(qs.record().indexOf("naziv")).toString());
        if (!q.exec())
            qDebug() << "Greska-st-" << q.lastError() << q.lastQuery();

    }
    //q.exec("update loc")
}

void frmrnalog::IspisA4(int rnalogID)
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QFile htmlTemplate("rNalog_Template.html");
    if (!htmlTemplate.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::warning(this,"Greska-ispis","Nije moguce naci template za ispis rnaloga","OK");
        return;
    }
    QString htmlContent;
    QTextStream in(&htmlTemplate);
    in.setCodec("utf-8");
    htmlContent = in.readAll();
    DokumentZaIspisA4 = new QTextDocument();


    htmlContent.replace("&lt;FIRMA_NAZIV&gt;",qApp->property("Firma_Ime").toString());
    htmlContent.replace("&lt;FIRMA_OIB&gt;",qApp->property("Firma_OIB").toString());
    htmlContent.replace("&lt;FIRMA_ADRESA&gt;",qApp->property("Firma_Adresa").toString());
    htmlContent.replace("&lt;FIRMA_MJESTO&gt;",QString("%1-%2").arg(qApp->property("Firma_Postanski").toString()).arg(qApp->property("Firma_Grad").toString()));

    if (rnalogID != 0)
    {
        QSqlQuery q("",QSqlDatabase::database("baza"));
        QString qveri = QString("select ifnull(r.dok,'U RADU') dokbr,ifnull(r.datumn,'') datumn,ifnull(r.datumz,'') datumz,r.kid,r.auto_id,r.kupac_opis,"
                                "r.vozilo_opis,c.oib,concat(c.adresa,',',c.mjesto) cadresa from rnalog1 r left join company c on r.kid=c.id where r.id=%1").arg(rnalogID);

        if (!q.exec(qveri))
        {
            QMessageBox::warning(this,"Greska-Ispisa","Greska kod dohvata podataka","OK");
            return;
        }
        q.next();
        htmlContent.replace("&lt;RNALOG_BR&gt;",q.value(q.record().indexOf("dokbr")).toString());
        htmlContent.replace("&lt;RNALOG_DATUMN&gt;",q.value(q.record().indexOf("datumn")).toString());
        htmlContent.replace("&lt;RNALOG_DATUMZ&gt;",q.value(q.record().indexOf("datumz")).toString());
        QString RnalogStatus = "U Radu";
        if (q.value(q.record().indexOf("datumz")).toString() != "")
        {
            RnalogStatus = "Zatvoren";
        }

        htmlContent.replace("&lt;RNALOG_STATUS&gt;",RnalogStatus);
        RnalogStatus = QString(",%1 - %2").arg(q.value(q.record().indexOf("kupac_opis")).toString())
                .arg(q.value(q.record().indexOf("cadresa")).toString());
        htmlContent.replace("&lt;KUPAC_NAZIV&gt;,&lt;KUPAC_ADRESA&gt;,&lt;KUPAC_MJESTO&gt;",RnalogStatus);
        htmlContent.replace("&lt;KUPAC_OIB&gt;",q.value(q.record().indexOf("oib")).toString());
        htmlContent.replace("&lt;KUPAC_VOZILO&gt;",q.value(q.record().indexOf("vozilo_opis")).toString());
        if (q.exec(QString("select opis_narucenih,opis_napomena from rnalog2 where rid=%1").arg(rnalogID)))
        {
            q.next();
            htmlContent.replace("<p name=\"NarudzbaKupca\"><br></p>",q.value(0).toString().replace("\n","<br>"));
            htmlContent.replace("<p name=\"NapomenaIzvrsitelja\"><br></p>",q.value(1).toString().replace("\n","<br>"));
        }
        if (q.exec(QString("select artsif,naziv,kol from rnalog3 where rid=%1").arg(rnalogID)))
        {
            QString TablicaStavke="<table width=\"100%\">";

            while (q.next())
            {
                TablicaStavke += QString("<tr><td width=\"20%\">%1</td><td width=\"60%\">%2</td><td width=\"20%\" align=\"right\">%L3</td></tr>").arg(q.value(0).toString())
                        .arg(q.value(1).toString()).arg(q.value(2).toDouble(),0,'f',2);
            }
            if (TablicaStavke != "<table width=\"100%\">")
            {
                TablicaStavke += "</table>";
                htmlContent.replace("<p name=\"UtroseniMaterijal\"><br></p>",TablicaStavke);
            }
        }
    }else
    {
        htmlContent.replace("&lt;RNALOG_BR&gt;","");
        htmlContent.replace("&lt;RNALOG_DATUMN&gt;","");
        htmlContent.replace("&lt;RNALOG_DATUMZ&gt;","");
        htmlContent.replace("&lt;RNALOG_STATUS&gt;","U radu");
        htmlContent.replace("&lt;KUPAC_NAZIV&gt;,&lt;KUPAC_ADRESA&gt;,&lt;KUPAC_MJESTO&gt;","");
        htmlContent.replace("&lt;KUPAC_OIB&gt;","");
        htmlContent.replace("&lt;KUPAC_VOZILO&gt;","");
        htmlContent.replace("<p name=\"NarudzbaKupca\"><br></p>","<p> </p><p></p><p></p><p></p><p></p><p></p><p></p><p></p><p></p><br>");
        htmlContent.replace("<p name=\"NapomenaIzvrsitelja\"><br></p>","<p> </p><p></p><p></p><p></p><p></p><p></p><p></p><p></p><p></p><br>");
        htmlContent.replace("<p name=\"UtroseniMaterijal\"><br></p>","<p> </p><br><p></p><p></p><p></p><p></p><p></p><br>");
    }


    DokumentZaIspisA4->setHtml(htmlContent);

    QPrinter printer(QPrinter::HighResolution);
//    printer.setResolution(QPrinter::HighResolution);
    QPrintPreviewDialog pw(&printer,this);

    connect(&pw,SIGNAL(paintRequested(QPrinter*)),SLOT(IspisPreview(QPrinter*)));
    printer.setPageSize(QPrinter::A4);

    printer.setPageMargins(25,20,20,20,QPrinter::Millimeter);

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("rNalog.pdf");
    DokumentZaIspisA4->print(&printer);
    pw.setWindowFlags(Qt::Window);
    pw.exec();
    delete DokumentZaIspisA4;
}

void frmrnalog::IspisPreview(QPrinter *printer)
{
    Q_UNUSED(printer);
    DokumentZaIspisA4->print(printer);
}

void frmrnalog::BrisiStavku()
{

    if (!RadniNalogEdit)
        return;
    if (!RadniNalogNovi)
        return;
    int ret = QMessageBox::warning(this,"Brisanje stavke","Da li zelite obrisati stavku?",QMessageBox::Yes,QMessageBox::No);
    if (ret != QMessageBox::Yes)
        return;
    QSqlQuery q("",QSqlDatabase::database("baza"));
    //ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt()
    q.exec(QString("delete from rnalog3 where id=%1").arg(ui->tableStavke->model()->data(ui->tableStavke->model()->index(ui->tableStavke->currentIndex().row(),0)).toInt()));
    UcitajStavke();
    CistiPoljaStavka();
}
