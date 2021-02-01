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




#ifndef FRMRAZDUZISKLAD_H
#define FRMRAZDUZISKLAD_H

#include <QDialog>

namespace Ui {
class frmrazduziSklad;
}

class frmrazduziSklad : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmrazduziSklad(QWidget *parent = 0);
    ~frmrazduziSklad();
    int SID;
    QString DokTIP;
    int DokID;
    void UcitajFRM(const QString DokOpis,const int SkladID,const QString Doktip,const int Dokid);

private slots:
    void on_btnRazduziSklad_released();

private:
    Ui::frmrazduziSklad *ui;
};

#endif // FRMRAZDUZISKLAD_H
