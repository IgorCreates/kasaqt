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




#ifndef FRMKASA_H
#define FRMKASA_H

#include <QMainWindow>
#include "ui_frmkasa.h"
#include <QWidget>
#include <QtGui>
#include <ui/frmpregart.h>
#include <ui/frmzakljuci.h>
#include <QMutex>
#include <ui/clsizracuni.h>
#include <ui/frmcrm.h>
#include <ui/frmzadnjiracpregled.h>


namespace Ui {
class frmKasa;
    QT_FORWARD_DECLARE_CLASS(QTableView)
}

class frmKasa : public QMainWindow
{
    Q_OBJECT

    
public:
    explicit frmKasa(QWidget *parent = 0);
    ~frmKasa();
    QString constStoPise;
    bool UcitavanjeForme;
    void SetLogRac(bool sto)
    {
        LogRac = sto;
    }
    bool GetLogRac()
    {
        return LogRac;
    }

public slots:
    void exec();
    void UcitajLocal();
    void UcitajStolove();
    void slotIzlazIzPrograma();
    void slotPregledRacuna();
    void slotPromijenaKonobara();
    void slotKrajSmjeneDana();
    void slotF4();
    void slotF12();
    void slotF2();
    void slotTest();
    void slotOdaberiStol();
    void slotDodajStol();
    void slotBrisiStol();
    void slotArtikli();
    void slotArtEdit();
    void slotBrziRacun();
    void UcitajSveNakonForme();
    //void slotSortColumn(int ar1,Qt::SortOrder ar2);
    void slotF8();
    void slotArtBrisiStavku();
    void prikaziSkladCjenik();
    void slotIspisStol(int StolBR);
    void slotUpitZaIspisStol();
    void porukaKreni(QString PrikaziPoruku, int  VrijemeTrajanja);
    void porukaStop();
    bool provjeraCjenika();
    void ispisDelStolova();
    bool ProvjeraKorisnikStol(); //dozvola za korisnika da kuca na stol/racun koji je otvorio neko drugi od prava 6>
    void SamoZakljucavanje(); //iz opcija Prog-SamoZakljucaj value je time u sec nakon koliko da pokrene slotF3
    void ProvjeraNePoslanihRacuna(); //provjera da li ima neposlanih racuna na poreznu
    bool ProvjeraPoruka(); //provjera tablice poruka i da li je dozvoljen rad ili ne


private:
    Ui::frmKasa *ui;
    frmZakljuci *fz;
    QAction *pItem;
    QSqlError err;
    QSqlDatabase dbLocal;
    void KliknutStol();
    bool constNastavakUcitavanja;
    void PosloziGrid();
    void KreirajMeniGumbice();
    QModelIndex TableStoloviIndexGdjeJe;
    bool UcitajOpcijeLocal();
    void UcitajPregledArt(QString StoTrazizm, bool grupa);
    QString PamtiStyle;
    bool brziRacunStoj;
    QMutex mutex;
    int BrziCounter;
    QLabel PorukaInfo;
    QTimer *SamoZakljucajTimer;
    bool IspisKuhinja;
    //int IspisKuhinjaStol;
    void SamoZakljucajTimerReset(); //resetira timer na pocetak nakon svakog eventa sa kompa
    bool ProvjeraKorisnikSifra();
    frmcrm *fcrm;
    //int _CRMID;
    //QVector< QMap<int, int> > _CRMID;
    QMap<int, int> _CRMID;
    void frmCRMLabelaPrikaz();
    bool ProvjeriStolVlasnik(int BrDokumenta,int NarID);
    QSqlQueryModel *modelArtikli;
    QStandardItemModel *StoloviModel;
    int OdabraniStol;
    int OdabraniNarID;
    bool LogRac;
    bool DozvolaBrisanjaArt();
    void tableStoloviDuploKliknut();
    QLabel *lblNarudzbaPoruka;
    frmZadnjiRacPregled *FormaZadnjiRacunPrikazi = new frmZadnjiRacPregled();




protected:
  virtual bool eventFilter(QObject *, QEvent *);

private slots:
    //void on_tableStolovi_clicked(const QModelIndex &index);
    //void on_frmKasa_onFocus();
    //void on_frmKasa_onFOcus();
    //void on_tableStolovi_doubleClicked(const QModelIndex &index);
    void on_tableStolovi_doubleClicked();
    void slotArtiklTrazi(QString artiklPart);
    void on_pushButton_pressed();
    void on_btnCRM_Pressed();
    void on_btnDokOdvajanje_Pressed();
    void CrmID(int crmid);
    void on_btnCRMMakniVezu_pressed();

    void on_chkStolSamoMoji_stateChanged(int arg1);
    void UpisArt(int aid,QString artsif,QString artnaziv,double artKol,
                      double artPC,double artRabat,double artPovratnaNaknada,int artGrupaID,int artIspisKuhinja);
    //AID,ArtSif,ArtNaziv,ArtKol,ArtPC,0,ArtPovratna,ArtGrupaID,ArtIspisKuhinja);
    void on_btnSnimiPostavke_released();
};

#endif // FRMKASA_H
