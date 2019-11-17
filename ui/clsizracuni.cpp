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




#include "clsizracuni.h"
#include "mojiheaderi.h"
#include <QtGui>
#include <QMessageBox>

clsIzracuni::clsIzracuni(QObject *parent) :
    QObject(parent)
{
    stavkaListaPoreza = new QStandardItemModel(0,5);

}

void clsIzracuni::DefVrijednostVarijabli()
{
    stavkaListaPoreza->clear();
     _stavkaAID = 0;
     _Rac2ID_Stari = 0;
     _stavkaArtSIF = "";
     _stavkaArtNaziv = "";
     _stavkaArtGrupaID = 0;
     _stavkaArtIspisKuhinja = 0;
     _stavkaKorisnikID = 0;
     _stavkaBrStola = 0;
     _stavkaBPDV = 0;
     _stavkaBPDVcista = 0;
     _stavkaSumPorezi = 0;
     _stavkaSumPoreziCista = 0;
     _stavkaPCcista = 0;
     _stavkaPC = 0;
     _stavkaNC = 0;
     _stavkaUkupno = 0;
     _stavkaUkupnoCista = 0;
     _stavkaRabatP = 0;
     _stavkaRabatK = 0;
     _stavkaKolicina = 1;
     _stavkaPovratnaNaknada=0;
     _crmid = 0;
     _stavkaNarucioUID = 0;
     _stavkaNarudzba = 0;


//racunH
     _Rac1ID = 0;
     //_RacVrsta = 'M'; //'M','V'
     _RacKupacID = 0;
     _RacSUM = 0;
     _RacSUMCisti = 0;
     _RacBPDV = 0;
     _RacBPDVCisiti = 0;
     _RacUID = 0;
     _RacObracun = '0'; //'0','1'
     _RacStol =0;
     _RacNP = 0;
     _RacStorno = 0;
     _RacRabatP = 0;
     _RacRabatK = 0;
     _RacTipRacuna = "";
     _RacStol = 1;
     _RacKupacAdresa="";
     _RacKupacMB = "";
     _RacKupacMjesto = "";
     _RacKupacNaziv = "";
     _RacKupacOIB = "";
     _RacKupacSifra= "";
     _RacKupacZemlja = "";
     _RacSumPovratnaNaknada=0;
     _RacOpisPolje="";
     _RacNarucioUID=0;
     _RacNarID=0;


     _PorezDostava = false;


}
inline double round2(double n, unsigned d)
{
    return floor(n * pow(10., d) + .5) / pow(10., d);
}

