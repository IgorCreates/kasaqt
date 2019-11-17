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




#include "frmopcije.h"
#include "ui_frmopcije.h"
#include <QtSql>
#include <QtGui>
#include "ui/frmfiskalporuke.h"
#include "ui/frmprinteropcije.h"
#include <QMessageBox>


frmOpcije::frmOpcije(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmOpcije)
{
    ui->setupUi(this);
    UcitajOpcije();
    this->setWindowTitle("FiskalKasa-Opcije");
    //ui->tab_6->setVisible(false);
#ifdef Q_WS_WIN
    ui->chkWINkoristiPosPrinting->setEnabled(true);
    ui->chkWINkoristiPosPrinting->setVisible(true);
#endif
#ifdef Q_WS_X11
    ui->chkWINkoristiPosPrinting->setVisible(true);
    ui->chkWINkoristiPosPrinting->setEnabled(true);
#endif
    ProvjeraPravila();
}

frmOpcije::~frmOpcije()
{
    delete ui;
}

void frmOpcije::ProvjeraPravila()
{
    if (qApp->property("KonobarPrava").toInt() != 99)
    {
        ui->tab_Certifikat->setEnabled(false);
        ui->tab_DokNaCekanju->setEnabled(false);
        ui->tab_OpcijePrograma->setEnabled(false);
        ui->tab_Polog->setEnabled(true);
        ui->tab_Printer->setEnabled(true);
    }else
    {
        ui->tab_Certifikat->setEnabled(true);
        ui->tab_DokNaCekanju->setEnabled(true);
        ui->tab_OpcijePrograma->setEnabled(true);
        ui->tab_Polog->setEnabled(true);
        ui->tab_Printer->setEnabled(true);
    }
    ui->tab_Printer->setFocus();
}

