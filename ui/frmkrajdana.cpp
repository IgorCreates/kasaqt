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




#include "frmkrajdana.h"
#include "ui_frmkrajdana.h"
#include <QtSql>
#include <QtGui>
#include "Ispis/ispismali.h"
#include "ui/frmispisodabir.h"
//#include "ui/QProgressIndicator.h"
#include <QMessageBox>

frmKrajDana::frmKrajDana(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmKrajDana)
{
    ui->setupUi(this);
    this->setWindowTitle("Fiskal Kasa");
    model = new QSqlQueryModel();
    modelKorisnik = new QSqlQueryModel();
    dbp = QSqlDatabase::database("baza");
    ui->txtPologKasa->setText(qApp->property("Polog-").toString());

    ui->btnIspisStanjeKase->setEnabled(false);
    ui->btnIspisZakljucakKorisnik->setEnabled(false);
    ui->txtStanjeDatumOd->setDate(QDate::currentDate());
    ui->txtStanjeDatumDo->setDate(QDate::currentDate());
    ui->txtStanjeSatDoN->setTime(QTime::currentTime());
    ui->txtZakljKSatDo->setTime(QTime::currentTime());
    ui->txtZakljKDatumOd->setDate(QDate::currentDate());
    ui->txtZakljKDatumDo->setDate(QDate::currentDate());
    ui->txtZakljKSatOd->setTime(QTime::fromString("00:00"));

    //ui->btnIspisStanjeKase_2->setVisible(false);

    //dbp
    //UcitajStanja();
}

frmKrajDana::~frmKrajDana()
{
    delete ui;
}
void frmKrajDana::UcitajStanja()
{
    if (!dbp.open())
    {
        qDebug() << "\nGreska db-"+dbp.lastError().text()+"\n";
        //qDebug() << err.ConnectionError;
        QMessageBox::warning(this, "Ne mogu otvoriti bazu", "greska Otvaranje konekcije: " + dbp.lastError().text());
    }
    //()<< QSqlDatabase::database().databaseName();
    model->setQuery(QString("select k.naziv as nacin_placanja,sum(sum) as suma,r1.kasaid from rac1 r1 left join kartice"
                    " k on r1.np=k.id where r1.kasaid=%1 and  date(datumr)=date(now()) group by k.id").arg(qApp->property("Firma_OznNapUr").toString()),dbp);
/*
    ui->tableView->setModel(model);
    ui->txtPologKasa->setText(qApp->property("Polog-").toString());
    modelDan = new QSqlQueryModel();
    modelDan->setQuery("select k.naziv as nacin_placanja,sum(sum) as suma from rac1 r1 left join "
                       " kartice k on r1.np=k.id where date(datumr)=date(now()) group by k.id" ,dbp);
    ui->tableZakljucakDan->setModel(modelDan);
*/
    modelKorisnik->setQuery(QString("select k.naziv as nacin_placanja,sum(sum) as suma,r1.kasaid from rac1 r1 left join"
                                    " kartice k on r1.np=k.id where date(datumr)=date(now()) and r1.uid=%1 and r1.kasaid=%2"
                                    " group by k.id union all select razlog,iznos,kasa_id from kasa_stanje where date(datum)=date(now()) and kasa_id=%2").arg(qApp->property("KonobarID").toString()).arg(qApp->property("Firma_OznNapUr").toString()),dbp);
    ui->tableZakljucakKorisnik->setModel(modelKorisnik);
    dbp.close();
    dbp.database().close();
}

void frmKrajDana::on_btnIspisZakljucakKorisnik_pressed()
{

    if (ui->tableZakljucakKorisnik->model()->rowCount() < 0)
        return;

    QDateTime DatumOD;
    QDateTime DatumDO;
    DatumOD.setDate(ui->txtZakljKDatumOd->date());
    DatumDO.setDate(ui->txtZakljKDatumDo->date());
    if (ui->radioZakljKvrijeme->isChecked())
    {
        //sa vremenenom
        DatumOD.setTime(ui->txtZakljKSatOd->time());
        DatumDO.setTime(ui->txtZakljKSatDo->time());
    }else
    {
        //bez vremena
        DatumOD.setTime(QTime::fromString("00:00"));
        DatumDO.setTime(QTime::fromString("23:59:59"));
    }
    ispisMali *isp = new ispisMali();
    isp->IspisKrajDanaKorisnik(DatumOD,DatumDO,qApp->property("KonobarID").toInt(),true);
}

