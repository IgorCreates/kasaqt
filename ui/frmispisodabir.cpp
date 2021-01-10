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




#include "frmispisodabir.h"
#include "ui_frmispisodabir.h"
#include "Ispis/ispismali.h"

//#include <ui/QProgressIndicator.h>
//#include <QtConcurrentRun>
#include <QThread>
#include <ui/smtp.h>
#if QT_VERSION > 0x050000
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtWidgets>
//#include <qrencode.h>
#include <qr/QrCode.hpp>
//#include "qrcode_generator.h"
#include <qr/qrcode_generator.h>

#endif



frmIspisOdabir::frmIspisOdabir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmIspisOdabir)
{
    ui->setupUi(this);
    RacunID = 0;
    NarID = 0;
    this->setWindowTitle(tr("Ispis"));
    RacTipRacuna = "rac1";
    RacuniOdDo = false;
    RacuniID.clear();
    prnImeFajla = "Ispis.pdf";
}

frmIspisOdabir::~frmIspisOdabir()
{
    delete ui;
}

void frmIspisOdabir::ProvjeraOpcija()
{
    if (qApp->property("Printer-ProzorOdabirIspisa").toString() != "1")
    {
        this->setHidden(true);
        if (qApp->property("Printer-DEFprinter-POS").toString() == "1")
            frmIspisOdabir::on_btnPOS_released();
        if (qApp->property("Printer-DEFprinter-Veliki").toString() == "1")
            frmIspisOdabir::on_btnA4_released();
    }
}

void frmIspisOdabir::on_btnPOS_released()
{
    if (RacunID != 0)
    {
        if (qApp->property("Printer-POS-RPT-ISPIS").toString() == "1")
        {
           ispisA4template("MaliRac",RacunID);
        }else
        {
            ispisMali *isp = new ispisMali();
    //        isp->IspisMaliPos(RacunID);
            if (RacTipRacuna  == "rac1")
            {
                isp->IspisMaliPos(QString("%1").arg(RacunID));
            }else
            {
                isp->IspisMaliPosVrac(QString("%1").arg(RacunID));
            }
        }
        this->close();
    }else if (RacuniOdDo)
    {
           ispisA4template("MaliRac",RacunID);
    }
}

void frmIspisOdabir::on_btnA4_released()
{

    //

    //
    if (RacunID !=0 || RacuniOdDo)
    {
//        qDebug() << "T2:" << QThread::currentThread();
        //ispisMali *isp = new ispisMali();
        //isp->ispisVeliki("ispRacun",QString("\"rid=int:%1\"").arg(RacunID));
        //isp->ispisA4template(RacunID);
       // if (qApp->property(""))

        if (qApp->property("Prog-IspisRnalogCustom").toString() == "1")
        {
            ispisMali *isp = new ispisMali();
            isp->ispisVeliki("ispRacun",QString("\"rid=%1\"").arg(RacunID));
        }else
        {
            ispisQRcreate(RacunID);
            ispisA4template("",RacunID);
        }
        this->deleteLater();
    }
}



void frmIspisOdabir::StartA4Ispis()
{
    //qDebug() << "T2:" << QThread::currentThread();
    ispisMali *isp = new ispisMali();
    isp->ispisVeliki("ispRacun",QString("\"rid=%1\"").arg(RacunID));
    this->close();

}
void frmIspisOdabir::StopA4Ispis()
{

}

void frmIspisOdabir::ispisA4template(QString Sto,int RID)
{
    QString Ispis;
    Ispis = "";
    if (RacuniOdDo)
    {
        QListIterator<int> Ridovi( RacuniID );


        int BrojIspisa= 0;
        while(Ridovi.hasNext())
        {
            BrojIspisa++;
            //qDebug() << "Prvi-" << BrojIspisa << "-" << Ridovi.next();
            Ridovi.next();
        }
        Ridovi.toFront();

        /*
        QProgressBar *pi = new QProgressBar();
        pi->setMinimum(1);
        pi->setMaximum(BrojIspisa);
        pi->setValue(0);
        connect(this,SIGNAL(Ridovi_Promjena(int)),pi,SLOT(setValue(int)));
        //pi->moveToThread();
*/
        int i = 0;
        while( Ridovi.hasNext() )
        {
            //qDebug() << "Drug-" << i;

//            if (prog.wasCanceled())
//                break;
            i++;
            emit Ridovi_Promjena(i);
            if (Sto == "")
            {
                Ispis += ispisVratiHtmlContent(Ridovi.next());
            }else
            {
                Ispis += ispisVratiHtmlContentMali(Ridovi.next());
            }
            Ispis += "<DIV style=\"page-break-after:always\"></DIV>";
        }


    }else
    {
        if (IspisMaliSto == "narudzba")
            Sto = IspisMaliSto;
        if (Sto == "")
        {
            Ispis = ispisVratiHtmlContent(RID);
        }else if (Sto == "narudzba")
        {
            Ispis += ispisVratiHtmlContentNarudzba(RID);
        }
        else
        {
            Ispis += ispisVratiHtmlContentMali(RID);
        }
    }
//    DokumentZaIspisA4->setHtml(ispisVratiHtmlContent(RID));

    DokumentZaIspisA4 = new QTextDocument();
    DokumentZaIspisA4->setHtml(Ispis);

    prnPrinter = new QPrinter(QPrinter::HighResolution);
    QPrintPreviewDialog pw(prnPrinter,this);
    connect(&pw,SIGNAL(paintRequested(QPrinter*)),SLOT(IspisPreview(QPrinter*)));

    if (Sto == "")
    {
        prnPrinter->setPageSize(QPrinter::A4);
    }else
    {
        int duljina = DokumentZaIspisA4->size().height();
    //    qDebug() << "******** DULJINA = " << duljina;
        duljina = duljina/3.7;
        prnPrinter->setPaperSize(QSizeF(80,duljina), QPrinter::Millimeter);
        prnPrinter->setPageMargins(3,5,5,2,QPrinter::Millimeter);
    }
    //    printer.setPageMargins(25,20,20,20,QPrinter::Millimeter);
    //prnPrinter->setOutputFormat(QPrinter::PdfFormat);
    //prnPrintersetOutputFileName("rRacun.pdf");
    //DokumentZaIspisA4->print(&printer);

    QList<QToolBar *> toolbarlist = pw.findChildren<QToolBar *>();

    prnImeFajla = "rRacun.pdf";
    QAction *newAct = new QAction(this);
    newAct->setText("Posalji mail-om");
    newAct->setIcon(QIcon("ikone/mail.png"));
    newAct->setStatusTip(tr("Posalji dokument mail-om"));
    toolbarlist.last()->addAction(newAct);
    connect(newAct,SIGNAL(triggered()),this,SLOT(PokaziSaljiMejl()));


    pw.setWindowFlags(Qt::Window);
    pw.setModal(true);
    pw.setWindowModality(Qt::WindowModal);

    /*
     * tu stavi ako je direkt ispis da ide */

    int MarginaTop = 1;
    int MarginaBotton = 1;
    int MarginaLeft = 1;
    int MarginaRight = 1;

    if (!qApp->property("Printer-POS_RPT_MarginaTOP").toString().isNull())
        MarginaTop = qApp->property("Printer-POS_RPT_MarginaTOP").toInt();
    if (!qApp->property("Printer-POS_RPT_MarginaBotton").toString().isNull())
        MarginaBotton = qApp->property("Printer-POS_RPT_MarginaBotton").toInt();
    if (!qApp->property("Printer-POS_RPT_MarginaLeft").isNull())
        MarginaLeft = qApp->property("Printer-POS_RPT_MarginaLeft").toInt();
    if (!qApp->property("Printer-POS_RPT_MarginaRight").isNull())
        MarginaRight = qApp->property("Printer-POS_RPT_MarginaRight").toInt();

    if (qApp->property("Printer-POS_RPT_DirektanIspis").toString() == "1")
    {
        prnPrinter->setPageMargins(MarginaLeft,MarginaTop,MarginaRight,MarginaBotton,QPrinter::Millimeter);
        DokumentZaIspisA4->print(prnPrinter);

    }
    else
    {
        prnPrinter->setPageMargins(MarginaLeft,MarginaTop,MarginaRight,MarginaBotton,QPrinter::Millimeter);
        pw.exec();
    }

    //pw.exec();
    delete DokumentZaIspisA4;
    /*
    if (qApp->property("Printer-POS_RPT_DirektanIspis").toString() == "1")
    {
        emit SignalOdjava();
    }
    */
    emit SignalOdjava();
}
void frmIspisOdabir::ispisHTMLtemplate(int RID, QString Sto)
{
    QString Ispis;
    Ispis = "";
    if (RacuniOdDo)
    {
        QListIterator<int> Ridovi( RacuniID );
        while( Ridovi.hasNext() )
        {
            Ispis += ispisVratiHtmlContent(Ridovi.next());
            Ispis += "<DIV style=\"page-break-after:always\"></DIV>";
        }

    }else
    {
        Ispis = ispisVratiHtmlContent(RID);
    }
//    DokumentZaIspisA4->setHtml(ispisVratiHtmlContent(RID));

    DokumentZaIspisA4 = new QTextDocument();
    DokumentZaIspisA4->setHtml(Ispis);

    //QPrinter printer(QPrinter::HighResolution);
    prnPrinter = new QPrinter(QPrinter::HighResolution);
    QPrintPreviewDialog pw(prnPrinter,this);
    connect(&pw,SIGNAL(paintRequested(QPrinter*)),SLOT(IspisPreview(QPrinter*)));
    prnPrinter->setPageSize(QPrinter::A4);
    prnPrinter->setPageMargins(25,20,20,20,QPrinter::Millimeter);

    QList<QToolBar *> toolbarlist = pw.findChildren<QToolBar *>();

    prnImeFajla = "rRacun.pdf";
    QAction *newAct = new QAction(this);
    newAct->setText(tr("Posalji mail-om"));
    newAct->setIcon(QIcon("ikone/mail.png"));
    newAct->setStatusTip(tr("Posalji dokument mail-om"));
    toolbarlist.last()->addAction(newAct);
    connect(newAct,SIGNAL(triggered()),this,SLOT(PokaziSaljiMejl()));


//    DokumentZaIspisA4->print(&printer);

    pw.setWindowFlags(Qt::Window);
    pw.exec();
    delete DokumentZaIspisA4;

}


