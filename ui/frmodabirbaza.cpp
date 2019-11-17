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




#include "frmodabirbaza.h"
#include "ui_frmodabirbaza.h"
#include <QtGui>
#include <QtSql>
#include "frmlogin.h"
#include <QMessageBox>

frmOdabirBaza::frmOdabirBaza(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmOdabirBaza)
{
    ui->setupUi(this);
    UcitajLocalBaze();
    this->setFixedSize(this->width(),this->height());
    ui->tableView->installEventFilter(this);
    ui->frame->hide();
    this->setWindowTitle("Fiskal Kasa-Baze");
    //OdabirStoRadim = new QString();
}

frmOdabirBaza::~frmOdabirBaza()
{
    delete ui;
}
void frmOdabirBaza::UcitajLocalBaze()
{

    //dbLocal = QSqlDatabase::addDatabase("QSQLITE","local.sqlite");
    //dbLocal.setDatabaseName("local.sqlite");
    dbLocal = QSqlDatabase::database("local.sqlite");
    dbLocal.open();
    if (!dbLocal.open())
    {
        qDebug() << "\nGreska db-"+dbLocal.lastError().text()+"\n";
        qDebug() << err.ConnectionError;
        QMessageBox::warning(this, "Ne mogu otvoriti bazu", "greska Otvaranje konekcije: " + dbLocal.lastError().text());
    }
    //qDebug()<< QSqlDatabase::database().databaseName();
    model = new QSqlQueryModel();
    model->setQuery("select * from baze",dbLocal);
    ui->tableView->setModel(model);
    dbLocal.close();
    dbLocal.database().close();
    PosloziGrid();

}
void frmOdabirBaza::PosloziGrid()
{
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
    ui->tableView->setColumnWidth(5,400);

}

bool frmOdabirBaza::eventFilter(QObject *target, QEvent *event)
{
    QString Pomoc = "F1-Pomoc\nEnter-potrvrda\nDel-brisanje";
    QKeyEvent *keyEvent = (QKeyEvent *)event;
    if (event->type() == QEvent::KeyPress)
    {
        switch (keyEvent->key())
        {
            case Qt::Key_Delete:
            {
                int ret = QMessageBox::warning(this,"Baze" ,"Da li zelite obrisati bazu?",
                         QMessageBox::Yes | QMessageBox::Default,
                         QMessageBox::No | QMessageBox::Escape);
                if (ret == QMessageBox::Yes)
                {
                    QSqlDatabase dbl = QSqlDatabase::database("local.sqlite");
                    QSqlQuery q("",dbLocal);
                    if (!q.exec(QString("delete from baze where id=%1").arg(model->record(ui->tableView->currentIndex().row()).value("id").toString())))
                    {
                        QMessageBox::information(this,"Baze","Greska kod brisanja","OK");
                        qDebug() << "Greska kod brisanja";
                        qDebug() << q.executedQuery();
                    }else{
                        QMessageBox::information(this,"Baze","Obrisano","OK");
                    }
                    UcitajLocalBaze();
                }
                break;
            }
            case Qt::Key_Insert:
                ui->frameOdabir->hide();
                ui->frame->show();
                ui->frame->move(5,5);
                ui->txtBhostname->setFocus();
                OdabirStoRadim = "Novi";
                break;
            case Qt::Key_F1:
                Pomoc += "\nInsert-Dodavanje nove baze\n,F12-izmijena postojece";
                QMessageBox::information(this,"Pomoc " ,Pomoc,"OK");
                return true;
                break;
            case Qt::Key_F12:
                ui->frameOdabir->hide();
                ui->frame->show();
                ui->frame->move(5,5);
                ui->txtBdatabase->setText(model->record(ui->tableView->currentIndex().row()).value("baza").toString());
                ui->txtBhostname->setText(model->record(ui->tableView->currentIndex().row()).value("host").toString());
                ui->txtBpassword->setText(model->record(ui->tableView->currentIndex().row()).value("password").toString());
                ui->txtBusername->setText(model->record(ui->tableView->currentIndex().row()).value("username").toString());
                ui->txtBopis->setText(model->record(ui->tableView->currentIndex().row()).value("opis").toString());
                ui->lblBid->setText(model->record(ui->tableView->currentIndex().row()).value("id").toString());
                OdabirStoRadim ="Izmijena";
                break;
            case Qt::Key_Escape:
                qApp->exit();
                break;
            case Qt::Key_Enter:
            case Qt::Key_Return:
                //upis u local.sqlite i setiranje default baze
/*
                QSqlDatabase::addDatabase("MySQL","baza");
                QSqlDatabase::setDatabaseName(model->record(ui->tableView->currentIndex().row()).value("database").toString());
                QSqlDatabase::setHostName(model->record(ui->tableView->currentIndex().row()).value("host").toString());
                QSqlDatabase::setUserName(model->record(ui->tableView->currentIndex().row()).value("username").toString());
                QSqlDatabase::setPassword(model->record(ui->tableView->currentIndex().row()).value("password").toString());
  */
                UpdateBaze();
                this->close();
                break;
        }
    }
    return QWidget::eventFilter(target,event);
}
void frmOdabirBaza::UpdateBaze()
{

    QSqlDatabase dbLocal = QSqlDatabase::database("local.sqlite");
    QSqlQuery q("",dbLocal);
    q.exec("update baze set izabrana=0");
    if (!q.exec(QString("update baze set izabrana=1 where id=%1").arg(model->record(ui->tableView->currentIndex().row()).value("id").toString())))
    {
        qDebug() << "Greska upis";
        qDebug() << q.executedQuery();
        QMessageBox::information(this,"Baze","Greska kod upisa","OK");
    }
    dbLocal.close();
    QSqlDatabase dbBaza =  QSqlDatabase::addDatabase("QMYSQL","baza");
    dbBaza.setDatabaseName(model->record(ui->tableView->currentIndex().row()).value("baza").toString());
    dbBaza.setHostName(model->record(ui->tableView->currentIndex().row()).value("host").toString());
    dbBaza.setUserName(model->record(ui->tableView->currentIndex().row()).value("username").toString());
    dbBaza.setPassword(model->record(ui->tableView->currentIndex().row()).value("password").toString());
    emit UpdateBazeSignal();

    //w.PromjenaKorisnikaP(false);
//    frmLogin *wl = qobject_cast<frmLogin*>(this->parentWidget());
    //MainWindow *mdiparent = qobject_cast<MainWindow*>(this->parentWidget());
    //mdiparent->spawnOtherChild();
//    wl->PromjenaKorisnikaP(false);

}