void frmKrajDana::on_pushButton_pressed()
{
    QString qveri = QString("select k.naziv as nacin_placanja,sum(sum) as suma,r1.kasaid,min(datumr) dat_start,max(datumr) dat_kraj from rac1 r1 "
                            " left join  kartice k on r1.np=k.id where r1.kasaid=%1 and ").arg(qApp->property("Firma_OznNapUr").toString());

    QDateTime DatumOD;
    QDateTime DatumDO;
    DatumOD.setDate(ui->txtStanjeDatumOd->date());
    DatumDO.setDate(ui->txtStanjeDatumDo->date());
    if (ui->radioStanjeVrijeme->isChecked())
    {
        //sa vremenenom
        //QDateTime DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " " + ui->txtStanjeSatOd->text(),"dd.MM.yyyy hh:mm");
        //QDateTime DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " " + ui->txtStanjeSatDo->text(),"dd.MM.yyyy hh:mm");
        DatumOD.setTime(ui->txtStanjeSatOdN->time());
        DatumDO.setTime(ui->txtStanjeSatDoN->time());
        qveri += QString("datumr between '%1' and '%2'").arg(DatumOD.date().toString("yyyy-MM-dd") + " " + DatumOD.time().toString("hh:mm")).arg(DatumDO.date().toString("yyyy-MM-dd") + " " + DatumDO.time().toString("hh:mm"));
    }else
    {
        //bez vremena
        /*
        QDateTime DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " 00:00","dd.MM.yyyy hh:mm");// + ui->txtStanjeSatOd->text();
        //QDateTime DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text(),"dd.MM.yyyy");// + ui->txtStanjeSatDo->text();
        QDateTime DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " 23:59:59","dd.MM.yyyy hh:mm:ss");
        */
        DatumOD.setTime(QTime::fromString("00:00"));
        DatumDO.setTime(QTime::fromString("23:59:59"));
        qveri += QString("date(datumr) between '%1' and '%2'").arg(DatumOD.date().toString("yyyy-MM-dd")).arg(DatumDO.date().toString("yyyy-MM-dd"));
    }

    qveri += " group by k.id";
    qveri += " union all select razlog,iznos,kasa_id,min(datum) dat_start,max(datum) dat_kraj from kasa_stanje where ";
    if (ui->radioStanjeVrijeme->isChecked())
    {
        /*
        QDateTime DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " " + ui->txtStanjeSatOd->text(),"dd.MM.yyyy hh:mm");
        QDateTime DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " " + ui->txtStanjeSatDo->text(),"dd.MM.yyyy hh:mm");
        */
        DatumOD.setTime(ui->txtStanjeSatOdN->time());
        DatumDO.setTime(ui->txtStanjeSatDoN->time());
        qveri += QString(" datum between '%1' and '%2' ").arg(DatumOD.toString("yyyy-MM-dd") + " " + DatumOD.time().toString("hh:mm"))
                .arg(DatumDO.toString("yyyy-MM-dd") + " " + DatumDO.time().toString("hh:mm"));
    }else
    {
        /*
        QDateTime DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " 00:00","dd.MM.yyyy hh:mm");
        QDateTime DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " 23:59:59","dd.MM.yyyy hh:mm:ss");
        //qDebug() << "T:" << DatumDO.time().toString("hh:mm:ss");
        */
        DatumOD.setTime(QTime::fromString("00:00"));
        DatumDO.setTime(QTime::fromString("23:59:59"));
        qveri += QString(" datum between '%1' and '%2' ").arg(DatumOD.toString("yyyy-MM-dd") + " " + DatumOD.time().toString("hh:mm"))
                .arg(DatumDO.toString("yyyy-MM-dd") + " " + DatumDO.time().toString("hh:mm:ss"));
    }
    qDebug() << "*" << qveri << "*";
    model->setQuery(qveri,dbp);
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
    ui->btnIspisStanjeKase->setEnabled(true);
}

