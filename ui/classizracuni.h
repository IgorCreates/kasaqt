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




#ifndef CLASSIZRACUNI_H
#define CLASSIZRACUNI_H
#include <QString>

class classIzracuni
{
public:
    classIzracuni();
    bool DodajStavku(QString ArtSifra,int aAID, int RACID, int BrStola,const double Kolicina);
    bool UpisiBrziRacun(int BrStola);
    bool NoviUpisRacun(int BrLokalniRac);

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
    double sPC;
    double sUkpPorezi;
    void izracunMPC();
    void izracunVPC();



    double setPC(double pc)
    {
        sPC = pc;
    }
    double setKol(double kol)
    {
        sKolicina = kol;
    }
    double setRabatP(double rbtp)
    {
        sRabatp = rbtp;
    }
    double setRabatK(double rbtk)
    {
        sRabatk = rbtk;
    }
    double getBPDV(void)
    {
        sBpdv = ((sKolicina*sPC)-sRabatk);
        return sBpdv;
    }

private:
    int BrojStola;
    int artID;
    int Rac2ID;
    QString iRabat;
    QString RadimSto;
    bool NeDiraj;
    void Suma();


};

#endif // CLASSIZRACUNI_H