void frmOpcije::on_btnPrinteriUpis_pressed()
{
    ui->btnPrinteriUpis->setEnabled(false);
    bool UspioUpis = true;
    QSqlDatabase dbO;
    dbO = QSqlDatabase::database("local.sqlite");
    if (!dbO.open())
    {
        ui->btnPrinteriUpis->setEnabled(true);
        QMessageBox::warning(this,"Opcije","Problem pristupa bazi","OK");
        return;
    }
    QSqlQuery q("",dbO);
    QString  qveri;
    qveri = QString("delete from opcije where sto='Printer'");
    if (!q.exec(qveri))
        UspioUpis = false;
    qveri = "insert into opcije (sto,sifra,value1)";
    qveri += QString("select 'Printer','Port','%1'").arg(ui->txtPrinterPort->text());
    /*
    if (!q.exec(qveri))
        UspioUpis = false;
    qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','IspisPoljeRacun','%1');").arg(ui->txtIspisPoljeRacun->text());
    if (!q.exec(qveri))
        UspioUpis = false;
    qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','IspisPoljeDatum','%1');").arg(ui->txtIspisPoljeDatum->text());
    if (!q.exec(qveri))
        UspioUpis = false;
    qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','IspisPoljeZaPlatiti','%1');").arg(ui->txtIspisPoljeZaPlatiti->text());
    if (!q.exec(qveri))
        UspioUpis = false;
    qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','IspisESC_Rezac','%1');").arg(ui->txtIspisESC_Rezac->text());
    if (!q.exec(qveri))
        UspioUpis = false;
    qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','IspisESC_BOLD','%1');").arg(ui->txtIspisESC_BOLD->text());
    if (!q.exec(qveri))
        UspioUpis = false;
        */
    qveri += QString(" union all select 'Printer','IspisPoljeRacun','%1'").arg(ui->txtIspisPoljeRacun->text());
    qveri += QString(" union all select 'Printer','IspisPoljeDatum','%1'").arg(ui->txtIspisPoljeDatum->text());
    qveri += QString(" union all select 'Printer','IspisPoljeZaPlatiti','%1'").arg(ui->txtIspisPoljeZaPlatiti->text());
    qveri += QString(" union all select 'Printer','IspisPoljeZahvala','%1'").arg(ui->txtIspisPoljeZahvala->text());
    qveri += QString(" union all select 'Printer','Printer',%1").arg(ui->cmbPrinter->model()->index(ui->cmbPrinter->currentIndex(),0).data().toInt());
//    qveri += QString(" union all select 'Printer','IspisESC_Rezac','%1'").arg(ui->txtIspisESC_Rezac->text());
//    qveri += QString(" union all select 'Printer','IspisESC_BOLD','%1'").arg(ui->txtIspisESC_BOLD->text());

    if (ui->prnRedova1->isChecked())
        qveri += QString(" union all select 'Printer','Redova',1");
    if (ui->prnRedova2->isChecked())
        qveri += QString(" union all select 'Printer','Redova',2");
    if (ui->chkRezac->isChecked())
        qveri += QString(" union all select 'Printer','Rezac',1");
    if (ui->chkLadica->isChecked())
    {
        qveri += QString(" union all select 'Printer','Ladica',1");
    }

    if (ui->chkWINkoristiPosPrinting->isChecked())
        qveri += QString(" union all select 'Printer','WINkoristiPOSPrinting',1");
    if (ui->radioIspisPOS->isChecked())
        qveri += QString(" union all select 'Printer','DEFprinter-POS',1");
    if (ui->radioIspisVeliki->isChecked())
        qveri += QString(" union all select 'Printer','DEFprinter-Veliki',1");
    if (ui->chkProzorOdabirIspisa->isChecked())
        qveri += QString(" union all select 'Printer','ProzorOdabirIspisa',1");
    if (ui->chkBoxRac->isChecked())
    {
        //qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','IspisPoljeRacun_B','1');");
        qveri += QString(" union all select 'Printer','IspisPoljeRacun_B','1'");
        //if (!q.exec(qveri)){ UspioUpis = false;}
    }
    if (ui->chkBoxDatum->isChecked())
    {
        //qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','IspisPoljeDatum_B','1');");
        qveri += QString(" union all select 'Printer','IspisPoljeDatum_B','1'");
//        if (!q.exec(qveri)){ UspioUpis = false;}
    }
    if (ui->chkBoxZaPlatiti->isChecked())
    {
        //qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','IspisPoljeZaPlatiti_B','1');");
        qveri += QString(" union all select 'Printer','IspisPoljeZaPlatiti_B','1'");
//        if (!q.exec(qveri)){ UspioUpis = false;}
    }
    if (ui->txtIspisKonverzija->text() != "" )
    {
//        qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','Konverzija','%1')").arg(ui->txtIspisKonverzija->text());
        qveri += QString(" union all select 'Printer','Konverzija','%1'").arg(ui->txtIspisKonverzija->text());
//        if (!q.exec(qveri)) { UspioUpis =false;)}
    }
    if (ui->txtIspisKonverzijaFrom->text() != "")
    {
        qveri += QString(" union all select 'Printer','KonverzijaFrom','%1'").arg(ui->txtIspisKonverzijaFrom->text());
    }else
    {
        qveri += QString(" union all select 'Printer','KonverzijaFrom','utf8'");
    }
    if (ui->txtIspisSirinaPapira->text().toDouble() > 0)
    {
        //qveri = QString("insert into opcije (sto,sifra,value1) values('Printer','SirinaPapira','%1'").arg(ui->txtIspisSirinaPapira->text());
        qveri += QString(" union all select 'Printer','SirinaPapira','%1'").arg(ui->txtIspisSirinaPapira->text());
//        if (!q.exec(qveri)) { UspioUpis = false;}
    }
    if (ui->chkPOS_RPT_ISPIS->isChecked())
    {
        qveri += " union all select 'Printer','POS-RPT-ISPIS','1'";
    }else
    {
        qveri += " union all select 'Printer','POS-RPT-ISPIS','0'";
    }
    if (ui->chkPOS_RPT_DirektIspis->isChecked())
    {
        qveri += " union all select 'Printer','POS_RPT_DirektanIspis','1'";
    }else
    {
        qveri += " union all select 'Printer','POS_RPT_DirektanIspis','0'";
    }

    qveri += QString(" union all select 'Printer','POS_RPT_MarginaTOP','%1'").arg(ui->POS_RPT_MarginaTOP->text());
    qveri += QString(" union all select 'Printer','POS_RPT_MarginaBotton','%1'").arg(ui->POS_RPT_MarginaBotton->text());
    qveri += QString(" union all select 'Printer','POS_RPT_MarginaLeft','%1'").arg(ui->POS_RPT_MarginaLeft->text());
    qveri += QString(" union all select 'Printer','POS_RPT_MarginaRight','%1'").arg(ui->POS_RPT_MarginaRight->text());

    qDebug() << qveri;
    if (!q.exec(qveri))
    {
        qDebug() << q.lastQuery();
        qDebug() << q.lastError();
        UspioUpis = false;
    }
    if (UspioUpis == false)
    {
        QMessageBox::warning(this,"Opcije","Greska kod upisa opcija","OK");
    }else
    {
        this->RefreshOpcije();
        QMessageBox::information(this,"Opcije","Upisano","OK");
    }
    ui->btnPrinteriUpis->setEnabled(true);
}