void frmKrajDana::on_pushButton_2_pressed()
{
        QString qveri = QString("select k.naziv as nacin_placanja,sum(sum) as suma,r1.kasaid,min(datumr) dat_start,max(datumr) dat_kraj from "
                                "rac1 r1 left join  kartice k on r1.np=k.id where r1.uid=%1 and r1.kasaid=%2 and ")
                .arg(qApp->property("KonobarID").toString()).arg(qApp->property("Firma_OznNapUr").toString());
        QDateTime DatumOD;
        QDateTime DatumDO;
        DatumOD.setDate(ui->txtZakljKDatumOd->date());
        DatumDO.setDate(ui->txtZakljKDatumDo->date());
        DatumOD.setTime(QTime::fromString("00:00"));
        DatumDO.setTime(QTime::fromString("23:59:59"));

        if (ui->radioZakljKvrijeme->isChecked())
        {
            //sa vremenenom
            /*
            QDateTime DatumOD = QDateTime::fromString(ui->txtZakljKDatumOd->text() + " " + ui->txtZakljKSatOd->text(),"dd.MM.yyyy hh:mm");
            QDateTime DatumDO = QDateTime::fromString(ui->txtZakljKDatumDo->text() + " " + ui->txtZakljKSatDo->text(),"dd.MM.yyyy hh:mm");
            */
            DatumOD.setTime(ui->txtZakljKSatOd->time());
            DatumDO.setTime(ui->txtZakljKSatDo->time());
            qveri += QString("datumr between '%1' and '%2'").arg(DatumOD.date().toString("yyyy-MM-dd") + " " + DatumOD.time().toString("hh:mm")).arg(DatumDO.date().toString("yyyy-MM-dd") + " " + DatumDO.time().toString("hh:mm"));
        }else
        {
            //bez vremena
            /*
            QDateTime DatumOD = QDateTime::fromString(ui->txtZakljKDatumOd->text() + " 00:00","dd.MM.yyyy hh:mm");// + ui->txtStanjeSatOd->text();
            QDateTime DatumDO = QDateTime::fromString(ui->txtZakljKDatumDo->text() + " 23:59:59","dd.MM.yyyy hh:mm:ss");// + ui->txtStanjeSatDo->text();
            */
            DatumOD.setTime(QTime::fromString("00:00"));
            DatumDO.setTime(QTime::fromString("23:59:59"));
            qveri += QString("date(datumr) between '%1' and '%2'").arg(DatumOD.date().toString("yyyy-MM-dd")).arg(DatumDO.date().toString("yyyy-MM-dd"));
        }

        qveri += " group by k.id";
        qveri += " union all select razlog,iznos,kasa_id,min(datum) dat_start,max(datum) dat_kraj from kasa_stanje where ";
        if (ui->radioStanjeVrijeme->isChecked())
        {
            /*
            QDateTime DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " " + ui->txtStanjeSatOd->text(),"dd.MM.yyyy hh:mm");
            QDateTime DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " " + ui->txtStanjeSatDo->text(),"dd.MM.yyyy hh:mm");
            */
            DatumOD.setTime(ui->txtZakljKSatOd->time());
            DatumDO.setTime(ui->txtZakljKSatDo->time());
            qveri += QString(" datum between '%1' and '%2' ").arg(DatumOD.toString("yyyy-MM-dd") + " " + DatumOD.time().toString("hh:mm"))
                    .arg(DatumDO.toString("yyyy-MM-dd") + " " + DatumDO.time().toString("hh:mm"));
        }else
        {
            /*
            QDateTime DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " 00:00","dd.MM.yyyy hh:mm");
            QDateTime DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " 23:59:59","dd.MM.yyyy hh:mm:ss");
            //qDebug() << "T:" << DatumDO.time().toString("hh:mm:ss");
            */
            qveri += QString(" datum between '%1' and '%2' ").arg(DatumOD.toString("yyyy-MM-dd") + " " + DatumOD.time().toString("hh:mm"))
                    .arg(DatumDO.toString("yyyy-MM-dd") + " " + DatumDO.time().toString("hh:mm:ss"));
        }
        qveri += QString(" and korisnik_id=%1").arg(qApp->property("KonobarID").toString());

        qDebug() << qveri;
        modelKorisnik->setQuery(qveri,dbp);
        ui->tableZakljucakKorisnik->setModel(modelKorisnik);
        ui->btnIspisZakljucakKorisnik->setEnabled(true);
}