QString frmIspisOdabir::ispisVratiHtmlContent(int RID)
{
    if (RID <1)
        return "";
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Greska-Ispis"));
    msgBox.addButton(QMessageBox::Ok);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QFile htmlTemplate("rRacun_Template.html");
    if (!htmlTemplate.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        //QMessageBox::warning(QApplication.parent(),"Greska-ispis","Nije moguce naci template za ispis rnaloga","OK");
        msgBox.setText(tr("Nije moguce naci template za ispis"));
        msgBox.show();
        return "";
    }
    QString htmlContent;
    QTextStream in(&htmlTemplate);
    in.setCodec("utf-8");
    htmlContent = in.readAll();



    htmlContent.replace("&lt;FIRMA_NAZIV&gt;",qApp->property("Firma_Ime").toString());
    htmlContent.replace("&lt;FIRMA_OIB&gt;",QString("OIB: %1").arg(qApp->property("Firma_OIB").toString()));
    htmlContent.replace("&lt;FIRMA_ADRESA&gt;",QString("%1,%2-%3").arg(qApp->property("Firma_Adresa").toString()).arg(qApp->property("Firma_Postanski").toString()).arg(qApp->property("Firma_Grad").toString()));
    htmlContent.replace("&lt;FIRMA_OSTALO&gt;",qApp->property("Firma_CB").toString());
//    htmlContent.replace("&lt;FIRMA_MJESTO&gt;",QString("%1-%2"));
    if (RID != 0)
    {
        QSqlQuery q("",QSqlDatabase::database("baza"));

        QString qveri;
        if (RacTipRacuna == "rac1")
        {
            qveri = QString("select v.*,ro.opis from v_ispis_rac1_1 v left join rac1opis ro on v.r1id=ro.rid where r1id=%1").arg(RID);
        }else if (RacTipRacuna == "vrac1")
        {
            qveri = QString("select v.*,ro.opis from v_ispis_rac1_v v left join vrac1opis ro on v.r1id=ro.rid where r1id=%1").arg(RID);
        }
        if (!q.exec(qveri))
        {
            //QMessageBox::warning(null,"Greska-Ispisa","Greska kod dohvata podataka","OK");
            msgBox.setText(tr("Greska dohvata podataka"));
            msgBox.show();
            return "";
        }
        q.next();
        QString VrstaRac = q.value(q.record().indexOf("vrsta")).toString();
        htmlContent.replace("&lt;RACUN_IZRADIO&gt;",q.value(q.record().indexOf("prodao")).toString());
        if (RacTipRacuna == "rac1")
        {
            QString RacunNaziv = "Racun ";
            if (VrstaRac == "V")
                RacunNaziv += qApp->property("Firma_Rtip").toString();
            htmlContent.replace("&lt;RACUN_OTPREMNICA&gt;",RacunNaziv);
            htmlContent.replace("&lt;RACUN_BR&gt;",QString("%1/%2/%3").arg(q.value(q.record().indexOf("br")).toString())
                            .arg(q.value(q.record().indexOf("oznpp")).toString()).arg(q.value(q.record().indexOf("kasaid")).toString()));
            htmlContent.replace("&lt;NACIN_PLACANJA&gt;",q.value(q.record().indexOf("nacin_placanja")).toString());
        }else if (RacTipRacuna == "vrac1")
        {
            htmlContent.replace("&lt;RACUN_OTPREMNICA&gt;",QString("OTPREMNICA "));
            htmlContent.replace("&lt;RACUN_BR&gt;",QString("%1").arg(q.value(q.record().indexOf("br")).toString()));
//                            .arg(q.value(q.record().indexOf("oznpp")).toString()).arg(q.value(q.record().indexOf("kasaid")).toString()));
            htmlContent.replace("&lt;NACIN_PLACANJA&gt;",q.value(q.record().indexOf("nacin_placanja")).toString());
        }
        htmlContent.replace("&lt;DATUM_RACUNA&gt;",q.value(q.record().indexOf("datumr")).toString());
        htmlContent.replace("&lt;RACUN_OPIS&gt;",q.value(q.record().indexOf("opis")).toString());

        //htmlContent.replace("&lt;DATUM_VALUTE&gt;",q.value(q.record().indexOf("datumv")).toString());
        QRegExp exp;
        exp.setCaseSensitivity(Qt::CaseInsensitive);
        QString RowStavke;
        if (VrstaRac == "V" || VrstaRac == "O")
        {
            htmlContent.replace("&lt;KUPAC_NAZIV&gt;",QString("Kupac: %1").arg(q.value(q.record().indexOf("kupac_naziv")).toString()));
            htmlContent.replace("&lt;KUPAC_ADRESA&gt;",q.value(q.record().indexOf("kupac_adresa")).toString());
            htmlContent.replace("&lt;KUPAC_MJESTO&gt;",QString(",%1,%2-%3").arg(q.value(q.record().indexOf("kupac_mjesto")).toString())
                                .arg(q.value(q.record().indexOf("countrycode")).toString()).arg(q.value(q.record().indexOf("country")).toString()));
            htmlContent.replace("&lt;KUPAC_OIB&gt;",QString("OIB: %1").arg(q.value(q.record().indexOf("kupac_oib")).toString()));
        }else
        {
            exp.setPattern("<table name=\"kupac\">(.*)</table name=\"kupac\">");
            RowStavke = htmlContent;
            exp.indexIn(RowStavke);
//            qDebug() << exp.cap(1);
            RowStavke = exp.cap(1);
            htmlContent.replace(RowStavke,"");
        }
        if (RacTipRacuna == "rac1")
        {
            htmlContent.replace("&lt;ZKI&gt;",QString("ZKI: %1").arg(q.value(q.record().indexOf("zki")).toString()));
            htmlContent.replace("&lt;JIR&gt;",QString("JIR: %1").arg(q.value(q.record().indexOf("jir")).toString()));
            if (q.value(q.record().indexOf("jir")).toString() == "" ) {
                htmlContent.replace("<img src=\"/tmp/ispQR.png\" />","");
            }else{
                QString PathQR_IMG = QString("%1/qrimg/%2/").arg(qApp->applicationDirPath()).arg(QDateTime::currentDateTime().toString("yyyy/MM/dd"));
                QString fileSave = QString("%1/qr-%2.png").arg(PathQR_IMG).arg(RID);
                htmlContent.replace("/tmp/ispQR.png",fileSave);
            }
        }else if (RacTipRacuna == "vrac1")
        {
            htmlContent.replace("&lt;ZKI&gt;","");
            htmlContent.replace("&lt;JIR&gt;","");
            htmlContent.replace("<img src=\"/tmp/ispQR.png\" />","");
        }
        htmlContent.replace("&lt;UKPOSNOVICA&gt;",QString("%L1 kn").arg(q.value(q.record().indexOf("bpdv")).toDouble(),0,'f',2));
        htmlContent.replace("&lt;UKPRABAT_KN&gt;",QString("%L1 kn").arg(q.value(q.record().indexOf("rabatk")).toDouble(),0,'f',2));
        htmlContent.replace("&lt;UKPRABAT_POSTO&gt;",QString("%L1").arg(q.value(q.record().indexOf("rabatp")).toDouble(),0,'f',2));
        htmlContent.replace("&lt;UKPZAPLATITI&gt;",QString("%L1 kn").arg(q.value(q.record().indexOf("sum")).toDouble(),0,'f',2));

        exp.setPattern("<tr name=\"tablica_stavke\" valign=\"top\">(.*)</tr name=\"tablica_stavke\">");
        RowStavke = htmlContent; //"<tr name=\"tablica_stavke\" easdasdasdasdasdasdasd</tr>"; //htmlContent;
        exp.indexIn(RowStavke);
        RowStavke = exp.cap(1);
        QString TablicaStavke;
        QString TablicaStavkeRow;
        q.exec("set @BR=0");
        qveri = "";
        if (RacTipRacuna == "rac1")
        {
            qveri = QString("select @BR:=(@BR+1) RB,r.* from rac2 r where r.rid=%1").arg(RID);
        }else if (RacTipRacuna == "vrac1")
        {
            qveri = QString("select @BR:=(@BR+1) RB,r.* from vrac2 r where r.rid=%1").arg(RID);
        }
        if (q.exec(qveri))
        {
            while (q.next())
            {
                TablicaStavkeRow = RowStavke;
                TablicaStavkeRow.prepend("<tr>");
                TablicaStavkeRow.replace("&lt;STAVKA_RB&gt;",q.value(q.record().indexOf("RB")).toString());
                TablicaStavkeRow.replace("&lt;STAVKA_NAZIV&gt;&lt;p&gt;<font size=\"1\" style=\"font-size: 8pt\">&lt;STAVKA_OPIS&gt;",q.value(q.record().indexOf("naziv")).toString());
                TablicaStavkeRow.replace("&lt;STAVKA_KOLICINA&gt;",QString("%L1").arg(q.value(q.record().indexOf("kol")).toDouble(),0,'f',2));
                TablicaStavkeRow.replace("&lt;STAVKA_JM&gt;","KOM");
                TablicaStavkeRow.replace("&lt;STAVKA_JEDCJENA&gt;",QString("%L1").arg(q.value(q.record().indexOf("pc")).toDouble(),0,'f',2));
                TablicaStavkeRow.replace("&lt;STAVKA_RBTP&gt;",QString("%L1").arg(q.value(q.record().indexOf("rabatp")).toDouble(),0,'f',2));
                TablicaStavkeRow.replace("&lt;STAVKA_RBTKN&gt;",QString("%L1").arg(q.value(q.record().indexOf("rabatk")).toDouble(),0,'f',2));
                TablicaStavkeRow.replace("&lt;STAVKA_UKUPNO&gt;",QString("%L1").arg(q.value(q.record().indexOf("ukupno")).toDouble(),0,'f',2));
                TablicaStavkeRow.replace("&lt;STAVKA_RB&gt;","");
                TablicaStavkeRow.replace("&lt;STAVKA_OPIS&gt;","");
                TablicaStavkeRow += "</tr>";
                TablicaStavke += TablicaStavkeRow;
            }
            //qDebug() << q.lastError() << q.lastQuery();
            htmlContent.replace("<tr name=\"tablica_stavke\" valign=\"top\">","");
            htmlContent.replace("</tr name=\"tablica_stavke\">","");
            htmlContent.replace(RowStavke,TablicaStavke);
        }else
        {
            qDebug() << q.lastError() << q.lastQuery();
        }
        exp.setPattern("<tr valign=\"top\" name=\"tablica_porezi_stavke\">(.*)</tr name=\"tablica_porezi_stavke\">");
        RowStavke = htmlContent;
        exp.indexIn(RowStavke);
        RowStavke = exp.cap(1);
        QString PorezPoruka= "";
        if (qApp->property("Firma_USustavuPDV").toString() == "false")
        {
            q.exec("select value1 from opcije where sifra='PdvObveznikPoruka'");
            if(q.next())
            {
                PorezPoruka = q.value(0).toString();
                PorezPoruka += "<br>";
            }
        }else
        {
            if (qApp->property("Firma_Rtip").toString() == "R2")
            {
                q.exec("select value1 from opcije where sifra='R2RacunNapomena'");
                if (q.next())
                    PorezPoruka += QString("%1<br>").arg(q.value(0).toString());
            }
        }
        qveri = "";
        if (RacTipRacuna == "rac1")
        {
            qveri = QString("select * from rac1porezi where rid=%1").arg(RID);
        }else if (RacTipRacuna == "vrac1")
        {
            qveri = QString("select * from vrac1porezi where rid=%1").arg(RID);
        }
        if (q.exec(qveri))
        {
            TablicaStavke = "";
            TablicaStavkeRow ="";
            while (q.next())
            {
                TablicaStavkeRow = RowStavke;
                TablicaStavkeRow.prepend("<tr>");
                TablicaStavkeRow.replace("&lt;POREZ_OPIS&gt;",q.value(q.record().indexOf("porez_sifra")).toString());
                TablicaStavkeRow.replace("&lt;POREZ_IZNOS&gt;",QString("%L1").arg(q.value(q.record().indexOf("iznos_poreza")).toDouble(),0,'f',2));
                TablicaStavkeRow += "</tr>";
                TablicaStavke += TablicaStavkeRow;
            }
            if (RacTipRacuna == "rac1")
            {
                if (q.exec(QString("select sum(kol*povratnanaknada) povratna from rac2dodatno where rid=%1").arg(RID)))
                {
                    if (q.next())
                    {
                        if (q.value(0).toDouble() != 0)
                            PorezPoruka += QString("Povratna naknada: %L1 kn").arg(q.value(0).toDouble(),0,'f',2);
                    }
                }
            }
            htmlContent.replace("&lt;POREZ_PORUKA&gt;",PorezPoruka);
            htmlContent.replace("<tr valign=\"top\" name=\"tablica_porezi_stavke\">","");
            htmlContent.replace("</tr name=\"tablica_porezi_stavke\">","");
            htmlContent.replace(RowStavke,TablicaStavke);
        }else
        {
            htmlContent.replace("&lt;POREZ_OPIS&gt;","");
            htmlContent.replace("&lt;POREZ_IZNOS&gt;","");
            htmlContent.replace("&lt;POREZ_PORUKA",PorezPoruka);
        }
        return htmlContent;
        //qDebug() << htmlContent;
    }
    return "";
}
QString frmIspisOdabir::ispisVratiHtmlContentMali(int RID)
{
    if (RID <1)
        return "";
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Greska-Ispis"));
    msgBox.addButton(QMessageBox::Ok);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QFile htmlTemplate("rRacunMali_Template.html");
    if (!htmlTemplate.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        //QMessageBox::warning(QApplication.parent(),"Greska-ispis","Nije moguce naci template za ispis rnaloga","OK");
        msgBox.setText(tr("Nije moguce naci template za ispis"));
        msgBox.show();
        return "";
    }
    QString htmlContent;
    QTextStream in(&htmlTemplate);
    in.setCodec("utf-8");
    htmlContent = in.readAll();


    htmlContent.replace("<title>RAC-Mali</title>",QString("<title>RacMali_id-%1</title>").arg(RID));

    htmlContent.replace("&lt;FIRMA_NAZIV&gt;",qApp->property("Firma_Ime").toString());
    htmlContent.replace("&lt;FIRMA_OIB&gt;",QString("OIB: %1").arg(qApp->property("Firma_OIB").toString()));
    htmlContent.replace("&lt;FIRMA_ADRESA&gt;",QString("%1,%2-%3").arg(qApp->property("Firma_Adresa").toString()).arg(qApp->property("Firma_Postanski").toString()).arg(qApp->property("Firma_Grad").toString()));
    htmlContent.replace("&lt;FIRMA_OSTALO&gt;",qApp->property("Firma_CB").toString());
//    htmlContent.replace("&lt;FIRMA_MJESTO&gt;",QString("%1-%2"));
    if (RID != 0)
    {
        QSqlQuery q("",QSqlDatabase::database("baza"));

        QString qveri;
        if (RacTipRacuna == "rac1")
        {
            qveri = QString("select v.*,ro.opis from v_ispis_rac1_1 v left join rac1opis ro on v.r1id=ro.rid where r1id=%1").arg(RID);
        }else if (RacTipRacuna == "vrac1")
        {
            qveri = QString("select v.*,ro.opis from v_ispis_rac1_v v left join vrac1opis ro on v.r1id=ro.rid where r1id=%1").arg(RID);
        }
        if (!q.exec(qveri))
        {
            //QMessageBox::warning(null,"Greska-Ispisa","Greska kod dohvata podataka","OK");
            msgBox.setText("Greska dohvata podataka");
            msgBox.show();
            return "";
        }
        q.next();
        QString VrstaRac = q.value(q.record().indexOf("vrsta")).toString();
        htmlContent.replace("&lt;RACUN_IZRADIO&gt;",q.value(q.record().indexOf("prodao")).toString());
        if (q.value(q.record().indexOf("narudzba")).toString() == "1")
        {
            htmlContent.replace("&lt;NARUDZBU_IZRADIO&gt;",q.value(q.record().indexOf("narucio")).toString());
        }else
        {
            htmlContent.replace("<tr><div >Narucio &lt;NARUDZBU_IZRADIO&gt;</div></tr>","");
        }

        if (RacTipRacuna == "rac1")
        {
            QString RacunNaziv = "Racun ";
            if (VrstaRac == "V")
                RacunNaziv += qApp->property("Firma_Rtip").toString();
            htmlContent.replace("&lt;RACUN_OTPREMNICA&gt;",RacunNaziv);
            htmlContent.replace("&lt;RACUN_BR&gt;",QString("%1/%2/%3").arg(q.value(q.record().indexOf("br")).toString())
                            .arg(q.value(q.record().indexOf("oznpp")).toString()).arg(q.value(q.record().indexOf("kasaid")).toString()));
            htmlContent.replace("&lt;NACIN_PLACANJA&gt;",q.value(q.record().indexOf("nacin_placanja")).toString());
        }else if (RacTipRacuna == "vrac1")
        {
            htmlContent.replace("&lt;RACUN_OTPREMNICA&gt;",QString("OTPREMNICA "));
            htmlContent.replace("&lt;RACUN_BR&gt;",QString("%1").arg(q.value(q.record().indexOf("br")).toString()));
//                            .arg(q.value(q.record().indexOf("oznpp")).toString()).arg(q.value(q.record().indexOf("kasaid")).toString()));
            htmlContent.replace("&lt;NACIN_PLACANJA&gt;",q.value(q.record().indexOf("nacin_placanja")).toString());
        }
        htmlContent.replace("&lt;DATUM_RACUNA&gt;",q.value(q.record().indexOf("datumr")).toString());
        htmlContent.replace("&lt;RACUN_OPIS&gt;",q.value(q.record().indexOf("opis")).toString());

        //htmlContent.replace("&lt;DATUM_VALUTE&gt;",q.value(q.record().indexOf("datumv")).toString());
        QRegExp exp;
        exp.setCaseSensitivity(Qt::CaseInsensitive);
        QString RowStavke;
        if (VrstaRac == "V" || VrstaRac == "O")
        {
            htmlContent.replace("&lt;KUPAC_NAZIV&gt;",QString("Kupac: %1").arg(q.value(q.record().indexOf("kupac_naziv")).toString()));
            htmlContent.replace("&lt;KUPAC_ADRESA&gt;",q.value(q.record().indexOf("kupac_adresa")).toString());
            htmlContent.replace("&lt;KUPAC_MJESTO&gt;",QString(",%1,%2-%3").arg(q.value(q.record().indexOf("kupac_mjesto")).toString())
                                .arg(q.value(q.record().indexOf("countrycode")).toString()).arg(q.value(q.record().indexOf("country")).toString()));
            htmlContent.replace("&lt;KUPAC_OIB&gt;",QString("OIB: %1").arg(q.value(q.record().indexOf("kupac_oib")).toString()));
        }else
        {
            exp.setPattern("<table name=\"kupac\">(.*)</table name=\"kupac\">");
            RowStavke = htmlContent;
            exp.indexIn(RowStavke);
//            qDebug() << exp.cap(1);
            RowStavke = exp.cap(1);
            htmlContent.replace(RowStavke,"");
        }
        if (RacTipRacuna == "rac1")
        {
            htmlContent.replace("&lt;ZKI&gt;",QString("ZKI: %1").arg(q.value(q.record().indexOf("zki")).toString()));
            htmlContent.replace("&lt;JIR&gt;",QString("JIR: %1").arg(q.value(q.record().indexOf("jir")).toString()));
        }else if (RacTipRacuna == "vrac1")
        {
            htmlContent.replace("&lt;ZKI&gt;","");
            htmlContent.replace("&lt;JIR&gt;","");
        }
        htmlContent.replace("&lt;UKPOSNOVICA&gt;",QString("%L1 kn").arg(q.value(q.record().indexOf("bpdv")).toDouble(),0,'f',2));
        htmlContent.replace("&lt;UKPRABAT_KN&gt;",QString("%L1 kn").arg(q.value(q.record().indexOf("rabatk")).toDouble(),0,'f',2));
        htmlContent.replace("&lt;UKPRABAT_POSTO&gt;",QString("%L1").arg(q.value(q.record().indexOf("rabatp")).toDouble(),0,'f',2));
        htmlContent.replace("&lt;UKPZAPLATITI&gt;",QString("%L1 kn").arg(q.value(q.record().indexOf("sum")).toDouble(),0,'f',2));

        exp.setPattern("<tr name=\"tablica_stavke\" valign=\"top\">(.*)</tr name=\"tablica_stavke\">");
        RowStavke = htmlContent; //"<tr name=\"tablica_stavke\" easdasdasdasdasdasdasd</tr>"; //htmlContent;
        exp.indexIn(RowStavke);
        RowStavke = exp.cap(1);
        QString TablicaStavke;
        QString TablicaStavkeRow;
        q.exec("set @BR=0");
        qveri = "";
        if (RacTipRacuna == "rac1")
        {
            qveri = QString("select @BR:=(@BR+1) RB,r.* from rac2 r where r.rid=%1").arg(RID);
        }else if (RacTipRacuna == "vrac1")
        {
            qveri = QString("select @BR:=(@BR+1) RB,r.* from vrac2 r where r.rid=%1").arg(RID);
        }
        if (q.exec(qveri))
        {
            while (q.next())
            {
                TablicaStavkeRow = RowStavke;
                TablicaStavkeRow.prepend("<tr>");
                TablicaStavkeRow.replace("&lt;STAVKA_RB&gt;",q.value(q.record().indexOf("RB")).toString());
                TablicaStavkeRow.replace("&lt;STAVKA_NAZIV&gt;",q.value(q.record().indexOf("naziv")).toString());
                TablicaStavkeRow.replace("&lt;STAVKA_KOLICINA&gt;",QString("%L1").arg(q.value(q.record().indexOf("kol")).toDouble(),0,'f',2));
                TablicaStavkeRow.replace("&lt;STAVKA_JM&gt;","KOM");
                TablicaStavkeRow.replace("&lt;STAVKA_JEDCJENA&gt;",QString("%L1").arg(q.value(q.record().indexOf("pc")).toDouble(),0,'f',2));
                if (q.value(q.record().indexOf("rabatk")).toDouble() != 0)
                {
                    TablicaStavkeRow.replace("&lt;STAVKA_RBTP&gt;",QString("<br>** Rabat %L1% -- ").arg(q.value(q.record().indexOf("rabatp")).toDouble(),0,'f',2));
                    TablicaStavkeRow.replace("&lt;STAVKA_RBTKN&gt;",QString("%L1 kn **").arg(q.value(q.record().indexOf("rabatk")).toDouble(),0,'f',2));
                }else
                {
                    TablicaStavkeRow.replace("&lt;STAVKA_RBTP&gt;","");
                    TablicaStavkeRow.replace("&lt;STAVKA_RBTKN&gt;","");
                }
                TablicaStavkeRow.replace("&lt;STAVKA_UKUPNO&gt;",QString("%L1").arg(q.value(q.record().indexOf("ukupno")).toDouble(),0,'f',2));
                TablicaStavkeRow.replace("&lt;STAVKA_RB&gt;","");
                TablicaStavkeRow.replace("&lt;STAVKA_OPIS&gt;","");
                TablicaStavkeRow += "</tr>";
                TablicaStavke += TablicaStavkeRow;
            }
            //qDebug() << q.lastError() << q.lastQuery();
            htmlContent.replace("<tr name=\"tablica_stavke\" valign=\"top\">","");
            htmlContent.replace("</tr name=\"tablica_stavke\">","");
            htmlContent.replace(RowStavke,TablicaStavke);
        }else
        {
            qDebug() << q.lastError() << q.lastQuery();
        }
        exp.setPattern("<tr valign=\"top\" name=\"tablica_porezi_stavke\">(.*)</tr name=\"tablica_porezi_stavke\">");
        RowStavke = htmlContent;
        exp.indexIn(RowStavke);
        RowStavke = exp.cap(1);
        QString PorezPoruka= "";
        if (qApp->property("Firma_USustavuPDV").toString() == "false")
        {
            q.exec("select value1 from opcije where sifra='PdvObveznikPoruka'");
            if(q.next())
            {
                PorezPoruka = q.value(0).toString();
                PorezPoruka += "<br>";
            }
        }else
        {
            if (qApp->property("Firma_Rtip").toString() == "R2")
            {
                q.exec("select value1 from opcije where sifra='R2RacunNapomena'");
                if (q.next())
                    PorezPoruka += QString("%1<br>").arg(q.value(0).toString());
            }
        }
        qveri = "";
        if (RacTipRacuna == "rac1")
        {
            qveri = QString("select * from rac1porezi where rid=%1").arg(RID);
        }else if (RacTipRacuna == "vrac1")
        {
            qveri = QString("select * from vrac1porezi where rid=%1").arg(RID);
        }
        if (q.exec(qveri))
        {
            TablicaStavke = "";
            TablicaStavkeRow ="";
            while (q.next())
            {
                TablicaStavkeRow = RowStavke;
                TablicaStavkeRow.prepend("<tr>");
                TablicaStavkeRow.replace("&lt;POREZ_OPIS&gt;",q.value(q.record().indexOf("porez_sifra")).toString());
                TablicaStavkeRow.replace("&lt;POREZ_IZNOS&gt;",QString("%L1").arg(q.value(q.record().indexOf("iznos_poreza")).toDouble(),0,'f',2));
                TablicaStavkeRow += "</tr>";
                TablicaStavke += TablicaStavkeRow;
            }
            if (RacTipRacuna == "rac1")
            {
                if (q.exec(QString("select sum(kol*povratnanaknada) povratna from rac2dodatno where rid=%1").arg(RID)))
                {
                    if (q.next())
                    {
                        if (q.value(0).toDouble() != 0)
                            PorezPoruka += QString("Povratna naknada: %L1 kn").arg(q.value(0).toDouble(),0,'f',2);
                    }
                }
            }
            htmlContent.replace("&lt;POREZ_PORUKA&gt;",PorezPoruka);
            htmlContent.replace("<tr valign=\"top\" name=\"tablica_porezi_stavke\">","");
            htmlContent.replace("</tr name=\"tablica_porezi_stavke\">","");
            htmlContent.replace(RowStavke,TablicaStavke);
        }else
        {
            htmlContent.replace("&lt;POREZ_OPIS&gt;","");
            htmlContent.replace("&lt;POREZ_IZNOS&gt;","");
            htmlContent.replace("&lt;POREZ_PORUKA",PorezPoruka);
        }
        return htmlContent;
        //qDebug() << htmlContent;
    }
    return "";
}