void frmOpcije::on_btnCertifikatiUpis_pressed()
{
    QProcess *DeKriptBaze = new QProcess(this);
    QString ProgPokreni = "/usr/bin/openssl des3 -d -in op.enc -out op.lic -k F1sk4lK";
    QStringList ProgArg;
    //KRIPTIRANJE ProgArg << " des3 -in op.lic -out op.enc ";
    ProgArg << " des3 -d -in op.enc -out op.lic -k F1sk4lK"; //dekriptiranje

    //DeKriptBaze->execute(ProgPokreni,ProgArg);
    DeKriptBaze->execute(ProgPokreni);
    QSqlDatabase dbLocal;
    dbLocal = QSqlDatabase::database("QSQLITE","op.lic");
//    dbLocal.setDatabaseName("op.lic");
    ui->btnCertifikatiUpis->setEnabled(false);
    bool UspioUpis = true;
    ui->btnCertifikatiUpis->setEnabled(true);
    DeKriptBaze->deleteLater();
    QSqlQuery q("",dbLocal);

    if (!dbLocal.open())
    {
        QMessageBox::warning(this,"Upis","Baza nije dostupna,podaci nisu spremljeni","OK");
        return;
    }
    q.exec("delete from certs");
    if (!q.exec(QString("insert into certs values('%1','%2')").arg(ui->txtCertsPath->text()).arg(ui->txtCertsSifra->text())))
    {
        qDebug() << q.lastError() << q.lastQuery();
        QMessageBox::warning(this,"Upis","Upis nije spremljen,pokusajte ponovno nakon restarta programa","OK");
    }
    else
    {

        QMessageBox::information(this,"Upis","Upis je spremljen,podaci ce biti valjani nakon restarta programa","OK");
    }

    this->RefreshOpcije();
return;
    QFile Brisi;

    if (!Brisi.remove("op.lic"))
    {
        qDebug() << "Greska- brisanje op";
        qDebug() << Brisi.errorString();
    }

}

void frmOpcije::on_btnPologUpis_pressed()
{
    bool UspioUpis = true;
    ui->btnPologUpis->setEnabled(false);
    QSqlDatabase dbO = QSqlDatabase::database("local.sqlite");
    if (!dbO.open())
    {
        ui->btnPologUpis->setEnabled(true);
        QMessageBox::warning(this,"Opcije","Problem pristupa bazi","OK");
        return;
    }
    QSqlQuery q("",dbO);
    QString  qveri;
    qveri = QString("delete from opcije where sto='Polog'");
    if (!q.exec(qveri))
        UspioUpis = false;
    qveri = QString("insert into opcije (sto,value1) values('Polog','%1')").arg(ui->txtPologUKasi->text());
    if (!q.exec(qveri))
        UspioUpis = false;
    if (UspioUpis == false)
    {
        QMessageBox::warning(this,"Opcije","Greska kod upisa opcija","OK");
    }else
    {
        this->RefreshOpcije();
        QMessageBox::information(this,"Opcije","Upisano","OK");
    }
    ui->btnPologUpis->setEnabled(false);
}

