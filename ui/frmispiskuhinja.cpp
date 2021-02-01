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




#include "frmispiskuhinja.h"
#include "ui_frmispiskuhinja.h"
#include <QtGui>
#include <QtSql>
#include <QTextStream>
#include <QAction>
#include <Qt>
#include <QDebug>
#include "mojiheaderi.h"
#include "Ispis/ispismali.h"
#include <QResource>
#include <QLibrary>
#include <QMessageBox>



frmIspisKuhinja::frmIspisKuhinja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmIspisKuhinja)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("ispis kuhinja"));

}

frmIspisKuhinja::~frmIspisKuhinja()
{
    delete ui;
}


void frmIspisKuhinja::UcitajPostavke()
{
    //'IspisStol','Razmak'/'IspisStol','SamoHrana',1/'IspisStol','NeIspisIspisane,1
    if (!qApp->property("IspisStol-Razmak").isNull())
        ui->txtRazmak->setText(qApp->property("IspisStol-Razmak").toString());
    if (qApp->property("IspisStol-SamoHrana").toString() == "1")
    {
        ui->chkIspisHrana->setChecked(true);
    }else
    {
        ui->chkIspisHrana->setChecked(false);
    }
    if (qApp->property("IspisStol-NeIspisIspisane").toString() == "1")
    {
        ui->chkIspisNeIspisIspisane->setChecked(true);
    }else
    {
        ui->chkIspisNeIspisIspisane->setChecked(false);
    }
    if (qApp->property("IspisStol-SaCijenama").toString() == "1")
    {
        ui->chkIspisSaCijenama->setChecked(true);
    }else
    {
        ui->chkIspisSaCijenama->setChecked(false);
    }
    if (BrStola > 0)
    {
        ui->txtStolBr->setText(QString("%1").arg(BrStola));
    }
    ui->txtStolBr->selectAll();
    ui->txtStolBr->setFocus();
}

void frmIspisKuhinja::on_btnIspis_pressed()
{
    //BrStola = 0;
    BrStola = ui->txtStolBr->text().toInt();
    if (BrStola>0)
    {
        slotIspisStol(BrStola);
    }else {
        this->close();
    }
}

