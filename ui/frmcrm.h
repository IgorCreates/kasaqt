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




#ifndef FRMCRM_H
#define FRMCRM_H

#include <QDialog>
#include <QtGui>
#include <ui/frmcrmpregled.h>
#include <QtSql>

namespace Ui {
class frmcrm;
}

class frmcrm : public QDialog
{
    Q_OBJECT

public:
    explicit frmcrm(QWidget *parent = 0);
    ~frmcrm();
    void set_CRM_ID(int crmid)
    {
        _CRM_ID = crmid;
    }
    int get_CRM_ID()
    {
        return _CRM_ID;
    }
    void set_CRM_KupacID(int kupacid)
    {
        _CRM_KupacID = kupacid;
    }
    int get_CRM_KupacID()
    {
        return _CRM_KupacID;
    }
    void set_CRM_ProdavacID(int prodavacid)
    {
        _CRM_ProdavacID = prodavacid;
    }
    int get_CRM_ProdavacID()
    {
        return _CRM_ProdavacID;
    }
    void set_CRM_UnioID(int unioid)
    {
        _CRM_UnioID = unioid;
    }
    int get_CRM_UnioID()
    {
        return _CRM_UnioID;
    }
    void set_CRM_Opis(QString opis)
    {
        _CRM_OPIS = opis;
    }
    QString get_CRM_OPIS()
    {
        return _CRM_OPIS;
    }


private:
    Ui::frmcrm *ui;

    int _CRM_ID;
    int _CRM_KupacID;
    int _CRM_ProdavacID;
    int _CRM_UnioID;
    QString _CRM_OPIS;
    void CistiVarijable();
    void FirmaTrazi();
    void ProdavacTrazi();
    bool PridruziRacunCRMu;
    void PridruziRacunCRMfunkcija();
    void MakniRacunCRMfunkcija();
    QSqlQueryModel *modelPregled;
//    QSqlQueryModel modelDokumenti;


private slots:
    void Upis();
    void VratiID(QString StoVraca,int StoID,QList<QString> Lista);
    void UcitajPregledDolazaka();

    void on_dgPregledDolazaka_doubleClicked(const QModelIndex &index);
    void on_btnPridruziRacun();

    void on_btnUcitajPregDok_pressed();

    void on_dgPregledDolazaka_clicked(const QModelIndex &index);
    void ProvjeriVezuNaRacune(int crmid);

    void on_btnPregDodajVezuRacun_pressed();

    void on_btnPregMakniVezuRacun_pressed();

    void on_btnPregBrisiCRM_pressed();

    void on_dgPregDok_doubleClicked(const QModelIndex &index);

signals:
    void VratiCRMID(int crmid);

protected:
  virtual bool eventFilter(QObject *target, QEvent *event);
};

#endif // FRMCRM_H