void clsIzracuni::izracunMPC()
{
    _stavkaPCcista = _stavkaPC-_stavkaPovratnaNaknada;
    _stavkaUkupno = _stavkaKolicina*_stavkaPC-(_stavkaKolicina*_stavkaPovratnaNaknada);
    _stavkaUkupno = round2(_stavkaUkupno,2);
    _stavkaUkupnoCista = _stavkaUkupno;
    _stavkaRabatK = (_stavkaUkupno * _stavkaRabatP)/100;
    _stavkaRabatK = round2(_stavkaRabatK,2);
    _stavkaUkupno = _stavkaUkupno - _stavkaRabatK;
    _stavkaUkupno = round2(_stavkaUkupno,2);
    QSqlQuery q("",QSqlDatabase::database("baza"));
    // if (!_PorezDostava)
    if (true)
    {
        q.exec(QString("select * from viz_porezi where aid='%1'").arg(_stavkaAID));
    }else
    {
        //testirano  05.05.2018 - upit vraca PNP nekim cudom (valjda zato sto je na prvom mjestu)
        q.exec(QString("select `a`.`aid` AS `aid`,case when p.poreztip='PDV' then pd.postotak else `p`.`postotak` end AS `postotak`,"
                       "(((case when p.poreztip='PDV' then pd.postotak else p.postotak end) * 100) / (100 + "
                       "(select sum((case when p.poreztip='PDV' then p2.postotak else p.postotak end))*100 from `artiklporez` `ap` "
                       "left join `porez` `p` on (ap.porezid=p.id),(select postotak from porez where id=1) p2  where   (`ap`.`aid` = a.aid)))) AS `koef`"
                       ",case when p.poreztip='PDV' then pd.id else `p`.`id`  end AS `id`,case when p.poreztip='PDV' then pd.sifra else `p`.`sifra` "
                       " end AS `sifra`  from (`artiklporez` `a` left join `porez` `p` on((`a`.`porezid` = `p`.`id`))),(select id,postotak,sifra from"
                       " porez where id=1) pd where a.aid=%1").arg(_stavkaAID));
    }
    int i=0;
    _stavkaSumPorezi = 0;
    _stavkaSumPoreziCista =0;
    int pKoef = q.record().indexOf("koef"); // koeficijent
    int pPostotak = q.record().indexOf("postotak"); //postotak
    int pSifra = q.record().indexOf("sifra"); //sifra poreza
    int pID = q.record().indexOf("id");

    stavkaListaPoreza->clear();
    while (q.next())
    {
        double Porez= _stavkaUkupno * q.value(pKoef).toDouble();
        double PorezCisti= _stavkaUkupnoCista * q.value(pKoef).toDouble();
        double Porez_Postotak = q.value(pPostotak).toDouble();
        _stavkaSumPorezi += Porez;
        _stavkaSumPoreziCista += PorezCisti;
        QStandardItem *red1 = new QStandardItem(QString("%1").arg(q.value(pSifra).toString()));
        QStandardItem *red2 = new QStandardItem(QString("%1").arg(q.value(pKoef).toDouble()));
        QStandardItem *red3 = new QStandardItem(QString("%1").arg(Porez));
        QStandardItem *red4 = new QStandardItem(QString("%1").arg(PorezCisti));
        QStandardItem *red5 = new QStandardItem(QString("%1").arg(q.value(pID).toInt()));
        QStandardItem *red6 = new QStandardItem(QString("%1").arg(Porez_Postotak));
        stavkaListaPoreza->setItem(i,0,red1);
        stavkaListaPoreza->setItem(i,1,red2);
        stavkaListaPoreza->setItem(i,2,red3);
        stavkaListaPoreza->setItem(i,3,red4);
        stavkaListaPoreza->setItem(i,4,red5);
        stavkaListaPoreza->setItem(i,5,red6);
        i++;
    }
    _stavkaBPDV = _stavkaUkupno-_stavkaSumPorezi;
    _stavkaBPDVcista = _stavkaUkupnoCista-_stavkaSumPoreziCista;
    _stavkaUkupnoCista += (_stavkaKolicina*_stavkaPovratnaNaknada);
    _stavkaUkupno += (_stavkaKolicina*_stavkaPovratnaNaknada);

}
void clsIzracuni::izracunVPC()
{

}
bool clsIzracuni::UpisStavkeUBazu()
{
    bool Dalje = true;
    QSqlDatabase db2;
    db2 = QSqlDatabase::database("local.sqlite");
    if (!db2.open())
    {
        Dalje = false;
        qDebug() << "\nGreska db-"+db2.lastError().text()+"\n";
    }

    QSqlQuery q("",db2);
    QString qveri;




    int noviRac2ID;
    qDebug() << "izrac naruid " << _stavkaNarucioUID;
    qveri = "insert into rac2 (dok,aid,artsif,naziv,kol,nc,pc,ukupno,rabatP,rabatK,ukpcisti,bpdv,grupaid"
            ",ispis_kuhinja,uid,bpdvcisti,povratnanaknada,datedodano,narudzba,narucio_uid,narid)";
    qveri += " values(:dok,:aid,:artsif,:naziv,:kol,:nc,:pc,:ukupno,:rabatp,:rabatk,:ukupcisti,:bpdv,:grupaid,"
            ":ispis_kuhinja,:uid,:bpdvcisti,:povratna,now(),:narudzba,:narucio_uid,:narid)";
    q.prepare(qveri);
    q.bindValue(":dok",_stavkaBrStola);
    q.bindValue(":aid",_stavkaAID);
    q.bindValue(":artsif",_stavkaArtSIF);
    q.bindValue(":naziv",_stavkaArtNaziv);
    q.bindValue(":kol",_stavkaKolicina);
    q.bindValue(":nc",0);
    q.bindValue(":pc",_stavkaPC),
    q.bindValue(":ukupno",_stavkaUkupno);
    q.bindValue(":rabatp",_stavkaRabatP);
    q.bindValue(":rabatk",_stavkaRabatK);
    q.bindValue(":ukpcisti",_stavkaUkupnoCista);
    q.bindValue(":bpdv",_stavkaBPDV);
    q.bindValue(":grupaid",_stavkaArtGrupaID);
    q.bindValue(":ispis_kuhinja",_stavkaArtIspisKuhinja);
    q.bindValue(":uid",qApp->property("KonobarID").toString());
    q.bindValue(":bpdvcisti",_stavkaBPDVcista);
    q.bindValue(":povratna",_stavkaPovratnaNaknada);
    q.bindValue(":narudzba",_stavkaNarudzba);
    q.bindValue(":narucio_uid",_stavkaNarucioUID);
    q.bindValue(":narid",_RacNarID);
    //if (!q.exec(qveri))
    if (!q.exec())
    {
        qDebug() << q.lastError();
        qDebug() << q.lastQuery();
        Dalje=  false;
        return false;
    }else
    {
        noviRac2ID = q.lastInsertId().toInt();
    }
    //qDebug() << " Pore " << stavkaListaPoreza->rowCount();
    if (stavkaListaPoreza->rowCount() > 0)
    {
        qveri = "insert into rac4 (dok,aid,pid,koeficijent_poreza,iznos_poreza,porez_sifra,r2id,porez_postotak,narid)  ";
        for (int i=0;i<stavkaListaPoreza->rowCount();i++)
        {
            if (i>0)
                qveri +=" union all ";
            qveri += QString(" select '%1','%2','%3','%4','%5','%6','%7','%8','%9'").arg(_stavkaBrStola).arg(_stavkaAID)
                    .arg(stavkaListaPoreza->index(i,4).data().toInt()).arg(stavkaListaPoreza->index(i,1).data().toDouble())
                    .arg(QString::number(stavkaListaPoreza->index(i,2).data().toDouble(),'f',2).toDouble())
                    .arg(stavkaListaPoreza->index(i,0).data().toString()).arg(noviRac2ID).arg(stavkaListaPoreza->index(i,5).data().toDouble())
                    .arg(_RacNarID);
        }
        if (!q.exec(qveri))
        {
            qDebug() << q.lastError();
            qDebug() << q.lastQuery();
            Dalje = false;
        }else{
            if (!q.exec("update rac4 r4,rac2 r2 set r4.osnovica=r2.bpdv where r4.r2id=r2.id and r4.dok=r2.dok"))
            {
                qDebug() << "Greska update osnovica r4 " << q.lastError() << q.lastQuery();
            }
        }
    }

    if (_Rac2ID_Stari != 0 && Dalje == true)
    {
        if (!q.exec(QString("delete from rac2 where id=%1").arg(_Rac2ID_Stari)))
            qDebug() << "Greska Del-" << q.lastError() << " " << q.lastQuery();
        if (!q.exec(QString("delete from rac4 where r2id=%1").arg(_Rac2ID_Stari)))
            qDebug() << "Greska Del-" << q.lastError() << " " << q.lastQuery();
    }
    if (db2.isOpen())
    {
       db2.close();
    }
    if (!Dalje)
    {
        if (!q.exec(QString("delete from rac2 where id=%1").arg(noviRac2ID)))
                qDebug() << "Greska kod upisa stavke : " << q.lastError() << q.lastQuery();
    }
    return Dalje;
}

