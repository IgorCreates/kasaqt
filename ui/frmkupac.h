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




#ifndef FRMKUPAC_H
#define FRMKUPAC_H

#include <QDialog>

namespace Ui {
class frmkupac;
}

class frmkupac : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmkupac(QWidget *parent = 0);
    bool NoviKupac;

    int KupacID;
    int VoziloID;
    void UcitajKupca(int Kupac_ID);
    void ChekirajForumIzgled();
    void set_PozvanaFormaIzvana(bool pozvana)
    {
        _PozvanaFormaIzvana = pozvana;
    }
    bool get_PozvanaFormaIzvana()
    {
        return _PozvanaFormaIzvana;
    }
    void set_TabEnabledVozila(bool vozila)
    {
        _TabEnabledVozila = vozila;
    }
    bool get_TabEnabledVozila()
    {
        return _TabEnabledVozila;
    }

    ~frmkupac();
signals:
    void ReloadGrida(int StoID);

private slots:
    void on_btnUpis_released();

    void on_btnVoziloUpis_released();

    void on_tabVozilo_selected(const QString &arg1);

    void on_btnVoziloIzmijena_released();
    void CistiPoljaVozilo();
    void UcitajVozilaTBLView();


private:
    bool NovoVozilo;
    Ui::frmkupac *ui;
    bool _PozvanaFormaIzvana;
    bool _TabEnabledVozila;

};

#endif // FRMKUPAC_H
