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




#ifndef FRMDODAJSTAVKU_H
#define FRMDODAJSTAVKU_H

#include <QDialog>
#include <ui_frmdodajstavku.h>
#include <QtGui>
#include "ui/clsizracuni.h"
#include <QPlainTextEdit>
#include <QVBoxLayout>

namespace Ui {
class frmDodajStavku;
}

class frmDodajStavku : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmDodajStavku(QWidget *parent = 0);
    ~frmDodajStavku();
    Ui::frmDodajStavku *ui;
    int BrojStola;
    int NarID;
    int NarucioUID;
    int artID;
    double sBpdv;
    double sKolicina;
    double sJedcj;
    double sRabatp;
    double sRabatk;
    double sPPOcifra;
    double sSSUMA;
    double sPPOkoef;
    double sPDVkoef;
    double sPDVcifra;
    double sUkpCisti;
    int artGrupaID;
    int artIspisKuhinja;
    int Rac2ID;

private slots:

    void on_btnIzlaz_clicked();

    void on_btnUpisiStavku_clicked();

    void on_txtArtKolicina_textChanged(const QString &arg1);
    void on_btnUpisiStavku_pressed();

    void on_txtArtRabatP_textChanged(const QString &arg1);


    //void iRabat = "";

    void on_pushButton_3_pressed();
    void on_pushButton_5_pressed();
    void on_pushButton_4_pressed();
    void on_pushButton_6_pressed();
    void on_btnNazivPokazi_released();
    void IzmijeniNaziv();


    void on_btnUpisiStavku_released();

    void on_chkDostava_pressed();
    void chkDostavaProv();

public slots:
    void Suma();
    void slotRadimSto(QString StoRadim);

private:
    QString iRabat;
    QString RadimSto;
    bool NeDiraj;
    QPushButton *NoviNazivUpis;
    QPushButton *NoviNazivIzlaz;
    QPlainTextEdit *txtNoviNaziv;
    QWidget *NoviNazivProzor;
    QVBoxLayout *vboxLayout;
    clsIzracuni *IZRAC; // = new clsIzracuni();

signals:
    void StisnutOK(int Stol);
    void StisnutCancel();

protected:
     //virtual bool eventFilter(QObject *, QEvent *);

};

#endif // FRMDODAJSTAVKU_H
