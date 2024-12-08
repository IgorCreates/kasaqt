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




#include "ui/frmpregdokum.h"
#include "ui_frmpregdokum.h"
#include <Qt>
#include <QtSql>
#include <QtGui>
#include "Ispis/ispismali.h"
#include "ui/mojiheaderi.h"
#include "ui/frmispisodabir.h"


frmPregDokum::frmPregDokum(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPregDokum)
{
    ui->setupUi(this);
    UcitajHeader();
    this->setWindowTitle("Pregled racuna");
    ui->btnSaljiPonovno->setVisible(false);
    ui->btnRekreiranjeZKI->setVisible(false);
    ui->toolBox->installEventFilter(this);
    ui->page->installEventFilter(this);
    ui->page_2->installEventFilter(this);
    ui->page_3->installEventFilter(this);
    ui->tableView->installEventFilter(this);

    ntPregStavke = new QTableView();
    ntPregStavke->installEventFilter(this);
    ntPregStavke->close();
    PregStavke = new QDialog(this);
    PregStavke->installEventFilter(this);
    PregStavke->close();
    vbLntPregStavke = new QVBoxLayout;
    vbLntPregStavke->addWidget(ntPregStavke);
    PregStavke->setLayout(vbLntPregStavke);
    PregStavke->setObjectName("PregStavke");
    PregStavke->setWindowTitle("Pregled stavke");
    ui->toolBtnPretraga->setVisible(true);
    ui->toolBtnPretraga->setEnabled(true);
    //FilterPolje->installEventFilter(this);
    ui->toolBokPretraga->setVisible(false);
    ui->lblPoruke->setText("");
    MaxID = 0;
    MinID = 0;
    PregNarudzbe=false;


}

frmPregDokum::~frmPregDokum()
{
    delete ui;
}

bool frmPregDokum::eventFilter(QObject *target, QEvent *event)
{
    QKeyEvent *keyEvent = (QKeyEvent *)event;
    //qDebug() << event->type();
    if (event->type() == 170 )  //== QEvent::MouseButtonPress)
    {
        if ( target == ui->page)
                UcitajRacune();
        if (target == ui->page_2)
                UcitajNeposlane();
        if (target == ui->page_3)
                UcitajVRacune();
    }
    if (target == ui->tableView)
    {
        if (event->type() == QEvent::KeyRelease)
        {
            switch (keyEvent->key())
            {
                case Qt::Key_F5:
                FilterPolje = new QLineEdit();
                FilterPolje->show();
                FilterPolje->setWindowTitle("Filter po polju");
                FilterPolje->installEventFilter(this);
                FilterPolje->setFocus();
                break;
            }
        }
    }
    if (target == FilterPolje)
    {
        if (event->type() == QEvent::KeyRelease)
        {
            switch (keyEvent->key())
            {
                case Qt::Key_Escape:
                case Qt::Key_Return:
                case Qt::Key_Enter:
                    FilterPolje->close();
                    ui->tableView->selectRow(0);
                    ui->tableView->setFocus();
                    return true;
                    break;
            }
            filter_model->setFilterKeyColumn(ui->tableView->currentIndex().column());
            filter_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            filter_model->setFilterRegExp(FilterPolje->text());
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QRegularExpression re(FilterPolje->text(), QRegularExpression::CaseInsensitiveOption);
            filter_model->setFilterRegularExpression(re);
#endif


            ui->lblPoruke->setText(QString("Filter: %1 redova").arg(filter_model->rowCount()));
        }
    }
    return QWidget::eventFilter(target,event);
}


