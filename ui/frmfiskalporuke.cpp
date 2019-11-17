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




#include "frmfiskalporuke.h"
#include "ui_frmfiskalporuke.h"
#include <QtGui>
#include "ui/mojiheaderi.h"
#include <QtSql>
#include <QMessageBox>

frmFiskalPoruke::frmFiskalPoruke(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmFiskalPoruke)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("fiskal poruke"));
    ui->groupBox->setVisible(true);
    ui->groupBox_2->setVisible(false);
    ui->txtOznPoslProstor->setText(qApp->property("Firma_OznPosPr").toString());
    connect(ui->radioButton,SIGNAL(pressed()),this,SLOT(slotRadio1()));
    connect(ui->radioButton_2,SIGNAL(pressed()),this,SLOT(slotRadio2()));
    UcitajPolja();
    ui->txtOznPoslProstor->setEnabled(false);
    ui->txtSpecNamj->setText(qApp->property("Firma_OIB").toString());
}

frmFiskalPoruke::~frmFiskalPoruke()
{
    delete ui;
}


void frmFiskalPoruke::slotRadio1()
{
    ui->groupBox->setVisible(true);
    ui->groupBox_2->setVisible(false);
}
void frmFiskalPoruke::slotRadio2()
{
    ui->groupBox->setVisible(false);
    ui->groupBox_2->setVisible(true);
    ui->groupBox_2->move(ui->groupBox->pos().x(),ui->groupBox->pos().y());
}