void frmIspisKuhinja::on_btnIzlaz_pressed()
{
    this->close();
}
void frmIspisKuhinja::slotIspisStol(int StolBR)
{

    if (StolBR == NULL || StolBR == 0)
        return;

    QFile file;
#ifdef Q_OS_LINUX
    file.setFileName("/tmp/ispStol.txt");
#endif
#ifdef Q_OS_WIN
    file.setFileName(QString("%1/ispStol.txt").arg(qApp->applicationDirPath()));
#endif

    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);
    ispisMali *isp = new ispisMali();
    QSqlDatabase dbLocal = QSqlDatabase::database("local.sqlite");
    QSqlQuery ql("",dbLocal);


    ispRac << QString(tr("Vrijeme ispisa: %1")).arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm")) << "\n";
    ispRac << QString(tr("Konobar: %1")).arg(qApp->property("KorisnikIme").toString()) << "\n";
    ispRac << QString(tr("Ispis Stol broj: %1")).arg(StolBR);
    ispRac << "\n";
    ispRac << tr("Naziv artikla              Kol");
    if (ui->chkIspisSaCijenama->isChecked())
        ispRac << tr(" Uk.Cijena");
    ispRac << "\n";
    ispRac << QString("=").repeated(40);
    ispRac << "\n";
    QString Razmak = "";
    QString mSQL = "";
    for (int i=0;i<ui->txtRazmak->text().toInt();i++)
    {
        Razmak +="\n";
    }

    if (ui->chkIspisNeIspisIspisane->isChecked())
    {
        mSQL = QString("select r2.naziv artnaziv,sum(r2.kol) r2kol,sum(r2.ukupno) ukupno,i.kol ikol,(sum(r2.kol)-i.kol) raz,r2.pc artPc "
                       "from rac2 r2 left join ispKuhinja i on r2.dok=i.dok and r2.aid=i.aid "
                       " where r2.dok=%1 ").arg(StolBR);
    }else
    {
        mSQL = QString("select r2.naziv artnaziv,sum(r2.kol) r2kol,sum(r2.ukupno) ukupno ,r2.pc artPc from rac2 r2 where r2.dok=%1 ").arg(StolBR);
    }
    if (ui->chkIspisHrana->isChecked())
    {
        mSQL += " and r2.ispis_kuhinja=1 ";
    }
    mSQL += " group by r2.aid,r2.pc order by r2.naziv";

    if (!ql.exec(mSQL))
        return;
    int Brojac = 0;
    int colArtNaziv = ql.record().indexOf("artnaziv");
    int colArtKol = ql.record().indexOf("r2kol");
    int colArtUkupno = ql.record().indexOf("ukupno");
    int colArtRazlika = ql.record().indexOf("raz");
    int colArtPC = ql.record().indexOf("artPc");
    double UkpSUMa = 0;
    while (ql.next())
    {
//         QString::number(q.value(1).toDouble(),"f",2);
  //QString(" ").repeated(VM(QString("%L1").arg(q.value(4).toDouble(),0,'f',2),9))  + QString("%L1").arg(q.value(4).toDouble(),0,'f',2)

        double IspKol = 0;
        if (ui->chkIspisNeIspisIspisane->isChecked())
        {
            if (!ql.value(colArtRazlika).isNull())
            {
                IspKol = ql.value(colArtRazlika).toDouble();
            }else
            {
                IspKol = ql.value(colArtKol).toDouble();
            }
        }else
        {
            IspKol = ql.value(colArtKol).toDouble();
        }
        if (!IspKol == 0 || !ui->chkIspisNeIspisIspisane->isChecked())
        {
            ispRac << ql.value(colArtNaziv).toString().mid(0,20);
            int nRaz =0;
            if (ql.value(colArtNaziv).toString().length() < 20)
            {
                nRaz = 20-ql.value(colArtNaziv).toString().length();
            }
            ispRac << QString(" ").repeated(nRaz);
            ispRac << " ";
            ispRac << QString(" ").repeated(isp->VM(QString("%1").arg(IspKol),5));
            ispRac << IspKol;
            if (ui->chkIspisSaCijenama->isChecked())
            {
                ispRac << " ";
                ispRac << ql.value(colArtPC).toString(); //QString(" ").repeated(isp->VM(QString("%1").arg(ql.value(colArtPC).toString()),5));
                //qDebug() << ql.value(colArtPC).toString();
                ispRac << QString(" ").repeated(isp->VM(QString("%1").arg(ql.value(colArtUkupno).toString()),8));
                ispRac << ql.value(colArtUkupno).toString();
                UkpSUMa += ql.value(colArtUkupno).toDouble();
            }
            ispRac << "\n";
            ispRac << Razmak;
            Brojac++;
        }
    }

    ispRac << QString("=").repeated(40);
    ispRac << QString(tr("\nUkupno: %L1")).arg(UkpSUMa,0,'f',2);
    ispRac << "\n\n\n\n\n\n\n\n";
    ispRac << isp->printerRezacPrefix;
    ispRac << isp->printerResetPrefix;
    file.close();
    if (Brojac<1)
    {
        this->close();
        return;
    }
    if (ui->chkIspisNeIspisIspisane->isChecked())
    {
        ql.exec(QString("delete from ispKuhinja where dok=%1").arg(BrStola));
        ql.exec(QString("insert into ispKuhinja (dok,aid,kol) select dok,aid,"
                        "sum(kol) from rac2 where dok=%1 group by aid").arg(BrStola));
    }

    isp->SaljiNaPrinter(file.fileName());


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
        Komanda = QString("cat /tmp/ispStol.txt | iconv -f UTF8 -t %1 -o %2" ).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
    }else
    {
        Komanda= QString("cat /tmp/ispStol.txt >> %1").arg(PrinterPort);
    }
    system(Komanda.toUtf8().constData());
    */

    dbLocal.close();
    this->close();

}


void frmIspisKuhinja::on_btnUpisIzmijene_pressed()
{
    QSqlDatabase dbLocal = QSqlDatabase::database("local.sqlite");
    QSqlQuery ql("",dbLocal);
    ql.exec("delete from opcije where sto='IspisStol'");
    //'IspisStol','Razmak'/'IspisStol','SamoHrana',1/'IspisStol','NeIspisIspisane,1

    QString mSQL = QString("insert into opcije (sto,sifra,value1) select 'IspisStol','Razmak',%1").arg(ui->txtRazmak->text());
    if (ui->chkIspisHrana->isChecked())
    {
        mSQL += " union all select 'IspisStol','SamoHrana',1";
        qApp->setProperty("IspisStol-SamoHrana","1");
    }else
    {
        qApp->setProperty("IspisStol-SamoHrana","");
    }
    if (ui->chkIspisNeIspisIspisane->isChecked())
    {
        mSQL += " union all select 'IspisStol','NeIspisIspisane',1";
        qApp->setProperty("IspisStol-NeIspisIspisane","1");
    }else
    {
        qApp->setProperty("IspisStol-NeIspisIspisane","");
    }
    if (ui->chkIspisSaCijenama->isChecked())
    {
        mSQL += " union all select 'IspisStol','SaCijenama',1";
        qApp->setProperty("IspisStol-SaCijenama","1");
    }else
    {
        //qApp->setProperty("IspisStol-SaCijenama","");
        qApp->property("IspisStol-SaCijenama").clear();
    }

    qApp->setProperty("IspisStol-Razmak",ui->txtRazmak->text());
    if (!ql.exec(mSQL))
        qDebug() << ql.lastError() << ql.lastQuery();
    QMessageBox::information(this,tr("Upis opcija"),tr("Upisano"),"OK");

}
