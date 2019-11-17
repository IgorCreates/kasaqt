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




#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QDialog>
#include <QtSql>

/*
extern QString Firma_Ime="DEMO";
extern QString Firma_Adresa="ADRESA";
extern QString Firma_PostBroj="10000";
extern QString Firma_Grad="ZAGREB";
extern QString Firma_OIB="123412312321";
extern QString Firma_Ostalo="DemoProgram";
extern QString Firma_ProdMjesto="P.J 1";
extern QString Firma_KasaID="Kasa1";
*/


namespace Ui {
class frmLogin;
}

class frmLogin : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmLogin(QWidget *parent = 0);
    void PromjenaKorisnikaP(const bool Promjena);

    ~frmLogin();

public slots:
    void slotPokaziKorisnike();
    void slotProvjeraLozinke();

    
private:
    Ui::frmLogin *ui;
    //QString dbBazaOpis = "";
    QSqlQueryModel *modK;
    void PustiDalje();
    void PromjenaBaza();
    bool PromjenaKorisnika;
    void SetDemoPostavke();
    bool Demo;
    QString LocalDBHost; //= "localhost";
    QString LocalDBUser; //= "kasauser";
    QString LocalDBPass; //= "KasaPasSwd";
    QString LocalDBBaza; // = "local";

public slots:
    void UcitajBazu();
    bool UcitajCryptoBazu();

protected:
  virtual bool eventFilter(QObject *, QEvent *);

private slots:
    void on_btnIzlaz_pressed();
};

#endif // FRMLOGIN_H
