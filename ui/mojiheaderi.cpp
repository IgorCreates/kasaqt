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




#include "mojiheaderi.h"
#include <QItemDelegate>
//#include <QtCrypto/QtCrypto>
#include <QtXml>
#include <QDomDocument>
#include <QFile>
#include <QtSql>
#include <QUuid>
#include <ui/frmispisodabir.h>
#include <QMessageBox>
#include <QJsonObject>




//#include "QtCrypto/qca.h"
/*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
*/

//#include <qtsoap.h>



mojiHeaderi::mojiHeaderi()
{
}
mojiHeaderi::mojiHeaderi(QObject *parent):QItemDelegate(parent)
{

}


void mojiHeaderi::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

//    drawDisplay(painter, myOption, myOption.rect,text);
//    drawFocus(painter, myOption, myOption.rect);

    if (text.toDouble()) {
      drawDisplay(painter, myOption, myOption.rect,QString::number(text.toDouble(), 'f', 2));
    } else {
      drawDisplay(painter, myOption, myOption.rect,text);
    }
    drawFocus(painter, myOption, myOption.rect);

}
bool mojiHeaderi::ZKI(int RacID)
{
//    QString ZKI = crZKI.ZKI(qApp->property("Firma_OIB").toString(),DatumRacuna.toString("dd.MM.yyyy hh:mm:ss"),q.value(0).toString(),
//    qApp->property("Firma_OznPosPr").toString()
//                            ,qApp->property("Firma_OznNapUr").toString(),QString::number(sSSUMA));
    QSqlDatabase db = QSqlDatabase::database("baza");
    QSqlQuery q("",db);
    if (!db.open())
        return false;
    if (!q.exec(QString("select datumr,br,sum,kasaid,oznPP from rac1 where id=%1").arg(RacID)))
        return false;
    q.next();
    int qDatumRac = q.record().indexOf("datumr");
    int qBrojRac = q.record().indexOf("br");
    int qUkpIznosRac = q.record().indexOf("sum");
    int qOznPP = q.record().indexOf("oznPP");
    int qKasaID = q.record().indexOf("kasaid");
    if (q.value(qOznPP).isNull())
        return false;
    if (q.value(qUkpIznosRac).toDouble() == 0)
        return false;
    if (q.value(qKasaID).isNull())
        return false;
    QString medj;
    QString medjN;
    QString FormatiraniBroj;
    medj += qApp->property("Firma_OIB").toString();
    medjN = qApp->property("Firma_OIB").toString();
    medj += q.value(qDatumRac).toDateTime().toString("dd.MM.yyyy hh:mm:ss");
    medjN += ";" + q.value(qDatumRac).toDateTime().toString("dd.MM.yyyy hh:mm:ss");
    medj += q.value(qBrojRac).toString();
    medjN += ";" + q.value(qBrojRac).toString();
//    medj += qApp->property("Firma_OznPosPr").toString();
//    medj += qApp->property("Firma_OznNapUr").toString();
    medj += q.value(qOznPP).toString();
    medjN += ";" + q.value(qOznPP).toString();
    medj += q.value(qKasaID).toString();
    medjN += ";" + q.value(qKasaID).toString();
    medj += FormatiraniBroj.sprintf("%15.2f",q.value(qUkpIznosRac).toDouble()).trimmed();
    medjN += ";" + FormatiraniBroj.sprintf("%15.2f",q.value(qUkpIznosRac).toDouble()).trimmed();
    //FormatiraniBroj.sprintf("%15.2f",qtmp.value(2).toDouble()).trimmed())
    //db.close();

    medjN += ";" + qApp->property("Certs_Path").toString() + ".pem;" + qApp->property("Certs_Sifra").toString();
    qDebug() << medj;
    qDebug() << medjN;


    QProcess *zk = new QProcess(this);
    //qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString()
    qDebug() << qApp->property("Certs_Path").toString() << qApp->property("Certs_Sifra").toString() ;
    QString Komand;
#ifdef Q_WS_X11
//    Komand = QString("mono Sign.exe %2 %3 \"%1\"").arg(medj).arg(qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString());
    Komand = QString("./p.py -z \"%1\"").arg(medjN);
    qDebug() << "command: " << Komand;
#endif
#ifdef Q_WS_WIN
    //zk->start();
    Komand = QString("%4/Sign.exe %4/%2 %3 \"%1\"").arg(medj).arg(qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString()).arg(qApp->applicationDirPath());
    qDebug() << "command: " << Komand;
#endif
//    zk->start(QString("mono Sign.exe %2 %3 \"%1\"").arg(medj).arg(qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString()));
    //qDebug() << Komand;
    zk->start(Komand);
    zk->setReadChannel(QProcess::StandardOutput);
    if (!zk->waitForStarted())
    {
        qDebug() << "Greska zki";
        return false;
    }else
    {
        zk->waitForFinished();
        QString ZKi;
        ZKi.append(zk->readAllStandardOutput().constData());
        qDebug() << zk->readAllStandardError();
        qDebug() << zk->readAllStandardOutput();
        qDebug() << zk->readAll();
        //zk->close();
        qDebug() << "ZKI:" << ZKi;
        if (q.exec(QString("update rac1 set zki='%1' where id=%2").arg(ZKi).arg(RacID)))
        {
            return true;
        }else
        {
            qDebug() << "Greska kod upisa ZKI " << q.lastQuery() << q.lastError();
            return false;
        }
    }
    return false;
}