void frmIspisOdabir::IspisPreview(QPrinter *printer)
{
    Q_UNUSED(printer);
    DokumentZaIspisA4->print(printer);
}


QString frmIspisOdabir::ispisVratiHtmlContentStanje(const QDateTime &DatumStart, const QDateTime &DatumEnd, const int &KasaID)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Greska-Ispis");
    msgBox.addButton(QMessageBox::Ok);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QFile htmlTemplate("rStanje_Template.html");
    if (!htmlTemplate.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        //QMessageBox::warning(QApplication.parent(),"Greska-ispis","Nije moguce naci template za ispis rnaloga","OK");
        msgBox.setText("Nije moguce naci template za ispis");
        msgBox.show();
        return "";
    }
    QString htmlContent;
    QTextStream in(&htmlTemplate);
    in.setCodec("utf-8");
    htmlContent = in.readAll();
    htmlContent.replace("&lt;FIRMA_NAZIV&gt;",qApp->property("Firma_Ime").toString());
    htmlContent.replace("&lt;FIRMA_OIB&gt;",QString("OIB: %1").arg(qApp->property("Firma_OIB").toString()));
    htmlContent.replace("&lt;FIRMA_ADRESA&gt;",QString("%1,%2-%3").arg(qApp->property("Firma_Adresa").toString()).arg(qApp->property("Firma_Postanski").toString()).arg(qApp->property("Firma_Grad").toString()));
    htmlContent.replace("&lt;FIRMA_NAZIV&gt;","");
    htmlContent.replace("&lt;FIRMA_OIB&gt;","");
    htmlContent.replace("&lt;FIRMA_ADRESA&gt;","");
    htmlContent.replace("&lt;FIRMA_OSTALO&gt;","");

    QSqlQuery q("",QSqlDatabase::database("baza"));
    /*
        create temporary table iz2 select r.rid,r1.np,r1.datumr,r.aid,(r.ukupno-(r.ukupno*r1.rabatp/100)) ukupno_po_rac,
        (r.bpdv-(r.bpdv*r1.rabatp/100)) bpdv_po_rac,ifnull((rp.iznos_poreza-(rp.iznos_poreza*r1.rabatp/100)),0) porez_po_rac,
        rp.porez_opis,((r.ukupno-(r.ukupno*r1.rabatp/100))-rd.dod)*ifnull(rp.koeficijent_poreza,0) porez2,
        ((r.ukupno-(r.ukupno*r1.rabatp/100))-rd.dod)-(((r.ukupno-(r.ukupno*r1.rabatp/100))-rd.dod)*ifnull(rp.koeficijent_poreza,0)) bpdv2,
        rd.dod from rac1 r1 left join rac2 r on r1.id=r.rid left join
        (select koeficijent_poreza,porez_opis,iznos_poreza,rid,aid,r2localid from rac2porezi group by rid,aid,r2localid) rp on
        (r.rid=rp.rid and r.aid=rp.aid and r.r2localid=rp.r2localid),
        (select rid,aid,sum(kol*povratnanaknada) dod from rac2dodatno group by rid,aid) rd  where r1.kid=%3 and
        r1.datumr between '%1' and '%2' and r.rid=rd.rid and r.aid=rd.aid group by r.rid,r.aid,r.r2localid;

        select k.naziv,sum(ukupno_po_rac) placeno,sum(bpdv_po_rac) bpdv_po_rac,sum(bpdv2) bpdv_izracun,porez_opis,
        sum(i.porez_po_rac) porez_po_rac,sum(i.porez2) porez_izracun,sum(dod) from iz2 i left join kartice k on i.np=k.id group by k.naziv,porez_opis;

     * */
    //frmIspisOdabir fis2;

