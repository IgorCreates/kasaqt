#include "ispismali.h"
#include "QString"
#include "QStringList"
#include "qsql.h"
#include <QtSql>
#include <QtScript/QScriptable>
#include <QDebug>
#include <QProcess>
#include <QtGui>
#include <QFile>
#include <QMessageBox>

ispisMali::ispisMali()
{
#ifdef Q_OS_LINUX
    PokretacJasper = "/opt/iReport/jasperstarter/bin/jasperstarter";
    PokretacJasperRunPath = "/opt/iReport/runjasperreports";
#endif
#ifdef Q_OS_WIN
    PokretacJasper = "jasperstarter";
    PokretacJasperRunPath = "c:\\opt\\iReport\\runjasperreports";
#endif
    UcitajPostavkePrintera();
    this->lenKolicina = 5;
    this->lenJedCijena = 6;
    this->lenUkpCijena = 7;
    //sirina predidena za naziv (ukupno - kolicina, cijena, ukupno i minus razmaci)
    //da bi dobili na prostoru nema razmaka izmedu naziva i kolicine
    this->lenSirNaziv = SirinaPapira - this->lenKolicina - this->lenJedCijena - this->lenUkpCijena - 2;

    //provjera parametara i popravak sirine naziva (samo ako su dva reda ce valjati)
    if (this->lenSirNaziv < 0)
    {
        this->lenSirNaziv = 0;
        this->SirinaPapira = this->lenKolicina + this->lenJedCijena + this->lenUkpCijena + 2;
    }
    this->bRacunStavkaJedanRed = (qApp->property("Printer-Redova").toString() == "1");
    if (this->lenSirNaziv < 12)
        this->bRacunStavkaJedanRed = false;
    InozemniRacun=false;
}


void ispisMali::ispisVeliki(const QString &ImeReporta, const QString &Parametri)
{
    QString Komanda=PokretacJasper;
    QStringList NK;
    NK << " pr" << "-t" << "mysql"  << "-H" << QSqlDatabase::database("baza").hostName() << "-u" << "kasauser" << "-p" << "KasaPasSwd";
    NK << "-n" << QSqlDatabase::database("baza").databaseName() << "-i" << QString("%1.jasper").arg(ImeReporta) << "-f" << "pdf";
    NK << "-P" << Parametri << "--jdbc-dir" << PokretacJasperRunPath << QString("-o %1/%2").arg(qApp->applicationDirPath()).arg(ImeReporta);

    QProcess zk1;
    zk1.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    zk1.setProcessChannelMode(QProcess::MergedChannels);
    zk1.start(Komanda.append(NK.join(" ")));
    if (!zk1.waitForStarted(10000))
    {
        qDebug() << "isp kom "  << Komanda.append(NK.join(" "));
        qDebug() << zk1.readAll() << zk1.readAllStandardOutput() << zk1.readAllStandardError() << zk1.readChannel();
        return ;
    }
    qDebug() << zk1.readAll() << zk1.readAllStandardOutput() << zk1.readAllStandardError() << zk1.readChannel();
    if (!zk1.waitForFinished(-1))
    {
        qDebug() << "isp kom "  << Komanda.append(NK.join(" "));
        qDebug() << zk1.readAll() << zk1.readAllStandardOutput() << zk1.readAllStandardError() << zk1.readChannel();
        return ;
    }
    qDebug() << zk1.readAll() << zk1.readAllStandardOutput() << zk1.readAllStandardError() << zk1.readChannel();
    qDebug() << "isp kom "  << Komanda.append(NK.join(" "));

#ifdef Q_OS_LINUX
    Komanda = QString("evince %1/%2.pdf").arg(qApp->applicationDirPath()).arg(ImeReporta);
#endif
#ifdef Q_OS_WIN
    Komanda = QString("%1/%2.pdf").arg(qApp->applicationDirPath()).arg(ImeReporta);
#endif

#ifdef Q_OS_LINUX
//    system(Komanda.toUtf8().constData());
    zk1.setReadChannel(QProcess::StandardOutput);
    zk1.setProcessChannelMode(QProcess::MergedChannels);
    zk1.start(Komanda);
    if (!zk1.waitForStarted(10000))
    {
        qDebug() << "Greska ispisVeliki ucitajPDF " << Komanda;
        qDebug() << zk1.readAll() << zk1.readAllStandardOutput() << zk1.readAllStandardError() << zk1.readChannel();
//        return ;
    }else
    {
        zk1.deleteLater();
    }
    if (!zk1.waitForFinished(-1))
    {
        qDebug() << "Greska ispisVeliki ucitajPDF";
        qDebug() << zk1.readAll() << zk1.readAllStandardOutput() << zk1.readAllStandardError() << zk1.readChannel();
    }
    //qDebug() << system(Komanda.toUtf8().constData());
#endif
#ifdef Q_OS_WIN
    QString pdfFajl = QString("/%1/%2.pdf").arg(qApp->applicationDirPath()).arg(ImeReporta);
    qDebug() << pdfFajl;
    QUrl pdfF = QUrl::fromLocalFile(pdfFajl);
    if (!QDesktopServices::openUrl(pdfF))
    {
        qDebug() << "Greska URL";
    }
#endif

}
void ispisMali::IspisRNalog(const int &RnalogID)
{
    QString ImeReporta = "isp_rNalog";
    QString ImeReportaPDF = ImeReporta;
    QString Parametri = QString("rid=integer:%1").arg(RnalogID);
    ImeReporta.prepend(QString("%1/").arg(qApp->applicationDirPath()));
#ifdef Q_OS_LINUX
    ImeReportaPDF.prepend("/tmp/");
#endif
    QString Komanda = QString("jasperstarter pr -t mysql -H %1 -u kasauser -p KasaPasSwd -n %2 -i %3.jasper"
                              " -f pdf -P %4 --jdbc-dir %5 -o %6").arg(QSqlDatabase::database("baza").hostName())
            .arg(QSqlDatabase::database("baza").databaseName()).arg(ImeReporta).arg(Parametri).arg(PokretacJasperRunPath).arg(ImeReportaPDF);
    //Komanda.prepend(PokretacJasper);
#ifdef Q_OS_LINUX
    Komanda.prepend("/opt/iReport/jasperstarter/bin/");
#endif
    qDebug() << Komanda;
    qDebug() << qApp->applicationDirPath();
    qDebug() << system(Komanda.toUtf8().constData());
#ifdef Q_OS_LINUX
    Komanda = QString("evince %1.pdf").arg(ImeReportaPDF);
#endif
#ifdef Q_OS_WIN
    Komanda = QString("%1.pdf").arg(ImeReportaPDF);
#endif

    qDebug() << Komanda;
    system(Komanda.toUtf8().constData());

/*
    QProcess pr;//= new QProcess();
    pr.start(Komanda);
    pr.close();
    */
}