bool clsIzracuni::UcitajArt()
{
    QSqlQuery q("",QSqlDatabase::database("baza"));
    QString nSQL;
    nSQL = QString("select * from viz_rac where artsif = '%1' and sid=%2 and cid=%3")
            .arg(_stavkaArtSIF).arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt());

    if (!q.exec(nSQL))
        return false;
    if (!q.next())
    {
        return false;
    }else
    {
        int columnGrupaID = q.record().indexOf("grupaid");
        int columnIspisKuhinja = q.record().indexOf("ispis_kuhinja");
        int columnNaziv = q.record().indexOf("naziv");
        int columnPC = q.record().indexOf("pc");
        int columnPovratnaNaknada = q.record().indexOf("povratnanaknada");
        _stavkaArtGrupaID = q.value(columnGrupaID).toInt();
        _stavkaArtIspisKuhinja = q.value(columnIspisKuhinja).toInt();
        _stavkaArtNaziv = q.value(columnNaziv).toString();
        _stavkaAID = q.value(0).toInt();
        _stavkaPovratnaNaknada = q.value(columnPovratnaNaknada).toDouble();
        if (_stavkaAID == 0)
            return false;
        _stavkaPC = q.value(columnPC).toDouble();
        if (_stavkaPC == 0)
            return false;
    }
    return true;
}

bool clsIzracuni::UcitajLocalRac()
{
    bool Dalje = true;
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    QString qveri = QString("select sum(ukupno),sum(kol*povratnanaknada) from rac2 where dok=%1 and narid=%2")
            .arg(_RacStol).arg(_RacNarID);
    if (!q.exec(qveri))
    {
        Dalje = false;
        qDebug() << "Greska UcitajLocalRac " << q.lastError();
    }
    if (q.next())
    {
        _RacSumPovratnaNaknada = q.value(1).toDouble();
        _RacSUM = q.value(0).toDouble()-q.value(1).toDouble();
        _RacSUMCisti = _RacSUM;
    }

    q.exec(QString("select porez_sifra,sum(iznos_poreza),pid from rac4 where dok=%1 and narid=%2 group by porez_sifra")
           .arg(_RacStol).arg(_RacNarID));
    int i=0;
    _RacSumPorezi =0;
    _RacSumPoreziCista = 0;
    while (q.next())
    {
        _RacSumPorezi += q.value(1).toDouble() -((q.value(1).toDouble()*_RacRabatP)/100);
        _RacSumPoreziCista += q.value(1).toDouble();
        QStandardItem *red1 = new QStandardItem(QString("%1").arg(q.value(0).toString()));
        QStandardItem *red2 = new QStandardItem(QString("%1").arg(q.value(1).toDouble()));
        QStandardItem *red3 = new QStandardItem(QString("%1").arg(q.value(2).toInt()));
        stavkaListaPoreza->setItem(i,0,red1);
        stavkaListaPoreza->setItem(i,1,red2);
        stavkaListaPoreza->setItem(i,2,red3);
        i++;
    }
    _RacBPDVCisiti = _RacSUMCisti - _RacSumPoreziCista;
    _RacBPDV = _RacSUM - _RacSumPorezi;
    _RacSUM = _RacSUM+_RacSumPovratnaNaknada;
    _RacSUMCisti = _RacSUM+_RacSumPovratnaNaknada;
    return Dalje;
}

bool clsIzracuni::izracunRAC()
{
    //qDebug() << "izracunRAC Start";
    if (_RacStol == 0)
        return false;
    _RacSUM = _RacSUM-_RacSumPovratnaNaknada;
    _RacSUMCisti = _RacSUMCisti-_RacSumPovratnaNaknada;
    _RacRabatK = ((_RacSUM*_RacRabatP)/100);
    _RacSUM = _RacSUM-_RacRabatK;
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    if (!q.exec(QString("select porez_sifra,sum(iznos_poreza),pid,porez_postotak from rac4 where dok=%1 and narid=%2 group by porez_sifra").arg(_RacStol).arg(_RacNarID)))
    {
        qDebug() << "Greska IZR.izrRac " << q.lastError() << q.lastQuery();
    }
    //qDebug() << q.lastQuery();
    int i=0;
    _RacSumPorezi =0;
    _RacSumPoreziCista = 0;

    while (q.next())
    {
        _RacSumPorezi += q.value(1).toDouble() -((q.value(1).toDouble()*_RacRabatP)/100);
        _RacSumPoreziCista += q.value(1).toDouble();
        //qDebug() << QString("Rad-%1 %2 %3").arg(i).arg(_RacSumPorezi).arg(_RacSumPoreziCista);
        QStandardItem *red1 = new QStandardItem(QString("%1").arg(q.value(0).toString()));
        QStandardItem *red2 = new QStandardItem(QString("%1").arg(_RacSumPorezi));  //new QStandardItem(QString("%1").arg(q.value(1).toDouble()));
        QStandardItem *red3 = new QStandardItem(QString("%1").arg(_RacSUMCisti));
        QStandardItem *red4 = new QStandardItem(QString("%1").arg(q.value(2).toInt()));
        QStandardItem *red5 = new QStandardItem(QString("%1").arg(q.value(4).toDouble()));

        stavkaListaPoreza->setItem(i,0,red1);
        stavkaListaPoreza->setItem(i,1,red2);
        stavkaListaPoreza->setItem(i,2,red3);
        stavkaListaPoreza->setItem(i,3,red4);
        stavkaListaPoreza->setItem(i,4,red5);
        i++;
    }
    _RacBPDV = _RacSUM - _RacSumPorezi;
    _RacSUM += _RacSumPovratnaNaknada;
    _RacSUMCisti += _RacSumPovratnaNaknada;

    return true;
}

