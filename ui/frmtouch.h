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




#ifndef FRMTOUCH_H
#define FRMTOUCH_H

#include <QMainWindow>
#include <QWidget>
#include "ui/frmtipkovnica.h"
#include <QDockWidget>
#include <QtGui>
#include "ui/clsizracuni.h"
#include <QMutex>


//#include <QListWidget>
#if QT_VERSION > 0x050000
#include <QtWidgets>
#endif

namespace Ui {
class frmtouch;
}

class frmtouch : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmtouch(QWidget *parent = 0);
    QString constStoPise;
    ~frmtouch();

private slots:
    void on_btnBrowse_released();
    //on_pushButton_3_released()
    void UcitajLocal();
    void txtStol_Tipka(QString Tipka); //(Qt::Key Tipka);
    void VratiTipku(QString Tipka);
//    void VratiTipkuGrupa(int GrupaID);
    void UcitajArtikle(int grupaID);
    void UcitajGrupe();

    void on_btnStolClear_released();

    void on_btnLogout_released();
    void UcitajSankove();
    void UcitajGumberNormalanGui();
    void NaruciSank(int SankID);
    void UcitajListRac2(int dok);

    void on_listRac_itemClicked(QListWidgetItem *item);

    void on_btnStolOdabir_released();
    void EditStavke(int r2id);
    void BrisiStavku(int r2id);

    void on_splitter_splitterMoved(int pos, int index);

    void on_btnMeni_released();

    void on_pushButton_pressed();
    void slotBtnBrziRacun();
    void slotBtnKrajSmjene();
    void SamoZakljucajTimerReset();
    void slotOdjava();
    void SamoZakljucavanje(int Vrijeme=0);
    bool ProvjeraKorisnikStol();
    void UcitajNarudzbe();

private:
    Ui::frmtouch *ui;
    frmTipkovnica *frmTipk;
    QDockWidget *dw;
    void UcitajFormu();
    //bool KreirajWidget(int artID,QString artNaziv,double artPC,QString artGrupa,QString artJMJ,QString artSlikaPath);
    QPushButton* KreirajGumbicArt(int artID,QString artSif, QString artNaziv, double artPC, QString artGrupa, QString artJMJ, QString artSlikaPath, QWidget *parent);
    //opis             | glavna | ispis_kuhinja | statisticka
    QWidget* KreirajGumbicGrupa(int grpID, QString grpOpis, int grpGlavna, int grpIspisKuhinja, QString grpStatisticka, QWidget *parent);
    bool UcitajOpcije();
    QGridLayout *desniGridLay;
    QListWidgetItem *itemArt;// = new QListWidgetItem();
    QSignalMapper *signalMapperArt;
    QSignalMapper *signalMapperGrupa;
    QSignalMapper *signalMapperNaruciGumbic;
    QSignalMapper *signalMapperArtBrisi;
    QSignalMapper *signalMapperArtEdit;
    void SplitterSetSizeKorisnik();
    void UcitajSankBazu();
    void prikaziSkladCjenik();
    bool ProvjeriStolVlasnik(int BrDokumenta);
    QMutex mutex;
    QTimer *SamoZakljucajTimer;
    bool ProvjeriDaLiJeDokumentPrazan(int BrojDokumenta, int SankID);




protected:
  virtual bool eventFilter(QObject *, QEvent *);

signals:
    void clicked(QString Tipka);
    void clickedGrupa(int Grupa);

};

#endif // FRMTOUCH_H