bool mojiHeaderi::jsonRacunZahtjev(const int RacunID, bool NaknadnoSlanje)
{

    int TraziCOLONU;
    QString FormatiraniBroj;
    bool ProvjeraImaElement=false;
    QSqlDatabase dbB = QSqlDatabase::database("baza");
    QSqlQuery q("",dbB);
    if (!dbB.open())
        return false;
    qDebug() << "json-start";
    QString xmlQveri = QString("select r1.id  id,r1.br  br,r1.vrsta  vrsta,"
                               "r1.idk  idk, r1.sum  sum,r1.bpdv  bpdv,r1.datumr  datumr,"
                               "r1.uid  uid, r1.obracun  obracun,r1.stol  stol,r1.np  np,"
                               "r1.storno  storno, r1.rabatp  rabatp,r1.rabatk  rabatk,r1.rsum  rsum"
                               ",r1.rbpdv  rbpdv, r1.storno_datum  storno_datum,r1.oib  oib,r1.zki  zki,"
                               "r1.sid  sid, r1.cid  cid,r1p.porez_sifra  porez_sifra, "
                               "sum(r1p.iznos_poreza)  iz,k.fiskal_znak,r1.kasaid,r1.oznPP "
                               " from (rac1 r1 left join  rac1porezi r1p on((r1.id = r1p.rid)) left "
                                                  " join kartice k on r1.np=k.id ) where r1.id=%1 group by r1.id").arg(RacunID);


//    if (!q.exec(QString("select * from v_xml_racZahtjev where id=%1").arg(RacunID)))
    if (!q.exec(xmlQveri))
    {
        qDebug() << "Greska kod izrade jsona-a";
        qDebug() << q.lastError() << " " << q.lastQuery();
        return false;
    }
    q.next();


    QJsonObject j_racun ;
    /*
    j_racun.insert("FirstName", QJsonValue::fromVariant("John"));
    j_racun.insert("LastName", QJsonValue::fromVariant("Doe"));
    j_racun.insert("Age", QJsonValue::fromVariant(43));
    */


//    json["DatumVrijeme"] = QDateTime::currentDateTime().toString("dd.MM.yyyyThh:mm:ss");
    j_racun.insert("Oib",QJsonValue::fromVariant(qApp->property("Firma_OIB").toString()));
    if (qApp->property("Firma_USustavuPDV").toString() == "true")
    {
        j_racun.insert("USustPdv", QJsonValue::fromVariant("true"));
    }else
    {
        j_racun.insert("USustPdv",QJsonValue::fromVariant("false"));
    }

    QDateTime dt = q.value(6).toDateTime();
    j_racun.insert("DatVrijeme",QJsonValue::fromVariant(dt.toString("dd.MM.yyyyThh:mm:ss")));

    //t1.appendChild(xmlPod("P"));
    QSqlQuery q1("",QSqlDatabase::database("baza"));
    q1.exec("select value1 from opcije where sifra='OznakaSljednosti'");
    QString OznSljed = "P";
    if (q1.next())
    {
        OznSljed = q1.value(0).toString();
        if (OznSljed != "N") // ako nije N onda je P (zastita od krive postavke)
            OznSljed = "P";
    }
    j_racun.insert("OznSlijed", QJsonValue::fromVariant(OznSljed));


    QJsonObject j_BrRac;
    j_BrRac.insert("BrOznRac", QJsonValue::fromVariant(q.value(1).toString()));

    //t2.appendChild(xmlPod(qApp->property("Firma_OznPosPr").toString()));
    TraziCOLONU = q.record().indexOf("oznPP");

    j_BrRac.insert("OznPosPr", QJsonValue::fromVariant(q.value(TraziCOLONU).toString()));

    TraziCOLONU = q.record().indexOf("kasaid");
    j_BrRac.insert("OznNapUr", QJsonValue::fromVariant(q.value(TraziCOLONU).toString()));

    j_racun.insert("BrRac",j_BrRac);


    QJsonObject j_Pdv;
    QJsonObject j_Porez;

    QSqlQuery qtmp("",dbB);
    /* potrgani qveri
    QString xmlQveri2 = QString("select (p.postotak*100) stopa,(sum(r2.ukupno)-(sum(r2.ukupno)*r1.rabatp/100)) osnovica,"
                  "(sum(r2p.iznos_poreza)-(sum(r2p.iznos_poreza)*r1.rabatp/100))   iznos,r2p.porez_opis,r2p.pid,p.poreztip "
                   " from rac2 r2 left join rac2porezi r2p on (r2.aid=r2p.aid and r2.rid=r2p.rid)  "
                   "  left join rac1 r1 on r2.rid=r1.id left join porez p on r2p.pid=p.id   where r2.rid=%1  and p.poreztip='PDV' group by r2p.pid").arg(RacunID);
    */
    QString xmlQveri2 = QString("select r1.id,rp.porez_postotak*100 as stopa,r1.sum,rp.osnovica,"
           "rp.iznos_poreza iznos,rp.porez_sifra,p.poreztip from rac1 r1 left join rac1porezi rp on r1.id=rp.rid "
                                " left join porez p on rp.pid=p.id where p.poreztip='PDV' and r1.id=%1").arg(RacunID);


 //   if (!qtmp.exec(QString("select * from v_xml_raczahtjev_porez where rid=%1 and tip='PDV'").arg(RacunID)))
    //qDebug() << xmlQveri;

    if (!qtmp.exec(xmlQveri2))
    {
        qDebug() << "Greska json_RacZahtjev-problem kod povlacenja poreza";
        qDebug() << qtmp.lastError() << qtmp.lastQuery();
        return false;
    }
    ProvjeraImaElement=false;
    int idStopa = qtmp.record().indexOf("stopa");
    int idIznos = qtmp.record().indexOf("iznos");
    int idOsnovica = qtmp.record().indexOf("osnovica");

    while (qtmp.next())
    {
        j_Porez.insert("Stopa", QJsonValue::fromVariant(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idStopa).toDouble()).trimmed()));
        j_Porez.insert("Osnovica",QJsonValue::fromVariant(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idOsnovica).toDouble()).trimmed()));
        j_Porez.insert("Iznos", QJsonValue::fromVariant(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idIznos).toDouble()).trimmed()));
        j_Pdv.insert("Porezi",j_Porez);
    }
    //j_Pdv.insert("Pdv",j_Porez);
    j_racun.insert("PDV",j_Pdv);
    j_Pdv = QJsonObject();
    j_Porez = QJsonObject();



    xmlQveri2 = QString("select r1.id,rp.porez_postotak*100 as stopa,r1.sum,rp.osnovica,"
           "rp.iznos_poreza iznos,rp.porez_sifra,p.poreztip from rac1 r1 left join rac1porezi rp on r1.id=rp.rid "
                                " left join porez p on rp.pid=p.id where p.poreztip='PNP' and r1.id=%1").arg(RacunID);


    if (!qtmp.exec(xmlQveri2))
    {
        qDebug() << "Greska json_RacZahtjev-problem kod povlacenja poreza";
        qDebug() << qtmp.lastError() << qtmp.lastQuery();
        return false;
    }


    idStopa = qtmp.record().indexOf("stopa");
    idIznos = qtmp.record().indexOf("iznos");
    idOsnovica = qtmp.record().indexOf("osnovica");

    while (qtmp.next())
    {
        j_Porez.insert("Stopa",QJsonValue::fromVariant(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idStopa).toDouble()).trimmed()));
        j_Porez.insert("Osnovica",QJsonValue::fromVariant(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idOsnovica).toDouble()).trimmed()));
        j_Porez.insert("Iznos",FormatiraniBroj.sprintf("%15.2f",qtmp.value(idIznos).toDouble()).trimmed());
        j_Pdv.insert("Porezi",j_Porez);
        ProvjeraImaElement= true;
    }
    if (ProvjeraImaElement)
            j_racun.insert("Pnp",j_Pdv);
    ProvjeraImaElement=false;
    j_Pdv = QJsonObject();
    j_Porez = QJsonObject();



    xmlQveri2 = QString("select sum(kol*povratnanaknada) from rac2dodatno where rid=%1").arg(RacunID);
    if (!qtmp.exec(xmlQveri2))
    {
        qDebug() << "Greska json_RacZahtjev_Povratna naknada";
        qDebug() << qtmp.lastError() << qtmp.lastQuery();
    }
    if (qtmp.next())
    {
    //samo ako ima povratna naknada (tablica rac2dodatno)
        if (qtmp.value(0).toDouble() >0){
            j_Porez.insert("NazivN",QJsonValue::fromVariant("Povratna naknada"));
            j_Porez.insert("IznosN",QJsonValue::fromVariant(FormatiraniBroj.sprintf("%15.2f",qtmp.value(0).toDouble()).trimmed()));
            ProvjeraImaElement=true;
        }
    }
    if (ProvjeraImaElement)
            j_racun.insert("Naknade",j_Porez);
    ProvjeraImaElement=false;
    j_Porez = QJsonObject();

    j_racun.insert("IznosUkupno",QJsonValue::fromVariant(FormatiraniBroj.sprintf("%15.2f",q.value(4).toDouble()).trimmed()));

    //t1.appendChild(xmlPod("G"));
    if (!q.value(23).isNull())
    {
        j_racun.insert("NacinPlac",QJsonValue::fromVariant(q.value(23).toString()));
    }else
    {
        j_racun.insert("NacinPlac",QJsonValue::fromVariant("G"));
    }
    j_racun.insert("OibOper",QJsonValue::fromVariant(q.value(17).toString()));

    //t1.appendChild(xmlPod("42246174001"));
    if (NaknadnoSlanje)
    {
        j_racun.insert("NakDost",QJsonValue::fromVariant("true"));
    }else{
        j_racun.insert("NakDost",QJsonValue::fromVariant("false"));
    }


    QJsonDocument doc(j_racun);
    //qDebug() << doc.toJson();
    //qDebug() << doc;
    QString ImeJsonFajla = QString("xml/Rac_%1_%2.json").arg(RacunID).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    QFile jsonFile(ImeJsonFajla);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Greska kod snimanja jsonfile";
    }
    // Write the current Json object to a file.
    //jsonFile.write(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
    jsonFile.write(QJsonDocument(j_racun).toJson(QJsonDocument::Indented));
    jsonFile.close();   // Close file


    QProcess *zk = new QProcess(this);
    //qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString()
    QString Komand;

    Komand = QString("c:/python27/python.exe p3.py %1").arg(ImeJsonFajla);
    zk->start(Komand);
    zk->setReadChannel(QProcess::StandardOutput);
    if (!zk->waitForStarted())
    {
        qDebug() << "Greska json";
        return false;
    }else
    {
        zk->waitForFinished();
        QString ZKi;
        ZKi.append(zk->readAllStandardOutput().constData());
        qDebug() << zk->readAllStandardError();
        qDebug() << zk->readAllStandardOutput();
        qDebug() << zk->readAll();
        //zk->close();
        qDebug() << "FISK_RETURN:" << ZKi;
        QString bb =  ZKi;   //"ZKI : bbd743f6fd3abe1ba2a145dff71c80d7\r\nRacunZahtjev reply errors:\r\nOIB iz poruke zahtjeva nije jednak OIB-u iz certifikata.\r\n('Done in ', 0.2754233, 'seconds')\r\nJIR is: c59de779-28d1-4714-9141-1bd2c10b852f";
        QStringList bL = bb.split("\r\n");
        QString ZKI="";
        QString JIR="";

        QFile file(QString("%1_Odgovor").arg(ImeJsonFajla));
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream <<  ZKi <<endl;
        }

        foreach (QString i, bL) {
            //qDebug() << "List items = " << i;
            if (i.contains("ZKI"))
            {
                qDebug() << i.split(":");
                ZKI =  QString(i.split(":")[1]).trimmed();
                //qDebug() << ZKI;
                if (!q.exec(QString("update rac1 set zki='%1' where id=%2").arg(ZKI).arg(RacunID)))
                    qDebug() << "Greska upisa ZKI-a " << q.lastError() << " " << q.lastQuery();
            }
            if (i.contains("JIR"))
            {
                qDebug() << i.split(":");
                JIR = QString(i.split(":")[1]).trimmed();
                //qDebug() << JIR;
                if (!q.exec(QString("insert into rac1fiskal (racid,datum_slanja,odgovor,potvrden) select %1,now(),'%2',1").arg(RacunID).arg(JIR)))
                    qDebug() << "Greska upisa JIR-a " << q.lastError() << " " << q.lastQuery();
            }
        }
     }


    return true;
}

