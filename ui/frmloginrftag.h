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




#ifndef FRMLOGINRFTAG_H
#define FRMLOGINRFTAG_H

#include <QDialog>

namespace Ui {
class frmloginrftag;
}

class frmloginrftag : public QDialog
{
    Q_OBJECT

public:
    explicit frmloginrftag(QWidget *parent = 0);
    ~frmloginrftag();


public slots:
    void StisnutEnter(QString TxtPolje);

private:
    Ui::frmloginrftag *ui;
    void UcitajFirmaInfo();

protected:
  //virtual bool eventFilter(QObject *, QEvent *);

private slots:
    void on_txtKorisnik_returnPressed();
};

#endif // FRMLOGINRFTAG_H
