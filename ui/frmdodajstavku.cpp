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




#include "frmdodajstavku.h"
#include "ui_frmdodajstavku.h"
#include <QtGui>
#include <QtSql>
#include <QTextStream>
#include <QString>
#include <QLocale>
#include "mojiheaderi.h"
#include <QMessageBox>


frmDodajStavku::frmDodajStavku(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmDodajStavku)
{
    ui->setupUi(this);
    NeDiraj = true;
    this->setWindowTitle(tr("Dodavanje stavke"));
    RadimSto = "Novi";
    Rac2ID = 0;
    NarID = 0;
    NarucioUID = 0;

    IZRAC = new clsIzracuni();
    IZRAC->DefVrijednostVarijabli();
    this->setWindowModality(Qt::WindowModal);
    connect(ui->chkDostava,SIGNAL(stateChanged(int)),this,SLOT(chkDostavaProv()));
}

frmDodajStavku::~frmDodajStavku()
{
    delete ui;
}

void frmDodajStavku::slotRadimSto(QString StoRadim)
{
    if (StoRadim == "")
        return;
    RadimSto = StoRadim;

}

void frmDodajStavku::Suma()
{
//    clsIzracuni *IZRAC = new clsIzracuni();
    NeDiraj= false;
    IZRAC->set_StavkaAID(artID,ui->txtArtSifra->text());
    IZRAC->set_StavkaKol(ui->txtArtKolicina->text().toDouble());
    IZRAC->set_StavkaPC(ui->txtArtJedCijena->text().toDouble());
    IZRAC->set_StavkaRabatP(ui->txtArtRabatP->text().toDouble());
    IZRAC->set_RacNarID(NarID);
    IZRAC->set_StavkaNarucioUID(NarucioUID);
    IZRAC->set_StavkaNarudzba(0);
    IZRAC->set_StavkaPovratnaNaknada(ui->txtArtPovratnaNaknada->text().toDouble());
    QString PovratnaSuma = QString::number(ui->txtArtPovratnaNaknada->text().toDouble()*ui->txtArtKolicina->text().toDouble());
    ui->txtArtPovratnaNaknadaUk->setText(PovratnaSuma);
    IZRAC->izracunMPC();
    ui->tablePorezi->setModel(IZRAC->stavkaListaPoreza);
    ui->txtArtRabatK->setText(QString("%1").arg(IZRAC->get_stavkaRabatK()));
    ui->txtArtUkupno->setText(QString("%1").arg(IZRAC->get_stavkaUkupnoCista()));
    ui->lblSuma->setText(QString("%1").arg(IZRAC->get_stavkaUkupno()));
    NeDiraj = true;

}

void frmDodajStavku::on_btnIzlaz_clicked()
{
    emit StisnutCancel();
    this->close();
}

void frmDodajStavku::on_btnUpisiStavku_clicked()
{
}

void frmDodajStavku::on_txtArtKolicina_textChanged(const QString &arg1)
{
    Suma();
}

void frmDodajStavku::on_btnUpisiStavku_pressed()
{
}
void frmDodajStavku::on_txtArtRabatP_textChanged(const QString &arg1)
{
    if (NeDiraj== true)
    {
        Suma();
    }
}

void frmDodajStavku::on_pushButton_3_pressed()
{
    double newKol = 0;
    newKol = ui->txtArtKolicina->text().toDouble()+1;
    ui->txtArtKolicina->setText(QString("%1").arg(newKol));

}

void frmDodajStavku::on_pushButton_5_pressed()
{
    double newKol = 0;
    newKol = ui->txtArtKolicina->text().toDouble()-1;
    if (newKol < 1)
        return;
    ui->txtArtKolicina->setText(QString("%1").arg(newKol));
}

void frmDodajStavku::on_pushButton_4_pressed()
{
    double newKol = 0;
    newKol = ui->txtArtRabatP->text().toDouble()+5;
    if (newKol >=100)
    {
        ui->txtArtRabatP->setText("100");
        return;
    }
    ui->txtArtRabatP->setText(QString("%1").arg(newKol));
}

void frmDodajStavku::on_pushButton_6_pressed()
{
    double newKol = 0;
    newKol = ui->txtArtRabatP->text().toDouble()-5;
    if (newKol <= 0)
    {   ui->txtArtRabatP->setText("0");
        Suma();
        return;
    }
    ui->txtArtRabatP->setText(QString("%1").arg(newKol));

}

void frmDodajStavku::on_btnNazivPokazi_released()
{

    //ui->txtNazivNovi->setVisible(true);

    txtNoviNaziv = new QPlainTextEdit;
    //NoviNazivProzor = new QWidget();
    NoviNazivProzor = new QDialog(this);
    NoviNazivUpis = new QPushButton(tr("UPIS"));
    NoviNazivIzlaz = new QPushButton(tr("IZLAZ"));
    vboxLayout = new QVBoxLayout;


    txtNoviNaziv->setObjectName(tr("NoviNaziv"));
    NoviNazivUpis->setObjectName(tr("NoviNazivG1"));
    vboxLayout->addWidget(txtNoviNaziv);
    vboxLayout->addWidget(NoviNazivUpis);
    vboxLayout->addWidget(NoviNazivIzlaz);
    NoviNazivProzor->setLayout(vboxLayout);

    NoviNazivProzor->show();

    txtNoviNaziv->setPlainText(ui->txtArtNazivOriginal->text());
//    txtNoviNaziv->installEventFilter(this);

    connect(NoviNazivIzlaz,SIGNAL(released()),NoviNazivProzor,SLOT(close()));
    connect(NoviNazivUpis,SIGNAL(released()),this,
            SLOT(IzmijeniNaziv()));
    connect(NoviNazivUpis,SIGNAL(released()),NoviNazivProzor,SLOT(close()));

}
void frmDodajStavku::IzmijeniNaziv()
{
//    ui->txtArtNazivOriginal->setText(NoviNazivTXT);
   // qDebug() << "JEE" << txtNoviNaziv->toPlainText();
    ui->txtArtNazivOriginal->setText(txtNoviNaziv->toPlainText());
}

void frmDodajStavku::on_btnUpisiStavku_released()
{
    //UpisiStavku(false);
    IZRAC->set_StavkaArtNaziv(ui->txtArtNazivOriginal->text());
    IZRAC->set_StavkaNarucioUID(NarucioUID);
    IZRAC->set_StavkaNarudzba(0);
    IZRAC->set_StavkaPodaci(BrojStola,artGrupaID,artIspisKuhinja,Rac2ID,NarID);
    if (!IZRAC->UpisStavkeUBazu())
    {
        QMessageBox::warning(this,tr("GRESKA"),tr("Greska kod upisa stavke u bazu"),"OK");
    }else
    {
        emit StisnutOK(BrojStola);
        this->close();
    }

}

void frmDodajStavku::on_chkDostava_pressed()
{
//    if ()
}
void frmDodajStavku::chkDostavaProv()
{
    if (ui->chkDostava->isChecked())
    {
        //05.05.2018 zbog krivog rada pri dostavi (upisuje PNP umjesto poreza) ovo iskljucujem
        //IZRAC->set_PorezDostava(true);
        IZRAC->set_PorezDostava(false);
        Suma();
    }else
    {
        IZRAC->set_PorezDostava(false);
        Suma();
    }
}