void frmOdabirBaza::on_pushButton_pressed()
{
    if (OdabirStoRadim == "" )
    {
        return;
    }
    //QMessageBox::information(this,"E","SS","OK");
    int ret = QMessageBox::warning(this,"Baze" ,"Da li zelite upisati promijene?",
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::No | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
    {
        if (ui->txtBopis->text() == "" )
        {
            ui->txtBopis->setText(ui->txtBhostname->text() + "-" + ui->txtBdatabase->text());
        }
        QString SQLu="";
        //QMessageBox::information(this,"Baze",OdabirStoRadim,"OK");
        if (OdabirStoRadim == "Novi")
        {
            SQLu = QString("insert into baze (host,username,password,baza,opis) values('%L1','%L2','%L3','%L4','%L5')").arg(ui->txtBhostname->text()).arg(ui->txtBusername->text()).arg(ui->txtBpassword->text()).arg(ui->txtBdatabase->text()).arg(ui->txtBopis->text());
        }else if (OdabirStoRadim == "Izmijena")
        {
            SQLu = QString("update baze set host='%L2',username='%L3',baza='%L4',password='%L5',opis='%L6' where id=%1").arg(ui->lblBid->text()).arg(ui->txtBhostname->text()).arg(ui->txtBusername->text()).arg(ui->txtBdatabase->text()).arg(ui->txtBpassword->text()).arg(ui->txtBopis->text());
        }
        //dbLocal = QSqlDatabase::addDatabase("QSQLITE","local.sqlite");
        //dbLocal.setDatabaseName("local.sqlite");
        dbLocal = QSqlDatabase::database("local.sqlite");
        dbLocal.open();
        if (!dbLocal.open())
        {
            qDebug() << "\nGreska db-"+dbLocal.lastError().text()+"\n";
            qDebug() << err.ConnectionError;
            QMessageBox::warning(this, "Ne mogu otvoriti bazu", "greska Otvaranje konekcije: " + dbLocal.lastError().text());
        }
       // qDebug()<< QSqlDatabase::database().databaseName();
        QSqlQuery q("",dbLocal);
        if (!q.exec(SQLu))
        {
            qDebug() << "greska-UpisBaza";
            qDebug() << OdabirStoRadim;
            qDebug() << q.lastError();
            qDebug() << q.executedQuery();
            qDebug() << "greska-UpisBaza";
            dbLocal.close();
            QMessageBox::warning(0,"Greska u sql-u", "Greska kod upisa Baza");
        }
        dbLocal.close();
        UcitajLocalBaze();
        ui->frame->hide();
        ui->frameOdabir->show();
    }
}
