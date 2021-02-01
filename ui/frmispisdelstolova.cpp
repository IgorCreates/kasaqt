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




#include "frmispisdelstolova.h"
#include "ui_frmispisdelstolova.h"
#include <QtSql>
#include "Ispis/ispismali.h"

frmIspisDelStolova::frmIspisDelStolova(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmIspisDelStolova)
{
    ui->setupUi(this);
    this->setWindowTitle("ispis stol");
    ui->txtDo->setDateTime(QDateTime::currentDateTime());
    ui->txtOd->setDateTime(QDateTime::currentDateTime());//QDateTime::fromString("01.01.2013 00:00:00","dd.MM.yyyy hh:mm:ss"));
}

frmIspisDelStolova::~frmIspisDelStolova()
{
    delete ui;
}

void frmIspisDelStolova::on_pushButton_pressed()
{
    QSqlDatabase dbL = QSqlDatabase::database("local.sqlite");
    dbL.open();
    QSqlQuery q("",dbL);
    if (!q.exec(QString("select naziv,sum(kol) kol,pc,sum(ukupno) ukupno from del_stol where datebrisano between '%1' and '%2' group by aid,pc")
           .arg(ui->txtOd->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(ui->txtDo->dateTime().toString("yyyy-MM-dd hh:mm:ss"))))
    {
        qDebug() << "Greska " << q.lastError();
        return;
    }
    QFile file("/tmp/ispDSStol.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);
    ispisMali *isp = new ispisMali();
    ispRac << QString("            ds-ukp\n");
    ispRac << "DS od " << ui->txtOd->dateTime().toString("dd.MM.yyyy hh:mm:ss") << "\n";
    ispRac << "DS do " << ui->txtDo->dateTime().toString("dd.MM.yyyy hh:mm:ss") << "\n";
    ispRac << QString(tr("Vrijeme ispisa: %1")).arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")) << "\n";
    ispRac << QString(tr("Konobar: %1")).arg(qApp->property("KorisnikIme").toString()) << "\n";
    ispRac << QString(tr("KasaID : ")) << qApp->property("Firma_OznNapUr").toString();

    double UkpSUMA=0;
    ispRac << "\n";
    ispRac << tr("Naziv artikla \n");
    ispRac << tr("      Kol                PC            UKP\n");
    ispRac << QString("=").repeated(isp->SirinaPapira);
    ispRac << "\n";
    while (q.next())
    {
        ispRac << q.value(0).toString();
        ispRac << "\n";
        ispRac << QString(" ").repeated(isp->VM(q.value(1).toString(),12)) << q.value(1).toString(); //kol
        ispRac << QString(" ").repeated(isp->VM(q.value(2).toString(),15)) << q.value(2).toString(); //pc
        ispRac << QString(" ").repeated(isp->VM(q.value(3).toString(),15)) << q.value(3).toString(); //ukupno
        UkpSUMA += q.value(3).toDouble();
        ispRac << "\n";
    }
    ispRac << QString("=").repeated(isp->SirinaPapira);
    ispRac << QString(tr("\nUkupno : %L1")).arg(UkpSUMA,0,'f',2);
    ispRac << "\n\n\n\n\n";
    file.close();
    QString Komanda;
    QString PrinterPort;
    if (!qApp->property("Printer-Port").isNull())
    {
        PrinterPort = qApp->property("Printer-Port").toString();
    }else
    {
        //QMessageBox::warning(qApp->desktop(),"Printer","Nemate definiran port za ispis\nPokusavam ispis na /dev/lp0","OK");
        PrinterPort = "/dev/lp0";
    }
    if (!qApp->property("Printer-Konverzija").isNull())
    {
        Komanda = QString("cat /tmp/ispDSStol.txt | iconv -f UTF8 -t %1 -o %2" ).arg(qApp->property("Printer-Konverzija").toString()).arg(PrinterPort);
    }else
    {
        Komanda= QString("cat /tmp/ispDSStol.txt >> %1").arg(PrinterPort);
    }
    system(Komanda.toUtf8().constData());
    q.finish();
    dbL.close();
    this->close();


}