void frmFiskalPoruke::on_pushButton_pressed()
{
    bool Adresa = false;
    if (ui->radioButton->isChecked() &&  (ui->txtBrojPoste->text() == "" || ui->txtKucniBroj->text() == "" || ui->txtNaselje->text() ==""
            || ui->txtOznPoslProstor->text() == "" || ui->txtRadnoVrijeme->text() == "" || ui->txtUlica->text() == ""))
    {
        QMessageBox::warning(this,tr("Paznja"),tr("Niste unijeli sve potrebne podatke"),"OK");
        return;
    }


    if (ui->radioButton_2->isChecked() && ui->txtOstaliTipoviPP->toPlainText() == "")
    {
        QMessageBox::warning(this,tr("Paznja"),tr("Niste unijeli sve potrebne podatke"),"OK");
        return;
    }
    if (ui->radioButton->isChecked())
    {
        Adresa = true;
        ui->txtOstaliTipoviPP->setPlainText("");
    }
    if (ui->radioButton_2->isChecked())
    {
        ui->txtBrojPoste->setText("");
        ui->txtUlica->setText("");
        ui->txtKucniBroj->setText("");
        ui->txtKucniBrojDodatak->setText("");
        ui->txtNaselje->setText("");
        ui->txtOpcina->setText("");
    }
    mojiHeaderi xm;
    QString xml;
    xml = xm.xmlPoslProstorZahtjev(qApp->property("Firma_OIB").toString(),qApp->property("Firma_OznPosPr").toString(),ui->txtUlica->text()
                             ,ui->txtKucniBroj->text(),ui->txtKucniBrojDodatak->text(),ui->txtBrojPoste->text(),ui->txtNaselje->text(),
                             ui->txtOpcina->text(),ui->txtRadnoVrijeme->text(),ui->txtDatumPocetkaPrimijene->text(),ui->txtSpecNamj->text(),
                             ui->txtOstaliTipoviPP->toPlainText(),ui->txtOznakaZatvaranja->text(),Adresa);
    //qDebug() << xml;
    QString vr= QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    QString ImeXMLFajla = QString("xml/xmlPPZ_%1.xml").arg(vr);
#ifdef Q_WS_WIN
    ImeXMLFajla.prepend(QString("%1/").arg(qApp->applicationDirPath()));
#endif
    QFile file(ImeXMLFajla);
    file.open(QIODevice::WriteOnly);
    QTextStream ispRac(&file);
    ispRac << xml;
    file.close();

    //system(Komanda.toUtf8().constData());
    //xmlsec1 --sign --id-attr:Id 'http://www.apis-it.hr/fin/2012/types/f73':RacunZahtjev --pkcs12 mycert2.pfx --pwd 1 --output  O.xml /tmp/xmlRacZahtjerv.xml
    QString ImeXML_sign_Fajla = QString("xml/xmlPPZ_%1_sign.xml").arg(vr);
    QString Komanda;
#ifdef Q_WS_WIN
    ImeXML_sign_Fajla.prepend(QString("%1/").arg(qApp->applicationDirPath()));
#endif
   //qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString()
#ifdef Q_WS_X11
    Komanda = QString("xmlsec1 --sign --id-attr:Id 'http://www.apis-it.hr/fin/2012/types/f73':PoslovniProstorZahtjev "
                              "--pkcs12 %3 --pwd %4 --output  %2 %1").arg(ImeXMLFajla).arg(ImeXML_sign_Fajla)
            .arg(qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString());
#endif
#ifdef Q_WS_WIN
    Komanda = QString("xmlsec1 --sign --id-attr:Id PoslovniProstorZahtjev "
                              "--pkcs12 %5/%3 --pwd %4 --output  %2 %1").arg(ImeXMLFajla).arg(ImeXML_sign_Fajla)
            .arg(qApp->property("Certs_Path").toString()).arg(qApp->property("Certs_Sifra").toString()).arg(qApp->applicationDirPath());
#endif
    qDebug() << Komanda;
    qDebug() <<  system(Komanda.toUtf8().constData());
/*
#ifdef Q_WS_X11
    //Komanda = QString("PERL_LWP_SSL_VERIFY_HOSTNAME=0 perl p.pl %1 >> %1.povratni").arg(ImeXML_sign_Fajla);
    Komanda = QString("python p.py %1 >> %1.povratni").arg(ImeXML_sign_Fajla);
#endif
#ifdef Q_WS_WIN
    Komanda = QString("%2/bin/p.bat %1 >> %1.povratni").arg(ImeXML_sign_Fajla).arg(qApp->applicationDirPath());
#endif
*/


    Komanda = QString("./p.py -m %1 >> %1.povratni").arg(ImeXML_sign_Fajla);
    qDebug() << "command:" <<  Komanda;
    qDebug() << "output: " << system(Komanda.toUtf8().constData());
    QMessageBox::information(this,tr("PP"),QString(tr("Poruka poslana\nodgovor je snimljen u %1.povratni")).arg(ImeXML_sign_Fajla),"OK");
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    q.exec("truncate fiskaloznpp");
    qDebug() << ui->txtDatumPocetkaPrimijene->text();
    qDebug() << QDate::fromString(ui->txtDatumPocetkaPrimijene->text(),"dd.MM.yyyy").toString("yyyy-MM-dd");
    qDebug() << "DalValja: " << QDate::fromString(ui->txtDatumPocetkaPrimijene->text(),"dd.MM.yyyy").isValid();
    qDebug() << "Datum start:" <<  QDate::fromString(ui->txtDatumPocetkaPrimijene->text(),"mm.MM.yyyy").toString("yyyy-MM-dd");
    QString qveri = "insert into fiskaloznpp (id,oznpp,datumstart,a_ulica,a_broj,a_dodatakbroju,a_brojposte,a_naselje"
            ",a_opcina,a_ostali_tipovi,radno_vrijeme,oznaka_zatvaranja,specificna_namjena,adresni_podatak)";
    qveri += QString(" values(0,'%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12'").arg(ui->txtOznPoslProstor->text())
            .arg(QDate::fromString(ui->txtDatumPocetkaPrimijene->text(),"dd.MM.yyyy").toString("yyyy-MM-dd")).arg(ui->txtUlica->text()).arg(ui->txtKucniBroj->text()).arg(ui->txtKucniBrojDodatak->text())
            .arg(ui->txtBrojPoste->text()).arg(ui->txtNaselje->text()).arg(ui->txtOpcina->text()).arg(ui->txtOstaliTipoviPP->toPlainText())
            .arg(ui->txtRadnoVrijeme->text()).arg(ui->txtOznakaZatvaranja->text()).arg(ui->txtSpecNamj->text());
    if (ui->radioButton->isChecked())
    {
        qveri += ",'A')";
    }else //if (ui->radioButton_2)
    {
        qveri += ",'O')";
    }
    if (!q.exec(qveri))
    {
        qDebug() << q.lastError() << q.lastQuery();
    }
}