QString ispisMali::NewH()
{
    /*
    QFile file("/tmp/ispNewT.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);
    */

    QSqlDatabase dbP = QSqlDatabase::database("local.sqlite");
    QSqlQuery q("",dbP);
    dbP.open();
    q.exec("select * from isppostavke where sto='header'");
    q.next();
    QString hh = q.value(1) .toString();
    if (hh.count("firma_naziv") >= 1 && hh.count("firma_oib")>=1)
    {
        hh.replace("firma_naziv",qApp->property("Firma_Ime").toString());
        if (InozemniRacun)
        {
            hh.replace("firma_oib",QString("OIB/VAT:HR%1").arg(qApp->property("Firma_OIB").toString()));
        }else
        {
            hh.replace("firma_oib",QString("OIB:%1").arg(qApp->property("Firma_OIB").toString()));
        }
        hh.replace("firma_adresa",qApp->property("Firma_Adresa").toString());
        hh.replace("firma_grad",qApp->property("Firma_Grad").toString());
        hh.replace("firma_zip",qApp->property("Firma_Postanski").toString());
        hh.replace("firma_cb",qApp->property("Firma_CB").toString());
        hh.replace("<b>",printerBoldPrefix);
        hh.replace("</b>",printerBoldSuffix);
    }else
    {
        hh = IspisHeader();
    }
    return hh;
    /*
    ispRac << hh;
    file.close();
    */
}
//void ispisMali::NewIsp(int BrRacuna)
void ispisMali::IspisMaliPos(const QString &BrRacuna)
{
    QString rc;
    QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
    QSqlDatabase dbB = QSqlDatabase::database("baza");
    QSqlQuery ql("",dbL);
    QSqlQuery q("",dbB);

    //QFile file("/tmp/ispNewT.txt");
    QFile file;
#ifdef Q_OS_LINUX
    //QFile file("/tmp/ispMali.txt");
    file.setFileName("/tmp/ispMali.txt");
#endif
#ifdef Q_OS_WIN
    //QFile file(QString("%1/ispMali.txt").arg(qApp->applicationDirPath()));
    file.setFileName(QString("%1/ispMali.txt").arg(qApp->applicationDirPath()));
    qDebug() << "PrintFile: " << QString(file.fileName());
#endif
    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);


    if (!q.exec(QString("select r1.*,k.naziv prodao,ka.naziv naziv_np from rac1 r1 left join korisnik k on r1.uid=k.id left join kartice ka on r1.np=ka.id where r1.id=%1").arg(BrRacuna)))
    {
        qDebug() << "Greska IspisMali - upit na bazu\n" << q.lastError();
        return;
    }
    if (q.next())
    {
        ql.exec("select * from isppostavke where sto='racun'");
        if (!ql.next())
        {
            qDebug() << "Greska isppostavke: " << ql.lastError() << ql.lastQuery();
            return;
        }
        rc = ql.value(1).toString();
        int qR1br = q.record().indexOf("br");
        int qR1oznPP = q.record().indexOf("oznPP");
        int qR1kasaID = q.record().indexOf("kasaid");
        int qR1datumr = q.record().indexOf("datumr");
        int qR1vrsta = q.record().indexOf("vrsta");
        int qR1idk = q.record().indexOf("idk");
        int qR1naziv_np = q.record().indexOf("naziv_np");
        int qR1prodao = q.record().indexOf("prodao");
        int qR1stol = q.record().indexOf("stol");
        int qR1bpdv = q.record().indexOf("bpdv");
        int qR1sum = q.record().indexOf("sum");
        int qR1rabatp = q.record().indexOf("rabatp");
        int qR1zki = q.record().indexOf("zki");

        QSqlQuery qC("",dbB);
        if (q.value(qR1vrsta).toString() == "V")
        {

            //if (qC.exec(QString("select naziv,adresa,oib,mjesto,countrycode from company where id='%1'").arg(q.value(qR1idk).toString())))
            if (qC.exec(QString("select c.naziv,c.adresa,c.oib,c.mjesto,c.countrycode,eu.countrylocal from company c left join euvatincountry eu on c.countrycode=eu.code where c.id='%1'").arg(q.value(qR1idk).toString())))
            {
                qC.next();
                rc.replace("kupac_ime",qC.value(0).toString());
                rc.replace("kupac_adresa",qC.value(1).toString());
                if ( qC.value(4).toString() != "HR" )
                {
                    rc.replace("kupac_oib",QString("VAT:%1%2").arg(qC.value(4).toString()).arg(qC.value(2).toString()));
                    rc.replace("kupac_mjesto",QString("%1\n%2").arg(qC.value(3).toString()).arg(qC.value(5).toString()));
                    InozemniRacun = true;
                }else{
                    rc.replace("kupac_oib",qC.value(2).toString());
                    rc.replace("kupac_mjesto",qC.value(3).toString());
                    InozemniRacun = false;
                }
                //rc.replace("kupac_mjesto",qC.value(3).toString());
            }else{
                rc.replace("kupac_ime","Kupac:");
                rc.replace("kupac_adresa","Adresa:");
                rc.replace("kupac_oib","Oib:");
                rc.replace("kupac_mjesto","Mjesto:");
            }
            rc.replace("rac_br",QString("%5 %1 %2/%3/%4").arg(qApp->property("Printer-IspisPoljeRacun").toString())
                       .arg(q.value(qR1br).toString()).arg(q.value(qR1oznPP).toString()).arg(q.value(qR1kasaID).toString())
                       .arg(qApp->property("Firma_Rtip").toString()));
        }else if (q.value(qR1vrsta).toString() == "M"){
            /*
            rc.replace("kupac_ime","");
            rc.replace("kupac_adresa","");
            rc.replace("kupac_oib","");
            */
            rc.replace(QRegExp(".*rac_br"),"\nrac_br");
            rc.replace("rac_br",QString("%1 %2/%3/%4").arg(qApp->property("Printer-IspisPoljeRacun").toString()).arg(q.value(qR1br).toString()).arg(q.value(qR1oznPP).toString()).arg(q.value(qR1kasaID).toString()));
        }

        rc.replace("rac_datum",QString("%1 %2").arg(qApp->property("Printer-IspisPoljeDatum").toString()).arg(q.value(qR1datumr).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        rc.replace("rac_prodao",QString("Osoblje:%1").arg(q.value(qR1prodao).toString()));
        if (qApp->property("List-Prikaz").toString() == "1")
        {
         //    rHeader += QString("Stol: %1 \n").arg(q.value(qR1stol).toString());
            rc.replace("rac_stol",QString("Stol: %1").arg(q.value(qR1stol).toString()));
        }
        rc.replace("rac_placeno",QString("Placeno:%1").arg(q.value(qR1naziv_np).toString()));
        QString rcPorezi;
        rcPorezi += QString("BPDV").leftJustified(7, ' ');
        rcPorezi += ":";
        rcPorezi += QString("%L1").arg(q.value(qR1bpdv).toDouble(),0,'f',2).rightJustified(15, ' ') + "\n";
        if (q.value(qR1rabatp).toDouble() > 0)
        {
            rcPorezi = QString("Rabat: %1 %").arg(q.value(qR1rabatp).toDouble(),0,'f',2);
            rcPorezi += "\n";
        }
        qC.exec(QString("select porez_sifra,iznos_poreza from rac1porezi where rid=%1").arg(BrRacuna));
        while (qC.next())
        {
            rcPorezi += QString("%1").arg(qC.value(0).toString()).leftJustified(7, ' ');
            rcPorezi += ":";
            rcPorezi += QString("%L1").arg(qC.value(1).toDouble(),0,'f',2).rightJustified(15, ' ') + "\n";
        }
        qC.exec(QString("select sum(kol*povratnanaknada) from rac2dodatno where rid=%1").arg(BrRacuna));
        if (qC.next())
        {
            if (qC.value(0).toDouble() != 0)
                rcPorezi += QString("Povratna naknada: %L1 kn\n").arg(qC.value(0).toDouble(),0,'f',2);
        }
        rc.replace("rac_porezi",rcPorezi);
        rc.replace("rac_za_platiti",QString("%1%2").arg(qApp->property("Printer-IspisPoljeZaPlatiti").toString())
                   .arg(QString(" ").repeated(VM(QString("%L1").arg(q.value(qR1sum).toDouble(),0,'f',2),9))  + QString("%L1").arg(q.value(qR1sum).toDouble(),0,'f',2)));

        //artikli
        QString artiklZaglavlje;
        if (this->bRacunStavkaJedanRed)
        {
            artiklZaglavlje = QString("Naziv").leftJustified(this->lenSirNaziv, ' ',true) + QString(" ")
                    + QString("Kol").leftJustified (this->lenKolicina - 1, ' ', true) + QString(" ")
                    + QString("J.CIJ.").leftJustified (this->lenJedCijena, ' ', true) + QString(" ")
                    + QString("Uk.Cij.").leftJustified (this->lenUkpCijena, ' ', true);
        }else
        {
            artiklZaglavlje = QString("Naziv").leftJustified(this->SirinaPapira, ' ',true) + "\n"
                    + QString(" ").repeated(this->lenSirNaziv) +  QString(" ")
                    + QString("Kol").leftJustified (this->lenKolicina - 1, ' ', true) + QString(" ")
                    + QString("J.CIJ.").leftJustified (this->lenJedCijena, ' ', true) + QString(" ")
                    + QString("Uk.Cij.").leftJustified (this->lenUkpCijena, ' ', true);
        }
        rc.replace("artikli_zaglavlje", artiklZaglavlje);

        //dohvat iz rac2 (sve stavke racuna)
        if (!qC.exec(QString("select * from rac2 where rid=%1").arg(BrRacuna)))
        {
            qDebug() << "Greska IspMali - upit na bazu\n" << q.lastError();
        }
        QString aDetail;
        int r2Naz = qC.record().indexOf("naziv");
        int r2Kol = qC.record().indexOf("kol");
        int r2Cij = qC.record().indexOf("pc");
        int r2Ukp = qC.record().indexOf("ukupno");
        int r2Rbt = qC.record().indexOf("rabatp");
        int r2RbtK = qC.record().indexOf("rabatk");
        int Brojac=0;
//        qDebug() << "*******Imaih:" << qC.size();
        while (qC.next())
        {
            //             if (!aDetail.isNull() || aDetail != "")
            //                 aDetail = "\n";

            if (Brojac>=1 && Brojac < qC.size())
                aDetail += "\n";
            QString artNaz = QString(qC.value(r2Naz).toString());
            //qDebug() << artNaz;
            if (this->bRacunStavkaJedanRed)
            {
                aDetail += QString(artNaz).leftJustified(this->lenSirNaziv, ' ', true);
            }else
            {
                aDetail += QString(artNaz).leftJustified(this->SirinaPapira, ' ', true)
                        + "\n" + QString(" ").repeated(this->lenSirNaziv);
            }
            aDetail += QString("%L1").arg(qC.value(r2Kol).toDouble(),0,'f',2).rightJustified(this->lenKolicina, ' ', true) + " ";  //kol
            aDetail += QString("%L1").arg(qC.value(r2Cij).toDouble(),0,'f',2).rightJustified(this->lenJedCijena, ' ', true) + " ";  //cijena
            aDetail += QString("%L1").arg(qC.value(r2Ukp).toDouble(),0,'f',2).rightJustified(this->lenUkpCijena, ' ', true) + "";  //ukupno
            if (qC.value(r2Rbt).toDouble() > 0)
            {
                aDetail += "\nRabat: " + qC.value(r2Rbt).toString() + "% ";
                aDetail += QString(" Rabat kn: %L1\n").arg(qC.value(r2RbtK).toDouble(),0,'f',2);
            }
            Brojac++;
         //ispRac << aDetail;
        }
        rc.replace("artikli",aDetail);
        rc.replace("rac_zki",QString("ZKI:%1").arg(q.value(qR1zki).toString()));
        
        if (qC.exec(QString("select odgovor from rac1fiskal where racid=%1 and potvrden=1").arg(BrRacuna)))
        {
            if (qC.next())
                rc.replace("rac_jir",QString("JIR:%1").arg(qC.value(0).toString()));
        }
        rc.replace("rac_jir","");
        rc.replace("<b>",printerBoldPrefix);
        rc.replace("</b>",printerBoldSuffix);

        rc.replace("rac_zahvala",qApp->property("Printer-IspisPoljeZahvala").toString());
        rc.replace("<z>=</z>",QString("=").repeated(SirinaPapira));
        rc.replace("<z>-</z>",QString("-").repeated(SirinaPapira));
        rc.replace("<z>_</z>",QString("_").repeated(SirinaPapira));
        rc.replace("<z>*</z>",QString("*").repeated(SirinaPapira));
        if (qApp->property("Firma_USustavuPDV").toString() != "true")
            rc.replace("rac_pdvobveznik","obveznik nije u sustavu pdv-a");
        if (qC.exec(QString("select opis from rac1opis where rid=%1").arg(BrRacuna)))
        {
            if (qC.next())
            {
                QString Napomena;
                Napomena = QString("\nNapomena:\n%1\n").arg(qC.value(0).toString());
                /*
                rc += "\nNapomena:\n";
                rc += qC.value(0).toString();
                rc += "\n";
                */
                rc.replace("rac_napomena",Napomena);
            }
        }
        rc.replace("rac_napomena","");
        ql.exec("select value1 from isppostavke where sto='redosljed'");
        int Redosljed;
        if (ql.next())
            Redosljed = ql.value(0).toInt();
        switch (Redosljed)
        {
            case 1:
            default:
                ispRac << NewH();
                ispRac << rc;
                break;
            case 2:
                ispRac << rc;
                ispRac << NewH();
            break;
        }
        if (qApp->property("Printer-Ladica").toString() == "1")
        {
            if (!printerLadicaPrefix.isNull())
            {
                ispRac << "\n";
                ispRac << printerLadicaPrefix;
            }
        }
        if (qApp->property("Printer-Rezac").toString() == "1")
        {
            if (!printerRezacPrefix.isNull())
            {
                ispRac << "\n";
                ispRac << printerRezacPrefix;
            }
        }
//        ispRac << printerResetPrefix;

        file.close();
        SaljiNaPrinter(file.fileName());

        /*
        QString Komanda;
        QString PrinterPort;
        if (!qApp->property("Printer-Port").isNull())
        {
            PrinterPort = qApp->property("Printer-Port").toString();
        }else
        {
            QMessageBox::warning(qApp->desktop(),"Printer","Nemate definiran port za ispis\nPokusavam ispis na /dev/lp0","OK");
            PrinterPort = "/dev/lp0";
        }
        if (!qApp->property("Printer-Konverzija").isNull())
        {
    //        qDebug() << qApp->property("Printer-Konverzija").toString();
#ifdef Q_OS_LINUX
            Komanda = QString("cat /tmp/ispMali.txt | iconv -f %1 -t %2 -o %3" ).arg(qApp->property("Printer-KonverzijaFrom").toString()).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
           // qDebug()<<Komanda;
#endif
#ifdef Q_OS_WIN
            if (qApp->property("Printer-WINkoristiPOSPrinting").toString() == "1")
            {
                Komanda = QString("POSPrinting").arg(PrinterPort);
            }else
            {
                //Komanda = QString("copy ispMali.txt %1").arg(PrinterPort);
                Komanda = QString("iconv -f %1 -t %2 ispMali.txt > %3" ).arg(qApp->property("Printer-KonverzijaFrom").toString()).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
            }

#endif
        }else

        {
#ifdef Q_OS_LINUX
            Komanda= QString("cat /tmp/ispMali.txt >> %1").arg(PrinterPort);
#endif
#ifdef Q_OS_WIN
            if (qApp->property("Printer-WINkoristiPOSPrinting").toString() == "1")
            {
                Komanda = QString("POSPrinting").arg(PrinterPort);
            }else
            {
                Komanda = QString("copy ispMali.txt %1").arg(PrinterPort);
            }

#endif

        }
        //system(QString("cat /tmp/ispMali.txt >> %1").arg(qApp.property("Printer-Port")toString());
        qDebug() << Komanda;
        qDebug() << system(Komanda.toUtf8().constData());
        */
    }
}


void ispisMali::IspisKrajDanaKorisnik(const QDateTime &DatumOd, const QDateTime &DatumDo, const int &KorisnikID, bool IspisiNaMali)
{
    if (DatumOd.isNull() || DatumDo.isNull())
        return;
    QString rDetail;
    rDetail += "\n";
    rDetail += QString("Datum od : %1 %2\n").arg(DatumOd.date().toString("dd.MM.yyyy")).arg(DatumOd.time().toString("hh:mm"));
    rDetail += QString("Datum do : %1 %2\n").arg(DatumDo.date().toString("dd.MM.yyyy")).arg(DatumDo.time().toString("hh:mm"));
    rDetail += QString("Korisnik : %1\n").arg(qApp->property("KorisnikIme").toString());
    rDetail += QString("Polog: %1\n").arg(qApp->property("Polog-").toString());
    rDetail += "NP            Iznos\n";
    rDetail += QString("=").repeated(SirinaPapira);
    rDetail +="\n";
    QSqlDatabase db = QSqlDatabase::database("baza");
    QSqlQuery q("",db);
    QString qveri = QString("select k.naziv as nacin_placanja,sum(sum) as suma,r1.kasaid from rac1 r1 left join  kartice k on r1.np=k.id where r1.kasaid=%1 and ").arg(qApp->property("Firma_OznNapUr").toString());
    qveri +=  QString("datumr between '%1' and '%2:59' ").arg(DatumOd.date().toString("yyyy-MM-dd") + " " +
             DatumOd.time().toString("hh:mm")).arg(DatumDo.date().toString("yyyy-MM-dd") + " " + DatumDo.time().toString("hh:mm"));

    if (KorisnikID != 0)
    {
        qveri += QString(" and r1.uid=%1").arg(KorisnikID);
    }
    qveri += " group by k.naziv";
    if (!q.exec(qveri))
    {
        qDebug()<< q.lastError();
        qDebug() << q.lastQuery();
        return;
    }
    while (q.next())
    {
        rDetail += q.value(0).toString();
        rDetail += QString(" ").repeated(VM(q.value(0).toString(),10));
        rDetail += QString(" ").repeated(VM(QString("%L1").arg(q.value(1).toDouble(),0,'f',2),15)) +
                QString("%L1\n").arg(q.value(1).toDouble(),0,'f',2) + "";
    }
    QString qveriSumUK = QString("select sum(sum) as suma from rac1 r1 left join  kartice k on r1.np=k.id where r1.kasaid=%1 and ").arg(qApp->property("Firma_OznNapUr").toString());
    qveriSumUK +=  QString("datumr between '%1' and '%2:59' ").arg(DatumOd.date().toString("yyyy-MM-dd") + " " +
             DatumOd.time().toString("hh:mm")).arg(DatumDo.date().toString("yyyy-MM-dd") + " " + DatumDo.time().toString("hh:mm"));
    if (KorisnikID != 0)
    {
        qveriSumUK += QString(" and r1.uid=%1").arg(KorisnikID);
    }
    if (!q.exec(qveriSumUK))
    {
        qDebug() << q.lastError();
        qDebug() << q.lastQuery();
    }
    while (q.next())
    {
        rDetail += QString("Ukupno : %L1\n").arg(q.value(0).toDouble(),0,'f',2);
    }
    qveri ="";
    qveri = QString("select rp.porez_sifra,sum(rp.iznos_poreza) suma from rac1porezi rp left join rac1 r1 on rp.rid=r1.id"
                    " where datumr between '%1' and '%2' and r1.kasaid=%3 group by rp.porez_sifra").arg(DatumOd.date().toString("yyyy-MM-dd") + " " + DatumOd.time().toString("hh:mm"))
            .arg(DatumDo.date().toString("yyyy-MM-dd") + " " + DatumDo.time().toString("hh:mm"))
            .arg(qApp->property("Firma_OznNapUr").toString());
    if (KorisnikID != 0)
    {
        qveri += QString(" and r1.uid=%1").arg(KorisnikID);
    }

    if (q.exec(qveri))
    {
        rDetail += QString("-").repeated(SirinaPapira);
        rDetail += "\n";
        while (q.next())
        {
            rDetail += q.value(0).toString();
            rDetail += QString(" ").repeated(VM(q.value(0).toString(),10));
            rDetail += QString(" ").repeated(VM(QString("%L1").arg(q.value(1).toDouble(),0,'f',2),15));
            rDetail += QString("%L1\n").arg(q.value(1).toDouble(),0,'f',2) + "";
        }
    }
    qveri = "";
    qveri = QString("select sum(kol*povratnanaknada) izn from rac1 r1 left join rac2dodatno r2 on r1.id=r2.rid "
                    " where datumr between '%1' and '%2' and r1.kasaid=%3").arg(DatumOd.date().toString("yyyy-MM-dd") + " " + DatumOd.time().toString("hh:mm"))
            .arg(DatumDo.date().toString("yyyy-MM-dd") + " " + DatumDo.time().toString("hh:mm"))
            .arg(qApp->property("Firma_OznNapUr").toString());
    if (KorisnikID != 0)
        qveri += QString(" and r1.uid=%1").arg(KorisnikID);
    qDebug() << qveri;
    if (q.exec(qveri))
    {
        rDetail += QString("-").repeated(SirinaPapira);
        rDetail += "\n";
        while (q.next())
        {
            rDetail += "Povratna naknada : ";
            rDetail += QString("%L1").arg(q.value(0).toDouble(),0,'f',2) + " \n";
        }
    }

    qveri ="";
    qveri = QString("select sum(iznos) izn from kasa_stanje where datum between '%1' and '%2:59' and kasa_id=%3").arg(DatumOd.toString("yyyy-MM-dd hh:mm"))
            .arg(DatumDo.toString("yyyy-MM-dd hh:mm")).arg(qApp->property("Firma_OznNapUr").toString());
    //qDebug() << "ISq:" << qveri;
    q.clear();

    if (KorisnikID != 0)
    {
        qveri += QString(" and korisnik_id=%1").arg(KorisnikID);
    }
    if (q.exec(qveri))
    {
        rDetail += QString("-").repeated(SirinaPapira);
        rDetail += "\n";
        while (q.next())
        {
            rDetail += QString("Pozajmica iz kase : %L1").arg(QString("%L1").arg(q.value(0).toDouble(),0,'f',2));
            //rDetail += VM(QString("%L1 \n").arg(q.value(0).toDouble(),0,'f',2),15);
//            qDebug() << q.value(0).toDouble();
//            rDetail += QString(" ").repeated(VM(QString("%L1").arg(q.value(0).toString(),0,'f',2),15));
            rDetail += "\n";
        }
    }
    rDetail += QString("=").repeated(SirinaPapira);
//    QFile file("/tmp/ispStanje1.txt");
    QFile file;
#ifdef Q_OS_LINUX
    file.setFileName("/tmp/ispStanje1.txt");
#endif
#ifdef Q_OS_WIN
    file.setFileName(QString("%1/ispStanje1.txt").arg(qApp->applicationDirPath()));
#endif

    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);
    ispRac << IspisHeader();
    ispRac << rDetail;
    ispRac << "\n\n\n\n\n\n";
    if (qApp->property("Printer-Rezac").toString() == "1")
    {
        if (!printerRezacPrefix.isNull())
            ispRac << printerRezacPrefix;
    }
   // ispRac << printerResetPrefix;



//    QByteArray ls = QByteArray::fromHex("1b27000d");
//    ispRac <<



    file.close();
    if (IspisiNaMali)
        SaljiNaPrinter(file.fileName());


}
void ispisMali::IspisStanjeKase(const QDateTime &DatumOd, const QDateTime &DatumDo)
{
    if (DatumOd.isNull() || DatumDo.isNull())
        return;
}

