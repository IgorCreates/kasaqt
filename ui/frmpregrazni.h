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




#ifndef FRMPREGRAZNI_H
#define FRMPREGRAZNI_H

#include <QDialog>
#include "ui/frmkupac.h"
#include <QtGui>
#include <QtSql>

namespace Ui {
class frmpregrazni;
}

class frmpregrazni : public QDialog
{
    Q_OBJECT

public:
    explicit frmpregrazni(QWidget *parent = 0);
    ~frmpregrazni();
    void set_StoID(int id)
    {
        _StoID = id;
    }
    int get_StoID()
    {
        return _StoID;
    }
    void set_Qveri(QString qver)
    {
        qveri = qver;
    }
    QString get_Qveri()
    {
        return qveri;
    }

    QString _frmPregledSto;
    void UcitajFormu();

signals:
    void VratiStoTrazi(QString StoVracam,int StoID,QList<QString> StoLista);

private:
    Ui::frmpregrazni *ui;
    int _StoID;
    QString qveri;
    QSqlQueryModel *model;




private slots:
    void PonovnoUcitajGrid(int CurrentID);
    void Vrati();
    void GumbicNovi();
    void PozoviFormuKupac(bool Novi);

protected:
  virtual bool eventFilter(QObject *target, QEvent *event);

};

#endif // FRMPREGRAZNI_H