void frmFiskalPoruke::UcitajPolja()
{
    ClearPolja();
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));
    if (!q.exec("select * from fiskaloznpp"))
        return;
    q.next();

    if (qApp->property("Firma_OznPosPr").toString() != q.value(q.record().indexOf("oznpp")).toString())
    {
        int ret = QMessageBox::warning(this,"PAZNJA","Razlicita oznaka PP u programu i u bazi\nDa li zelite nastaviti?",QMessageBox::Yes,QMessageBox::No);
        if (ret != QMessageBox::Yes)
            return;
    }
    ui->txtOznPoslProstor->setText(qApp->property("Firma_OznPosPr").toString());
    if (q.value(q.record().indexOf("adresni_podatak")).toString() == "A")
    {
        ui->txtBrojPoste->setText(q.value(q.record().indexOf("a_brojposte")).toString());
        ui->txtKucniBroj->setText(q.value(q.record().indexOf("a_broj")).toString());
        ui->txtKucniBrojDodatak->setText(q.value(q.record().indexOf("a_dodatakbroju")).toString());
        ui->txtNaselje->setText(q.value(q.record().indexOf("a_naselje")).toString());
        ui->txtOpcina->setText(q.value(q.record().indexOf("a_opcina")).toString());
        ui->txtUlica->setText(q.value(q.record().indexOf("a_ulica")).toString());
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setChecked(false);
        slotRadio1();
    }else
    {
        ui->txtOstaliTipoviPP->setPlainText(q.value(q.record().indexOf("a_ostali_tipovi")).toString());
        ui->radioButton_2->setChecked(true);
        ui->radioButton->setChecked(false);
        slotRadio2();
    }
    ui->txtOznakaZatvaranja->setText(q.value(q.record().indexOf("oznaka_zatvaranja")).toString());
//    ui->txtOznPoslProstor->setText(q.value(q.record().indexOf("oznpp")).toString());
    ui->txtRadnoVrijeme->setText(q.value(q.record().indexOf("radno_vrijeme")).toString());
    QString DatumStart = QDate::fromString(q.value(q.record().indexOf("datumstart")).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy");
    ui->txtDatumPocetkaPrimijene->setText(DatumStart);
    ui->txtOznPoslProstor->setText(qApp->property("Firma_OznPosPr").toString());
    //ui->txtOznPoslProstor->setFocus();
    ui->txtDatumPocetkaPrimijene->setFocus();


}
void frmFiskalPoruke::ClearPolja()
{
    ui->txtBrojPoste->setText("");
    ui->txtDatumPocetkaPrimijene->setText("");
    ui->txtKucniBroj->setText("");
    ui->txtKucniBrojDodatak->setText("");
    ui->txtNaselje->setText("");
    ui->txtOpcina->setText("");
    ui->txtOstaliTipoviPP->setPlainText("");
    ui->txtOznakaZatvaranja->setText("");
    ui->txtOznPoslProstor->setText("");
    ui->txtRadnoVrijeme->setText("");
    ui->txtUlica->setText("");
    ui->lblKonBrojPoste->setText(qApp->property("Firma_Postanski").toString());
    ui->lblKonKucniBroj->setText("");
    ui->lblKonKucniBrojDodatak->setText("");
    ui->lblKonNaselje->setText(qApp->property("Firma_Grad").toString());
    ui->lblKonOpcina->setText("");
    ui->lblKonUlica->setText(qApp->property("Firma_Adresa").toString());

}
