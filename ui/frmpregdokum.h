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




#ifndef FRMPREGDOKUM_H
#define FRMPREGDOKUM_H

#include <QDialog>
#include <QtSql>
#include <QtGui>
#if QT_VERSION > 0x050000
#include <QtWidgets>
#endif

namespace Ui {
class frmPregDokum;
}

class frmPregDokum : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmPregDokum(QWidget *parent = 0);
    ~frmPregDokum();
private slots:

    void on_toolBtnStorno_pressed();

    void on_toolBtnIspis_pressed();

    void on_btnPrikaziNeposlane_pressed();

    void on_btnSaljiPonovno_pressed();


    void on_btnZKI_pressed();


    void on_btnRekreiranjeZKI_pressed();

    void on_toolBtnIspis_2_pressed();

    void on_btnPrikaziNeposlane_3_released();

    void on_btnPrikaziNeposlane_2_released();

    void on_btnPrikaziStavke_released();

    void on_toolBtnPretraga_pressed();

    void on_btnFilter_pressed();

    void on_btnFilterZatvori_pressed();

    void on_btnFilterLjevo1000_released();

    void on_btnFilterDesno1000_released();

    void on_btnKreirajXRacuna_pressed();

    void on_toolBtnIspisOdDo_pressed();

    void on_btnIzlaz_released();

    void on_btnPrikazNarudzbe_clicked(bool checked);

    void on_btnRekreirajJson_pressed();

private:
    void UcitajRacune();
    void UcitajVRacune();

    Ui::frmPregDokum *ui;
    void UcitajHeader();
    void UcitajDetail();
    void UcitajNeposlane();
    void UcitajNeZKI();
    QSqlQueryModel *modelHeader;
    QSqlDatabase dbH; // = QSqlDatabase::database("baza");

    QWidget *PregStavke;
    QVBoxLayout *vbLntPregStavke;
    QTableView *ntPregStavke;
    QSqlQueryModel *nmodPregStavke;
    QSortFilterProxyModel *filter_model;
    QLineEdit *FilterPolje;
    int MaxID;
    int MinID;
    bool PregNarudzbe;

protected:
  virtual bool eventFilter(QObject *, QEvent *);
};

#endif // FRMPREGDOKUM_H