void ispisMali::UcitajPostavkePrintera()
{
    SirinaPapira = 36;
    if (!qApp->property("Printer-SirinaPapira").isNull())
    {
        SirinaPapira = qApp->property("Printer-SirinaPapira").toInt();
    }
    dbP = QSqlDatabase::database("baza");
    QSqlQuery q("",dbP);

    //tRezalica = QByteArray::fromHex(q.value(0).toByteArray());
    q.exec(QString("select prefix,prefix_nastavak,suffix,suffix_nastavak,funkcija from printer_postavke where prnid=%1").arg(qApp->property("Printer-Printer").toInt()));
    int colFunkcija = q.record().indexOf("funkcija");
    while (q.next())
    {
        if (q.value(colFunkcija).toString() == "ladica")
        {
            //QString PrinterKod = q.value(0).toString();

            /*
            QStringList PrinterKod = q.value(0).toString().split(",");
            QByteArray bbb;
            QTextStream ttt;

            for (int i=0;i< PrinterKod.size();++i)
            {

                qDebug() << "PrinterKod-" << PrinterKod.at(i);
                //printerLadicaPrefix += QString("0x%1").arg(PrinterKod.at(i));
                //bbb << char("\x1b\x70\x0\x1\x40");
               bbb.append(QByteArray::fromHex(PrinterKod.at(i).toAscii()));
            }
            printerLadicaPrefix = bbb;

            qDebug() << "PR";
            qDebug() << "CjeliKod" << printerLadicaPrefix << " BB:" << bbb;
            qDebug() << "END";
            printerLadicaPrefix += q.value(1).toString();

            */
            /*
            printerLadicaPrefix = QByteArray::fromHex(q.value(0).toByteArray());
            printerLadicaPrefix += q.value(1).toString();
            printerLadicaSuffix = QByteArray::fromHex(q.value(2).toByteArray());
            printerLadicaSuffix += q.value(3).toString();
            */


        }
        if (q.value(colFunkcija).toString() == "bold")
        {
            printerBoldPrefix = QByteArray::fromHex(q.value(0).toByteArray());
            printerBoldPrefix += q.value(1).toString();
            printerBoldSuffix = QByteArray::fromHex(q.value(2).toByteArray());
            printerBoldSuffix += q.value(3).toString();
        }
        if (q.value(colFunkcija).toString() == "rezac")
        {
            printerRezacPrefix = QByteArray::fromHex(q.value(0).toByteArray());
            printerRezacPrefix += q.value(1).toString();
            printerRezacSuffix = QByteArray::fromHex(q.value(2).toByteArray());
            printerRezacSuffix += q.value(3).toString();
        }
        /*
        if (q.value(colFunkcija).toString() == "reset")
        {
            printerResetPrefix = QByteArray::fromHex(q.value(0).toByteArray());
            printerResetPrefix += q.value(1).toString();
            printerResetSuffix = QByteArray::fromHex(q.value(2).toByteArray());
            printerResetSuffix += q.value(3).toString();
        }
        */
    }
}