void frmKrajDana::on_btnIspisStanjeKase_pressed()
{
    QDateTime DatumOD;
    QDateTime DatumDO;
    DatumOD.setDate(ui->txtStanjeDatumOd->date());
    DatumDO.setDate(ui->txtStanjeDatumDo->date());
    DatumOD.setTime(QTime::fromString("00:00"));
    DatumDO.setTime(QTime::fromString("23:59:59"));

    if (ui->radioStanjeVrijeme->isChecked())
    {
        //sa vremenenom
        /*
        DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " " + ui->txtStanjeSatOd->text(),"dd.MM.yyyy hh:mm");
        DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " " + ui->txtStanjeSatDo->text(),"dd.MM.yyyy hh:mm");
        */
        DatumOD.setTime(ui->txtStanjeSatOdN->time());
        DatumDO.setTime(ui->txtStanjeSatDoN->time());
    }else
    {
        //bez vremena
        /*
        DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " 00:00","dd.MM.yyyy hh:mm");// + ui->txtStanjeSatOd->text();
        DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " 23:59:59","dd.MM.yyyy hh:mm:ss");// + ui->txtStanjeSatDo->text();
        */
    }
    //novi
    ispisMali *isp = new ispisMali();
    //isp->IspisStanjeKase(DatumOD,DatumDO);
    if (qApp->property("Printer-POS_RPT_DirektanIspis").toString() != "1")
    {
        isp->IspisKrajDanaKorisnik(DatumOD,DatumDO,0,true);
    }else
    {
        isp->IspisKrajDanaKorisnik(DatumOD,DatumDO,0,false);

        frmIspisOdabir *fis = new frmIspisOdabir();
        fis->ispisPOS_poFile("/tmp/ispStanje1.txt");
    }

}

void frmKrajDana::on_btnUzimanjeGotovine_released()
{
    UzimanjeGotovineIznos = new QLineEdit;
    UzimanjeGotovineOpis = new QPlainTextEdit;
    //NoviNazivProzor = new QWidget();
    dialogUzimanjeGotovineGotovine = new QDialog(this);
    UzimanjeGotovineUpis = new QPushButton("UPIS");
    UzimanjeGotovineIzlaz = new QPushButton("IZLAZ");
    vboxLayout = new QVBoxLayout;
    QLabel *lbl = new QLabel("Unesite iznos :");
    QLabel *lbl2 = new QLabel("Unesite razlog uzimanja \ngotovine iz kase");

    UzimanjeGotovineIznos->setText(0);
    UzimanjeGotovineIznos->setAlignment(Qt::AlignRight);
    //UzimanjeGotovineIznos->setMask();

    UzimanjeGotovineIznos->setObjectName("txtUzimanjeGotovine");
    UzimanjeGotovineUpis->setObjectName("btnUzimanjeGotovineUpis");
    vboxLayout->addWidget(lbl);
    vboxLayout->addWidget(UzimanjeGotovineIznos);
    vboxLayout->addWidget(lbl2);
    vboxLayout->addWidget(UzimanjeGotovineOpis);
    vboxLayout->addWidget(UzimanjeGotovineUpis);
    vboxLayout->addWidget(UzimanjeGotovineIzlaz);
    dialogUzimanjeGotovineGotovine->setLayout(vboxLayout);
    dialogUzimanjeGotovineGotovine->show();


    connect(UzimanjeGotovineIzlaz,SIGNAL(released()),dialogUzimanjeGotovineGotovine,SLOT(close()));
    //connect(NoviNazivUpis,SIGNAL(released()),this,SLOT(IzmijeniNaziv()));
    connect(UzimanjeGotovineUpis,SIGNAL(released()),this,SLOT(uzimanjeGotovineSlot()));
//    connect(UzimanjeGotovineUpis,SIGNAL(released()),dialogUzimanjeGotovineGotovine,SLOT(close()));

}

void frmKrajDana::uzimanjeGotovineSlot()
{
    int ret = QMessageBox::question(this,"CASH",QString("Da li zelite uzeti iz blagajne : %1 kn\nza potrebe: %2").arg(UzimanjeGotovineIznos->text()).arg(UzimanjeGotovineOpis->toPlainText()),QMessageBox::Yes,QMessageBox::No|QMessageBox::Default);
    if (ret != QMessageBox::Yes)
        return;
    QSqlQuery q("",QSqlDatabase::database("baza"));
    q.prepare("insert into kasa_stanje (datum,kasa_id,korisnik_id,iznos,razlog) values(now(),:KID,:UID,:IZNOS,:RAZLOG)");
    q.bindValue(":KID",qApp->property("Firma_OznNapUr").toString());
    q.bindValue(":UID",qApp->property("KonobarID").toString());
    q.bindValue(":IZNOS",UzimanjeGotovineIznos->text().toDouble());
    q.bindValue(":RAZLOG",UzimanjeGotovineOpis->toPlainText());
    if (!q.exec())
    {
        qDebug() << "Greska upisa CASHE: " << q.lastError() << q.lastQuery();
        QMessageBox::warning(this,"CASH","Greska kod upisa\n pokusajte ponovno nakon ponovnog pokretanja programa\nu slucaju da se greska opet pojavi prijavite problem informaticarima","ok");
        return;
        dialogUzimanjeGotovineGotovine->close();
    }else
    {
        QMessageBox::information(this,"CASH","Upisano");
        dialogUzimanjeGotovineGotovine->close();
    }
}