void frmOpcije::on_btnUpisDokNaCekanju_pressed()
{
    bool UspioUpis = true;
    ui->btnUpisDokNaCekanju->setEnabled(false);
    QSqlDatabase dbO = QSqlDatabase::database("local.sqlite");
    if (!dbO.open())
    {
        ui->btnUpisDokNaCekanju->setEnabled(true);
        QMessageBox::warning(this,"Opcije","Problem pristupa bazi","OK");
        return;
    }
    QSqlQuery q("",dbO);
    QString  qveri;
    qveri = QString("delete from opcije where sto='List'");
    if (!q.exec(qveri))
        UspioUpis = false;
    qveri = QString("insert into opcije (sto,value1) values('List','%1')").arg(ui->txtDokNaCekanjuPise->text());
    if (!q.exec(qveri))
        UspioUpis = false;
    if (ui->chkKoristiStol->isChecked())
    {
        qveri = "insert into opcije (sto,sifra,value1) values('List','Prikaz',1)";
        if (!q.exec(qveri))
            UspioUpis = false;
    }
    if (UspioUpis == false)
    {
        QMessageBox::warning(this,"Opcije","Greska kod upisa opcija","OK");
    }else
    {
        this->RefreshOpcije();
        QMessageBox::information(this,"Opcije","Upisano","OK");
    }
    ui->btnUpisDokNaCekanju->setEnabled(true);
}
void frmOpcije::RefreshOpcije()
{
    QSqlDatabase dbO = QSqlDatabase::database("local.sqlite");
    QSqlQuery q("",dbO);
    if (!dbO.open())
    {
        QMessageBox::warning(this,"Opcije","Greska kod ucitavanja opcija","OK");
        return;
    }
    QString qveri;
    //qveri = "select sto,sifra,value1 from opcije where sto in ('Printer','Certifikati','List','Polog','Prog')";
    qveri = "select sto,sifra,value1 from opcije where sto in ('Printer','Certifikati','List','Polog','Prog')";
    if (!q.exec(qveri))
    {
        QMessageBox::warning(this,"Opcije","Greska kod ucitavanja opcija","OK");
        return;
    }
    while (q.next())
    {
        QString PropImeT = QString("%1-%2").arg(q.value(0).toString()).arg(q.value(1).toString());
        //PropIme += "-";
        //PropIme += q.value(1).toChar();
        QByteArray ba = PropImeT.toUtf8();
        const char* PropIme = ba.data();
        QVariant PropVal = q.value(2).toString();
        qApp->setProperty(PropIme,PropVal.toString());
    }
   // QMessageBox::information(this,"Opcije","Nakon upisa opcija restartajte aplikaciju","OK");
    //emit constStoPise = qApp->property("List-").toString();
}
void frmOpcije::UcitajOpcije()
{
    //ui->txtCertifikatCA->setText(qApp->property("Certifikati-CA").toString());
    //ui->txtCertifikatClient->setText(qApp->property("Certifikati-Client").toString());
    ui->txtCertsPath->setText(qApp->property("Certs_Path").toString());
    ui->txtDokNaCekanjuPise->setText(qApp->property("List-").toString());
//    ui->txtIspisESC_BOLD->setText(qApp->property("Printer-IspisESC_BOLD").toString());
//    ui->txtIspisESC_Rezac->setText(qApp->property("Printer-IspisESC_Rezac").toString());
    ui->txtIspisPoljeDatum->setText(qApp->property("Printer-IspisPoljeDatum").toString());
    ui->txtIspisPoljeRacun->setText(qApp->property("Printer-IspisPoljeRacun").toString());
    ui->txtIspisPoljeZaPlatiti->setText(qApp->property("Printer-IspisPoljeZaPlatiti").toString());
    ui->txtPologUKasi->setText(qApp->property("Polog-").toString());
    ui->txtPrinterPort->setText(qApp->property("Printer-Port").toString());
    ui->txtIspisKonverzija->setText(qApp->property("Printer-Konverzija").toString());
    ui->txtIspisKonverzijaFrom->setText(qApp->property("Printer-KonverzijaFrom").toString());
    ui->txtIspisPoljeZahvala->setText(qApp->property("Printer-IspisPoljeZahvala").toString());
    ui->txtIspisSirinaPapira->setText(qApp->property("Printer-SirinaPapira").toString());


    if (qApp->property("Prog-SamoZakljucaj").toString() == "1")
    {
        ui->chkSamoZakljucaj->setChecked(true);
    }else
    {
        ui->chkSamoZakljucaj->setChecked(false);
    }
    if (!qApp->property("Prog-SamoZakljucajTime").isNull())
    {
        ui->txtSamoZakljucaj->setText(qApp->property("Prog-SamoZakljucajTime").toString());
    }else
    {
        ui->txtSamoZakljucaj->setText("5");
    }
    if (qApp->property("Printer-WINkoristiPOSPrinting").toString() == "1")
    {
        ui->chkWINkoristiPosPrinting->setChecked(true);
    }else
    {
        ui->chkWINkoristiPosPrinting->setChecked(false);
    }
    if (qApp->property("Printer-DEFprinter-POS").toString() == "1")
    {
        ui->radioIspisPOS->setChecked(true);
    }else
    {
        ui->radioIspisPOS->setChecked(false);
    }
    if (qApp->property("Printer-DEFprinter-Veliki").toString() == "1")
    {
        ui->radioIspisVeliki->setChecked(true);
    }else
    {
        ui->radioIspisVeliki->setChecked(false);
    }
    if (qApp->property("Printer-ProzorOdabirIspisa").toString() == "1")
    {
        ui->chkProzorOdabirIspisa->setChecked(true);
    }else
    {
        ui->chkProzorOdabirIspisa->setChecked(false);
    }
    if (qApp->property("Printer-IspisPoljeRacun_B").toString() == "1")
    {
        ui->chkBoxRac->setChecked(true);
    }
    if (qApp->property("Printer-IspisPoljeDatum_B").toString() == "1")
    {
        ui->chkBoxDatum->setChecked(true);
    }
    if (qApp->property("Printer-IspisPoljeZaPlatiti_B").toString() == "1")
    {
        ui->chkBoxZaPlatiti->setChecked(true);
    }
    if (qApp->property("Printer-Ladica").toString() == "1")
        ui->chkLadica->setChecked(true);
    if (qApp->property("Printer-Rezac").toString() == "1")
        ui->chkRezac->setChecked(true);
    if (qApp->property("Printer-Redova").toString() == "1")
    {
        ui->prnRedova1->setChecked(true);
    }else
    {
        ui->prnRedova2->setChecked(true);
    }
    if (qApp->property("Prog-PretragaArtLike").toString() == "1")
    {
        ui->chkProgArtSifLike->setChecked(true);
    }else
    {
        ui->chkProgArtSifLike->setChecked(false);
    }
    if (qApp->property("Prog-ArtPitaKolicinu").toString() == "1")
    {
        ui->chkArtPitajKol->setChecked(true);
    }else
    {
        ui->chkArtPitajKol->setChecked(false);
    }
    if (qApp->property("List-BrziRacun").toString() == "1")
    {
        ui->chkProgBTNBRziRacun->setChecked(true);
    }else {
        ui->chkProgBTNBRziRacun->setChecked(false);
    }
    if (qApp->property("Prog-PretragaArtSif").toString() == "1")
    {
        ui->chkProgPretragaPoSifri->setChecked(true);
    }else
    {
        ui->chkProgPretragaPoSifri->setChecked(false);
    }
    if (qApp->property("Prog-AkoImaArt+1kol").toString() == "1")
    {
        ui->chkProgRacArtSifra->setChecked(true);
    }else
    {
        ui->chkProgRacArtSifra->setChecked(false);
    }
    if (qApp->property("Prog-PretragaArtBcode").toString() == "1")
    {
        ui->chkProgPretragaPoBarcode->setChecked(true);
    }
    if (qApp->property("List-Prikaz").toString() == "1")
    {
        ui->chkKoristiStol->setChecked(true);
    }else
    {
        ui->chkKoristiStol->setChecked(false);
    }
    if (qApp->property("Prog-IspisRnalogCustom").toString() == "1")
    {
        ui->chkProgA4IspisCustom->setChecked(true);
    }else
    {
        ui->chkProgA4IspisCustom->setChecked(false);
    }
    if (qApp->property("Prog-MakniPitajBrisanjeStavke").toString() == "1")
    {
        ui->chkProgPorukaBrisanjeStavke->setChecked(true);
    }else
    {
        ui->chkProgPorukaBrisanjeStavke->setChecked(false);
    }
    if (qApp->property("Prog-Fiskalizacija").toString() != "0")
    {
        ui->chkProgFiskalizacija->setChecked(true);
    }else
    {
        ui->chkProgFiskalizacija->setChecked(false);
    }
    if (qApp->property("Prog-RnalogRazSklad").toString() == "1")
    {
        ui->chkRnalogRazduziSklad->setChecked(true);
    }else
    {
        ui->chkRnalogRazduziSklad->setChecked(false);
    }
    if (qApp->property("Prog-LogRac").toString() == "1")
    {
        ui->chkLogRac->setChecked(true);
    }else
    {
        ui->chkLogRac->setChecked(false);
    }
    if (qApp->property("Prog-SamoSvojDok").toString() == "1")
    {
        ui->chkSamoSvojDok->setChecked(true);
    }else
    {
        ui->chkSamoSvojDok->setChecked(false);
    }
    if (qApp->property("Prog-ZabranaBrisanjaArt").toString() == "1")
    {
        ui->chkZabranaBrisanjaArt->setChecked(true);
    }else
    {
        ui->chkZabranaBrisanjaArt->setChecked(false);
    }
    if (qApp->property("Printer-POS-RPT-ISPIS").toString() == "1")
    {
        ui->chkPOS_RPT_ISPIS->setChecked(true);
    }else
    {
        ui->chkPOS_RPT_ISPIS->setChecked(false);
    }
    if (qApp->property("Printer-POS_RPT_DirektanIspis").toString() == "1")
    {
        ui->chkPOS_RPT_DirektIspis->setChecked(true);
    }else
    {
        ui->chkPOS_RPT_DirektIspis->setChecked(false);
    }

    if (qApp->property("Mail-server").toString().length() > 0)
    {
        ui->txtMailServer->setText(qApp->property("Mail-server").toString());
    }
    if (qApp->property("Mail-port").toString().length() > 0)
        ui->txtMailPort->setText(qApp->property("Mail-port").toString());
    if (qApp->property("Mail-username").toString().length() > 0)
        ui->txtMailUsername->setText(qApp->property("Mail-username").toString());
    if (qApp->property("Mail-password").toString().length() > 0)
        ui->txtMailPassword->setText(qApp->property("Mail-password").toString());


    //if (qveri += QString(" union all select 'Printer','POS_RPT_MarginaTOP','%1'").arg(ui->POS_RPT_MarginaTOP);)
    if (!qApp->property("Printer-POS_RPT_MarginaTOP").toString().isEmpty())
    {
        ui->POS_RPT_MarginaTOP->setText(qApp->property("Printer-POS_RPT_MarginaTOP").toString());
    }
    if (!qApp->property("Printer-POS_RPT_MarginaBotton").toString().isEmpty())
    {
        ui->POS_RPT_MarginaBotton->setText(qApp->property("Printer-POS_RPT_MarginaBotton").toString());
    }
    if (!qApp->property("Printer-POS_RPT_MarginaLeft").toString().isNull())
    {
        ui->POS_RPT_MarginaLeft->setText(qApp->property("Printer-POS_RPT_MarginaLeft").toString());
    }
    if (!qApp->property("Printer-POS_RPT_MarginaRight").toString().isNull())
    {
        ui->POS_RPT_MarginaRight->setText(qApp->property("Printer-POS_RPT_MarginaRight").toString());
    }

    if (!qApp->property("Prog-KasaZadnjiRacunPrikaz").toString().isNull())
    {
        ui->chkKasaPrikazZadnjeSume->setChecked(true);
    }else
    {
        ui->chkKasaPrikazZadnjeSume->setChecked(false);
    }

    QSqlDatabase dbO = QSqlDatabase::database("baza");
    QSqlQueryModel *modelO = new QSqlQueryModel();
    QSqlQueryModel *modelO2 = new QSqlQueryModel();
    QSqlQueryModel *modelO3 = new QSqlQueryModel();

    modelO->setQuery("select id,naziv from matsklad",dbO);
    ui->cmbSklad->setModel(modelO);
    ui->cmbSklad->setModelColumn(1);
    for (int i=0; i<= ui->cmbSklad->model()->rowCount();i++)
    {
        if (ui->cmbSklad->model()->index(i,0).data().toString() == qApp->property("Prog-SID").toString())
        {
            ui->cmbSklad->setCurrentIndex(i);
        }
    }
    modelO2->setQuery("select id,naziv from cjenik where aktivan='Y' order by glavni desc",dbO);
    ui->cmbCjenik->setModel(modelO2);
    ui->cmbCjenik->setModelColumn(1);
    for (int i=0; i<= ui->cmbCjenik->model()->rowCount();i++)
    {
        if (ui->cmbCjenik->model()->index(i,0).data().toString() == qApp->property("Prog-CID").toString())
        {
            ui->cmbCjenik->setCurrentIndex(i);
        }
    }
    modelO3->setQuery("select id,naziv from printeri",dbO);
    ui->cmbPrinter->setModel(modelO3);
    ui->cmbPrinter->setModelColumn(1);
    //modelO3->index(ui->cmbPrinter)
    for (int i=0; i <= ui->cmbPrinter->model()->rowCount();i++)
    {
        if (ui->cmbPrinter->model()->index(i,0).data().toString() == qApp->property("Printer-Printer").toString())
        {
            ui->cmbPrinter->setCurrentIndex(i);
        }
    }
    ProvjeraPravila();
}