bool mojiHeaderi::xmlRacunZahtjev(const int RacunID, bool NaknadnoSlanje)
{

    qDebug() << "Ne koristi se vise xmlRacZahtjev";
    return false;

    QDomElement t1;
    QDomElement t2;
    QDomElement t3;
    int TraziCOLONU;

    QString FormatiraniBroj;

    bool ProvjeraImaElement=false;

    QSqlDatabase dbB = QSqlDatabase::database("baza");
    QSqlQuery q("",dbB);
    if (!dbB.open())
        return false;

    QString xmlQveri = QString("select r1.id  id,r1.br  br,r1.vrsta  vrsta,"
                               "r1.idk  idk, r1.sum  sum,r1.bpdv  bpdv,r1.datumr  datumr,"
                               "r1.uid  uid, r1.obracun  obracun,r1.stol  stol,r1.np  np,"
                               "r1.storno  storno, r1.rabatp  rabatp,r1.rabatk  rabatk,r1.rsum  rsum"
                               ",r1.rbpdv  rbpdv, r1.storno_datum  storno_datum,r1.oib  oib,r1.zki  zki,"
                               "r1.sid  sid, r1.cid  cid,r1p.porez_sifra  porez_sifra, "
                               "sum(r1p.iznos_poreza)  iz,k.fiskal_znak,r1.kasaid,r1.oznPP "
                               " from (rac1 r1 left join  rac1porezi r1p on((r1.id = r1p.rid)) left "
                                                  " join kartice k on r1.np=k.id ) where r1.id=%1 group by r1.id").arg(RacunID);


//    if (!q.exec(QString("select * from v_xml_racZahtjev where id=%1").arg(RacunID)))
    if (!q.exec(xmlQveri))
    {
        qDebug() << "Greska kod izrade xml-a";
        qDebug() << q.lastError() << " " << q.lastQuery();
        return false;
    }
    q.next();
    QDomDocument doc;//"?xml version=\"1.0\" encoding=\"UTF-8\"?");
    QDomProcessingInstruction header = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(header);


//    doc.appendChild(header);

    QDomText t;
    QDomElement xmlSoapBody;
    QDomElement xmlSoap = doc.createElement("soapenv:Envelope");
    xmlSoap.setAttribute("xmlns:soapenv","http://schemas.xmlsoap.org/soap/envelope/");
    xmlSoapBody = doc.createElement("soapenv:Body");

    QDomElement xmlRacunZahtjev = doc.createElement("tns:RacunZahtjev");
    xmlRacunZahtjev.setAttribute("xmlns:tns","http://www.apis-it.hr/fin/2012/types/f73");
    xmlRacunZahtjev.setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    xmlRacunZahtjev.setAttribute("Id","racunId");
    xmlRacunZahtjev.setAttribute("xsi:schemaLocation","http://www.apis-it.hr/fin/2012/types/f73 schema/FiskalizacijaSchema.xsd");


    QDomElement xmlZaglavlje = doc.createElement("tns:Zaglavlje");
    xmlRacunZahtjev.appendChild(xmlZaglavlje);
    QDomElement xmlZaglavljeIdPoruke = doc.createElement("tns:IdPoruke");
    //QDomText t = doc.createTextNode("");
    t = doc.createTextNode(QUuid::createUuid().toString().remove("{").remove("}"));  //"a95632f8-0bd8-4b6f-9c28-2a427b47655c");
    //qDebug() << "UUID " << QUuid::createUuid().toString();

    xmlZaglavljeIdPoruke.appendChild(t);
    xmlZaglavlje.appendChild(xmlZaglavljeIdPoruke);
    //tag.appendChild(t);
    QDomElement xmlZaglavljeDatVrijeme = doc.createElement("tns:DatumVrijeme");
    t = doc.createTextNode(QDateTime::currentDateTime().toString("dd.MM.yyyyThh:mm:ss"));
    xmlZaglavljeDatVrijeme.appendChild(t);
    xmlZaglavlje.appendChild(xmlZaglavljeDatVrijeme);
    //kreiranje rac
    QDomElement root = doc.createElement("tns:Racun");
    t1 = doc.createElement("tns:Oib");
    t = doc.createTextNode(qApp->property("Firma_OIB").toString());
    t1.appendChild(t);
    root.appendChild(t1);

    t1 = doc.createElement("tns:USustPdv");
    //qDebug() << qApp->property("Firma_USustavuPDV").toString();
    if (qApp->property("Firma_USustavuPDV").toString() == "true")
    {
        t1.appendChild(xmlPod("true"));
    }else
    {
        t1.appendChild(xmlPod("false"));
    }
    root.appendChild(t1);

    t1 = doc.createElement("tns:DatVrijeme");
    QDateTime dt = q.value(6).toDateTime();
    t1.appendChild(xmlPod(dt.toString("dd.MM.yyyyThh:mm:ss")));
    root.appendChild(t1);

    // 06.05.2018 - citanje postavke oznake slijednosti, a ne 'P'
    t1 = doc.createElement("tns:OznSlijed");
    //t1.appendChild(xmlPod("P"));
    QSqlQuery q1("",QSqlDatabase::database("baza"));
    q1.exec("select value1 from opcije where sifra='OznakaSljednosti'");
    QString OznSljed = "P";
    if (q1.next())
    {
        OznSljed = q1.value(0).toString();
        if (OznSljed != "N") // ako nije N onda je P (zastita od krive postavke)
            OznSljed = "P";
    }
    t1.appendChild(xmlPod(OznSljed));
    root.appendChild(t1);

    t1 = doc.createElement("tns:BrRac");
    t2 = doc.createElement("tns:BrOznRac");
    t2.appendChild(xmlPod(q.value(1).toString()));
    t1.appendChild(t2);
    t2= doc.createElement("tns:OznPosPr");

    //t2.appendChild(xmlPod(qApp->property("Firma_OznPosPr").toString()));
    TraziCOLONU = q.record().indexOf("oznPP");
    t2.appendChild(xmlPod(q.value(TraziCOLONU).toString()));
    t1.appendChild(t2);
    t2 = doc.createElement("tns:OznNapUr");
    //t2.appendChild(xmlPod(qApp->property("Firma_OznNapUr").toString()));
    TraziCOLONU = q.record().indexOf("kasaid");
    t2.appendChild(xmlPod(q.value(TraziCOLONU).toString()));
    t1.appendChild(t2);
    root.appendChild(t1);


    t1 = doc.createElement("tns:Pdv");
    QSqlQuery qtmp("",dbB);
    /* potrgani qveri
    QString xmlQveri2 = QString("select (p.postotak*100) stopa,(sum(r2.ukupno)-(sum(r2.ukupno)*r1.rabatp/100)) osnovica,"
                  "(sum(r2p.iznos_poreza)-(sum(r2p.iznos_poreza)*r1.rabatp/100))   iznos,r2p.porez_opis,r2p.pid,p.poreztip "
                   " from rac2 r2 left join rac2porezi r2p on (r2.aid=r2p.aid and r2.rid=r2p.rid)  "
                   "  left join rac1 r1 on r2.rid=r1.id left join porez p on r2p.pid=p.id   where r2.rid=%1  and p.poreztip='PDV' group by r2p.pid").arg(RacunID);
    */
    QString xmlQveri2 = QString("select r1.id,rp.porez_postotak*100 as stopa,r1.sum,rp.osnovica,"
           "rp.iznos_poreza iznos,rp.porez_sifra,p.poreztip from rac1 r1 left join rac1porezi rp on r1.id=rp.rid "
                                " left join porez p on rp.pid=p.id where p.poreztip='PDV' and r1.id=%1").arg(RacunID);


 //   if (!qtmp.exec(QString("select * from v_xml_raczahtjev_porez where rid=%1 and tip='PDV'").arg(RacunID)))
    //qDebug() << xmlQveri;

    if (!qtmp.exec(xmlQveri2))
    {
        qDebug() << "Greska xml_RacZahtjev-problem kod povlacenja poreza";
        qDebug() << qtmp.lastError() << qtmp.lastQuery();
        return false;
    }
    ProvjeraImaElement=false;
    int idStopa = qtmp.record().indexOf("stopa");
    int idIznos = qtmp.record().indexOf("iznos");
    int idOsnovica = qtmp.record().indexOf("osnovica");

    while (qtmp.next())
    {
        t2 = doc.createElement("tns:Porez");
        t3 = doc.createElement("tns:Stopa");

        t3.appendChild(xmlPod(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idStopa).toDouble()).trimmed()));