bool clsIzracuni::UpisiRac()
{
    int RacID;
        bool UspjesnoUpisan = true;
        bool Dalje =false;
        bool MozeDalje = true;


        QSqlDatabase dbB = QSqlDatabase::database("baza");
        QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
        QSqlQuery q("",dbB);
        QSqlQuery qLoc("",dbL);
        QString qveri;
        if (!dbB.open())
        {
            return false;
        }
        if ( _RacSUM == 0 )
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

        if (_RacTipRacuna == "O")
        {
            TMP_Table_Rac1 = "vrac1";
            TMP_Table_Rac2 = "vrac2";
            TMP_Table_Rac1p = "vrac1porezi";
            TMP_Table_Rac2p = "vrac2porezi";
        }else
        {
            TMP_Table_Rac1 = "rac1";
            TMP_Table_Rac2 = "rac2";
            TMP_Table_Rac1p = "rac1porezi";
            TMP_Table_Rac2p = "rac2porezi";
        }



        if (_RacVrstaRac == "V") {
            QSqlQuery qProvKupac("",QSqlDatabase::database("baza"));
            QString qverKupac = QString("select * from company where sifra='%1' and (oib='%2' or naziv='%3')")
                                    .arg(_RacKupacSifra).arg(_RacKupacOIB).arg(_RacKupacNaziv);

            if (qProvKupac.exec(qverKupac))
            {
                if (qProvKupac.next())
                {
                    _RacKupacID = qProvKupac.value(0).toInt();
                    q.prepare(QString("update company set naziv=:naziv,oib=:oib,adresa=:adresa,mb=:mb,mjesto=:mjesto,sifra=:sifra,countrycode=:countrycode where id=%1").arg(_RacKupacID));
                    q.bindValue(":naziv",_RacKupacNaziv);
                    q.bindValue(":adresa",_RacKupacAdresa);
                    q.bindValue(":oib",_RacKupacOIB);
                    q.bindValue(":mb",_RacKupacMB);
                    q.bindValue(":sifra",_RacKupacSifra);
                    q.bindValue(":mjesto",_RacKupacMjesto);
                    q.bindValue(":countrycode",_RacKupacZemlja);
                    q.exec();
                }else
                {
                    q.prepare("insert into company (naziv,adresa,mjesto,oib,mb,sifra,countrycode) "
                              "values(:naziv,:adresa,:mjesto,:oib,:mb,:sifra,:countrycode)");
                    q.bindValue(":naziv",_RacKupacNaziv);
                    q.bindValue(":adresa",_RacKupacAdresa);
                    q.bindValue(":mjesto",_RacKupacMjesto);
                    q.bindValue(":oib",_RacKupacOIB);
                    q.bindValue(":mb",_RacKupacMB);
                    q.bindValue(":sifra",_RacKupacSifra);
                    q.bindValue(":countrycode",_RacKupacZemlja);

                    if (!q.exec())
                    {
                        QMessageBox::warning(new QWidget,"Upis kupca","Problem kod upisa kupca,\rprovjerite da li kupac sa istim nazivom,oibom vec postoji u bazi","OK");
                        UspjesnoUpisan=  false;
                        qDebug() << q.lastError() << q.lastQuery();
                        //return false;
                        MozeDalje = false;
                        UspjesnoUpisan = false;
                        return false;
                    }else
                    {
                        _RacKupacID = q.lastInsertId().toInt();
                    }
                }
            }
        }

        QString qver = QString("insert into %1 (vrsta,idk,sum,bpdv,datumr,uid,obracun,stol,np,rabatp,rabatk,rsum,"
                               "rbpdv,cid,sid,oib,kasaid,oznPP").arg(TMP_Table_Rac1);
        if (_RacNarID >0)
        {
            qver += ",narudzba,narucio_uid,narid)";
        }else
        {
            qver += ",narid) ";
        }
        qver +=  " values(:vrsta,:idk,:sum,:bpdv,:datumr,:uid,:obracun,:stol,:np,:rabatp,:rabatk,"
                  ":rsum,:rbpdv,:cid,:sid,:oib,:kasaid,:oznPP";
        if (_RacNarID >0)
        {
            qver += ",:narudzba,:narucio_uid,:narid)";
        }else
        {
            qver += ",:narid)";
        }

        q.prepare(qver);
                // ovo dodaj samo ako je multi kasa and kasaid=%1").arg(qApp->property("Firma_OznNapUr").toString()));

        //qveri.bindValue(":br",nBrojRacuna);
        if (_RacTipRacuna == "O")
        {
            q.bindValue(":vrsta","O");
        }else
        {
            q.bindValue(":vrsta",_RacVrstaRac);
        }
        q.bindValue(":idk",_RacKupacID);
        q.bindValue(":sum",_RacSUM);
        q.bindValue(":bpdv",_RacBPDV);
        q.bindValue(":datumr",QDateTime::currentDateTime());
        q.bindValue(":obracun","N");
        q.bindValue(":uid",qApp->property("KonobarID").toString());
        q.bindValue(":stol",_RacStol);
        q.bindValue(":np",_RacIDNacinPlacanja);
        q.bindValue(":rabatp",_RacRabatP);
        q.bindValue(":rabatk",_RacRabatK);
        q.bindValue(":rsum",_RacSUMCisti);
        q.bindValue(":rbpdv",_RacBPDVCisiti);
        //q.bindValue(":vrstaU",_RacVrstaRac);
        q.bindValue(":cid",qApp->property("Prog-CID").toInt());
        q.bindValue(":sid",qApp->property("Prog-SID").toInt());
        q.bindValue(":oib",qApp->property("KonobarOIB").toString());
        q.bindValue(":kasaid",qApp->property("Firma_OznNapUr").toString());
        q.bindValue(":oznPP",qApp->property("Firma_OznPosPr").toString());
        if (_RacNarID>0)
        {
            q.bindValue(":narudzba","1");
            q.bindValue(":narucio_uid",_RacNarucioUID);
        }else
        {
//            q.bindValue();
        }
        q.bindValue(":narid",_RacNarID);

        if (!q.exec())
        {
            qDebug() << q.lastError() <<  q.lastQuery();
            UspjesnoUpisan = false;
            //return false;
            MozeDalje = false;
        }else
        {
            RacID = q.lastInsertId().toInt();
            if (_RacTipRacuna == "O")
                q.exec(QString("update vrac1 set datumv=datumr where id=%1").arg(RacID));
            qDebug() << RacID;
        }
        //qDebug() << "UKP=" << _RacSUM << ",BPDV=" << _RacBPDV << ",BPDVCisti=" << _RacBPDVCisiti;




//ovo bi trebalo preracunat po sum(osnovica)*faktor_poreza da dobijem sumu poreza
        if (!qLoc.exec(QString("select dok,aid,pid,koeficijent_poreza,sum(iznos_poreza) iznos,porez_sifra,porez_postotak,sum(osnovica) osn from rac4 where dok=%1 group by pid").arg(_RacStol)))
        {
            //return false;
            qDebug() << "GRESKA upisarac clsizracun linija 562 " << q.lastError() << q.lastQuery();
            MozeDalje = false;
            UspjesnoUpisan = false;
        }
        qveri = QString("insert into %1 (rid,pid,porez_sifra,iznos_poreza,porez_postotak,osnovica)").arg(TMP_Table_Rac1p);
        int i=0;
        int colP1porez_postotak = qLoc.record().indexOf("porez_postotak");
        int colP1osnovica = qLoc.record().indexOf("osn");
        int colP1iznos_poreza = qLoc.record().indexOf("iznos");
        int colP1pid = qLoc.record().indexOf("pid");
        int colP1porez_sifra = qLoc.record().indexOf("porez_sifra");

        while (qLoc.next())
        {
            if (i>0)
            {
                qveri += " union all ";
            }
            double izn = qLoc.value(colP1iznos_poreza).toDouble() - ((qLoc.value(colP1iznos_poreza).toDouble()*_RacRabatP)/100);
            double osnovica = qLoc.value(colP1osnovica).toDouble() - ((qLoc.value(colP1osnovica).toDouble()*_RacRabatP)/100);
            qveri += QString(" select '%1','%2','%3','%4','%5','%6'").arg(RacID).arg(qLoc.value(colP1pid).toInt())
                    .arg(qLoc.value(colP1porez_sifra).toString()).arg(izn).arg(qLoc.value(colP1porez_postotak).toDouble())
                    .arg(osnovica);
            i++;
            Dalje = true;
        }
        if (Dalje)
        {
            if (!q.exec(qveri))
            {
                qDebug() << q.lastError() << q.lastQuery();
                //return false;
                MozeDalje = false;
                UspjesnoUpisan = false;

            }
        }




        //f1- rac2-stavke racuna
        if (!qLoc.exec(QString("select * from rac2 where dok='%1' and narid=%2").arg(_RacStol).arg(_RacNarID)))
        {
            qDebug() << qLoc.lastError() << qLoc.lastQuery();
            UspjesnoUpisan = false;
            //return false;
            MozeDalje = false;
        }else
        {
            QString TMP_Table_Rac2_PoljeBPDVcisti = "bpdvcisti";
            if (_RacTipRacuna == "O")
            {
                TMP_Table_Rac2_PoljeBPDVcisti = "pcbpdv";
            }
            qveri = QString("insert into %1 (rid,aid,kol,pc,ukupno,rabatp,rabatk,ukpcisti,naziv,bpdv,r2localid,%2,artsif ").arg(TMP_Table_Rac2).arg(TMP_Table_Rac2_PoljeBPDVcisti);
            i=0;
            Dalje = false;
            int colR2locID = qLoc.record().indexOf("id");
            int colAID = qLoc.record().indexOf("aid");
            int colArtSif = qLoc.record().indexOf("artsif");
            int colArtNaz = qLoc.record().indexOf("naziv");
            int colKol = qLoc.record().indexOf("kol");
            int colPC = qLoc.record().indexOf("pc");
            int colUkupno = qLoc.record().indexOf("ukupno");
            int colRabatP = qLoc.record().indexOf("rabatp");
            int colRabatK = qLoc.record().indexOf("rabatk");
            int colUkpCist = qLoc.record().indexOf("ukpcisti");
            int colBpdvCist = qLoc.record().indexOf("bpdvcisti");
            int colBpdv = qLoc.record().indexOf("bpdv");
            int colPovratna = qLoc.record().indexOf("povratnanaknada");
            int R2ID =0;
//            int colGrpId = qLoc.record().indexOf("grupaid");
//            int colIspKuh = qLoc.record().indexOf("ispis_kuhinja");
            while (qLoc.next())
            {
                Dalje = true;
                int ArtID = qLoc.value(colAID).toInt();
                qveri = QString("insert into %1 (rid,aid,kol,pc,ukupno,rabatp,rabatk,ukpcisti,naziv,bpdv,r2localid,%2,artsif").arg(TMP_Table_Rac2).arg(TMP_Table_Rac2_PoljeBPDVcisti);
                if (TMP_Table_Rac2 == "rac2")
                {
                    qveri += ",povratna)";
                }else
                {
                    qveri += ")";
                }
                qveri += " select :rid,:aid,:kol,:pc,:ukupno,:rabatp,:rabatk,:ukpcisti,:naziv,:bpdv,:r2localid,:bpdvcisti,:artsif";
                if (TMP_Table_Rac2 == "rac2")
                {
                    qveri += ",:povratna;";
                }else
                {
                    qveri += ";";
                }
                q.prepare(qveri);
                q.bindValue(":rid",RacID);
                q.bindValue(":aid",ArtID);
                q.bindValue(":kol",qLoc.value(colKol).toDouble());
                q.bindValue(":pc",qLoc.value(colPC).toDouble());
                q.bindValue(":ukupno",qLoc.value(colUkupno).toDouble());
                q.bindValue(":rabatp",qLoc.value(colRabatP).toDouble());
                q.bindValue(":rabatk",qLoc.value(colRabatK).toDouble());
                q.bindValue(":ukpcisti",qLoc.value(colUkpCist).toDouble());
                q.bindValue(":naziv",qLoc.value(colArtNaz).toString());
                q.bindValue(":bpdv",qLoc.value(colBpdv).toDouble());
                q.bindValue(":r2localid",qLoc.value(colR2locID).toInt());
                q.bindValue(":bpdvcisti",qLoc.value(colBpdvCist).toDouble());
                q.bindValue(":artsif",qLoc.value(colArtSif).toString());
                if (TMP_Table_Rac2 == "rac2")
                {
                    q.bindValue(":povratna",qLoc.value(colPovratna).toDouble());
                }
                if (!q.exec())
                {
                    qDebug() << "Greska-upisa rac2 : " << q.lastQuery() << q.lastError();
                    MozeDalje = false;
                    Dalje = false;
                    //Dalje2 = false;
                    UspjesnoUpisan = false;
                }else
                {
                    q.exec("select LAST_INSERT_ID()");

                    if (q.next())
                    {
                        R2ID = q.value(0).toInt();
                    }else
                    {
                        qDebug() << "greska dohvata-r2id last_insert_id";
                    }

                }

                QSqlQuery qPor("",dbL);
                QString qveri2;
                qPor.exec(QString("select id,dok,aid,pid,koeficijent_poreza,iznos_poreza,porez_sifra,r2id,osnovica,porez_postotak from rac4 where dok=%1 and aid=%2 and r2id=%3").arg(_RacStol).arg(qLoc.value(colAID).toInt()).arg(qLoc.value(colR2locID).toInt()));
                qveri2 =QString("insert into %1 (rid,aid,pid,koeficijent_poreza,iznos_poreza,porez_opis,r2localid,osnovica,porez_postotak,r2id) ").arg(TMP_Table_Rac2p);
                int y = 0;
                bool Dalje2 = false;
                int colPpid = qPor.record().indexOf("pid");
                int colPkoef = qPor.record().indexOf("koeficijent_poreza");
                int colPiznosp = qPor.record().indexOf("iznos_poreza");
                int colPsifra = qPor.record().indexOf("porez_sifra");
                int colPr2id = qPor.record().indexOf("r2id");
                int colPr2Osnovica = qPor.record().indexOf("osnovica");
                int colPr2porez_postotak = qPor.record().indexOf("porez_postotak");
                while (qPor.next())
                {
                    Dalje2 = true;
                    if (y>0)
                        qveri2 += " union all ";
                    //qDebug() << "\nOSNovica " << qPor.value(colPr2Osnovica).toDouble() << "\n";
                    qveri2 += QString(" select '%1','%2','%3','%4','%5','%6','%7','%8','%9','%10'").arg(RacID).arg(ArtID).arg(qPor.value(colPpid).toInt())
                            .arg(qPor.value(colPkoef).toDouble()).arg(qPor.value(colPiznosp).toDouble()).arg(qPor.value(colPsifra).toString())
                            .arg(qPor.value(colPr2id).toInt()).arg(qPor.value(colPr2Osnovica).toDouble()).arg(qPor.value(colPr2porez_postotak).toDouble()).arg(R2ID);
                    y++;
                }
                //qDebug() << qveri2;
                if (Dalje2)
                {
                    //qDebug() << qveri2;
                    if (!q.exec(qveri2))
                    {
                        qDebug() << q.lastError() << q.lastQuery();
                        UspjesnoUpisan = false;
                        //return false;
                        MozeDalje  = false;
                    }
                }
                i++;
            }
            if (Dalje)
            {
                if(!q.exec(QString("update vrac2 set pcbpdv=pcbpdv/kol where rid=%1").arg(RacID)))
                        qDebug() << QString("Greska update vrac2 pcbpdv racid %1 - ").arg(RacID) << q.lastError() << q.lastQuery();
            }
        }
        mojiHeaderi *t = new mojiHeaderi();
        if (UspjesnoUpisan == true)
        {

            QString ImeTabliceR1 = "";
            QString ImeTabliceR1p = "";
            QString ImeTabliceR1o = "";
            QString ImeTabliceR2 = "";
            QString ImeTabliceR2p = "";
            QString ImeTabliceR2d = "";
            QString ImeTabliceR3 = "";
            QString VrstaBrojRacuna = "R";
            if (_RacTipRacuna == "O")
            {
                ImeTabliceR1 = " vrac1 ";
                ImeTabliceR1p = " vrac1porezi ";
                ImeTabliceR1o = " vrac1opis ";
                ImeTabliceR2 = " vrac2 ";
                ImeTabliceR2p = " vrac2porezi ";
                ImeTabliceR3 = " vrac3 ";
                ImeTabliceR1o= " vrac1opis ";
                VrstaBrojRacuna = "O";
            }else
            {
                ImeTabliceR1 = " rac1 ";
                ImeTabliceR1p = " rac1porezi ";
                ImeTabliceR1o = " rac1opis ";
                ImeTabliceR2 = " rac2 ";
                ImeTabliceR2p = " rac2porezi ";
                ImeTabliceR2d = " rac2dodatno ";
                ImeTabliceR3 = " rac3 ";
                ImeTabliceR1o = " rac1opis ";
                VrstaBrojRacuna = "R";

            }
            QString sqlR;
            if (_RacOpisPolje != "")
            {
                qveri = QString("insert into %1 select '%2','%3'").arg(ImeTabliceR1o).arg(RacID).arg(_RacOpisPolje);
                if (!q.exec(qveri))
                {
                    qDebug() << q.lastError() << q.lastQuery();
                    q.exec(QString("create table zPukoRac1opis_%1 select '%1','%2'").arg(RacID).arg(_RacOpisPolje));
                    qDebug() << q.lastError() << q.lastQuery();
                }
            }

            if (MozeDalje)
            {
                _Rac1ID = RacID;
                //****
                q.exec("select value1 from opcije where sifra='OznakaSljednosti'");
                QString OznSljed = "P";
                if (q.next())
                {
                    OznSljed = q.value(0).toString();
                    if (OznSljed != "N") // ako nije N onda je P (zastita od krive postavke)
                        OznSljed = "P";
                }
                if (VrstaBrojRacuna != "O")
                    VrstaBrojRacuna = "M";

                q.exec(QString("select sum from %1 where id=%2").arg(ImeTabliceR1).arg(_Rac1ID));
                q.next();
                if (q.value(0).toDouble() != 0 )
                {
                    sqlR = QString("call proc_DohvatBrojaDokumenta('%5','%1','%2','%3','%4',@NoviBR);update %6 set br=@NoviBR where id=%4")
                            .arg(qApp->property("Firma_OznNapUr").toString()).arg(qApp->property("Firma_OznPosPr").toString())
                            .arg(OznSljed).arg(_Rac1ID).arg(VrstaBrojRacuna).arg(ImeTabliceR1);

                    if (!q.exec(sqlR))
                    {
                        qDebug()<< "Greska update br rac " << q.lastError() << q.lastQuery();
                        MozeDalje = false;
                        UspjesnoUpisan= false;
                    }else
                    {
                        /*
                        if(!q.exec(QString("update rac2porezi r2p,rac2 r2 set r2p.r2id=r2.id where r2p.rid=r2.rid  and r2p.r2localid=r2.r2localid and r2p.rid=%1").arg(_Rac1ID)))
                        {
                            qDebug() << "Greska update rac2porezi r2id " << q.lastError() << q.lastQuery() ;
                        }
                        */
                    }

                }else
                {
                    //QMessageBox::warning(this,"PROBLEM KOD UPISA RACUNA",QString("Doslo je do problema kod upisa racuna ID-").arg(_Rac1ID),"OK");
                    MozeDalje = false;
                    UspjesnoUpisan = false;
                }
            }
        }
        if (UspjesnoUpisan && MozeDalje){
            if (_RacNarID>0)
            {
                if (!q.exec(QString("update narudzba1 set status='zatvorena' where id=%1").arg(_RacNarID)))
                    qDebug() << "Greska update nar statusa-" << q.lastError() << q.lastQuery();
            }

            if (_RacTipRacuna != "O")
            {
                int brojac=0;
                while (!t->ZKI(_Rac1ID))
                {
                    if (brojac>=3)
                        break;
                    brojac++;
                }
                if (qApp->property("Prog-Fiskalizacija").toString() != "0")
                {
                    //qDebug() << "xml-ide " << QDateTime::currentDateTime().toString("hhmmss");
                    //t->xmlRacunZahtjev(_Rac1ID,false);
                    t->jsonRacunZahtjev(_Rac1ID,false);
                    //qDebug() << "xml-gotov " << QDateTime::currentDateTime().toString("hhmmss");
                    if (dbL.isOpen())
                        dbL.open();
                    //qDebug() << QDateTime::currentDateTime().toString("hhmmss");
                }
            }
            int Brojac = 0;
            while (!qLoc.exec(QString("delete from rac2 where dok=%1 and narid=%2").arg(_RacStol).arg(_RacNarID)))
            {
                if (Brojac >= 5)
                    break;
                dbL.close();
                dbL.open();
                qDebug() << qLoc.lastError() << qLoc.lastQuery();
                Brojac++;
            }
            Brojac = 0;
            while (!qLoc.exec(QString("delete from rac4 where dok=%1 and narid=%2").arg(_RacStol).arg(_RacNarID)))
            {
                if (Brojac >= 5)
                    break;
                dbL.close();
                dbL.open();
                qDebug() << qLoc.lastError() << qLoc.lastQuery();
                Brojac++;
            }
            Brojac = 0;
            while (!qLoc.exec(QString("delete from ispKuhinja where dok=%1 and narid=%2").arg(_RacStol).arg(_RacNarID)))
            {
                if (Brojac >= 5)
                    break;
                dbL.close();
                dbL.open();
                qDebug() << qLoc.lastError() << qLoc.lastQuery();
                Brojac++;
            }
            //qDebug() << QDateTime::currentDateTime().toString("hhmmss");
            UpdateCRMrac();
            return true;
        }else{
            qDebug() << "******************\nGreska kod upisa racuna\n******************\n";
            return false;
        }
}