void frmOpcije::on_btnUpisOpcijeProg_pressed()
{
    ui->btnUpisOpcijeProg->setEnabled(false);
    bool UspioUpis = true;
    QSqlDatabase dbO;
    dbO = QSqlDatabase::database("local.sqlite");
    if (!dbO.open())
    {
        ui->btnPrinteriUpis->setEnabled(true);
        QMessageBox::warning(this,"Opcije","Problem pristupa bazi","OK");
        return;
    }
    QSqlQuery q("",dbO);
    QString  qveri;
    qveri = QString("delete from opcije where sto='Prog';delete from opcije where sto='List' and sifra='BrziRacun';");
    if (!q.exec(qveri))
        UspioUpis = false;
    qveri = "insert into opcije (sto,sifra,value1)";
    qveri += QString(" select 'Prog','SID',%1").arg(ui->cmbSklad->model()->index(ui->cmbSklad->currentIndex(),0).data().toInt());
    qveri += QString(" union all select 'Prog','CID',%1").arg(ui->cmbCjenik->model()->index(ui->cmbCjenik->currentIndex(),0).data().toInt());
    if (ui->chkProgRacArtSifra->isChecked())
    {
        qveri += " union all select 'Prog','AkoImaArt+1kol',1";
    }
    if (ui->chkProgArtSifLike->isChecked())
    {
        qveri += " union all select 'Prog','PretragaArtLike',1";
    }
    if (ui->chkProgBTNBRziRacun->isChecked())
    {
        qveri += " union all select 'List','BrziRacun',1 ";
    }
    if (ui->chkProgPretragaPoSifri->isChecked())
    {
        qveri += " union all select 'Prog','PretragaArtSif',1 ";
    }
    if (ui->chkArtPitajKol->isChecked())
    {
        qveri += " union all select 'Prog','ArtPitaKolicinu',1 ";
    }
    if (ui->chkProgPretragaPoBarcode->isChecked())
    {
        qveri += " union all select 'Prog','PretragaArtBcode',1 ";
    }
    if (ui->chkProgA4IspisCustom->isChecked())
    {
        qveri += " union all select 'Prog','IspisRnalogCustom',1 ";
    }
    if (ui->chkProgPorukaBrisanjeStavke->isChecked())
    {
        qveri += " union all select 'Prog','MakniPitajBrisanjeStavke',1 ";
    }
    if (ui->chkProgFiskalizacija->isChecked())
    {
        qveri += " union all select 'Prog','Fiskalizacija',1 ";
    }else
    {
        qveri += " union all select 'Prog','Fiskalizacija',0 ";
    }
    if (ui->chkRnalogRazduziSklad->isChecked())
    {
        qveri += " union all select 'Prog','RnalogRazSklad',1 ";
    }
    if (ui->chkLogRac->isChecked())
    {
        qveri += " union all select 'Prog','LogRac',1 ";
    }
    if (ui->chkSamoSvojDok->isChecked())
    {
        qveri += " union all select 'Prog','SamoSvojDok',1 ";
    }
    if (ui->chkZabranaBrisanjaArt->isChecked())
    {
        qveri += " union all select 'Prog','ZabranaBrisanjaArt',1 ";
    }
    if (ui->chkSamoZakljucaj->isChecked())
    {
        qveri += " union all select 'Prog','SamoZakljucaj',1 ";

    }
    if (ui->txtSamoZakljucaj->text().toInt() > 0)
    {
        qveri += QString(" union all select 'Prog','SamoZakljucajTime',%1 ").arg(ui->txtSamoZakljucaj->text().toInt());
    }
    if (ui->chkKasaPrikazZadnjeSume->isChecked())
    {
        qveri += QString(" union all select 'Prog','KasaZadnjiRacunPrikaz',1 ");
    }

    if (!q.exec(qveri))
    {
       UspioUpis = false;
       qDebug() << "Greska upisa opcije: " << q.lastError() << q.lastQuery();
    }
    if (UspioUpis == false)
    {
        QMessageBox::warning(this,"Opcije","Greska kod upisa opcija","OK");
    }else
    {
        this->RefreshOpcije();
        QMessageBox::information(this,"Opcije","Upisano","OK");
        QMessageBox::warning(this,"VAZNO","Nakon upisa opcija ponovno pokrenite program","OK");
    }

    //ui->btnPologUpis->setEnabled(false);
    ui->btnUpisOpcijeProg->setEnabled(true);
}