//        qDebug() << "**** " << nn.sprintf("%15.2f",qtmp.value(2).toDouble()).trimmed();

        t2.appendChild(t3);
        t3 = doc.createElement("tns:Osnovica");
        t3.appendChild(xmlPod(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idOsnovica).toDouble()).trimmed()));// qtmp.value(2).toString()));
        t2.appendChild(t3);
        t3 = doc.createElement("tns:Iznos");
        t3.appendChild(xmlPod(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idIznos).toDouble()).trimmed())); //qtmp.value(3).toString()));
        t2.appendChild(t3);
        t1.appendChild(t2);
        ProvjeraImaElement = true;
    }
    if (ProvjeraImaElement)
        root.appendChild(t1);
    ProvjeraImaElement = false;
    t1 = doc.createElement("tns:Pnp");
/*potrgan qveri
    xmlQveri2 = QString("select (p.postotak*100) stopa,(sum(r2.ukupno)-(sum(r2.ukupno)*r1.rabatp/100)) osnovica,"
                  "(sum(r2p.iznos_poreza)-(sum(r2p.iznos_poreza)*r1.rabatp/100))   iznos,r2p.porez_opis,r2p.pid,p.poreztip "
                   " from rac2 r2 left join rac2porezi r2p on (r2.aid=r2p.aid and r2.rid=r2p.rid)  "
                   "  left join rac1 r1 on r2.rid=r1.id left join porez p on r2p.pid=p.id   where r2.rid=%1  and p.poreztip='PNP' group by r2p.pid").arg(RacunID);
*/

    // ovdje je greška jer se koristi osnovica iz r1: r1.bpdv osnovica
    // xmlQveri2 = QString("select r1.id,rp.porez_postotak*100 as stopa,r1.sum,r1.bpdv osnovica,"
    //       "rp.iznos_poreza iznos,rp.porez_sifra,p.poreztip from rac1 r1 left join rac1porezi rp on r1.id=rp.rid "
    //                            " left join porez p on rp.pid=p.id where p.poreztip='PNP' and r1.id=%1").arg(RacunID);