bool clsIzracuni::UcitajLocalSUMrac()
{
    if (_RacStol < 1)
        return false;
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    if (!q.exec(QString("select sum(ukupno) ukupno1,sum(bpdv) bpdv1,sum(kol*povratnanaknada) povratna1 from rac2 where dok=%1 and narid=%2").arg(_RacStol).arg(_RacNarID)))
        return false;
    if (!q.next())
        return false;
    _RacSUM = q.value(q.record().indexOf("ukupno1")).toDouble();
    _RacBPDV = q.value(q.record().indexOf("bpdv1")).toDouble();
    _RacSumPovratnaNaknada = q.value(q.record().indexOf("povratna1")).toDouble();

}
void clsIzracuni::UpdateCRMrac()
{
    if (_crmid == 0 )
        return;
    QSqlQuery q("",QSqlDatabase::database("baza"));
    if (!q.exec(QString("insert into crmrac (crmid,vrsta,rid) values(%1,'%2',%3)").arg(_crmid).arg(_RacTipRacuna).arg(_Rac1ID)))
    {
        qDebug() << "greska upisa crmrac" << q.lastError();
    }
}


//Dohvat sifre artikla prema barcodu
bool  clsIzracuni::FindArtiklSifraByBarcode(QString barcode, QString& sifraArtiklaOUT)
{
    // postaviti sifru artikla na prazno (ako se ne nade)
    sifraArtiklaOUT = "";

    if (barcode.trimmed().length()==0)
        return false;

    QSqlQuery q("",QSqlDatabase::database("baza"));
    QString nSQL;

    // SQL za dohvat ID artikla
    nSQL = QString("select id from art where barcode = '%1'").arg(barcode);

    if (!q.exec(nSQL))
        return false;
    else
    {
        int  artiklID = 0;

        if (!q.next())
            return false;
        else
        {
            int columnArtiklID = q.record().indexOf("id");
            artiklID = q.value(columnArtiklID).toInt();
            if (artiklID == 0)
                return false;
            else // dohvat sifre prema ID artikla iz view
                return this->FindArtiklSifraByArtiklID(artiklID, sifraArtiklaOUT);
        }
    }
}

