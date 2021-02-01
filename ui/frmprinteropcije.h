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




#ifndef FRMPRINTEROPCIJE_H
#define FRMPRINTEROPCIJE_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class frmPrinterOpcije;
}

class frmPrinterOpcije : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmPrinterOpcije(QWidget *parent = 0);
    ~frmPrinterOpcije();
    
private slots:
    void UcitajTable();
    void CleanPolja();
    void on_pushButton_pressed();

    void on_btnUpisIspisPostavke_2_pressed();
    void UcitajOpcije();

    void on_cmbPrinter_currentIndexChanged(int index);

    //void on_tableView_pressed(const QModelIndex &index);

    void on_btnUpis_pressed();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_btnBrisi_2_pressed();

    void on_btnBrisi_pressed();

private:
    Ui::frmPrinterOpcije *ui;

};

#endif // FRMPRINTEROPCIJE_H
