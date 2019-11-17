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




#ifndef FRMOPCIJE_H
#define FRMOPCIJE_H

#include <QDialog>

namespace Ui {
class frmOpcije;
}

class frmOpcije : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmOpcije(QWidget *parent = 0);
    ~frmOpcije();
    
private slots:
    void on_btnPrinteriUpis_pressed();

    void on_btnCertifikatiUpis_pressed();

    void on_btnPologUpis_pressed();

    void on_btnUpisDokNaCekanju_pressed();

    void on_btnUpisOpcijeProg_pressed();

    void on_cmbSklad_currentIndexChanged(int index);

    //void on_pushButton_pressed();

    void on_btnPoslovniProstor_pressed();

    void on_btnESCpostavke_pressed();


    void on_btnUpisOpcijeMail_released();

private:
    Ui::frmOpcije *ui;
    void RefreshOpcije();
    void UcitajOpcije();
    void ProvjeraPravila();
};

#endif // FRMOPCIJE_H