// stavljen isti upit kao i kod PDV poreza
    xmlQveri2 = QString("select r1.id,rp.porez_postotak*100 as stopa,r1.sum,rp.osnovica,"
           "rp.iznos_poreza iznos,rp.porez_sifra,p.poreztip from rac1 r1 left join rac1porezi rp on r1.id=rp.rid "
                                " left join porez p on rp.pid=p.id where p.poreztip='PNP' and r1.id=%1").arg(RacunID);


    if (!qtmp.exec(xmlQveri2))
    {
        qDebug() << "Greska xml_RacZahtjev-problem kod povlacenja poreza";
        qDebug() << qtmp.lastError() << qtmp.lastQuery();
        return false;
    }


    idStopa = qtmp.record().indexOf("stopa");
    idIznos = qtmp.record().indexOf("iznos");
    idOsnovica = qtmp.record().indexOf("osnovica");

    while (qtmp.next())
    {
        t2 = doc.createElement("tns:Porez");
        t3 = doc.createElement("tns:Stopa");
        t3.appendChild(xmlPod(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idStopa).toDouble()).trimmed())); //qtmp.value(5).toString()));
        t2.appendChild(t3);
        t3 = doc.createElement("tns:Osnovica");
        t3.appendChild(xmlPod(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idOsnovica).toDouble()).trimmed()));//qtmp.value(2).toString()));
        t2.appendChild(t3);
        t3 = doc.createElement("tns:Iznos");
        t3.appendChild(xmlPod(FormatiraniBroj.sprintf("%15.2f",qtmp.value(idIznos).toDouble()).trimmed())); //qtmp.value(3).toString()));
        t2.appendChild(t3);
        t1.appendChild(t2);
        ProvjeraImaElement=true;
    }
    if (ProvjeraImaElement)
        root.appendChild(t1);
    xmlQveri2 = QString("select sum(kol*povratnanaknada) from rac2dodatno where rid=%1").arg(RacunID);
    if (!qtmp.exec(xmlQveri2))
    {
        qDebug() << "Greska xml_RacZahtjev_Povratna naknada";
        qDebug() << qtmp.lastError() << qtmp.lastQuery();
    }
    if (qtmp.next())
    {
    //samo ako ima povratna naknada (tablica rac2dodatno)
        t1 = doc.createElement("tns:Naknade");
        t2 = doc.createElement("tns:Naknada");
        t3 = doc.createElement("tns:NazivN");
        t3.appendChild(xmlPod("Povratna naknada"));
        t2.appendChild(t3);
        t3 = doc.createElement("tns:IznosN");
        t3.appendChild(xmlPod(FormatiraniBroj.sprintf("%15.2f",qtmp.value(0).toDouble()).trimmed()));
        t2.appendChild(t3);
        t1.appendChild(t2);
        if (qtmp.value(0).toDouble() > 0)
            ProvjeraImaElement=true;
    }
    if (ProvjeraImaElement)
        root.appendChild(t1);
    //
    ProvjeraImaElement = false;
    t1 = doc.createElement("tns:IznosUkupno");
    t1.appendChild(xmlPod(FormatiraniBroj.sprintf("%15.2f",q.value(4).toDouble()).trimmed()));//q.value(4).toString()));
    root.appendChild(t1);

    t1 = doc.createElement("tns:NacinPlac");
    //t1.appendChild(xmlPod("G"));
    if (!q.value(23).isNull())
    {
        t1.appendChild(xmlPod(q.value(23).toString()));
    }else
    {
        t1.appendChild(xmlPod("G"));
    }
    root.appendChild(t1);
    t1 = doc.createElement("tns:OibOper");
    t1.appendChild(xmlPod(q.value(17).toString()));
    //t1.appendChild(xmlPod("42246174001"));
    root.appendChild(t1);
    t1 = doc.createElement("tns:ZastKod");
    t1.appendChild(xmlPod(q.value(18).toString()));
    //t1.appendChild(xmlPod("e4d909c290d0fb1ca068ffaddf22cbd0"));
    root.appendChild(t1);
    t1 = doc.createElement("tns:NakDost");
    if (NaknadnoSlanje)
    {
        t1.appendChild(xmlPod("true"));
    }else{
        t1.appendChild(xmlPod("false"));
    }