/*
    QProgressIndicator *pi = new QProgressIndicator();
    pi->show();
    pi->setFixedSize(300,300);
    pi->startAnimation();
    QFuture<void> thr = QtConcurrent::run(frmIspisOdabir::IzvrtiSQL,DatumStart,DatumEnd,KasaID);
    //thr.waitForFinished();
    thr.waitForFinished();

*/
    QRegExp exp;
    exp.setCaseSensitivity(Qt::CaseInsensitive);
    QString RowStavke;
    exp.setPattern("<tr name=\"tablica_stavke\" valign=\"top\">(.*)</tr name=\"tablica_stavke\">");
    RowStavke = htmlContent; //"<tr name=\"tablica_stavke\" easdasdasdasdasdasdasd</tr>"; //htmlContent;
    exp.indexIn(RowStavke);
    RowStavke = exp.cap(1);
    QString TablicaStavke;
    QString TablicaStavkeRow;

//    QString qveri = "";
    QString qveri = QString("create temporary table iz2 select r.rid,r1.np,r1.datumr,r.aid,(r.ukupno-(r.ukupno*r1.rabatp/100)) ukupno_po_rac,"
                            " (r.bpdv-(r.bpdv*r1.rabatp/100)) bpdv_po_rac,ifnull((rp.iznos_poreza-(rp.iznos_poreza*r1.rabatp/100)),0) porez_po_rac, "
                            " rp.porez_opis,((r.ukupno-(r.ukupno*r1.rabatp/100))-rd.dod)*ifnull(rp.koeficijent_poreza,0) porez2, "
                            " ((r.ukupno-(r.ukupno*r1.rabatp/100))-rd.dod)-(((r.ukupno-(r.ukupno*r1.rabatp/100))-rd.dod)*ifnull(rp.koeficijent_poreza,0)) bpdv2, "
                            " rd.dod from rac1 r1 left join rac2 r on r1.id=r.rid left join "
                            " (select koeficijent_poreza,porez_opis,iznos_poreza,rid,aid,r2id from rac2porezi where rid between @MinRID and @MaxRID  group by aid,r2id) rp on "
                            " (r.id=rp.r2id),(select rid,aid,sum(kol*povratnanaknada) dod from rac2dodatno where rid between @MinRID and @MaxRID group by rid,aid) rd  where r.rid between @MinRID and @MaxRID and r.rid=rd.rid and r.aid=rd.aid group by r.rid,r.aid,r.id;"
                            ).arg(KasaID);
    //qDebug() << qveri;
    q.exec("drop table iz2");
    if (!q.exec(QString("set @MinRID=(select min(id) from rac1 where datumr>'%1')").arg(DatumStart.toString("yyyy-MM-dd hh:mm:ss"))))
    {
        QMessageBox::warning(this,"Problem","Problem kod dohvacanja podtaka","OK");
        return "";
    }
    if (!q.exec(QString("set @MaxRID=(select max(id) from rac1 where datumr<='%2')").arg(DatumEnd.toString("yyyy-MM-dd hh:mm:ss"))))
    {
        QMessageBox::warning(this,"Problem","Problem kod dohvacanja podtaka","OK");
        return "";
    }
    if (!q.exec(qveri))
    {
        QMessageBox::warning(this,"Problem","Problem kod dohvacanja podtaka","OK");
        qDebug() << q.lastError() << q.lastQuery();
        return "";
    }

    qveri = "select k.naziv,sum(ukupno_po_rac) placeno,sum(bpdv_po_rac) bpdv_po_rac,sum(bpdv2) bpdv_izracun,porez_opis,sum(i.porez_po_rac) porez_po_rac,sum(i.porez2) porez_izracun,sum(dod) dod from iz2 i left join kartice k on i.np=k.id group by k.naziv,porez_opis;";
    //qDebug() << qveri;
    if (!q.exec(qveri))
    {
        QMessageBox::warning(this,"Problem","Problem kod dohvacanja podtaka","OK");
        qDebug() << q.lastError() << q.lastQuery();
        return "";
    }
    while (q.next())
    {
        TablicaStavkeRow = RowStavke;
        TablicaStavkeRow.prepend("<tr>");
        TablicaStavkeRow.replace("&lt;STAVKA_NP&gt;",q.value(q.record().indexOf("naziv")).toString());
        TablicaStavkeRow.replace("&lt;STAVKA_Placeno&gt;",QString("%L1").arg(q.value(q.record().indexOf("placeno")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;STAVKA_Osnovica_po_racunima&gt;",QString("%L1").arg(q.value(q.record().indexOf("bpdv_po_rac")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;STAVKA_Osnovica_izracun&gt;",QString("%L1").arg(q.value(q.record().indexOf("bpdv_izracun")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;STAVKA_Porez_opis&gt;",q.value(q.record().indexOf("porez_opis")).toString());
        TablicaStavkeRow.replace("&lt;STAVKA_Porez_po_racunima&gt;",QString("%L1").arg(q.value(q.record().indexOf("porez_po_rac")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;STAVKA_Porez_izracun&gt;",QString("%L1").arg(q.value(q.record().indexOf("porez_izracun")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;STAVKA_PovratnaNaknada&gt;",QString("%L1").arg(q.value(q.record().indexOf("dod")).toDouble(),0,'f',2));
        TablicaStavkeRow += "</tr>";
        TablicaStavke += TablicaStavkeRow;
    }
    htmlContent.replace(RowStavke,TablicaStavke);
    htmlContent.replace("&lt;STAVKA_NP&gt;","");
    htmlContent.replace("&lt;STAVKA_Placeno&gt;","");
    htmlContent.replace("&lt;STAVKA_Osnovica_po_racunima&gt;","");
    htmlContent.replace("&lt;STAVKA_Osnovica_izracun&gt;","");
    htmlContent.replace("&lt;STAVKA_Porez_opis&gt;","");
    htmlContent.replace("&lt;STAVKA_Porez_po_racunima&gt;","");
    htmlContent.replace("&lt;STAVKA_Porez_izracun&gt;","");
    htmlContent.replace("&lt;STAVKA_PovratnaNaknada&gt;","");


    exp.setPattern("<tr name=\"tablica_porez\" valign=\"top\">(.*)</tr name=\"tablica_porez\">");
    RowStavke = htmlContent; //"<tr name=\"tablica_sume\" easdasdasdasdasdasdasd</tr>"; //htmlContent;
    exp.indexIn(RowStavke);
    RowStavke = exp.cap(1);
    TablicaStavke = "";
    TablicaStavkeRow = "";
    if (!q.exec("select sum(ukupno_po_rac) placeno,sum(bpdv_po_rac) bpdv_po_rac,sum(bpdv2) bpdv_izracun,porez_opis,sum(i.porez_po_rac) porez_po_rac,sum(i.porez2) porez_izracun,sum(dod) dod from iz2 i group by porez_opis"))
        qDebug() << q.lastError() << q.lastQuery();
    while (q.next())
    {
        qDebug() << "TuSAM";
        TablicaStavkeRow = RowStavke;
        TablicaStavkeRow.prepend("<tr>");
        TablicaStavkeRow.replace("&lt;Porez_NP&gt;","");
        TablicaStavkeRow.replace("&lt;Porez_Placeno&gt;",QString("%L1").arg(q.value(q.record().indexOf("placeno")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Porez_Osnovica_po_racunima&gt;",QString("%L1").arg(q.value(q.record().indexOf("bpdv_po_rac")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Porez_Osnovica_izracun&gt;",QString("%L1").arg(q.value(q.record().indexOf("bpdv_izracun")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Porez_Porez_opis&gt;",q.value(q.record().indexOf("porez_opis")).toString());
        TablicaStavkeRow.replace("&lt;Porez_Porez_po_racunima&gt;",QString("%L1").arg(q.value(q.record().indexOf("porez_po_rac")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Porez_Porez_izracun&gt;",QString("%L1").arg(q.value(q.record().indexOf("porez_izracun")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Porez_PovratnaNaknada&gt;",QString("%L1").arg(q.value(q.record().indexOf("dod")).toDouble(),0,'f',2));
        TablicaStavkeRow += "</tr>";
        TablicaStavke += TablicaStavkeRow;

    }
    htmlContent.replace(RowStavke,TablicaStavke);

    exp.setPattern("<tr name=\"tablica_ukp\" valign=\"top\">(.*)</tr name=\"tablica_ukp\">");
    RowStavke = htmlContent; //"<tr name=\"tablica_sume\" easdasdasdasdasdasdasd</tr>"; //htmlContent;
    exp.indexIn(RowStavke);
    RowStavke = exp.cap(1);
    TablicaStavke = "";
    TablicaStavkeRow = "";
    if (!q.exec("select sum(ukupno_po_rac) placeno,sum(bpdv_po_rac) bpdv_po_rac,sum(bpdv2) bpdv_izracun,porez_opis,sum(i.porez_po_rac) porez_po_rac,sum(i.porez2) porez_izracun,sum(dod) dod from iz2 i"))
        qDebug() << q.lastError() << q.lastQuery();
    while (q.next())
    {
//        qDebug() << "TuSAM";
        TablicaStavkeRow = RowStavke;
        TablicaStavkeRow.prepend("<tr>");
        TablicaStavkeRow.replace("&lt;Ukupno_NP&gt;"," Ukupno : ");
        TablicaStavkeRow.replace("&lt;Ukupno_Placeno&gt;",QString("%L1").arg(q.value(q.record().indexOf("placeno")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Ukupno_Osnovica_po_racunima&gt;",QString("%L1").arg(q.value(q.record().indexOf("bpdv_po_rac")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Ukupno_Osnovica_izracun&gt;",QString("%L1").arg(q.value(q.record().indexOf("bpdv_izracun")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Ukupno_Porez_opis&gt;","");
        TablicaStavkeRow.replace("&lt;Ukupno_Porez_po_racunima&gt;",QString("%L1").arg(q.value(q.record().indexOf("porez_po_rac")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Ukupno_Porez_izracun&gt;",QString("%L1").arg(q.value(q.record().indexOf("porez_izracun")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;Ukupno_PovratnaNaknada&gt;",QString("%L1").arg(q.value(q.record().indexOf("dod")).toDouble(),0,'f',2));
        TablicaStavkeRow += "</tr>";
        TablicaStavke += TablicaStavkeRow;
    }
    htmlContent.replace(RowStavke,TablicaStavke);


    if (q.exec("select min(datumr),max(datumr) from iz2"))
    {
        q.next();
        htmlContent.replace("&lt;DATUM_START&gt;",q.value(0).toString());
        htmlContent.replace("&lt;DATUM_END&gt;",q.value(1).toString());
    }else
    {
        htmlContent.replace("&lt;DATUM_START&gt;","");
        htmlContent.replace("&lt;DATUM_END&gt;","");
    }
    return htmlContent;
}

void frmIspisOdabir::ispisA4templateStanje(QDateTime DatumStart, QDateTime DatumEnd, int KasaID, QString MaliVeliki)
{


    QString Ispis;
    Ispis = "";
    DokumentZaIspisA4 = new QTextDocument();
    //qDebug() << this->thread()->currentThreadId();
    if (MaliVeliki == "Veliki")
    {
        Ispis = ispisVratiHtmlContentStanje(DatumStart,DatumEnd,KasaID);

        DokumentZaIspisA4->setHtml(Ispis);
    }else
    {
        //(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //QString text = file.readAll();
    QFile file;
#ifdef Q_OS_LINUX
    file.setFileName("/tmp/ispStanje1.txt");
#endif
#ifdef Q_OS_WIN
    file.setFileName(QString("%1/ispStanje1.txt").arg(qApp->applicationDirPath()));
#endif

    qDebug() <<  file.fileName();
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            try
            {
                /*
                QTextEdit *edit = new QTextEdit();
                edit->setPlainText(file.readAll());
                DokumentZaIspisA4->clear();
                edit->setDocument(DokumentZaIspisA4);
                */
                DokumentZaIspisA4->setPlainText(file.readAll());
            }catch ( ... )
            {
                goto finally;
            }
            finally:
            {
                file.close();
            }
        }else
        {
            QMessageBox::warning(this,"Ispis","Datoteka nije napravljena","OK");
            return;
        }
    }


    //QPrinter printer(QPrinter::HighResolution);
    prnPrinter = new QPrinter(QPrinter::HighResolution);

    pw = new QPrintPreviewDialog(prnPrinter,this);
    //QPrintPreviewDialog pw(prnPrinter,this);
    QList<QToolBar *> toolbarlist = pw->findChildren<QToolBar *>();

//    toolbarlist.last()->addAction(QIcon("ikone/mail.png"), tr("Posalji na"));

    prnPrinter->setPageSize(QPrinter::A4);
    prnPrinter->setPageMargins(25,20,20,20,QPrinter::Millimeter);
   // DokumentZaIspisA4->print(&printer);
    prnImeFajla = QString("ispStanje_%1.pdf").arg(QDate::currentDate().toString("yyyyMMdd"));

    QAction *newAct = new QAction(this);
    newAct->setText("Posalji mail-om");
    newAct->setIcon(QIcon("ikone/mail.png"));
    newAct->setStatusTip(tr("Posalji dokument mail-om"));

    toolbarlist.last()->addAction(newAct);
    connect(pw,SIGNAL(paintRequested(QPrinter*)),SLOT(IspisPreview(QPrinter*)));
    //connect(btnMail,SIGNAL(pressed()),this,SLOT(SaljiMejl()));
    connect(newAct,SIGNAL(triggered()),this,SLOT(PokaziSaljiMejl()));

    pw->setWindowFlags(Qt::Window);
    pw->exec();

    delete DokumentZaIspisA4;
}



void frmIspisOdabir::IzvrtiSQL(QDateTime DatumStart, QDateTime DatumEnd, int KasaID)
{


    //qDebug() << QThread::currentThreadId();
    QSqlQuery q("",QSqlDatabase::database("baza"));
    QString qveri = QString("drop table if exists iz2;create temporary table iz2 select r.rid,r1.np,r1.datumr,r.aid,(r.ukupno-(r.ukupno*r1.rabatp/100)) ukupno_po_rac,"
            " (r.bpdv-(r.bpdv*r1.rabatp/100)) bpdv_po_rac,ifnull((rp.iznos_poreza-(rp.iznos_poreza*r1.rabatp/100)),0) porez_po_rac, "
            " rp.porez_opis,((r.ukupno-(r.ukupno*r1.rabatp/100))-rd.dod)*ifnull(rp.koeficijent_poreza,0) porez2, "
            " ((r.ukupno-(r.ukupno*r1.rabatp/100))-rd.dod)-(((r.ukupno-(r.ukupno*r1.rabatp/100))-rd.dod)*ifnull(rp.koeficijent_poreza,0)) bpdv2, "
            " rd.dod from rac1 r1 left join rac2 r on r1.id=r.rid left join "
            " (select koeficijent_poreza,porez_opis,iznos_poreza,rid,aid,r2localid from rac2porezi group by rid,aid,r2localid) rp on "
            " (r.rid=rp.rid and r.aid=rp.aid and r.r2localid=rp.r2localid),"
            " (select rid,aid,sum(kol*povratnanaknada) dod from rac2dodatno group by rid,aid) rd  where r1.kasaid=%3 and "
            " r1.datumr between '%1' and '%2' and r.rid=rd.rid and r.aid=rd.aid group by r.rid,r.aid,r.r2localid").arg(DatumStart.toString("yyyy-MM-dd hh:mm:ss")).arg(DatumEnd.toString("yyyy-MM-dd hh:mm:ss")).arg(KasaID);
    //qDebug() << qveri;
    if (!q.exec(qveri))
        qDebug() << q.lastError() << q.lastQuery();

}

void frmIspisOdabir::SaljiMejl()
{
    if (txtMailTO->text().isEmpty())
    {
        QMessageBox::warning(this,"Slanje mail-a","Niste upisali kome da saljem","OK");
        return;
    }
    if (qApp->property("Mail-username").toString().length() < 1)
    {
        QMessageBox::warning(this,"Slanje mail-a","Nemate definiran mail racun\nMail racune definirate u opcijama","OK");
        return;
    }

    if (prnImeFajla.isEmpty())
    {
        QMessageBox::warning(this,"Greska","Nije napravljen PDF","OK");
        return;
    }

    prnPrinter->setOutputFormat(QPrinter::PdfFormat);
    prnPrinter->setOutputFileName(prnImeFajla);

    DokumentZaIspisA4->print(prnPrinter);
    QMessageBox::information(this,"Slanje mail-a","Saljem mejl","OK");

    Smtp* smtp = new Smtp(qApp->property("Mail-username").toString(), qApp->property("Mail-password").toString(),
                          qApp->property("Mail-server").toString(), qApp->property("Mail-port").toInt());
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
    QStringList files;
    files.append(prnPrinter->outputFileName());
    if (txtMailSubject->text().isEmpty())
        txtMailSubject->setText(QString("Slanje dokumenta %1").arg(prnImeFajla));
    if (txtMailPoruka->toPlainText().isEmpty())
        txtMailPoruka->setPlainText("U prilogu");
    smtp->sendMail(qApp->property("Mail-username").toString(), txtMailTO->text() , txtMailSubject->text(),txtMailPoruka->toPlainText(),files);

}

void frmIspisOdabir::PokaziSaljiMejl()
{
    mm = new QWidget(this);
    QGridLayout *gl = new QGridLayout();
    QPushButton *btnSalji = new QPushButton();
    QPushButton *btnIzlaz = new QPushButton();
    btnSalji->setText("Salji mejl");
    btnIzlaz->setText("Izlaz");
    QLabel *l1 = new QLabel();
    QLabel *l2 = new QLabel();
    QLabel *l3 = new QLabel();
    txtMailTO = new QLineEdit();
    txtMailSubject = new QLineEdit();
    txtMailPoruka = new QPlainTextEdit();

    connect(btnSalji,SIGNAL(released()),SLOT(SaljiMejl()));
    connect(btnSalji,SIGNAL(released()),mm,SLOT(close()));
    connect(btnIzlaz,SIGNAL(released()),mm,SLOT(close()));
    l1->setText("Kome saljemo :");
    l2->setText("Naslov poruke :");
    l3->setText("Poruka :");
    gl->addWidget(l1,0,0);
    gl->addWidget(txtMailTO,0,1);
    gl->addWidget(l2,1,0);
    gl->addWidget(txtMailSubject,1,1);
    gl->addWidget(l3,3,0,1,2);
    gl->addWidget(txtMailPoruka,4,0,1,2);
    gl->addWidget(btnSalji,5,0,1,2);
    gl->addWidget(btnIzlaz,6,0,1,2);
    mm->setWindowTitle("Podaci za slanje");
    mm->setLayout(gl);
    mm->setWindowFlags(Qt::Window);
    mm->setWindowModality(Qt::WindowModal);
    mm->show();
    mm->setFocus();

}

void frmIspisOdabir::mailSent(QString status)
{
    if(status == "Message sent")
    {
        QMessageBox::warning( 0, tr( "Slanje mail-a" ), tr( "Poruka poslana!\n\n" ) );
    }
}


void frmIspisOdabir::ispisPOS_poFile(QString PathDoDatoteke)
{

    QString Komanda = QString("lpr %1").arg(PathDoDatoteke);
    system(Komanda.toUtf8().constData());

    return;
    DokumentZaIspisA4 = new QTextDocument();

    QFile file(PathDoDatoteke);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString text = file.readAll();

    DokumentZaIspisA4->setPlainText(text);


    prnPrinter = new QPrinter(QPrinter::HighResolution);
    QPrintPreviewDialog pw(prnPrinter,this);
    connect(&pw,SIGNAL(paintRequested(QPrinter*)),SLOT(IspisPreview(QPrinter*)));

    int duljina = DokumentZaIspisA4->size().height();
    duljina = duljina/3.7;
    //prnPrinter->setPaperSize(QSizeF(80,duljina), QPrinter::Millimeter);
    //prnPrinter->setPageMargins(3,5,5,2,QPrinter::Millimeter);

    QList<QToolBar *> toolbarlist = pw.findChildren<QToolBar *>();

    //prnImeFajla = ;
    QAction *newAct = new QAction(this);
    newAct->setText("Posalji mail-om");
    newAct->setIcon(QIcon("ikone/mail.png"));
    newAct->setStatusTip(tr("Posalji dokument mail-om"));
    toolbarlist.last()->addAction(newAct);
    connect(newAct,SIGNAL(triggered()),this,SLOT(PokaziSaljiMejl()));
    pw.setWindowFlags(Qt::Window);

    int MarginaTop = 1;
    int MarginaBotton = 1;
    int MarginaLeft = 1;
    int MarginaRight = 1;

    if (!qApp->property("Printer-POS_RPT_MarginaTOP").toString().isNull())
        MarginaTop = qApp->property("Printer-POS_RPT_MarginaTOP").toInt();
    if (!qApp->property("Printer-POS_RPT_MarginaBotton").toString().isNull())
        MarginaBotton = qApp->property("Printer-POS_RPT_MarginaBotton").toInt();
    if (!qApp->property("Printer-POS_RPT_MarginaLeft").isNull())
        MarginaLeft = qApp->property("Printer-POS_RPT_MarginaLeft").toInt();
    if (!qApp->property("Printer-POS_RPT_MarginaRight").isNull())
        MarginaRight = qApp->property("Printer-POS_RPT_MarginaRight").toInt();

    if (qApp->property("Printer-POS_RPT_DirektanIspis").toString() == "1")
    {
        prnPrinter->setPageMargins(MarginaLeft,MarginaTop,MarginaRight,MarginaBotton,QPrinter::Millimeter);
        DokumentZaIspisA4->print(prnPrinter);
    }
    else
    {
        prnPrinter->setPageMargins(MarginaLeft,MarginaTop,MarginaRight,MarginaBotton,QPrinter::Millimeter);
        pw.exec();
    }
    //pw.exec();
    delete DokumentZaIspisA4;

}
QString frmIspisOdabir::ispisVratiHtmlContentNarudzba(int RID)
{
    if (RID <1)
        return "";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Greska-Ispis");
    msgBox.addButton(QMessageBox::Ok);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QFile htmlTemplate("rNarudzba.html");
    //QFile htmlTemplate("rNarudzbaT.html");
    if (!htmlTemplate.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        //QMessageBox::warning(QApplication.parent(),"Greska-ispis","Nije moguce naci template za ispis rnaloga","OK");
        msgBox.setText("Nije moguce naci template za ispis");
        msgBox.show();
        return "";
    }
    QString htmlContent;
    QTextStream in(&htmlTemplate);
    in.setCodec("utf-8");
    htmlContent = in.readAll();


/*
    htmlContent.replace("&lt;FIRMA_NAZIV&gt;",qApp->property("Firma_Ime").toString());
    htmlContent.replace("&lt;FIRMA_OIB&gt;",QString("OIB: %1").arg(qApp->property("Firma_OIB").toString()));
    htmlContent.replace("&lt;FIRMA_ADRESA&gt;",QString("%1,%2-%3").arg(qApp->property("Firma_Adresa").toString()).arg(qApp->property("Firma_Postanski").toString()).arg(qApp->property("Firma_Grad").toString()));
    htmlContent.replace("&lt;FIRMA_OSTALO&gt;",qApp->property("Firma_CB").toString());
    */
//    htmlContent.replace("&lt;FIRMA_MJESTO&gt;",QString("%1-%2"));
    QSqlQuery q("",QSqlDatabase::database("baza"));// QSqlDatabase::database("local.sqlite"));

    QString qveri;
    qveri = QString("select n.*,k.puninaziv from narudzba1 n left join korisnik k on n.narucio_uid=k.id  where n.id=%1").arg(RID);//.arg(NarID);
    if (!q.exec(qveri))
    {
        //QMessageBox::warning(null,"Greska-Ispisa","Greska kod dohvata podataka","OK");
        msgBox.setText("Greska dohvata podataka");
        msgBox.show();
        return "";
    }
    q.first();
    QString RowStavke;
    QRegExp exp;
    exp.setCaseSensitivity(Qt::CaseInsensitive);
    exp.setPattern("<tr name=\"tablica_stavke\" valign=\"top\">(.*)</tr name=\"tablica_stavke\">");
    RowStavke = htmlContent; //"<tr name=\"tablica_stavke\" easdasdasdasdasdasdasd</tr>"; //htmlContent;
    exp.indexIn(RowStavke);
    RowStavke = exp.cap(1);
    qDebug() << "RowSt: " << RowStavke;
    QString TablicaStavke;
    QString TablicaStavkeRow;
    int brojac = 0;

    /* ispis tko je narudzbu narucio
    q.first();
    QSqlQuery qkor("",QSqlDatabase::database("baza"));
    QString qverikor =QString("select * from korisnik where id=%1").arg(q.value(q.record().indexOf("narucio_uid")).toString());
    if (!qkor.exec(qverikor))
    {
        qDebug() << qkor.lastError() << qkor.lastQuery();
    }else
    {
        qkor.first();
        qDebug() << "Narucio:" << qkor.value(q.record().indexOf("puninaziv")).toString();
        htmlContent.replace("&lt;NARUDZBU_IZRADIO&gt;",qkor.value(q.record().indexOf("puninaziv")).toString());
    }
    */
    htmlContent.replace("&lt;Narudzba&gt;",QString("Narudzba br: %1").arg(q.value(q.record().indexOf("br")).toString()));


    htmlContent.replace("&lt;NARUDZBU_IZRADIO&gt;",q.value(q.record().indexOf("puninaziv")).toString());
    htmlContent.replace("&lt;DATUM_NARUDZBE&gt;",q.value(q.record().indexOf("datumn")).toString());
//    htmlContent.replace("&lt;Narudzba&gt;",QString("Narudzba br: %1").arg(q.value(q.record().indexOf("dok")).toString()));

    qveri = QString("select * from narudzba2 where narid=%2").arg(RID);//.arg(NarID);
    q.clear();

    q.exec(qveri);
    while (q.next())
    {
        //qDebug() << "Brojac :" << brojac;
        TablicaStavkeRow = RowStavke;

        //exp.setCaseSensitivity(Qt::CaseInsensitive);
        TablicaStavkeRow.prepend("<tr>");
        TablicaStavkeRow.replace("&lt;STAVKA_RB&gt;","");
        TablicaStavkeRow.replace("&lt;STAVKA_NAZIV&gt;",q.value(q.record().indexOf("naziv")).toString());
        TablicaStavkeRow.replace("&lt;STAVKA_KOLICINA&gt;",QString("%L1").arg(q.value(q.record().indexOf("kol")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;STAVKA_JM&gt;","KOM");
        /*
        TablicaStavkeRow.replace("&lt;STAVKA_JEDCJENA&gt;",QString("%L1").arg(q.value(q.record().indexOf("pc")).toDouble(),0,'f',2));
        if (q.value(q.record().indexOf("rabatk")).toDouble() != 0)
        {
            TablicaStavkeRow.replace("&lt;STAVKA_RBTP&gt;",QString("<br>** Rabat %L1% -- ").arg(q.value(q.record().indexOf("rabatp")).toDouble(),0,'f',2));
            TablicaStavkeRow.replace("&lt;STAVKA_RBTKN&gt;",QString("%L1 kn **").arg(q.value(q.record().indexOf("rabatk")).toDouble(),0,'f',2));
        }else
        {
            TablicaStavkeRow.replace("&lt;STAVKA_RBTP&gt;","");
            TablicaStavkeRow.replace("&lt;STAVKA_RBTKN&gt;","");
        }
        TablicaStavkeRow.replace("&lt;STAVKA_UKUPNO&gt;",QString("%L1").arg(q.value(q.record().indexOf("ukupno")).toDouble(),0,'f',2));
        TablicaStavkeRow.replace("&lt;STAVKA_RB&gt;","");
        */
        TablicaStavkeRow.replace("&lt;STAVKA_OPIS&gt;","");
        TablicaStavkeRow += "</tr>";
        TablicaStavke += TablicaStavkeRow;
        brojac++;
        //qDebug() << q.lastError() << q.lastQuery();
    }
    htmlContent.replace("<tr name=\"tablica_stavke\" valign=\"top\">","");
    htmlContent.replace("</tr name=\"tablica_stavke\">","");
    htmlContent.replace(RowStavke,TablicaStavke);

    //RowStavke = htmlContent;
    //exp.indexIn(RowStavke);
    //RowStavke = exp.cap(1);
//        qveri = "";
        //qDebug() << htmlContent;

    //qDebug() << htmlContent;
    return htmlContent;
}

void frmIspisOdabir::ispisQRcreate(int RID)
{

    //QRcode *qr = QRcode_encodeString("https://porezna.gov.hr/rn?jir=10e736a8-a088-41e8-add5-211328549a81&datv=20170913_1344&izn=501700",1,QR_ECLEVEL_L,QR_MODE_8,0);

    QSqlQuery q("",QSqlDatabase::database("baza"));

    QString qveri;
    qveri = QString("select v.* from v_ispis_rac1_1 v where r1id=%1").arg(RID);
    if (!q.exec(qveri))
    {
        QMessageBox::warning(this,"ERROR","Greska dohavata podataka");
    }
    q.next();
    QString r_jir = q.value(q.record().indexOf("jir")).toString();
    QString r_datv =  q.value(q.record().indexOf("datumr")).toDateTime().toString("yyyyMMdd_hhmm");
    QString r_izn = QString("%1").arg(q.value(q.record().indexOf("sum")).toDouble(),0,'f',2);

    QString url = QString("https://porezna.gov.hr/rn?jir=%1&datv=%2&izn=%3").arg(r_jir)
            .arg(r_datv).arg(r_izn.replace(",","").replace(".",""));

    QPixmap qrcode_pixmap;
    bool res;
    //QString text = "https://porezna.gov.hr/rn?jir=10e736a8-a088-41e8-add5-211328549a81&datv=20170913_1344&izn=501700";

    res = QRCodeGenerator::GeneratePixmapFromText(url,qrcode_pixmap,128,128);


    if (true == res){
        //ui->qrcodepixmapLabel->setPixmap(qrcode_pixmap);
        bool saveRes;
        QString PathQR_IMG = QString("%1/qrimg/%2/").arg(qApp->applicationDirPath()).arg(QDateTime::currentDateTime().toString("yyyy/MM/dd"));
        QString fileSave = QString("%1/qr-%2.png").arg(PathQR_IMG).arg(RID);
        //QString fileSave = QString("/tmp/ispQR.png");

        QDir dir(PathQR_IMG);
        if (!dir.exists())
            dir.mkpath(PathQR_IMG);

        if (QFileInfo(fileSave).exists()){
            qDebug() << "QR exists - removing ...";
            QFile::remove(fileSave);
        }



        saveRes = qrcode_pixmap.save(fileSave);
        if (true != saveRes )
        {
            qDebug() << "Greska QR saveRes";
            return;
        }


    } else {
        qDebug() << "Greska QR res";
        return;
    }

}