void frmPregDokum::UcitajHeader()
{
    dbH = QSqlDatabase::database("baza");
    dbH.open();
    modelHeader = new QSqlQueryModel();
    //modelHeader->setQuery(QString("select * from rac1 where kasaid=%1 order by id desc").arg(qApp->property("Firma_OznNapUr").toString()),dbH);

    modelHeader->setQuery(QString("select * from v_kasa_pregleddok where kasaid=%1 order by id desc limit 1000").arg(qApp->property("Firma_OznNapUr").toString()),dbH);

    filter_model = new QSortFilterProxyModel();
    filter_model->setSourceModel(modelHeader);
    //ui->tableView->setModel(modelHeader);
    ui->tableView->setModel(filter_model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(3,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(4,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(5,new mojiHeaderi(this));
    ui->lblPoruke->setText("Ucitano: 1000redova");
}

void frmPregDokum::on_toolBtnStorno_pressed()
{
    //ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),1)).toString())
    //QMessageBox::information(this,"Demo","U DEMO radu programa nije moguce napraviti storno","OK");
//    nmod->record(nt->currentIndex().row()).value("adresa").toString()
//    if (ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),13)).toString() == "1")
    if (modelHeader->record(ui->tableView->currentIndex().row()).value("storno").toInt() == 1)
    {
        QMessageBox::information(this,"Storno","Racun je vec storniran","OK");
        return;
    }

    QString nRacID;

    int RacID = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt();
    QString StorniraniRac = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),2)).toString();
    int ret = QMessageBox::warning(this,"STORNO - Racuna" ,QString("Da li zelite napraviti storno za racun br: %1 ?").arg(StorniraniRac),
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::No | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {
        bool UspjesnoUpisan = true;
        QSqlQuery q("",QSqlDatabase::database("baza"));
        q.exec("select value1 from opcije where sifra='OznakaSljednosti'");
        QString OznSljed = "P";
        if (q.next())
        {
            OznSljed = q.value(0).toString();
            if (OznSljed != "N") // ako nije N onda je P (zastita od krive postavke)
                OznSljed = "P";
        }
        QString qveri = QString("call proc_DohvatBrojaDokumenta('M','%1','%2','%3','%4',@NoviBR);"
                  "insert into rac1 (br,vrsta,idk,sum,bpdv,datumr,uid,obracun,stol,np,rabatp,rabatk,rsum,rbpdv,oib,cid,sid,kasaid,oznPP,zki)"
                  " select @NoviBR br,vrsta,idk,(sum*-1),(bpdv*-1),now(),uid,'N',stol,np,"
                  "rabatp,(rabatk*-1),(rsum*-1),(rbpdv*-1),oib,cid,sid,kasaid,oznPP,0 from rac1 where id=%4;set @NoviRacID=LAST_INSERT_ID();")
                  .arg(qApp->property("Firma_OznNapUr").toString()).arg(qApp->property("Firma_OznPosPr").toString()).arg(OznSljed).arg(RacID);
        //qDebug() << qveri;
        q.clear();
        if (!q.exec(qveri))
        {
            QMessageBox::warning(this,"Storno","Problem kod storno racuna","OK");
            qDebug() << q.lastQuery();
            qDebug() << q.lastError();
            UspjesnoUpisan = false;
            return;
        }else
        {
            QSqlQuery q2("",QSqlDatabase::database("baza"));
            QString qveri2 = "";
            q.exec("select @NoviRacID");
            if (q.next())
            {
                nRacID = q.value(0).toString();
            }
            qveri2 = QString("insert into rac1opis (rid,opis) values(%1,'%2')").arg(nRacID).arg(QString("Storno racuna- %1").arg(StorniraniRac));
            if (!q2.exec(qveri2))
            {
                qDebug() << q2.lastError() << q2.lastQuery();
            }
            qveri2 = QString("insert into rac2 (rid,aid,kol,pc,ukupno,rabatp,rabatk,bpdv,ukpcisti,naziv,artsif,bpdvcisti,r2localid,povratna)"
                      " select %1,aid,(kol*-1),(pc*-1),(ukupno*-1),rabatp,(rabatk*-1),(bpdv*-1),"
                             "(ukpcisti*-1),naziv,artsif,(bpdvcisti*-1),r2localid,(povratna*-1) from rac2 where rid=%2").arg(nRacID).arg(RacID);
            if (!q2.exec(qveri2))
            {
                QMessageBox::warning(this,"Storno","Problem kod storno racuna","OK");
                UspjesnoUpisan = false;
                qDebug() << q2.lastQuery();
                qDebug() << q2.lastError();
            }
            qveri2 =  QString("insert into rac1porezi (rid,pid,porez_sifra,iznos_poreza,porez_postotak,osnovica) select "
                       " %1,pid,porez_sifra,(iznos_poreza*-1),porez_postotak,(osnovica*-1) from rac1porezi where rid=%2").arg(nRacID).arg(RacID);
            if (!q2.exec(qveri2))
            {
                qDebug() << q2.lastQuery();
                qDebug() << q2.lastError();
                QMessageBox::warning(this,"Storno","Problem kod storno racuna","OK");
                UspjesnoUpisan = false;
            }
            qveri2 =  QString("insert into rac2porezi (rid,aid,pid,koeficijent_poreza,iznos_poreza,porez_opis,r2localid,porez_postotak,osnovica) select "
                              "%1,aid,pid,koeficijent_poreza,(iznos_poreza*-1),porez_opis,r2localid,porez_postotak,(osnovica*-1) from rac2porezi where rid=%2").arg(nRacID).arg(RacID);
            if (!q2.exec(qveri2))
            {
                qDebug() << q2.lastQuery();
                qDebug() << q2.lastError();
                QMessageBox::warning(this,"Storno","Problem kod storno racuna","OK");
                UspjesnoUpisan = false;
            }
            if(!q2.exec(QString("update rac2porezi r2p,rac2 r2 set r2p.r2id=r2.id where r2p.rid=r2.rid  and r2p.r2localid=r2.r2localid and r2p.rid=%1").arg(nRacID)))
            {
                qDebug() << "Greska update rac2porezi r2id " << q2.lastError() << q2.lastQuery() ;
            }

//            mojiHeaderi potpZKI;
//            potpZKI.ZKI()
        }
        q.exec(QString("update rac1 set storno='1',storno_datum=now() where id='%1'").arg(RacID));
        if (!q.exec())
            UspjesnoUpisan = false;

        q.finish();
        if (UspjesnoUpisan)
        {
            QMessageBox::information(this,"STORNO","Racun storniran","OK");
            mojiHeaderi *t = new mojiHeaderi();
            // int brojac = 0;
            // while (!t->ZKI(nRacID.toInt()))
            // {
            //     if (brojac>=3)
            //         break;
            //     brojac++;
            // }
//            t->xmlRacunZahtjev(nRacID.toInt(),false);
            t->jsonRacunZahtjev(nRacID.toInt(),false);
            UcitajHeader();
//            ispisMali *isp=new ispisMali();
//            isp->IspisMaliPos(nRacID);
            frmIspisOdabir *fisp = new frmIspisOdabir();
            fisp->show();
            fisp->RacunID = nRacID.toInt();  //IZRAC->get_Rac1ID(); // IzracRac->get_Rac1ID();
            fisp->ProvjeraOpcija();


            //qDebug() << nRacID;
        }else
        {
            QMessageBox::warning(this,"Storno","Problem kod storniranja","OK");
            UcitajHeader();
        }
    }
}

void frmPregDokum::on_toolBtnIspis_pressed()
{
    /*
    //QMessageBox::information(this,"Demo","U DEMO radu programa nije moguce naknadni ispis","OK");
    ispisMali *isp = new ispisMali();
    isp->IspisMaliPos(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toString());
    //isp->NewIsp(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt());
    */
    frmIspisOdabir *ispOd = new frmIspisOdabir(this);
    ispOd->show();
    ispOd->RacunID = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt();
    if (PregNarudzbe)
        ispOd->IspisMaliSto = "narudzba";
    ispOd->ProvjeraOpcija();
}



void frmPregDokum::on_btnPrikaziNeposlane_pressed()
{
    ui->btnRekreiranjeZKI->setVisible(false);
    UcitajNeposlane();
}
void frmPregDokum::UcitajNeposlane()
{
    dbH = QSqlDatabase::database("baza");
    dbH.open();
    modelHeader = new QSqlQueryModel();
    modelHeader->setQuery("select r1.*,rf.odgovor,rf.potvrden from rac1 r1 left join rac1fiskal rf on r1.id=rf.racid "
                          " where length(r1.zki)=32 and rf.odgovor is null group by r1.id order by id desc",dbH);
    ui->tableView->setModel(modelHeader);
    ui->tableView->setColumnHidden(0,true);
    ui->btnSaljiPonovno->setVisible(true);
    ui->lblPoruke->setText("Ucitani neposlani");
    PregNarudzbe=false;

}

void frmPregDokum::on_btnSaljiPonovno_pressed()
{
    ui->btnSaljiPonovno->setVisible(false);
    if (ui->tableView->model()->rowCount() < 1)
        return;


    int ret = QMessageBox::warning(this,"OPREZ!!!!!" ,"Da li SIGURNO zelite poslati\nove podatke na potpisivanje u PU?",
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::No | QMessageBox::Escape);
    if (ret != QMessageBox::Yes)
        return;



    ui->btnSaljiPonovno->setEnabled(false);

    QProgressBar *PP = new QProgressBar(this);
    QLabel *Poruka = new QLabel(this);
    QFont  serifFont("Times", 18, QFont::Bold);
    PP->setMaximum(ui->tableView->model()->rowCount());
    PP->setFixedHeight(200);
    PP->setFixedWidth(this->width());
    Poruka->setText("Pricekajte dok ne\nzavrsi ponovna slanja");
    Poruka->setFont(serifFont);
    PP->show();
    Poruka->setFixedSize(300,100);
    Poruka->show();
    Poruka->move((this->width()/2)-(Poruka->width()/2),(this->height()/2)-(Poruka->height()/2));
    Poruka->setStyleSheet("color: rgb(255, 0, 0);");
    //qApp->setProperty("Certs_Sifra","1");
    mojiHeaderi xml_salji;
    for (int i=0;i<ui->tableView->model()->rowCount();i++)
    {
        //qDebug() << ui->tableView->model()->index(i,0).data().toInt();
        //xml_salji.xmlRacunZahtjev(ui->tableView->model()->index(i,0).data().toInt(),true);
        xml_salji.jsonRacunZahtjev(ui->tableView->model()->index(i,0).data().toInt(),true);
        PP->setValue(i);
        if (Poruka->styleSheet() == "color: rgb(255, 0, 0);")
        {
           Poruka->setStyleSheet("color: rgb(255, 0, 0);");
        }else
        {
           Poruka->setStyleSheet("color: rgb(0, 0, 0);");
        }
    }
    PP->close();
    Poruka->close();
    UcitajNeposlane();
    ui->btnSaljiPonovno->setEnabled(true);
}

void frmPregDokum::on_btnZKI_pressed()
{
    ui->btnSaljiPonovno->setVisible(false);
    UcitajNeZKI();
}
void frmPregDokum::UcitajNeZKI()
{
    dbH = QSqlDatabase::database("baza");
    dbH.open();
    modelHeader = new QSqlQueryModel();
    modelHeader->setQuery("select r1.*,rf.odgovor,rf.potvrden from rac1 r1 left join rac1fiskal "
                          " rf on r1.id=rf.racid where length(r1.zki)<32 and rf.odgovor is null group by r1.id order by id desc",dbH);
    ui->tableView->setModel(modelHeader);
    ui->tableView->setColumnHidden(0,true);
    ui->btnRekreiranjeZKI->setVisible(true);
    ui->lblPoruke->setText("Ucitani neZKI");
}

void frmPregDokum::on_btnRekreiranjeZKI_pressed()
{
    ui->btnRekreiranjeZKI->setVisible(false);
    int ret = QMessageBox::warning(this,"OPREZ!!!!!" ,"Da li SIGURNO zelite rekreirati ZKI?",
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::No | QMessageBox::Escape);
    if (ret != QMessageBox::Yes)
        return;

    QProgressBar *PP = new QProgressBar(this);
    QLabel *Poruka = new QLabel(this);
    QFont  serifFont("Times", 18, QFont::Bold);
    PP->setMaximum(ui->tableView->model()->rowCount());
    PP->setFixedHeight(200);
    PP->setFixedWidth(this->width());
    Poruka->setText("Pricekajte dok ne\nzavrsi rekreiranje ZKI");
    Poruka->setFont(serifFont);
    PP->show();
    Poruka->setFixedSize(300,100);
    Poruka->show();
    Poruka->move((this->width()/2)-(Poruka->width()/2),(this->height()/2)-(Poruka->height()/2));
    Poruka->setStyleSheet("color: rgb(255, 0, 0);");
    //qApp->setProperty("Certs_Sifra","1");
    mojiHeaderi fZKI;
    for (int i=0;i<ui->tableView->model()->rowCount();i++)
    {
        int RacID = ui->tableView->model()->index(i,0).data().toInt();
        //fZKI.ZKI(RacID);
        fZKI.jsonRacunZahtjev(RacID,false);
        PP->setValue(i);
        if (Poruka->styleSheet() == "color: rgb(255, 0, 0);")
        {
           Poruka->setStyleSheet("color: rgb(255, 0, 0);");
        }else
        {
           Poruka->setStyleSheet("color: rgb(0, 0, 0);");
        }
    }
    PP->close();
    Poruka->close();
    UcitajNeZKI();
}

void frmPregDokum::on_toolBtnIspis_2_pressed()
{
//    ispisMali *isp = new ispisMali();
//    isp->IspisMaliPosVrac(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toString());
    frmIspisOdabir *ispOd = new frmIspisOdabir(this);
    ispOd->show();
    ispOd->RacunID = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt();
    ispOd->RacTipRacuna = "vrac1";
    ispOd->ProvjeraOpcija();
}

void frmPregDokum::UcitajRacune()
{
    PregNarudzbe=false;
    dbH = QSqlDatabase::database("baza");
    dbH.open();
    modelHeader = new QSqlQueryModel();


    QSqlQuery q("set @MaxID=(select max(id) from rac1)",dbH);
    if (!q.exec())
    {
        qDebug() << "Greska MaxID " << q.lastError() << q.lastQuery();
        modelHeader->setQuery(QString("select * from v_kasa_pregleddok where kasaid=%1 order by id desc limit 1000").arg(qApp->property("Firma_OznNapUr").toString()),dbH);
    }else
    {
        q.next();
        MaxID = q.value(0).toInt();
        modelHeader->setQuery(QString("select * from v_kasa_pregleddok where id>(@MaxID-1000) and kasaid=%1 order by id desc limit 1000").arg(qApp->property("Firma_OznNapUr").toString()),dbH);
    }

//    modelHeader->setQuery(QString("select * from v_kasa_pregleddok where kasaid=%1 order by id desc limit 1000").arg(qApp->property("Firma_OznNapUr").toString()),dbH);

    filter_model = new QSortFilterProxyModel();
    filter_model->setSourceModel(modelHeader);
    //ui->tableView->setModel(modelHeader);
    ui->tableView->setModel(filter_model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(3,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(4,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(5,new mojiHeaderi(this));
    ui->lblPoruke->setText("Ucitani racuni - 1000redova");

}
void frmPregDokum::UcitajVRacune()
{

    dbH = QSqlDatabase::database("baza");
    dbH.open();
    modelHeader = new QSqlQueryModel();
    modelHeader->setQuery(QString("select * from vrac1 where kasaid=%1 and vrsta='O' order by id desc").arg(qApp->property("Firma_OznNapUr").toString()),dbH);
    ui->tableView->setModel(modelHeader);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(3,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(4,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(5,new mojiHeaderi(this));
    ui->lblPoruke->setText("Ucitane otpremnice");
}


void frmPregDokum::on_btnPrikaziNeposlane_3_released()
{
    UcitajRacune();
}

void frmPregDokum::on_btnPrikaziNeposlane_2_released()
{
    UcitajVRacune();
}

void frmPregDokum::on_btnPrikaziStavke_released()
{
    nmodPregStavke = new QSqlQueryModel();
    nmodPregStavke->setQuery(QString("select a.artsif,r.naziv,r.kol,r.pc,r.ukupno,r.bpdv,r.rabatp,r.rabatk,r.ukpcisti from rac2 r "
                                     " left join art a on r.aid=a.id where r.rid=%1")
                             .arg(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toString())
                             ,QSqlDatabase::database("baza"));
    ntPregStavke->setModel(nmodPregStavke);
    ntPregStavke->setColumnWidth(0,150);
    ntPregStavke->setColumnWidth(1,400);
    PregStavke->showMaximized();
}

void frmPregDokum::on_toolBtnPretraga_pressed()
{
    ui->toolBokPretraga->setVisible(true);
}

void frmPregDokum::on_btnFilter_pressed()
{
    dbH = QSqlDatabase::database("baza");
    dbH.open();
    modelHeader = new QSqlQueryModel();
    QString qveri="";
    QString Upiti="";
    QString UpitiMSG="";
    qveri = "select * from v_kasa_pregleddok ";
    if (ui->txtFilterBroj->text().length()>0)
    {
        Upiti = QString(" and br=%1").arg(ui->txtFilterBroj->text());
        UpitiMSG += QString(" Broj:%1").arg(ui->txtFilterBroj->text());
    }
    if (ui->txtFilterDa->text().length()>0)
    {
//        if (Upiti != "")
//            Upiti +=  " and ";
        Upiti += QString(" and year(datumr)='%1' ").arg(ui->txtFilterDa->text());
        UpitiMSG += QString(" Datum: %1").arg(ui->txtFilterDa->text());
    }
    if (ui->txtFilterDatum_Mjesec->text().length()>0)
    {
        Upiti += QString(" and month(datumr)='%1' ").arg(ui->txtFilterDatum_Mjesec->text());
        UpitiMSG += QString(" Mjesec: %1").arg(ui->txtFilterDatum_Mjesec->text());
    }

    if (ui->txtFilterVrsta->text().length()>0)
    {
        Upiti += QString(" and vrsta='%1' ").arg(ui->txtFilterVrsta->text());
        UpitiMSG += QString(" Vrsta:%1").arg(ui->txtFilterVrsta->text());
    }
    if (ui->txtFilterProdavac->text().length()>0)
    {
        Upiti += QString(" and prodavac like('%%1%') ").arg(ui->txtFilterProdavac->text());
        UpitiMSG += QString(" Prodavac:%1").arg(ui->txtFilterProdavac->text());
    }
    if (ui->chkStorno->isChecked())
    {
        Upiti += " and storno='1' ";
        UpitiMSG += " STORNIRANI ";
    }
    qveri += QString(" where kasaid=%1 %2 order by id").arg(qApp->property("Firma_OznNapUr").toString()).arg(Upiti);
    //qDebug() << qveri;
    modelHeader->setQuery(qveri,dbH);
    ui->tableView->setModel(modelHeader);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(3,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(4,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(5,new mojiHeaderi(this));
    if (UpitiMSG == "")
    {
        UpitiMSG = " Svi ";
    }
    UpitiMSG += QString(" ,%1 redova").arg(modelHeader->rowCount());
    ui->lblPoruke->setText(QString("Ucitan filter %1").arg(UpitiMSG));
}

void frmPregDokum::on_btnFilterZatvori_pressed()
{
    ui->toolBokPretraga->setVisible(false);
}

void frmPregDokum::on_btnFilterLjevo1000_released()
{

}

void frmPregDokum::on_btnRekreirajJson_pressed()
{

    QProgressBar *PP = new QProgressBar(this);
    QLabel *Poruka = new QLabel(this);
    QFont  serifFont("Times", 18, QFont::Bold);
    Poruka->setText("Pricekajte dok ne\nzavrsi rekreiranje JSONa");
    Poruka->setFont(serifFont);
    Poruka->setFixedSize(300,100);
    Poruka->show();
    Poruka->move((this->width()/2)-(Poruka->width()/2),(this->height()/2)-(Poruka->height()/2));
    Poruka->setStyleSheet("color: rgb(255, 0, 0);");

    mojiHeaderi fZKI;
    int RacID =ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt();
    //fZKI.ZKI(RacID);
    fZKI.jsonRacunZahtjev(RacID,false);

    if (Poruka->styleSheet() == "color: rgb(255, 0, 0);")
    {
        Poruka->setStyleSheet("color: rgb(255, 0, 0);");
    }else
    {
        Poruka->setStyleSheet("color: rgb(0, 0, 0);");
    }
}

void frmPregDokum::on_btnFilterDesno1000_released()
{


}

void frmPregDokum::on_btnKreirajXRacuna_pressed()
{
    if (ui->txtKreirajXRacunaBr->text().isEmpty())
    {
        QMessageBox::warning(this,"Paznja","Morate izabrati koliko racuna zelite kreirati","OK");
        ui->txtKreirajXRacunaBr->setFocus();
        return;
    }
    if (ui->txtKreirajXRacunaBr->text().toInt() < 1 )
        return;
    QModelIndexList selectedList = ui->tableView->selectionModel()->selectedRows();
    if (selectedList.count() != 1)
    {
        QMessageBox::warning(this,"Pazanja","Morate izabrati jedan racun","OK");
        return;
    }


    int ret = QMessageBox::warning(this,"PAZNJA - OPASNA AKCIJA",QString("Da li zelite napraviti %1 racuna od trenutno izabranog racuna?"
                                   "\n***PAZNJA-Ako izaberete potvrdno jedno mozete stornirati te racune***").arg(ui->txtKreirajXRacunaBr->text()),
                                   QMessageBox::Yes,QMessageBox::No|QMessageBox::Default);
    if (ret != QMessageBox::Yes)
        return;
    ui->btnKreirajXRacuna->setEnabled(false);
    QProgressBar *PP = new QProgressBar(this);
    QLabel *Poruka = new QLabel(this);
    QFont  serifFont("Times", 18, QFont::Bold);
    PP->setMaximum(ui->txtKreirajXRacunaBr->text().toInt());
    PP->setFixedHeight(200);
    PP->setFixedWidth(this->width());
    PP->setTextVisible(true);
    Poruka->setText("Pricekajte dok ne\nzavrsi kreiranje racuna");
    Poruka->setFont(serifFont);
    PP->show();
    Poruka->setFixedSize(300,100);
    Poruka->show();
    Poruka->move((this->width()/2)-(Poruka->width()/2),(this->height()/2)-(Poruka->height()/2));
    Poruka->setStyleSheet("color: rgb(255, 0, 0);");
    //qApp->setProperty("Certs_Sifra","1");
    int RID = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt();
    bool Greska= false;

    QSqlQuery q("",QSqlDatabase::database("baza"));

    q.exec("select value1 from opcije where sifra='OznakaSljednosti'");
    QString OznSljed = "P";
    if (q.next())
    {
        OznSljed = q.value(0).toString();
        if (OznSljed != "N") // ako nije N onda je P (zastita od krive postavke)
            OznSljed = "P";
    }
    QString VrstaBrojRacuna = "M";

    for (int i=1;i<=ui->txtKreirajXRacunaBr->text().toInt();i++)
    {
        int NoviRID = 0;
        PP->setValue(i);
        //setFormat("Your text here. "+QString::number(progress)+"%");
        QString PPporuka = QString("Kreiram racun %1/%2   %p%").arg(i).arg(ui->txtKreirajXRacunaBr->text());
        PP->setFormat(PPporuka);
        if (Poruka->styleSheet() == "color: rgb(255, 0, 0);")
        {
           Poruka->setStyleSheet("color: rgb(255, 0, 0);");
        }else
        {
           Poruka->setStyleSheet("color: rgb(0, 0, 0);");
        }
        q.clear();
        if (!q.exec(QString("insert into rac1 (id,br,vrsta,idk,sum,bpdv,datumr,uid,obracun,stol,np,storno,rabatp,rabatk,rsum,rbpdv"
                           ",storno_datum,oib,zki,sid,cid,kasaid,oznpp) select 0,0,vrsta,idk,sum,bpdv,now(),%2,obracun,111,1,'0',rabatp,"
                           "rabatk,rsum,rbpdv,null,'%3',null,sid,cid,'%4','%5' from rac1 where id=%1").arg(RID).arg(qApp->property("KonobarID").toString())
                   .arg(qApp->property("KonobarOIB").toString()).arg(qApp->property("Firma_OznNapUr").toString())
                   .arg(qApp->property("Firma_OznPosPr").toString())))
        {
            qDebug() << q.lastError() << q.lastQuery();
            Greska = true;
            QMessageBox::warning(this,"GRESKA","Greska-upisa","OK");
            return;
        }else
        {
            QString qveri = "";
            QSqlQuery qs("",QSqlDatabase::database("baza"));
            qveri = "set @NoviRID=(select LAST_INSERT_ID())";
            if (!q.exec(qveri))
            {
                QMessageBox::warning(this,"Greska","Greska kod upisa","OK");
                Greska = true;
                return;
            }else
            {
                q.exec("select @NoviRID");
                q.next();
                NoviRID = q.value(0).toInt();
                if (!q.exec(QString("call proc_DohvatBrojaDokumenta('%1','%2','%3','%4',@NoviRID,@NoviBR);"
                               "update rac1 set br=@NoviBR,zki=null where id=@NoviRID;").arg(VrstaBrojRacuna)
                       .arg(qApp->property("Firma_OznNapUr").toString()).arg(qApp->property("Firma_OznPosPr").toString()).arg(OznSljed)))
                {
                    qDebug() << q.lastError() << q.lastQuery();
                    QMessageBox::warning(this,"GRESKA","Greska-upisa","OK");
                    Greska = true;
                    return;
                }
            }
            qveri = QString("insert into rac1opis (rid,opis) values (@NoviRID,'%1')").arg(ui->txtKreirajXRacunaOpis->toPlainText());
            qveri += QString(";insert into rac1porezi (rid,pid,porez_sifra,iznos_poreza,porez_postotak) select @NoviRID,pid,porez_sifra,"
                             "iznos_poreza,porez_postotak from rac1porezi  where rid=%1;").arg(RID);
            //qDebug() << "QP-" << qveri;
            if (!q.exec(qveri))
            {
                Greska = true;
                qDebug() << q.lastError() << q.lastQuery();
            }
            qs.exec(QString("select * from rac2 where rid=%1").arg(RID));
            while (qs.next())
            {
                int r2id = qs.value(qs.record().indexOf("id")).toInt();
                qveri = QString("insert into rac2 (rid,aid,artsif,kol,nc,pc,ukupno,bpdv,rabatp,rabatk,ukpcisti,bpdvcisti,naziv,id,r2localid) "
                             " select @NoviRID,aid,artsif,kol,nc,pc,ukupno,bpdv,rabatp,rabatk,ukpcisti,bpdvcisti,naziv,0,r2localid "
                             " from rac2 where id=%1; set @NR2ID=(LAST_INSERT_ID()); insert into rac2porezi (rid,aid,pid,koeficijent_poreza"
                                 ",iznos_poreza,porez_opis,porez_postotak,r2id,osnovica,r2localid) select @NoviRID,aid,pid,koeficijent_poreza"
                                 ",iznos_poreza,porez_opis,porez_postotak,@NR2ID,osnovica,r2localid from rac2porezi where r2id=%1;").arg(r2id);
                //qDebug() << "QP-" << qveri;
                if (!q.exec(qveri))
                {
                    qDebug() << q.lastError() << q.lastQuery();
                    Greska = true;
                }
            }
            if (!Greska)
            {
                PP->setFormat("Kreiram ZKI");
                mojiHeaderi fZKI;
                fZKI.ZKI(NoviRID);
            }
            //q.exec(qveri);
        }
    }
    if (Greska)
        QMessageBox::warning(this,"GRESKA","Greska kod upisa racuna","OK");
    PP->close();
    Poruka->close();
    QMessageBox::information(this,"Kreiranje zavrseno","Pokrenite rekraciju ZKI i ponovno slanje JIR-a\nNakon toga pokrenite ispis od-do");
    ui->btnKreirajXRacuna->setEnabled(true);

}

void frmPregDokum::on_toolBtnIspisOdDo_pressed()
{
    int row;
    int rid=0;
    int rid2=0;
    frmIspisOdabir *ispOd = new frmIspisOdabir(this);
    ispOd->show();
    ispOd->RacuniOdDo = true;




    QModelIndexList selectedList = ui->tableView->selectionModel()->selectedRows();
    for( int i=0; i<selectedList.count(); i++)
    {
        //QMessageBox::information(this,"", QString::number(selectedList.at(i).row()));
        ispOd->RacuniID << ui->tableView->model()->data(ui->tableView->model()->index(selectedList.at(i).row(),0)).toInt();
    }
    //ispOd->RacunID = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt();
    //ispOd->RacuniID = Brojjj; //<< 1 << 2 << 3 << 4 << 5 << 6 <<7 << 8 << 9 << 10 ;//= ui->tableView->selectionMode()
    ispOd->ProvjeraOpcija();

}

void frmPregDokum::on_btnIzlaz_released()
{
    int ret = QMessageBox::question(this,"Pregled","Da li zelite izaci iz pregleda?",QMessageBox::Yes,QMessageBox::No);

    if (ret != QMessageBox::Yes)
        return;
    this->close();
}

void frmPregDokum::on_btnPrikazNarudzbe_clicked(bool checked)
{
    dbH = QSqlDatabase::database("baza");
    dbH.open();
    modelHeader = new QSqlQueryModel();


    modelHeader->setQuery(QString("select * from narudzba1 order by id desc"),dbH);

//    modelHeader->setQuery(QString("select * from v_kasa_pregleddok where kasaid=%1 order by id desc limit 1000").arg(qApp->property("Firma_OznNapUr").toString()),dbH);

    filter_model = new QSortFilterProxyModel();
    filter_model->setSourceModel(modelHeader);
    //ui->tableView->setModel(modelHeader);
    ui->tableView->setModel(filter_model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(3,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(4,new mojiHeaderi(this));
    ui->tableView->setItemDelegateForColumn(5,new mojiHeaderi(this));
    ui->lblPoruke->setText("Ucitani racuni - 1000redova");
    PregNarudzbe=true;
}