void frmOpcije::on_cmbSklad_currentIndexChanged(int index)
{
//    qDebug() << ui->cmbSklad->model()->index(ui->cmbSklad->currentIndex(),0).data().toString();
}

void frmOpcije::on_btnPoslovniProstor_pressed()
{
    frmFiskalPoruke *fp = new frmFiskalPoruke();
    fp->show();
    fp->setFocus();
}


void frmOpcije::on_btnESCpostavke_pressed()
{
    frmPrinterOpcije *fp = new frmPrinterOpcije();
    fp->show();
}

void frmOpcije::on_btnUpisOpcijeMail_released()
{
    int ret = QMessageBox::question(this,"Upis opcije","Da li zelite spremiti opcije?",QMessageBox::Yes,QMessageBox::No);
    if (ret != QMessageBox::Yes)
        return;
    QSqlQuery q("",QSqlDatabase::database("local.sqlite"));

    QString qveri = "delete from opcije where sto='Mail'";
    q.exec(qveri);
    qveri = "insert into opcije (sto,sifra,opis,value1) ";
    qveri += QString(" select 'Mail','server','Mail-server','%1'").arg(ui->txtMailServer->text());
    qveri += QString(" union all select 'Mail','port','Mail-server-port','%1'").arg(ui->txtMailPort->text());
    qveri += QString(" union all select 'Mail','username','Mail-username','%1'").arg(ui->txtMailUsername->text());
    qveri += QString(" union all select 'Mail','password','Mail-password','%1'").arg(ui->txtMailPassword->text());

    if (!q.exec(qveri))
    {
        qDebug() << "greska mail upis : " << q.lastError() << q.lastQuery();
        QMessageBox::warning(this,"Upis opcija","Greska kod upisa","OK");

    }else
    {
        QMessageBox::information(this,"Upis opcija","Upisano","OK");
    }
}