//    t1.appendChild(xmlPod("false"));
    root.appendChild(t1);


    xmlRacunZahtjev.appendChild(root);
    root = doc.createElement("Signature");
    root.setAttribute("xmlns","http://www.w3.org/2000/09/xmldsig#");
    t1 = doc.createElement("SignedInfo");
    t2 = doc.createElement("CanonicalizationMethod");
    t2.setAttribute("Algorithm","http://www.w3.org/2001/10/xml-exc-c14n#");
    t1.appendChild(t2);
    t2 = doc.createElement("SignatureMethod");
    t2.setAttribute("Algorithm","http://www.w3.org/2000/09/xmldsig#rsa-sha1");
    t1.appendChild(t2);

    //Reference
    t2 = doc.createElement("Reference");
    t2.setAttribute("URI","#racunId");
    t3 = doc.createElement("Transforms");
    QDomElement t4;
    t4 = doc.createElement("Transform");
    t4.setAttribute("Algorithm","http://www.w3.org/2000/09/xmldsig#enveloped-signature");
    t3.appendChild(t4);
    t4 = doc.createElement("Transform");
    t4.setAttribute("Algorithm","http://www.w3.org/2001/10/xml-exc-c14n#");
    t3.appendChild(t4);
    t2.appendChild(t3);
    t3 = doc.createElement("DigestMethod");
    t3.setAttribute("Algorithm","http://www.w3.org/2000/09/xmldsig#sha1");
    t2.appendChild(t3);
    t3 = doc.createElement("DigestValue");
    t2.appendChild(t3);
    t1.appendChild(t2);
    root.appendChild(t1);
    t1 = doc.createElement("SignatureValue");
    root.appendChild(t1);
    t1 = doc.createElement("KeyInfo");
    t2 = doc.createElement("X509Data");
    t3 = doc.createElement("X509Certificate");
    t2.appendChild(t3);
    t3 = doc.createElement("X509IssuerSerial");
    t2.appendChild(t3);
    t1.appendChild(t2);
    root.appendChild(t1);

    xmlRacunZahtjev.appendChild(root);

    //QString xml = doc.toString();

    xmlSoapBody.appendChild(xmlRacunZahtjev);
    xmlSoap.appendChild(xmlSoapBody);
    doc.appendChild(xmlSoap);
    //qDebug() << doc.toString();

    QString PathXML;
#ifdef Q_WS_WIN
    PathXML = QString("%1/xml/").arg(qApp->applicationDirPath());
#endif
#ifdef Q_WS_X11
    PathXML = "xml/";
#endif
    //PathXML = "";
    QString ImeXMLFajla;
    if (NaknadnoSlanje)
    {
        ImeXMLFajla = QString("xmlRacZ_%1_%2.xml").arg(RacunID).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    }else{
        ImeXMLFajla = QString("xmlRacZ_%1.xml").arg(RacunID);
    }
   // ImeXMLFajla = QString("xml/xmlRacZ_%1.xml").arg(RacunID);
    ImeXMLFajla.prepend(PathXML);
    QFile file(ImeXMLFajla);
    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);
    ispRac << doc.toString().toUtf8();
    file.close();
qDebug() << "Ime xmlfajla:" << ImeXMLFajla;
    //system(Komanda.toUtf8().constData());