QString ispisMali::IspisHeader()
{
    QString rHeader;
    rHeader = "";
    rHeader += qApp->property("Firma_Ime").toString() + "\n";
    rHeader += qApp->property("Firma_Adresa").toString() + ", ";
    rHeader += qApp->property("Firma_Grad").toString();
    rHeader += " " + qApp->property("Firma_Postanski").toString() + "\n";
    rHeader += "OiB:" + qApp->property("Firma_OIB").toString() + "\n";
    rHeader += qApp->property("Firma_CB").toString() + "";
    rHeader += "\n";
    return rHeader;
}

int ispisMali::VM(const QString &Polje, const int &PredvidenaDuljina)
{
    int DuljinaPolja = QString(Polje).length();
    if (PredvidenaDuljina <= DuljinaPolja)
    {
        return 0;
    }else {
        //qDebug() << "VM-" << PredvidenaDuljina << "-dulj-" << DuljinaPolja;
        return  (PredvidenaDuljina - DuljinaPolja);
    }
}


void ispisMali::IspisMaliPosVrac(const QString &BrRacuna)
{
    QString rc;
    QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
    QSqlDatabase dbB = QSqlDatabase::database("baza");
    QSqlQuery ql("",dbL);
    QSqlQuery q("",dbB);

    //QFile file("/tmp/ispNewT.txt");

    QFile file;
#ifdef Q_OS_LINUX
    file.setFileName("/tmp/ispMali.txt");
#endif
#ifdef Q_OS_WIN
    file.setFileName(QString("%1/ispMali.txt").arg(qApp->applicationDirPath()));
#endif
    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);


    if (!q.exec(QString("select r1.*,k.naziv prodao,ka.naziv naziv_np from vrac1 r1 left join korisnik k on r1.uid=k.id left join kartice ka on r1.np=ka.id where r1.id=%1").arg(BrRacuna)))
    {
        qDebug() << "Greska IspisMali - upit na bazu\n" << q.lastError();
        return;
    }
    if (q.next())
    {
        ql.exec("select * from isppostavke where sto='racun'");
        if (!ql.next())
            return;
        rc = ql.value(1).toString();
        int qR1br = q.record().indexOf("br");
        int qR1oznPP = q.record().indexOf("oznPP");
        int qR1kasaID = q.record().indexOf("kasaid");
        int qR1datumr = q.record().indexOf("datumr");
        int qR1vrsta = q.record().indexOf("vrsta");
        int qR1idk = q.record().indexOf("idk");
        int qR1naziv_np = q.record().indexOf("naziv_np");
        int qR1prodao = q.record().indexOf("prodao");
        int qR1stol = q.record().indexOf("stol");
        int qR1bpdv = q.record().indexOf("bpdv");
        int qR1sum = q.record().indexOf("sum");
        int qR1rabatp = q.record().indexOf("rabatp");
        int qR1zki = q.record().indexOf("zki");

        QSqlQuery qC("",dbB);
        if (q.value(qR1vrsta).toString() == "V" || q.value(qR1vrsta).toString() == "O")
        {

            if (qC.exec(QString("select naziv,adresa,oib,mjesto from company where id='%1'").arg(q.value(qR1idk).toString())))
            {
                qC.next();
                rc.replace("kupac_ime",qC.value(0).toString());
                rc.replace("kupac_adresa",qC.value(1).toString());
                rc.replace("kupac_oib",qC.value(2).toString());
                rc.replace("kupac_mjesto",qC.value(3).toString());
            }else{
                rc.replace("kupac_ime","Kupac:");
                rc.replace("kupac_adresa","Adresa:");
                rc.replace("kupac_oib","Oib:");
                rc.replace("kupac_mjesto","Mjesto:");
            }
            /*
            rc.replace("rac_br",QString("%5 %1 %2/%3/%4").arg(qApp->property("Printer-IspisPoljeRacun").toString())
                       .arg(q.value(qR1br).toString()).arg(q.value(qR1oznPP).toString()).arg(q.value(qR1kasaID).toString())
                       .arg(qApp->property("Firma_Rtip").toString()));
                       */
            rc.replace("rac_br",QString("Otpremnica br: %1").arg(q.value(qR1br).toString()));
        }else if (q.value(qR1vrsta).toString() == "M"){
            /*
            rc.replace("kupac_ime","");
            rc.replace("kupac_adresa","");
            rc.replace("kupac_oib","");
            */
            rc.replace(QRegExp(".*rac_br"),"\nrac_br");
            rc.replace("rac_br",QString("%1 %2/%3/%4").arg(qApp->property("Printer-IspisPoljeRacun").toString()).arg(q.value(qR1br).toString()).arg(q.value(qR1oznPP).toString()).arg(q.value(qR1kasaID).toString()));
        }

        rc.replace("rac_datum",QString("%1 %2").arg(qApp->property("Printer-IspisPoljeDatum").toString()).arg(q.value(qR1datumr).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        rc.replace("rac_prodao",QString("Osoblje:%1").arg(q.value(qR1prodao).toString()));
        if (qApp->property("List-Prikaz").toString() == "1")
        {
         //    rHeader += QString("Stol: %1 \n").arg(q.value(qR1stol).toString());
            rc.replace("rac_stol",QString("Stol: %1").arg(q.value(qR1stol).toString()));
        }
        //rc.replace("rac_placeno",QString("Placeno:%1").arg(q.value(qR1naziv_np).toString()));
        rc.replace("rac_placeno","");
        QString rcPorezi;
//        rcPorezi =  "\n" + QString("-").repeated(SirinaPapira) + "\n";
        rcPorezi += "BPDV";
        rcPorezi += QString(" ").repeated(VM("BPDV",7));
        rcPorezi += ":";
        rcPorezi += QString(" ").repeated(VM(QString("%L1").arg(q.value(qR1bpdv).toDouble(),0,'f',2),15)) + QString("%L1").arg(q.value(qR1bpdv).toDouble(),0,'f',2) + "\n";
        if (q.value(qR1rabatp).toDouble() > 0)
        {
            rcPorezi = QString("Rabat: %1 %").arg(q.value(qR1rabatp).toDouble(),0,'f',2);
            rcPorezi += "\n";
        }
        qC.exec(QString("select porez_sifra,iznos_poreza from vrac1porezi where rid=%1").arg(BrRacuna));
        while (qC.next())
        {
            rcPorezi += QString("%1").arg(qC.value(0).toString());
            rcPorezi += QString(" ").repeated(VM(qC.value(0).toString(),7));
            rcPorezi += ":";
            rcPorezi += QString(" ").repeated(VM(QString("%L1").arg(qC.value(1).toDouble(),0,'f',2),15)) + QString("%L1").arg(qC.value(1).toDouble(),0,'f',2) + "\n";
        }

        rc.replace("rac_porezi",rcPorezi);
        rc.replace("rac_za_platiti",QString("%1%2").arg(qApp->property("Printer-IspisPoljeZaPlatiti").toString())
                   .arg(QString(" ").repeated(VM(QString("%L1").arg(q.value(qR1sum).toDouble(),0,'f',2),9))  + QString("%L1").arg(q.value(qR1sum).toDouble(),0,'f',2)));
        //artikli
        if (qApp->property("Printer-Redova").toString() == "1")
        {
            rc.replace("artikli_zaglavlje","Naziv                   Kol J.CIJ. Uk.Cij.");
        }else
        {
            rc.replace("artikli_zaglavlje","Naziv\n Kol    j.CIJ  Uk.Cij.");
        }
        if (!qC.exec(QString("select * from vrac2 where rid=%1").arg(BrRacuna)))
        {
            qDebug() << "Greska IspMali - upit na bazu\n" << q.lastError();
        }
        QString aDetail;
        int Brojac=0;
        int qR2naziv = qC.record().indexOf("naziv");
        int qR2kol = qC.record().indexOf("kol");
        int qR2pc = qC.record().indexOf("pc");
        int qR2ukupno = qC.record().indexOf("ukupno");
        int qR2rabatp = qC.record().indexOf("rabatp");
        int qR2rabatk = qC.record().indexOf("rabatk");
        while (qC.next())
        {

            //             if (!aDetail.isNull() || aDetail != "")
            //                 aDetail = "\n";
            if (Brojac>=1 && Brojac < qC.record().count())
                aDetail += "\n";
            QString artNaz = QString(qC.value(qR2naziv).toString()).left(21);
            //qDebug() << artNaz.left(24);
            aDetail += artNaz;// + "\n"; //naziv prvi red
            if (qApp->property("Printer-Redova").toString() == "1")
            {
                aDetail += QString(" ").repeated(VM(artNaz,21));
            }else
            {
                aDetail += "\n";
            }
            //rDetail += QString(" ").repeated(VM(QString("%L1").arg(q.value(2).toDouble(),0,'f',2),6)) +  QString("%L1").arg(q.value(2).toDouble(),0,'f',2) + " ";  //kol
            aDetail += QString(" ").repeated(VM(QString("%L1").arg(qC.value(qR2kol).toDouble(),0,'f',2),6)) +  QString("%L1").arg(qC.value(qR2kol).toDouble(),0,'f',2).left(6) + " ";  //kol
            aDetail += QString(" ").repeated(VM(QString("%L1").arg(qC.value(qR2pc).toDouble(),0,'f',2),6)) +  QString("%L1").arg(qC.value(qR2pc).toDouble(),0,'f',2).left(6) + " ";  //cijena
            aDetail += QString(" ").repeated(VM(QString("%L1").arg(qC.value(qR2ukupno).toDouble(),0,'f',2),7)) +  QString("%L1").arg(qC.value(qR2ukupno).toDouble(),0,'f',2).left(6) + "";  //ukupno
            if (qC.value(qR2rabatp).toDouble() > 0)
            {
                aDetail += "\nRabat: " + qC.value(qR2rabatp).toString() + "% BezRabat " + qC.value(qR2rabatk).toString() + "\n";
            }
            Brojac++;
         //ispRac << aDetail;
        }
        rc.replace("artikli",aDetail);
        //rc.replace("rac_zki",QString("ZKI:%1").arg(q.value(qR1zki).toString()));
        rc.replace("rac_zki","");
/*
        if (qC.exec(QString("select odgovor from rac1fiskal where racid=%1 and potvrden=1").arg(BrRacuna)))
        {
            if (qC.next())
                rc.replace("rac_jir",QString("JIR: %1").arg(qC.value(0).toString()));
        }
        */
        rc.replace("rac_jir","");
        rc.replace("<b>",printerBoldPrefix);
        rc.replace("</b>",printerBoldSuffix);

        rc.replace("rac_zahvala",qApp->property("Printer-IspisPoljeZahvala").toString());
        rc.replace("<z>=</z>",QString("=").repeated(SirinaPapira));
        rc.replace("<z>-</z>",QString("-").repeated(SirinaPapira));
        rc.replace("<z>_</z>",QString("_").repeated(SirinaPapira));
        rc.replace("<z>*</z>",QString("*").repeated(SirinaPapira));

        if (qC.exec(QString("select opis from vrac1opis where rid=%1").arg(BrRacuna)))
        {
            if (qC.next())
            {
                QString Napomena;
                Napomena = QString("\nNapomena:\n%1\n").arg(qC.value(0).toString());
                rc.replace("rac_napomena",Napomena);
            }
        }
        rc.replace("rac_napomena","");



        if (qApp->property("Firma_USustavuPDV").toString() != "true")
            rc.replace("rac_pdvobveznik","obveznik nije u sustavu pdv-a");

        ql.exec("select value1 from isppostavke where sto='redosljed'");
        int Redosljed;
        if (ql.next())
            Redosljed = ql.value(0).toInt();
        switch (Redosljed)
        {
            case 1:
            default:
                ispRac << NewH();
                ispRac << rc;
                break;
            case 2:
                ispRac << rc;
                ispRac << NewH();
            break;
        }
        if (qApp->property("Printer-Ladica").toString() == "1")
        {
            if (!printerLadicaPrefix.isNull())
            {
                ispRac << "\n";
                ispRac << printerLadicaPrefix;
            }
        }
        if (qApp->property("Printer-Rezac").toString() == "1")
        {
            if (!printerRezacPrefix.isNull())
            {
                ispRac << "\n";
                ispRac << printerRezacPrefix;
            }
        }
//        ispRac << printerResetPrefix;

        file.close();
        qDebug() << file.fileName();
        SaljiNaPrinter(file.fileName());

        /*
        QString Komanda;
        QString PrinterPort;
        if (!qApp->property("Printer-Port").isNull())
        {
            PrinterPort = qApp->property("Printer-Port").toString();
        }else
        {
            QMessageBox::warning(qApp->desktop(),"Printer","Nemate definiran port za ispis\nPokusavam ispis na /dev/lp0","OK");
            PrinterPort = "/dev/lp0";
        }
        if (!qApp->property("Printer-Konverzija").isNull())
        {
    //        qDebug() << qApp->property("Printer-Konverzija").toString();
#ifdef Q_OS_LINUX
            //Komanda = QString("cat /tmp/ispMali.txt | iconv -f UTF8 -t %1 -o %2" ).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
            Komanda = QString("cat /tmp/ispMali.txt | iconv -f %1 -t %2 -o %3" ).arg(qApp->property("Printer-KonverzijaFrom").toString()).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
#endif
#ifdef Q_OS_WIN
            if (qApp->property("Printer-WINkoristiPOSPrinting").toString() == "1")
            {
                Komanda = QString("POSPrinting").arg(PrinterPort);
            }else
            {
                //Komanda = QString("copy ispMali.txt %1").arg(PrinterPort);
                Komanda = QString("iconv -f %1 -t %2 ispMali.txt > %3" ).arg(qApp->property("Printer-KonverzijaFrom").toString()).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
            }
#endif
        }else
        {
#ifdef Q_OS_LINUX
            Komanda= QString("cat /tmp/ispMali.txt >> %1").arg(PrinterPort);
#endif
#ifdef Q_OS_WIN
//            Komanda = QString("copy ispMali.txt %1").arg(PrinterPort);
            if (qApp->property("Printer-WINkoristiPOSPrinting").toString() == "1")
            {
                Komanda = QString("POSPrinting").arg(PrinterPort);
            }else
            {
                Komanda = QString("copy ispMali.txt %1").arg(PrinterPort);
            }
#endif
        }
        //system(QString("cat /tmp/ispMali.txt >> %1").arg(qApp.property("Printer-Port")toString());
        system(Komanda.toUtf8().constData());
        */
    }
}


void ispisMali::SaljiNaPrinter(QString FileZaIspis)
{
    QString Komanda;
    QString PrinterPort;
    if (!qApp->property("Printer-Port").isNull())
    {
        PrinterPort = qApp->property("Printer-Port").toString();
    }else
    {
        QMessageBox::warning(NULL,"Printer","Nemate definiran port za ispis\nPokusavam ispis na /dev/lp0","OK");

        PrinterPort = "/dev/lp0";
    }
    if (!qApp->property("Printer-Konverzija").isNull())
    {
#ifdef Q_OS_LINUX
        Komanda = QString("cat %1 | iconv -f %2 -t %3 -o %4" ).arg(FileZaIspis).arg(qApp->property("Printer-KonverzijaFrom").toString()).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
       // qDebug()<<Komanda;
#endif
#ifdef Q_OS_WIN
        if (qApp->property("Printer-WINkoristiPOSPrinting").toString() == "1")
        {
            Komanda = QString("POSPrinting -f%1").arg(FileZaIspis);
        }else
        {
            //Komanda = QString("copy ispMali.txt %1").arg(PrinterPort);
            Komanda = QString("iconv -f %1 -t %2 %4 > %3" ).arg(qApp->property("Printer-KonverzijaFrom").toString()).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort).arg(FileZaIspis);
        }
#endif
    }else
    {
#ifdef Q_OS_X11
        Komanda= QString("cat %1 >> %2").arg(FileZaIspis).arg(PrinterPort);
#endif
#ifdef Q_OS_WIN
        if (qApp->property("Printer-WINkoristiPOSPrinting").toString() == "1")
        {
            Komanda = QString("POSPrinting -f%1").arg(FileZaIspis);
        }else
        {
            Komanda = QString("copy %1 %2").arg(FileZaIspis).arg(PrinterPort);
        }

#endif

    }
    //system(QString("cat /tmp/ispMali.txt >> %1").arg(qApp.property("Printer-Port")toString());
    qDebug() << Komanda;
    qDebug() << system(Komanda.toUtf8().constData());

}

