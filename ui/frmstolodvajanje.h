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




#ifndef FRMSTOLODVAJANJE_H
#define FRMSTOLODVAJANJE_H

#include <QDialog>
#include <QtGui>
#include <QtSql>


namespace Ui {
class frmstolodvajanje;
}

class frmstolodvajanje : public QDialog
{
    Q_OBJECT

public:
    explicit frmstolodvajanje(QWidget *parent = 0);
    ~frmstolodvajanje();
    int DokBR;
    void UcitajFormu();


private:
    Ui::frmstolodvajanje *ui;
    void UcitajStol();
    void UcitajStolNovi();
    void IzracunajDG1();
    void IzracunajDG2();
    void PrebaciStavku();
    void VratiStavku();
    void PrebaciStavkuPlus();

private slots:
    void on_btnPlus();
    void on_btnMinus();
    void on_btnPrebaciStavku();
    void on_btnVratiStavku();
    void on_btnIzlaz();
};

#endif // FRMSTOLODVAJANJE_H