//    qApp->setProperty("Certs_Path","mycert2.pfx");
//    qApp->setProperty("Certs_Sifra","1");

    //xmlsec1 --sign --id-attr:Id 'http://www.apis-it.hr/fin/2012/types/f73':RacunZahtjev --pkcs12 mycert2.pfx --pwd 1 --output  O.xml /tmp/xmlRacZahtjerv.xml
    QString ImeXML_sign_Fajla;
    if (NaknadnoSlanje)
    {
        ImeXML_sign_Fajla = QString("xmlRacZ_%1_%2_sign.xml").arg(RacunID).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    }else
    {
        ImeXML_sign_Fajla = QString("xmlRacZ_%1_sign.xml").arg(RacunID);

    }
    ImeXML_sign_Fajla.prepend(PathXML);
   // ImeXML_sign_Fajla = QString("xml/xmlRacZ_%1_sign.xml").arg(RacunID);

    QProcess *zk = new QProcess(this);

QString KomandaXMLSEC;
#ifdef Q_WS_WIN
    KomandaXMLSEC = QString("%1/bin/xmlsec ").arg(qApp->applicationDirPath());
#endif
#ifdef Q_WS_X11
    KomandaXMLSEC = "xmlsec1 ";
#endif

    QString Komanda;
    QStringList NK;
//    Komanda = QString(" --sign --id-attr:Id 'http://www.apis-it.hr/fin/2012/types/f73': RacunZahtjev --pkcs12 %3 --pwd %4 --output  %2 %1").arg(ImeXMLFajla).arg(ImeXML_sign_Fajla).arg(qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString());
#ifdef Q_WS_X11
    Komanda = QString(" --sign --id-attr:Id RacunZahtjev --pkcs12 %3 --pwd %4 --output  %2 %1").arg(ImeXMLFajla).arg(ImeXML_sign_Fajla).arg(qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString());
#endif
//#ifdef Q_WS_WIN
    NK <<"--sign" << "--id-attr:Id RacunZahtjev" << "--pkcs12" << QString("%1/%2").arg(qApp->applicationDirPath()).arg(qApp->property("Certs_Path").toString())
      << "--pwd" << qApp->property("Certs_Sifra").toString()
      << "--output" << ImeXML_sign_Fajla << ImeXMLFajla;
    Komanda = QString(" --sign --id-attr:Id RacunZahtjev --pkcs12 %3 --pwd %4 --output  %2 %1").arg(ImeXMLFajla).arg(ImeXML_sign_Fajla).arg(qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString());
//#endif
    //Komanda.prepend(KomandaXMLSEC);

    qDebug() << "Komand " << KomandaXMLSEC  << Komanda;
    qDebug() << "NK " << NK.join(" ");

    //    qDebug() << system(Komanda.toUtf8().constData());

    zk->start(KomandaXMLSEC.append(NK.join(" ")).toUtf8().constData());
    zk->setReadChannel(QProcess::StandardOutput);
    if (!zk->waitForStarted())
    {
        qDebug() << "Greska - potpis xml " << zk->readAll();
    }else
    {
        zk->waitForFinished();
        QString Povrat;
        Povrat.append(zk->readAllStandardOutput().constData());
        qDebug() << Povrat;
    }


    //qDebug() << system(KomandaXMLSEC.append(NK.join(" ")).toUtf8().constData());
/* Maknuto i prebaceno sve na python
#ifdef Q_WS_X11
//    Komanda = QString("PERL_LWP_SSL_VERIFY_HOSTNAME=0 perl p.pl %1 >> %1.povratni").arg(ImeXML_sign_Fajla);
    Komanda = QString("python p.py %1 >> %1.povratni").arg(ImeXML_sign_Fajla);
#endif
#ifdef Q_WS_WIN
    Komanda = QString("%2/bin/p.bat %1 >> %1.povratni").arg(ImeXML_sign_Fajla).arg(qApp->applicationDirPath());
#endif
*/
    Komanda = QString("./p.py %1 >> %1.povratni").arg(ImeXML_sign_Fajla);
#ifdef Q_WS_WIN
    Komanda = QString("python p2.py %1 >> %1.povratni").arg(ImeXML_sign_Fajla);
#endif

    NK.clear();
    NK << QString("%1/%2").arg(qApp->applicationDirPath()).arg("p.pl") << QString("%1/%2").arg(qApp->applicationDirPath()).arg(ImeXML_sign_Fajla)
       << " >> " << QString("%1/%2.povratni").arg(qApp->applicationDirPath()).arg(ImeXML_sign_Fajla);

    qDebug() <<"KOM: "  << Komanda;
    //qDebug() <<"NK: " << NK;

    qDebug() << system(Komanda.toUtf8().constData());

    QString Jir;
    QString JirError;
    UcitajXML_PovratniRacun(QString("%1.povratni").arg(ImeXML_sign_Fajla),Jir,JirError);

    qDebug() << "Jir je = " << Jir;
    qDebug() << "Greska je = " << JirError;
    if (Jir != "")
    {
        if (!q.exec(QString("insert into rac1fiskal (racid,datum_slanja,odgovor,potvrden) select %1,now(),'%2',1").arg(RacunID).arg(Jir)))
            qDebug() << "Greska upisa JIR-a " << q.lastError() << " " << q.lastQuery();
    }

    return true;
}

QDomText mojiHeaderi::xmlPod(QString Sto)
{
    QDomDocument nd;
    QDomText t = nd.createTextNode(Sto);
    return t;
}
bool mojiHeaderi::UcitajXML_PovratniRacun(QString xmlFileFullPath, QString& jirValue, QString& errValue)
{
    bool bFoundedJir = false;
    bool bFoundedError = false;
    QDomDocument doc;
    QFile file(xmlFileFullPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Necem otvorit";
        return false;
    }
    if (!doc.setContent(&file))
    {
        file.close();
        qDebug() << "Greska otvaranja";
        return false;
    }
    file.close();

    jirValue.clear();
    errValue.clear();

    QDomElement de = doc.documentElement();
    QDomElement e,e1,e2;
    QDomNode dn = de.firstChild();

    while (!dn.isNull())
    {
        e = dn.toElement();
        if (!e.isNull())
        {
            qDebug() << e.tagName();
            if (e.nodeName() == "soap:Body")
            {
                break;
            }
        }
        dn = dn.nextSibling();
    }

    if (!e.isNull())
    {
        dn = e.firstChild();
        while (!dn.isNull())
        {
            e1 = dn.toElement();
            if (!e1.isNull())
            {
                qDebug() << e1.tagName();
                if (e1.nodeName() == "tns:RacunOdgovor")
                {
                    break;
                }
            }
            dn = dn.nextSibling();
        }

        if (!e1.isNull())
        {
            dn = e1.firstChild();
            while (!dn.isNull())
            {
                e2 = dn.toElement();
                if (!e2.isNull())
                {
                    if (e2.nodeName() == "tns:Jir")
                    {
                        jirValue.append(e2.text());
                        bFoundedJir = true;
                        break;
                    }
                    if (e2.nodeName() == "tns:Error")
                    {
                        errValue.append(e2.nodeValue());
                        bFoundedError = true;
                        break;
                    }
                }
                dn = dn.nextSibling();
            }
        }
    }
    qDebug() << jirValue ;
    return bFoundedJir;
}




