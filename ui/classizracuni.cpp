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




#include "classizracuni.h"
#include <QtSql>
#include <QString>
#include "ui/mojiheaderi.h"
#include "Ispis/ispismali.h"
#include "QtGui"
#include <QMessageBox>

//
// NE KORISTI SE VISE
//

classIzracuni::classIzracuni()
{
    sBpdv=0;
    sKolicina = 0;
    sJedcj = 0;
    sRabatp = 0;
    sRabatk =0;
    sPPOcifra = 0;
    sSSUMA = 0;
    sPPOkoef = 0;
    sPDVkoef = 0;
    sPDVcifra = 0;
    sUkpCisti = 0;

}
bool classIzracuni::DodajStavku(QString ArtSifra,int aAID, int RACID,int BrStola, const double Kolicina)
{
    sBpdv=0;
    sKolicina = 0;
    sJedcj = 0;
    sRabatp = 0;
    sRabatk =0;
    sPPOcifra = 0;
    sSSUMA = 0;
    sPPOkoef = 0;
    sPDVkoef = 0;
    sPDVcifra = 0;
    sUkpCisti = 0;
    int AID=0;
    //QString ArtSifra = "";
    QString ArtNaziv = "";
    int noviRac2ID = 0;



    sKolicina = Kolicina + 1;

    QSqlDatabase dbB = QSqlDatabase::database("baza");
    QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
    QSqlQuery q("",dbB);
    QSqlQuery ql("",dbL);
    QString qveri;
    QString qveri2;

    QString nSQL;
    nSQL = QString("select * from viz_rac where artsif = '%1' and sid=%2 and cid=%3").arg(ArtSifra).arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt());


    if (!q.exec(nSQL))
        return false;
    if (!q.next())
    {
        return false;
    }else
    {
        int artGrupaID;
        int artIspisKuhinja;
        int columnGrupaID = q.record().indexOf("grupaid");
        int columnIspisKuhinja = q.record().indexOf("ispis_kuhinja");
        artGrupaID = q.value(columnGrupaID).toInt();
        artIspisKuhinja = q.value(columnIspisKuhinja).toInt();
        ArtNaziv = q.value(2).toString();
        AID = q.value(0).toInt();
        if (AID == 0 && aAID == 0)
            return false;
        if (AID == 0)
        {
            AID = aAID;
        }
        sJedcj = q.value(3).toDouble();
        sSSUMA = sKolicina * sJedcj;
        sSSUMA = QString::number(sSSUMA,'f',2).toDouble();
        q.exec(QString("select * from viz_porezi where aid='%1'").arg(AID));
        qDebug() << AID;
        int i=0;
        double PoreziSUMA = 0;
        while (q.next())
        {
            double Porez= sSSUMA * q.value(2).toDouble();
//            Porez = mojiHeaderi.DecTocka(Porez);
            qDebug() << "Porez suma:" << sSSUMA << ",val2:" << q.value(2).toDouble();
            PoreziSUMA += Porez;
            qDebug() << "Porez:" << PoreziSUMA;
//            Porez = QString::number(Porez,'f',2).toDouble();
        }
        if (PoreziSUMA == 0)
        {
            sBpdv = sSSUMA;
        }else
        {
            sBpdv = sSSUMA - PoreziSUMA;
//            sBpdv = mojiHeaderi.DecTocka(sBpdv);
            qDebug() << "***" << sBpdv << ",p " << PoreziSUMA
                     << "," << sSSUMA << "," << sBpdv - PoreziSUMA;

        }

        qveri = "insert into rac2 (dok,aid,artsif,naziv,kol,nc,pc,ukupno,rabatP,rabatK,ukpcisti,bpdv,grupaid,ispis_kuhinja,uid)";
        qveri += QString("values (\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",0").arg(BrStola).arg(AID).arg(ArtSifra)
                .arg(ArtNaziv).arg(sKolicina);
        qveri += QString(",\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",\"%6\",\"%7\",\"%8\",\"%9\")").arg(sJedcj).arg(sSSUMA).arg(sRabatp).arg(sRabatk)
                .arg(sSSUMA).arg(sBpdv).arg(artGrupaID).arg(artIspisKuhinja).arg(qApp->property("KonobarID").toString());

        if (!ql.exec(qveri))
        {
            //QMessageBox::warning(this,"Greska u upisu","Molim provjerite unesene podatke");
            qDebug() << ql.lastError();
            qDebug() << ql.lastQuery();
            return false;
        }else
        {
            noviRac2ID = ql.lastInsertId().toInt();
        }

        q.exec(QString("select * from viz_porezi where aid='%1'").arg(AID));
        i=0;
        PoreziSUMA = 0;
        while (q.next())
        {
            qDebug() << "RC  " << i << q.record().count();
            double Porez =0;
            Porez= sSSUMA * q.value(2).toDouble();
            PoreziSUMA += Porez;
            Porez = QString::number(Porez,'f',2).toDouble();
            qveri2 = "insert into rac4 (dok,aid,pid,koeficijent_poreza,iznos_poreza,porez_sifra,r2id)  ";
            qveri2 += QString(" select '%1','%2','%3','%4','%5','%6','%7'").arg(BrStola).arg(AID).arg(q.value(3).toInt())
                .arg(q.value(2).toDouble()).arg(QString::number(Porez,'f',2).toDouble())
                .arg(q.value(4).toString()).arg(noviRac2ID);
            i++;
            if (!ql.exec(qveri2))
            {
                qDebug() << ql.lastError();
                qDebug() << ql.lastQuery();
            }
        }
        if (!ql.exec(QString("delete from rac2 where id=%1").arg(RACID)))
            qDebug() << "Greska Del-" << q.lastError() << " " << q.lastQuery();
        if (!ql.exec(QString("delete from rac4 where r2id=%1").arg(RACID)))
            qDebug() << "Greska Del-" << q.lastError() << " " << q.lastQuery();
        return true;
    }
}
bool classIzracuni::UpisiBrziRacun(int BrStola)
{
        int RacID;
        bool UspjesnoUpisan = true;
        bool Dalje =false;

        QSqlDatabase dbB = QSqlDatabase::database("baza");
        QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
        QSqlQuery q("",dbB);
        QSqlQuery qLoc("",dbL);
        QString qveri;
        qLoc.exec(QString("select sum(ukupno) ukp,sum(bpdv) bpdv from rac2 where dok=%1").arg(BrStola));
        if (qLoc.next())
        {
            sSSUMA = qLoc.value(0).toDouble();
            sBpdv = qLoc.value(1).toDouble();
        }else{
            return false;
        }

        if (!dbB.open())
        {
            return false;
        }
        if ( sSSUMA == 0 )
        {
            UspjesnoUpisan = false;
            Dalje = false;
            return false;
        }
        QString mojTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        QString TMP_Table_Rac1 = "tmp_rac1_" + mojTime;
        QString TMP_Table_Rac2 = "tmp_rac2_" + mojTime;
        QString TMP_Table_Rac1p = "tmp_rac1p_" + mojTime;
        QString TMP_Table_Rac2p = "tmp_rac2p_" + mojTime;


        q.exec(QString("CREATE temporary TABLE `%1` (`id` int(6) NOT NULL AUTO_INCREMENT,"
               "`vrsta` enum('M','V') DEFAULT NULL,"
               "`idk` int(6) DEFAULT NULL,"
               "`sum` double(14,4) DEFAULT NULL,"
               "`bpdv` double(14,4) DEFAULT NULL,"
               "`datumr` datetime DEFAULT NULL,"
               "`uid` int(6) DEFAULT NULL,"
               "`obracun` enum('N','Y') DEFAULT NULL,"
               "`stol` varchar(10) DEFAULT NULL,"
               "`np` int(3) DEFAULT NULL,"
               "`storno` enum('0','1') DEFAULT '0',"
               "`rabatp` double(14,4) DEFAULT NULL,"
               "`rabatk` double(14,4) DEFAULT NULL,"
               "`rsum` double(14,4) DEFAULT NULL,"
               "`rbpdv` double(14,4) DEFAULT NULL,"
               "`storno_datum` datetime DEFAULT NULL,"
               "`oib` char(11) NOT NULL,"
               "`zki` char(32) NOT NULL,"
               "`sid` int(10) NOT NULL,"
               "`cid` int(10) NOT NULL,"
               "`kasaid` int(10) DEFAULT NULL,"
                       "oznPP varchar(20),"
               "PRIMARY KEY (`id`)"
                       ") ENGINE=InnoDB").arg(TMP_Table_Rac1));
        q.exec(QString("CREATE temporary TABLE %1 ("
                       "`rid` int(6) DEFAULT NULL,"
                       "`aid` int(9) DEFAULT NULL,"
                       "`kol` double(14,4) DEFAULT NULL,"
                       "`nc` double(14,4) DEFAULT NULL,"
                       "`pc` double(14,4) DEFAULT NULL,"
                       "`ukupno` double(14,4) DEFAULT NULL,"
                       "`bpdv` double(14,4) DEFAULT NULL,"
                       "`rabatP` double(14,4) DEFAULT NULL,"
                       "`rabatK` double(14,4) DEFAULT NULL,"
                       "`ukpcisti` double(14,4) DEFAULT NULL,"
                       "`naziv` varchar(200) DEFAULT NULL,"
                       "`id` int(10) NOT NULL AUTO_INCREMENT,"
                       "`r2localid` int(10) DEFAULT NULL,"
                       "PRIMARY KEY (`id`)"
                       ") ENGINE=InnoDB DEFAULT CHARSET=utf8 ").arg(TMP_Table_Rac2));
        q.exec(QString("CREATE temporary TABLE %1 ("
                       "`rid` int(10) NOT NULL,"
                       "`pid` int(10) unsigned DEFAULT NULL,"
                       "`porez_sifra` char(5) DEFAULT NULL,"
                       "`iznos_poreza` double(14,4) DEFAULT NULL"
                       ") ENGINE=InnoDB DEFAULT CHARSET=utf8").arg(TMP_Table_Rac1p));
        q.exec(QString("CREATE temporary TABLE %1 ("
                       "`rid` int(10) NOT NULL,"
                       "`aid` int(10) NOT NULL,"
                       "`pid` int(10) NOT NULL,"
                       "`koeficijent_poreza` double(14,6) NOT NULL DEFAULT '0.000000',"
                       "`iznos_poreza` double(14,6) NOT NULL DEFAULT '0.000000',"
                       "`porez_opis` char(5) DEFAULT NULL,"
                       "`r2localid` int(10) DEFAULT NULL"
                     ") ENGINE=InnoDB DEFAULT CHARSET=utf8 ").arg(TMP_Table_Rac2p));
        q.prepare(QString("insert into %1 (vrsta,idk,sum,bpdv,datumr,uid,obracun,stol,np,rabatp,rabatk,rsum,rbpdv,cid,sid,oib,kasaid,oznPP) "
                      "values(:vrsta,:idk,:sum,:bpdv,:datumr,:uid,:obracun,:stol,:np,:rabatp,:rabatk,"
                          ":rsum,:rbpdv,:cid,:sid,:oib,:kasaid,:oznPP)").arg(TMP_Table_Rac1));
                // ovo dodaj samo ako je multi kasa and kasaid=%1").arg(qApp->property("Firma_OznNapUr").toString()));

        q.bindValue(":vrsta","M");
        q.bindValue(":idk","");
        q.bindValue(":sum",sSSUMA);
        q.bindValue(":bpdv",sBpdv);
        q.bindValue(":datumr",QDateTime::currentDateTime());
        q.bindValue(":obracun","N");
        q.bindValue(":uid",qApp->property("KonobarID").toString());
        q.bindValue(":stol",BrStola);
        q.bindValue(":np","1");
        q.bindValue(":rabatp",0);
        q.bindValue(":rabatk",0);
        q.bindValue(":rsum",0);
        q.bindValue(":rbpdv",0);
        q.bindValue(":vrstaU","M");
        q.bindValue(":cid",qApp->property("Prog-CID").toInt());
        q.bindValue(":sid",qApp->property("Prog-SID").toInt());
        q.bindValue(":oib",qApp->property("KonobarOIB").toString());
        q.bindValue(":kasaid",qApp->property("Firma_OznNapUr").toString());
        q.bindValue(":oznPP",qApp->property("Firma_OznPosPr").toString());
        if (!q.exec())
        {
            qDebug() << q.lastQuery();
            qDebug() << q.lastError();
            UspjesnoUpisan = false;
            return false;
        }else
        {
            RacID = q.lastInsertId().toInt();
            qDebug() << RacID;
        }


        qLoc.exec(QString("select dok,aid,pid,koeficijent_poreza,sum(iznos_poreza) iznos,porez_sifra from rac4 where dok=%1 group by pid").arg(BrStola));
        qveri = QString("insert into %1 (rid,pid,porez_sifra,iznos_poreza)").arg(TMP_Table_Rac1p);
        int i=0;
        while (qLoc.next())
        {
            if (i>0)
            {
                qveri += " union all ";
            }
            double izn = qLoc.value(4).toDouble();
            qveri += QString(" select '%1','%2','%3','%4'").arg(RacID).arg(qLoc.value(2).toInt())
                    .arg(qLoc.value(5).toString()).arg(izn);
            i++;
            Dalje = true;
        }
        if (Dalje)
            q.exec(qveri);




        //f1- rac2-stavke racuna
        if (!qLoc.exec(QString("select * from rac2 where dok='%1'").arg(BrStola)))
        {
            qDebug() << qLoc.lastError();
            UspjesnoUpisan = false;
            return false;
        }else
        {
            qDebug() << "d11-b " << QDateTime::currentDateTime().toString("hhmmss");

            qveri = QString("insert into %1 (rid,aid,kol,pc,ukupno,rabatp,rabatk,ukpcisti,naziv,bpdv,r2localid) ").arg(TMP_Table_Rac2);
            i=0;
            Dalje = false;
            while (qLoc.next())
            {
                qDebug() << "b1 " << QDateTime::currentDateTime().toString("hhmmss");

                Dalje = true;
                int ArtID = qLoc.value(2).toInt();
                if (i>0)
                    qveri += " union all ";

                //int uAID = qLoc.record().indexOf("aid");
                qveri += QString(" select '%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11'").arg(RacID)
                        .arg(ArtID).arg(qLoc.value(5).toDouble()).arg(qLoc.value(7).toDouble()).arg(qLoc.value(8).toDouble())
                        .arg(qLoc.value(9).toDouble()).arg(qLoc.value(10).toDouble()).arg(qLoc.value(11).toDouble())
                        .arg(qLoc.value(4).toString()).arg(qLoc.value(12).toDouble()).arg(qLoc.value(0).toInt());
                QSqlQuery qPor("",dbL);
                QString qveri2;
                qPor.exec(QString("select * from rac4 where dok=%1 and aid=%2 and r2id=%3").arg(BrStola).arg(qLoc.value(2).toInt()).arg(qLoc.value(0).toInt()));
                qveri2 =QString("insert into %1 (rid,aid,pid,koeficijent_poreza,iznos_poreza,porez_opis,r2localid) ").arg(TMP_Table_Rac2p);
                int y = 0;
                bool Dalje2 = false;
                while (qPor.next())
                {
                    Dalje2 = true;
                    if (y>0)
                        qveri2 += " union all ";

                    qveri2 += QString(" select '%1','%2','%3','%4','%5','%6','%7'").arg(RacID).arg(ArtID).arg(qPor.value(3).toInt())
                            .arg(qPor.value(4).toDouble()).arg(qPor.value(5).toDouble()).arg(qPor.value(6).toString()).arg(qPor.value(7).toInt());
                    y++;
                }
                //qDebug() << qveri2;
                if (Dalje2)
                {
                    if (!q.exec(qveri2))
                    {
                        qDebug() << q.lastError() << q.lastQuery();
                        UspjesnoUpisan = false;
                        return false;
                    }
                }
                i++;
            }
           // qDebug() << qveri;
            if (Dalje)
            {
                if(!q.exec(qveri))
                {
                    qDebug() << q.lastError() << q.lastQuery();
                    UspjesnoUpisan = false;
                    return false;
                }
            }
        }
        mojiHeaderi *t = new mojiHeaderi();
        if (UspjesnoUpisan == true)
        {
            QString sqlR = QString("insert into rac1 select '',0,r.vrsta,r.idk,r.sum,r.bpdv,r.datumr,r.uid,r.obracun,r.stol,r.np,"
                                   "r.storno,r.rabatp,r.rabatk,r.rsum,r.rbpdv,r.storno_datum,r.oib,r.zki,r.sid,r.cid,r.kasaid,r.oznPP from %1 r")
                    .arg(TMP_Table_Rac1);//.arg(qApp->property("Firma_OznNapUr").toString());//.arg(qApp->property("Firma_OznPosPr").toString()).arg(qApp->property("P").toString());

            if (!q.exec(sqlR))
            {
                qDebug() << q.lastQuery() << q.lastError();
                UspjesnoUpisan = false;
                return false;
            }else{
                RacID = q.lastInsertId().toInt();
                sqlR = QString("call proc_DohvatBrojaDokumenta('M','%1','%2','%3','%4',@NoviBR);update rac1 set br=@NoviBR where id=%4")
                        .arg(qApp->property("Firma_OznNapUr").toString()).arg(qApp->property("Firma_OznPosPr").toString())
                        .arg(qApp->property("P").toString()).arg(RacID);

                if (!q.exec(sqlR))
                    qDebug()<< "Greska update br rac " << q.lastError() << q.lastQuery();
            }
            if (!q.exec(QString("insert into rac1porezi select %1,pid,porez_sifra,iznos_poreza from %2").arg(RacID).arg(TMP_Table_Rac1p)))
            {
                qDebug() << q.lastQuery() << q.lastError();
                q.exec(QString("create table zPukoRac1p_%1 select * from %2").arg(RacID).arg(TMP_Table_Rac1p));
            }
            if (!q.exec(QString("insert into rac2 select %1,aid,kol,nc,pc,ukupno,bpdv,rabatp,rabatk,ukpcisti,naziv,0,r2localid from %2").arg(RacID).arg(TMP_Table_Rac2)))
            {

                QString greska = q.lastError().databaseText();
                qDebug() << q.lastQuery() << q.lastError();
                if (greska.contains("Recursive limit 0 (as set by the max_sp_recursion_depth variable) was exceeded for routine proc_rac3_insert_normativ"))
                {
                    QWidget *bb = new QWidget();
                    QMessageBox::warning(bb,"PROBLEM UPISA","Racun nije ispravno upisan\n"
                                         "Neki artikal vam ima krivo podesene normative\ni zavrtio je upis u petlju\nNazovite support za rjesavanje problema","OK");
                }
                q.exec(QString("create table zPukoRac2_%1 select %1,aid,kol,nc,pc,ukupno,bpdv,rabatp,rabatk,ukpcisti,naziv,r2localid from %2").arg(RacID).arg(TMP_Table_Rac2));
            }
            if (!q.exec(QString("insert into rac2porezi select %1,aid,pid,koeficijent_poreza,iznos_poreza,porez_opis,r2localid from %2").arg(RacID).arg(TMP_Table_Rac2p)))
            {
                qDebug() << q.lastQuery() << q.lastError();
                q.exec(QString("Create table zPukoRac2p_%1 select %1,aid,pid,koeficijent_poreza,iznos_poreza,porez_opis,r2localid from %2").arg(RacID).arg(TMP_Table_Rac2p));
            }
            int brojac=0;
            while (!t->ZKI(RacID))
            {
                if (brojac>=3)
                    break;
                brojac++;
            }
        }
        if (UspjesnoUpisan == true){

            qDebug() << "xml-ide " << QDateTime::currentDateTime().toString("hhmmss");
            //t->xmlRacunZahtjev(RacID,false);
            t->jsonRacunZahtjev(RacID,false);
            qDebug() << "xml-gotov " << QDateTime::currentDateTime().toString("hhmmss");

            if (dbL.isOpen())
                dbL.open();
            int Brojac = 0;
            qDebug() << QDateTime::currentDateTime().toString("hhmmss");
            while (!qLoc.exec(QString("delete from rac2 where dok=%1").arg(BrStola)))
            {
                if (Brojac >= 5)
                    break;
                dbL.close();
                dbL.open();
                qDebug() << qLoc.lastError() << qLoc.lastQuery();
                Brojac++;
            }
            while (!qLoc.exec(QString("delete from rac4 where dok=%1").arg(BrStola)))
            {
                if (Brojac >= 5)
                    break;
                dbL.close();
                dbL.open();
                qDebug() << qLoc.lastError() << qLoc.lastQuery();
                Brojac++;
            }
            while (!qLoc.exec(QString("delete from ispKuhinja where dok=%1").arg(BrStola)))
            {
                if (Brojac >= 5)
                    break;
                dbL.close();
                dbL.open();
                qDebug() << qLoc.lastError() << qLoc.lastQuery();
                Brojac++;
            }
            qDebug() << QDateTime::currentDateTime().toString("hhmmss");

            return true;
        }else{
            qDebug() << "******************\nGreska kod upisa racuna\n******************\n";
            return false;
        }
}


bool classIzracuni::NoviUpisRacun(int BrLokalniRac)
{


}