void frmKrajDana::on_btnPrikaziDatum_released()
{
        ui->tableView->setColumnHidden(3,false);
        ui->tableView->setColumnHidden(4,false);
}

void frmKrajDana::on_btnIspisStanjeKase_2_released()
{


//    QProgressIndicator *pi = new QProgressIndicator();

    //pi->setParent(this);
//    qDebug() << "this_thr:" << this->thread()->currentThreadId();
    //QFuture<void> t1 = QtConcurrent::run(pi,"startAnimation");
//    pi->show();
    //pi->move(this->width()/2,this->height()/2);
//    pi->setFixedSize(300,300);
//    pi->startAnimation();
//    qDebug() << "thr-id:" << thr->currentThreadId();
//    qDebug() << "pi-thrid:" << pi->thread()->currentThreadId();





    int ret= QMessageBox::warning(this,"Detaljan PDF","Izrada detaljnog PDF-a moze potrajati zavisno od jacine racunala i kolicine podataka\nDa li zelite pokrenuti?",
                                  QMessageBox::Yes,QMessageBox::No|QMessageBox::Default);
    if (ret != QMessageBox::Yes)
        return;


    frmIspisOdabir *fis = new frmIspisOdabir();
    QDateTime DatumOD;
    QDateTime DatumDO;
    DatumOD.setDate(ui->txtStanjeDatumOd->date());
    DatumDO.setDate(ui->txtStanjeDatumDo->date());
    DatumOD.setTime(QTime::fromString("00:00"));
    DatumDO.setTime(QTime::fromString("23:59:59"));


    if (ui->radioStanjeVrijeme->isChecked())
    {
        //sa vremenenom
        /*
        DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " " + ui->txtStanjeSatOd->text(),"dd.MM.yyyy hh:mm");
        DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " " + ui->txtStanjeSatDo->text(),"dd.MM.yyyy hh:mm");
        */
        DatumOD.setTime(ui->txtStanjeSatOdN->time());
        DatumDO.setTime(ui->txtStanjeSatDoN->time());
    }else
    {
        //bez vremena
        /*
        DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " 00:00","dd.MM.yyyy hh:mm");// + ui->txtStanjeSatOd->text();
        DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " 23:59:59","dd.MM.yyyy hh:mm:ss");// + ui->txtStanjeSatDo->text();
        */
    }
    //fis->ispisA4stanjeTemplate(DatumOD,DatumDO,qApp->property("Firma_OznNapUr").toString());

    fis->ispisA4templateStanje(DatumOD,DatumDO,qApp->property("Firma_OznNapUr").toInt(),"Veliki");
    //pi->stopAnimation();
}

void frmKrajDana::on_btnIspisStanjeKaseVeliki_released()
{
    QDateTime DatumOD;
    QDateTime DatumDO;
    DatumOD.setDate(ui->txtStanjeDatumOd->date());
    DatumDO.setDate(ui->txtStanjeDatumDo->date());
    DatumOD.setTime(QTime::fromString("00:00"));
    DatumDO.setTime(QTime::fromString("23:59:59"));

    if (ui->radioStanjeVrijeme->isChecked())
    {
        //sa vremenenom
        /*
        DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " " + ui->txtStanjeSatOd->text(),"dd.MM.yyyy hh:mm");
        DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " " + ui->txtStanjeSatDo->text(),"dd.MM.yyyy hh:mm");
        */
        DatumOD.setTime(ui->txtStanjeSatOdN->time());
        DatumDO.setTime(ui->txtStanjeSatDoN->time());

    }else
    {
        //bez vremena
        /*
        DatumOD = QDateTime::fromString(ui->txtStanjeDatOd->text() + " 00:00","dd.MM.yyyy hh:mm");// + ui->txtStanjeSatOd->text();
        DatumDO = QDateTime::fromString(ui->txtStanjeDatDo->text() + " 23:59:59","dd.MM.yyyy hh:mm:ss");// + ui->txtStanjeSatDo->text();
        */
    }
    ispisMali *isp = new ispisMali();
    //isp->IspisStanjeKase(DatumOD,DatumDO);
    isp->IspisKrajDanaKorisnik(DatumOD,DatumDO,0,false);
    frmIspisOdabir *fis = new frmIspisOdabir();
    fis->ispisA4templateStanje(DatumOD,DatumDO,qApp->property("Firma_OznNapUr").toInt(),"Mali");


}