QString mojiHeaderi::xmlPoslProstorZahtjev(QString ppOib, QString ppOznPoslProstora, QString ppUlica, QString ppKucniBroj,
                                        QString ppKucniBrojDodatak, QString ppBrojPoste, QString ppNaselje, QString ppOpcina,
                                        QString ppRadnoVrijeme, QString ppDatumPocetkaPrimijene, QString ppSpecNamj,
                                        QString ppOstaliTipoviPP, QString ppOznakaZatvaranja,bool ppAdresa)
{

    //Poslovni prostor se ne moze slavti vise preko programa - jedino preko porezna-uprava portala
    return "";

    QString PorukaXML;
    PorukaXML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\""
            "><soapenv:Header/><soapenv:Body><f73:PoslovniProstorZahtjev xmlns:f73=\"http://www.apis-it.hr/fin/2012/types/f73\" Id=\"poslovniProstorId\"><f73:Zaglavlje>";
    PorukaXML += QString("<f73:IdPoruke>%1</f73:IdPoruke>").arg(QUuid::createUuid().toString().remove("{").remove("}"));
    PorukaXML += QString("<f73:DatumVrijeme>%1</f73:DatumVrijeme>").arg(QDateTime::currentDateTime().toString("dd.MM.yyyyThh:mm:ss"));
    PorukaXML += "</f73:Zaglavlje><f73:PoslovniProstor>";
    PorukaXML += QString("<f73:Oib>%1</f73:Oib>").arg(ppOib);
    PorukaXML += QString("<f73:OznPoslProstora>%1</f73:OznPoslProstora>").arg(ppOznPoslProstora);
    PorukaXML += "<f73:AdresniPodatak>";
    //provjera da li je adresa ili ostalo podatak
    if (ppAdresa)
    {
        PorukaXML += QString("<f73:Adresa><f73:Ulica>%1</f73:Ulica><f73:KucniBroj>%2</f73:KucniBroj>").arg(ppUlica).arg(ppKucniBroj);
    //ako ima kucni broj dodatak
        if (ppKucniBrojDodatak != "")
            PorukaXML += QString("<f73:KucniBrojDodatak>%1</f73:KucniBrojDodatak>").arg(ppKucniBrojDodatak);
        PorukaXML += QString("<f73:BrojPoste>%1</f73:BrojPoste><f73:Naselje>%2</f73:Naselje>").arg(ppBrojPoste).arg(ppNaselje);
    //ako ima opcina
        PorukaXML += QString("<f73:Opcina>%1</f73:Opcina>").arg(ppOpcina);
        PorukaXML += "</f73:Adresa>";
    }else{
    //ako nije adresa-ulica onda je otsali tipovi
        PorukaXML += QString("<f73:OstaliTipoviPP>%1</f73:OstaliTipoviPP>").arg(ppOstaliTipoviPP);
    }
    PorukaXML += "</f73:AdresniPodatak>";
    PorukaXML += QString("<f73:RadnoVrijeme>%1</f73:RadnoVrijeme>").arg(ppRadnoVrijeme);
    PorukaXML += QString("<f73:DatumPocetkaPrimjene>%1</f73:DatumPocetkaPrimjene>").arg(ppDatumPocetkaPrimijene);
    if (ppOznakaZatvaranja == "Z")
    {
        PorukaXML += QString("<f73:OznakaZatvaranja>%1</f73:OznakaZatvaranja>").arg(ppOznakaZatvaranja);
    }
    PorukaXML += QString("<f73:SpecNamj>%1</f73:SpecNamj>").arg(ppSpecNamj);
    PorukaXML += "</f73:PoslovniProstor><Signature xmlns=\"http://www.w3.org/2000/09/xmldsig#\"><SignedInfo><CanonicalizationMethod Algorithm=\"http://www.w3.org/2001/10/xml-exc-c14n#\"/>"
             "<SignatureMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#rsa-sha1\"/><Reference URI=\"#poslovniProstorId\"><Transforms>"
               "<Transform Algorithm=\"http://www.w3.org/2000/09/xmldsig#enveloped-signature\"/><Transform Algorithm=\"http://www.w3.org/2001/10/xml-exc-c14n#\"/>"
              "</Transforms><DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\"/><DigestValue/></Reference></SignedInfo>"
            "<SignatureValue/><KeyInfo><X509Data><X509Certificate/><X509IssuerSerial/></X509Data></KeyInfo></Signature>";
    PorukaXML += QString("</f73:PoslovniProstorZahtjev></soapenv:Body></soapenv:Envelope>");

    return PorukaXML;
}

double mojiHeaderi::DecTocka(const double BrojZaDec)
{
    /*
Broj_Za_Dec = Broj_Za_Dec * 100
Broj_Za_Dec = Broj_Za_Dec + 0.5
Broj_Za_Dec = Int(Broj_Za_Dec)
Broj_Za_Dec = Broj_Za_Dec / 100
DecTocka = Broj_Za_Dec
    */
    double Broj_Za_Dec = BrojZaDec * 100;
    Broj_Za_Dec = Broj_Za_Dec + 0.5;
    Broj_Za_Dec = int(Broj_Za_Dec);
    Broj_Za_Dec = Broj_Za_Dec /100;
    return Broj_Za_Dec;
}