//Dohvat sifre artikla prema nazivu
bool  clsIzracuni::FindArtiklSifraByName(QString nameStartWith, QString& sifraArtiklaOUT)
{
    // postaviti sifru artikla na prazno (ako se ne nade)
    sifraArtiklaOUT = "";

    if (nameStartWith.trimmed().length()==0)
        return false;

    QSqlQuery q("",QSqlDatabase::database("baza"));
    QString nSQL;

    // SQL za dohvat ID artikla
    nSQL = QString("select id from art where naziv LIKE '%1%'").arg(nameStartWith);

    if (!q.exec(nSQL))
        return false;
    else
    {
        int  artiklID = 0;

        if (!q.next())
            return false;
        else
        {
            int columnArtiklID = q.record().indexOf("id");
            artiklID = q.value(columnArtiklID).toInt();
            if (artiklID == 0)
                return false;
            else // dohvat sifre prema ID artikla iz view
            {
                if (q.next())
                    return false; //ima vise od jednog artikla
                else //samo jedan artikl slicnog naziva
                    return this->FindArtiklSifraByArtiklID(artiklID, sifraArtiklaOUT);
            }
        }
    }
}

//Dohvat sifre artikla prema sifra
bool  clsIzracuni::FindArtiklSifraBySifra(QString sifraArtikla, QString& sifraArtiklaOUT)
{
    // postaviti izlaznu sifru artikla na prazno (ako se ne nade)
    sifraArtiklaOUT = "";

    if (sifraArtikla.trimmed().length()==0)
        return false;

    QSqlQuery q("",QSqlDatabase::database("baza"));
    QString nSQL;

    // SQL za dohvat ID artikla
    nSQL = QString("select id from art where artsif = '%1'").arg(sifraArtikla);

    if (!q.exec(nSQL))
        return false;
    else
    {
        int  artiklID = 0;

        if (!q.next())
            return false;
        else
        {
            int columnArtiklID = q.record().indexOf("id");
            artiklID = q.value(columnArtiklID).toInt();
            if (artiklID == 0)
                return false;
            else // dohvat sifre prema ID artikla iz view (zbog provjere raspolozivosti)
                return this->FindArtiklSifraByArtiklID(artiklID, sifraArtiklaOUT);
        }
    }
}

