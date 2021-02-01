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




#ifndef MOJIHEADERI_H
#define MOJIHEADERI_H
#include <QItemDelegate>
#include <QString>
#include <QDateTime>
#include <QDomDocument>
//#include <QtNetwork/QNetworkReply>

class mojiHeaderi: public QItemDelegate
{
    Q_OBJECT





public:
    mojiHeaderi();
    mojiHeaderi(QObject *parent);
    //QString ZKI(QString OIB,QDateTime DatumRacuna,QString BrRacuna,QString OzPoslProstora,QString OzNaplatniUredjaj,QString UkpIznosRacuna);
//    QString ZKI(QString zkOIB,QString zkDatumRac,QString zkBrojRacuna,QString zkOznPoslProstora,QString OzNaplUredjaja,QString UkpIznosracuna);
    bool ZKI(int RacID);

    bool xmlRacunZahtjev(const int RacunID,bool NaknadnoSlanje);
    bool jsonRacunZahtjev(const int RacunID,bool NaknadnoSlanje);
    void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const;
    bool UcitajXML_PovratniRacun(QString xmlFileFullPath, QString& jirValue, QString& errValue);
    QString xmlPoslProstorZahtjev(QString ppOib,QString ppOznPoslProstora,QString ppUlica,QString ppKucniBroj,QString ppKucniBrojDodatak
                               ,QString ppBrojPoste,QString ppNaselje,QString ppOpcina,QString ppRadnoVrijeme,QString ppDatumPocetkaPrimijene
                               ,QString ppSpecNamj,QString ppOstaliTipoviPP, QString ppOznakaZatvaranja, bool ppAdresa);

    double DecTocka(const double BrojZaDec);


private:
    QDomText xmlPod(QString Sto);
};

#endif // MOJIHEADERI_H
