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




#ifndef FRMPREGART_H
#define FRMPREGART_H

#include <QDialog>
#include <QWidget>
#include <QtGui>
#include <QtSql>
#include "clsizracuni.h"
#include <QMutex>

//#include "ui/mojiHeaderi.h"

namespace Ui {
class frmPregart;
}

class frmPregart : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmPregart(QWidget *parent = 0);
    ~frmPregart();
    int BrojStola;
    int NarID;
    int NarucioUID;
    QString StoTrazim;
    QString GrupaTrazim;
    bool StalnoUpaljen;


public slots:
//    void exec();
    void UcitajArtove();
    void UcitajArtoveTop20();
    void stisnutEnter();

private:
    Ui::frmPregart *ui;
    QSqlQueryModel *model;
    void PosloziGrid();
    int grupeMaticna_GID;
    int grupeParent_GID;
    int grupeID;
    int grupeRB;
    QString grupeNaziv;
    void grupaBack();
    void UcitajGrupe();
    void grupeNapraviGumb(QString NazivGumba,int GrupaID,int ParentID,int GrupaRB,int MaticnaID);
    void grupeCistiLayout();
    QMap<QString, QString> *grupeLista;
    void UpisiDirektArt();
    QMutex mutex;


protected:
  virtual bool eventFilter(QObject *, QEvent *);

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);
    void slotGrupe(QString ImeGumba);

    void on_btnTop20_pressed();

signals:
    void VratiAID(int aid,QString artsif,QString artnaziv,double artKol,
                  double artPC,double artRabat,double artPovratnaNaknada,int artGrupaID,int artIspisKuhinja);
};

#endif // FRMPREGART_H