//Dohvat sifre artikla prema ID artikla iz view dostupnih artikala (vidljivi, neobrisani i slicno)
//kontrolira da li je dostupan za prodaju
bool clsIzracuni::FindArtiklSifraByArtiklID(int ArtiklID, QString& sifraArtiklaOUT)
{
    // postaviti sifru artikla na prazno (ako se ne nade)
    sifraArtiklaOUT = "";
    if (ArtiklID == 0)
        return false;

    QSqlQuery q("",QSqlDatabase::database("baza"));
    QString nSQL;

    nSQL = QString("select * from viz_rac where id = %1 and sid=%2 and cid=%3")
            .arg(ArtiklID).arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt());

    if (!q.exec(nSQL))
        return false;
    else
    {
        if (!q.next()) //nema ni jedan record
            return false;
        else
        {
            double prodajnaCijena;

            int columnArtiklSifra = q.record().indexOf("artsif");
            int columnPC = q.record().indexOf("pc");
            sifraArtiklaOUT = q.value(columnArtiklSifra).toString();
            prodajnaCijena = q.value(columnPC).toDouble();
            if (prodajnaCijena == 0)
            {
                // ne smije prodajna cijena biti 0
                sifraArtiklaOUT = "";
                return false;
            }
            else
                return true;
        }
    }
}
