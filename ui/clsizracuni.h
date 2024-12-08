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




#ifndef CLSIZRACUNI_H
#define CLSIZRACUNI_H

#include <QObject>

#include <QtGui>
#include <QtSql>
#include <QString>


class clsIzracuni : public QObject
{
    Q_OBJECT
public:

    explicit clsIzracuni(QObject *parent = 0);

    QStandardItemModel *stavkaListaPoreza;// = QStandardItemModel(0,5);

    void DefVrijednostVarijabli();

    void izracunMPC();
    void izracunVPC();
    bool UpisStavkeUBazu();
    bool UcitajArt();

    bool izracunRAC();
    bool UcitajLocalRac();
    bool UpisiRac();
    bool UcitajLocalSUMrac();

    //Dohvat sifre artikla prema barcodu
    bool FindArtiklSifraByBarcode(QString barcode, QString& sifraArtiklaOUT);

    //Dohvat sifre artikla prema nazivu
    bool FindArtiklSifraByName(QString nameStartWith, QString& sifraArtiklaOUT);

    //Dohvat sifre artikla prema nazivu
    bool FindArtiklSifraBySifra(QString sifraArtikla, QString& sifraArtiklaOUT);

    //Dohvat sifre artikla prema ID artikla
    bool FindArtiklSifraByArtiklID(int ArtiklID, QString& sifraArtiklaOUT);

    //setovi stavka
    void set_RacNarID(int narid)
    {
        _RacNarID = narid;
    }

    void set_StavkaKol(double kol)
    {
        _stavkaKolicina = kol;
    }
    void set_StavkaPC(double pc)
    {
        _stavkaPC = pc;
    }
    void set_StavkaRabatP(double rp)
    {
        _stavkaRabatP = rp;
    }
    void set_StavkaRabatK(double rk)
    {
        _stavkaRabatK = rk;
    }
    void  set_StavkaAID(int aid,QString artsif)
    {
        _stavkaAID = aid;
        _stavkaArtSIF = artsif;
    }
    void set_StavkaArtNaziv(QString artnaz)
    {
        _stavkaArtNaziv = artnaz;
    }
    void set_StavkaPodaci(int brstola,int artgrupaid,int artispiskuhinja,int rac2id,int narid)
    {
        _stavkaArtGrupaID = artgrupaid;
        _stavkaArtIspisKuhinja = artispiskuhinja;
        _stavkaBrStola = brstola;
        _Rac2ID_Stari = rac2id;
        _RacNarID = narid;
    }
    void set_StavkaPovratnaNaknada(double povratna)
    {
        _stavkaPovratnaNaknada = povratna;
    }
    void set_StavkaNarucioUID(int narucio_id)
    {
        _stavkaNarucioUID = narucio_id;
    }

    //getovi stavka
    double get_stavkaBPDV()
    {
        return _stavkaBPDV;
    }
    double get_stavkaBPDVcista()
    {
        return _stavkaBPDVcista;
    }
    double get_stavkaPC()
    {
        return _stavkaPC;
    }
    double get_stavkaPCCista()
    {
        return _stavkaPCcista;
    }
    double get_stavkaUkupno()
    {
        return _stavkaUkupno;
    }
    double get_stavkaUkupnoCista()
    {
        return _stavkaUkupnoCista;
    }
    double get_stavkaRabatP()
    {
        return _stavkaRabatP;
    }
    double get_stavkaRabatK()
    {
        return _stavkaRabatK;
    }
    double get_stavkaPovratnaNaknada()
    {
        return _stavkaPovratnaNaknada;
    }
    QString get_stavkaNaziv()
    {
        return _stavkaArtNaziv;
    }
    int get_stavkaAID()
    {
        return _stavkaAID;
    }
    int get_stavkaArtGrupaID()
    {
        return _stavkaArtGrupaID;
    }
    int get_stavkaIspisKuhinja()
    {
        return _stavkaArtIspisKuhinja;
    }



    int get_RacNarID()
    {
        return _RacNarID;
    }

    //setovi RacH
    void set_RacRabatP(double rb)
    {
        _RacRabatP = rb;
    }
/*
    void set_RacLocalDOK(int dok)
    {
        _RacLocalDOK = dok;
    }
    */
    void set_RacStol(int stol)
    {
        _RacStol = stol;
    }

