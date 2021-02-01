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




#ifndef BAZE_H
#define BAZE_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


static bool dbLocalConn()
{
    QSqlDatabase dbLocal;
    //dbLocal = QSqlDatabase::addDatabase("QSQLITE","dbLocal");
    dblocal = QSqlDatabase::database("local.sqlite");
    //dbLocal.setDatabaseName("local.sqlite");

    if (!dbLocal.open())
    {
        qDebug() << dbLocal.lastError();
        QMessageBox::critical(0,"Konekcija-problem","Doslo je do greske kod konekcije na bazu",QMessageBox::Ok);
        return false;
    }else
    {
        QSqlDatabase::database().open();
        return true;
    }

}



#endif // BAZE_H
