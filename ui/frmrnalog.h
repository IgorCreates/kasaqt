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




#ifndef FRMRNALOG_H
#define FRMRNALOG_H

#include <QDialog>
#include "QtSql"
#include "QtGui"
#include <QTextDocument>
#if QT_VERSION > 0x050000
#include <QtWidgets>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#endif

namespace Ui {
class frmrnalog;
}

class frmrnalog : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmrnalog(QWidget *parent = 0);
    ~frmrnalog();
    void IspisA4(int rnalogID);

private slots:
    void IspisPreview(QPrinter *printer);
    void on_pushButton_4_pressed();
    void on_pushButton_5_pressed();
    void NT_Vozila(const QModelIndex &StoDeb);
    void NT_Kupci(const QModelIndex &StoDeb);
    void NT_Stavke(const QModelIndex &StoDeb);
    void NT_Rnalozi(const QModelIndex &StoDeb);
    void NT_RnaloziZ(const QModelIndex &StoDeb);

    void TraziKupca();
    void TraziVozilo();
    void TraziStavku();
    void TraziRnalozi();
    void TraziRnalozZatvoreni();


    void on_btnStavkaDodaj_released();

    void on_btnStavkaBrisi_released();
    void UcitajStavke();
    void UcitajRnalog(const int RnalogID);
    bool ProvjeraRnalog();
    void CistiPoljaStavka();
    void CistiSvaPolja();
    void NapraviRacunKasa();
    void NapraviRacunBOF();

    void on_pushButton_8_released();

    void on_pushButton_9_released();

    void on_txtKupacInfo_lostFocus();

    void on_txtVoziloInfo_lostFocus();

    void on_pushButton_6_released();

    void on_pushButton_11_released();

    void on_pushButton_10_released();

    void on_pushButton_5_released();

    void on_btnNapraviRac_released();



private:
    Ui::frmrnalog *ui;
    int KupacID;
    int VoziloID;
    int RadniNalogBR;
    int RadniNalogID;
    int StavkaAID;
    bool RadniNalogNovi;
    bool RadniNalogEdit;

    QWidget *PntK;
    QWidget *PntV;
    QWidget *PntS;
    QWidget *PntR;
    QWidget *PntRZ;
    QVBoxLayout *vbLntK;
    QVBoxLayout *vbLntV;
    QVBoxLayout *vbLntS;
    QVBoxLayout *vbLntR;
    QVBoxLayout *vbLntRZ;

    QString StavkaArtSIF;
    QWidget *nw;
    QTableView *ntK;
    QTableView *ntV;
    QTableView *ntS;
    QTableView *ntR;
    QTableView *ntRZ;
    QSqlQueryModel *nmod;
    QSqlQueryModel *modelStavke;

    QWidget *NapraviRacunProzor;
    QPushButton *btnNapraviRacunKasa;
    QPushButton *btnNapraviRacunBOF;
    QPushButton *btnNapraviRacunIZLAZ;
    QVBoxLayout *NapraviRacunVboxLay;
    QTextDocument *DokumentZaIspisA4;
    void EditStavke();
    void BrisiStavku();

protected:
     virtual bool eventFilter(QObject *, QEvent *);
};

#endif // FRMRNALOG_H