    void set_RacKupac(int kID = 0,QString kNaz ="",QString kAdre = "",QString kOib = "",QString kMjesto = "",QString kSifra = "",QString kMB= "",QString kZemlja="")
    {
        _RacKupacAdresa = kAdre;
        _RacKupacMB = kMB;
        _RacKupacMjesto = kMjesto;
        _RacKupacNaziv = kNaz;
        _RacKupacOIB = kOib;
        _RacKupacSifra = kSifra;
        _RacKupacID = kID;
        _RacKupacZemlja = kZemlja;
    }
    void set_RacNP(int rnp)
    {
        _RacIDNacinPlacanja = rnp;
    }
    void set_RacVrstaRac(QString vrst)
    {
        _RacVrstaRac = vrst;
    }
    void set_RacTiprac(QString tip)
    {
        _RacTipRacuna = tip;
    }
    void set_RacOpisPolje(QString opis)
    {
        _RacOpisPolje = opis;
    }
    bool set_PorezDostava(bool por)
    {
        _PorezDostava = por;
    }


    void set_StavkaNarudzba(int narudzba)
    {
        _stavkaNarudzba = narudzba;
    }


    //getovi RacH
    int get_Rac1ID()
    {
        return _Rac1ID;
    }

    double get_RacBPDV()
    {
        return _RacBPDV;
    }
    double get_RacBPDVCisti()
    {
        return _RacBPDVCisiti;
    }
    double get_RacSUM()
    {
        return _RacSUM;
    }
    double get_RacSumCisti()
    {
        return _RacSUMCisti;
    }
    double get_RacRabatK()
    {
        return _RacRabatK;
    }
    double get_RacSumPovratnaNaknada()
    {
        return _RacSumPovratnaNaknada;
    }
    QString get_RacOpisPolje()
    {
        return _RacOpisPolje;
    }
    bool get_PorezDostava()
    {
        return _PorezDostava;
    }
    void set_CRMID(int crmid)
    {
        _crmid = crmid;
    }
    int get_CRMID()
    {
        return _crmid;
    }
    int get_stavkaNarucioUID()
    {
        return _stavkaNarucioUID;
    }
    int get_stavkaNarudzba()
    {
        return _stavkaNarudzba;
    }

private:
//stavka
    int _stavkaAID;
    int _Rac2ID_Stari;
    QString _stavkaArtSIF;
    QString _stavkaArtNaziv;
    int _stavkaArtGrupaID;
    int _stavkaArtIspisKuhinja;
    int _stavkaKorisnikID;
    int _stavkaBrStola;
    double _stavkaBPDV;
    double _stavkaBPDVcista;
    double _stavkaSumPorezi;
    double _stavkaSumPoreziCista;
    double _stavkaPCcista;
    double _stavkaPC;
    double _stavkaNC;
    double _stavkaUkupno;
    double _stavkaUkupnoCista;
    double _stavkaRabatP;
    double _stavkaRabatK;
    double _stavkaKolicina;
    double _stavkaPovratnaNaknada;
//racunH
    int _Rac1ID;
    //char _RacVrsta;
    int _RacKupacID;
    double _RacSUM;
    double _RacSUMCisti;
    double _RacBPDV;
    double _RacBPDVCisiti;
    int _RacUID;
    char _RacObracun;
    int _RacStol;
    int _RacNP;
    int _RacStorno;
    double _RacRabatP;
    double _RacRabatK;
    int _RacLocalDOK;
    double _RacSumPorezi;
    double _RacSumPoreziCista;
    double _RacSumPovratnaNaknada;
    QString _RacTipRacuna;
    QString _RacKupacOIB;
    QString _RacKupacSifra;
    QString _RacKupacAdresa;
    QString _RacKupacNaziv;
    QString _RacOpisPolje;
    int _RacIDNacinPlacanja;
    QString _RacKupacMB;
    QString _RacKupacMjesto;
    QString _RacKupacZemlja;
    QString _RacVrstaRac;
    int _RacNarucioUID;
    int _RacNarID;

    int _crmid;
    void UpdateCRMrac();
    bool _PorezDostava;
    int _stavkaNarucioUID;
    int _stavkaNarudzba;

    

signals:
    
public slots:
    
};

#endif // CLSIZRACUNI_H
