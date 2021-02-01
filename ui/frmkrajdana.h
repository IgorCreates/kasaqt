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




#ifndef FRMKRAJDANA_H
#define FRMKRAJDANA_H

#include <QDialog>
//#include "staticlib/src/tdpreviewdialog.h"
#include <QtSql>
#include <QtGui>
#if QT_VERSION > 0x050000
#include <QtPrintSupport/QPrinter>
#endif
#include <QPlainTextEdit>
#include <QVBoxLayout>

namespace Ui {
class frmKrajDana;
}

class frmKrajDana : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmKrajDana(QWidget *parent = 0);
    ~frmKrajDana();
    
private slots:
     void on_btnIspisZakljucakKorisnik_pressed();

     void on_pushButton_pressed();

     void on_pushButton_2_pressed();

     void on_btnIspisStanjeKase_pressed();

     void on_btnUzimanjeGotovine_released();
     void uzimanjeGotovineSlot();

     void on_btnPrikaziDatum_released();

     void on_btnIspisStanjeKase_2_released();

     void on_btnIspisStanjeKaseVeliki_released();

private:
    Ui::frmKrajDana *ui;
    void UcitajStanja();
    //TDPreviewDialog::Grids grid;
    QPrinter *printer;
    QSqlQueryModel *model;
    QSqlQueryModel *modelKorisnik;
    QSqlDatabase dbp;

    QPlainTextEdit *UzimanjeGotovineOpis;
    QLineEdit *UzimanjeGotovineIznos;
    QDialog *dialogUzimanjeGotovineGotovine;
    QPushButton *UzimanjeGotovineUpis;
    QPushButton *UzimanjeGotovineIzlaz;
    QVBoxLayout *vboxLayout;
    QFuture<void> *future;
    QFutureWatcher<void> *watcher;

};

#endif // FRMKRAJDANA_H
