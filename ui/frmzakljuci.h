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




#ifndef FRMZAKLJUCI_H
#define FRMZAKLJUCI_H

#include <QDialog>
#include "ui/frmpopup_preg.h"
#include <QtGui>
#include <QtSql>
#include "ui/clsizracuni.h"
#include <QWidget>
#include <QPlainTextEdit>
#include <QTableView>
#include <QVBoxLayout>

namespace Ui {
class frmZakljuci;
}

class frmZakljuci : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmZakljuci(QWidget *parent = 0);
    ~frmZakljuci();
    int BrojStola;
    void ucSuma();
    void Suma();
    void ucSumaNN();
    void SumaNN();
    void BrziRacGotovina();
    QString TipRacuna;
    int _CRMID;


public slots:
    //void StoKlinut(const QString &Sto);
    void StoStisnuto(const QString &Sto);
    bool ProvjeraBrRacuna();
    void slotUpisiRaucn();
    void slotF2_M();
    void slotF2_V();
    void slotF3();
    void slotESC();
    void slotF11();
    void slotF12();

private slots:
    void on_btnRacM_clicked();
    void on_btnRacV_clicked();
    void NT_Nplacanja(const QModelIndex &StoDeb);
    void NT_Kupci(const QModelIndex &StoDeb);
    void on_btnIzlaz_pressed();
    void on_btnPonuda_pressed();

    void on_btnRacOtpremnica_pressed();
    void Funkcija_Napomena();
    void DodajOpis();
    void slotOdabranNP();
    void slotOdabranKupac();




    void on_btnNapomena_released();

    void on_btnNapomena_2_released();

private:
    Ui::frmZakljuci *ui;
    frmPopUP_Preg *fpopup;
    double sBPDVc;
    double sBPDVp;
    double sSUMAc;
    double sSUMAp;
    double sRabatP;
    double sRabatK;
    double sPLACENO;
    double sZaVratiti;
    int IDNacinPlacanja;
    QWidget *nw;
    QTableView *ntNP;
    QTableView *ntKU;
    QSqlQueryModel *nmodNP;
    QSqlQueryModel *nmodKU;
    void BrisanjeDokNaCekanju();
    int KupacID;
    QString StoSprema;
    QString OpisPolje;
    QStandardItemModel *mod;

    QPushButton *OpisUpis;
    QPushButton *OpisIzlaz;
    QPlainTextEdit *txtOpis;
    QWidget *OpisProzor;
    QVBoxLayout *vboxLayout;
    clsIzracuni *IzracRac;
    void IspisRac(int racID);
    void RabatUKP_Suma();



protected:
     virtual bool eventFilter(QObject *, QEvent *);
};

#endif // FRMZAKLJUCI_H
