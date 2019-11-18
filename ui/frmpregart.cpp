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




#include "frmpregart.h"
#include "ui_frmpregart.h"
#include <QtGui>
#include <QtSql>
//#include "connection.h"
#include <QTextStream>
#include "ui/frmdodajstavku.h"
#include "mojiheaderi.h"
#include <QMessageBox>
#include "ui/clsizracuni.h"


//#include "ui_frmdodajstavku.h"

frmPregart::frmPregart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPregart)
{
    ui->setupUi(this);
    ui->tableView->installEventFilter(this);
    //UcitajArtove();
    this->setWindowTitle("Fiskal Kasa");
    grupeRB = 0;
    grupeMaticna_GID = 0;
    grupeParent_GID =0;
    grupeID = 0;
    NarID =0;
    NarucioUID=0;
    ui->btnGrupeBack->installEventFilter(this);
    grupeLista = new QMap<QString,QString>;
    UcitajGrupe();
    StalnoUpaljen = false;
}

frmPregart::~frmPregart()
{
    delete ui;
}
void frmPregart::grupeCistiLayout()
{
    for (int i=0;i<ui->scrollLay->count();i++)
    {
        if (ui->scrollLay->itemAt(i)->widget()->objectName().mid(0,2) == "g-")
        {
            ui->scrollLay->itemAt(i)->widget()->removeEventFilter(this);
            ui->scrollLay->itemAt(i)->widget()->deleteLater();
            ui->scrollLay->itemAt(i)->widget()->setHidden(true);
            //delete ui->scrollLay->itemAt(0);
        }
    }

}

void frmPregart::grupaBack()
{
    if (grupeRB == 0)
    {
        grupeCistiLayout();
        UcitajGrupe();
        GrupaTrazim = "";
        UcitajArtove();
        return;
    }
    if (grupeParent_GID == 0 )
    {
        grupeCistiLayout();
        UcitajGrupe();
        GrupaTrazim = "";
        UcitajArtove();
        return;
    }
    grupeRB = grupeRB-1;
    //qDebug() << "back-RB-GID-MID-PID: " << grupeRB << "-" << grupeID << "-" << grupeMaticna_GID << "-" << grupeParent_GID;
    QSqlQuery q("",QSqlDatabase::database("baza"));
    if (!q.exec(QString("select gn.rb,gn.maticna_grid,gn.parent_grid,gn.grupa_id,g.opis "
     "from grupe_norm gn left join grupe g on gn.grupa_id=g.id where gn.maticna_grid=%1 and gn.rb=%2 and gn.parent_grid=%3")
                .arg(grupeMaticna_GID).arg(grupeRB).arg(grupeParent_GID)))
    {
        qDebug() << "Greska - grupa back";
        return;
    }
    //qDebug() << "gumb back:" << q.lastQuery();
    grupeCistiLayout();
    //QString NazivGumba, int GrupaID, int ParentID, int GrupaRB, int MaticnaID
    grupeLista->clear();
    while (q.next())
    {
        //grupeNapraviGumb(q.value(q.record().indexOf("opis")).toString(),q.value(3).toInt(),q.value(2).toInt(),grupeRB,q.value(1).toInt());
        grupeNapraviGumb(q.value(q.record().indexOf("opis")).toString(),
                         q.value(q.record().indexOf("grupa_id")).toInt(),q.value(q.record().indexOf("parent_grid")).toInt(),
                         grupeRB,q.value(q.record().indexOf("maticna_grid")).toInt());
    }
    ui->scrollArea->setMaximumHeight(85);
    //ui->lblGrupaParent->setText(Lista[4]);
    ui->lblGrupaParent->setText(grupeNaziv);
    //ui->lblGrupaParent->setText("");
#if QT_VERSION < 0x050000

    ui->scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 20px; }");
#endif

    GrupaTrazim = QString("grupa-%1").arg(grupeParent_GID);
    UcitajArtove();


}
void frmPregart::slotGrupe(QString ImeGumba)
{
    QSqlQuery q("",QSqlDatabase::database("baza"));

//    QStringList Lista = ImeGumba.split("-",QString::SkipEmptyParts);
//    qDebug() << "Imegumba-" << ImeGumba;
//    qDebug() << "U listi-" << grupeLista->value(ImeGumba);
//    qDebug() << "GrLista cnt: " << grupeLista->count();
/* debug da vidim sto je u listi
    foreach (QString e, grupeLista->keys())
    {
      qDebug() << e << "," << grupeLista->value(e);
    }
*/
    QStringList Lista = grupeLista->value(ImeGumba).split(";",QString::SkipEmptyParts);
    grupeMaticna_GID =  Lista[4].toInt();
    grupeParent_GID = Lista[1].toInt();
    grupeID = Lista[0].toInt();
    grupeNaziv = Lista[3];
//QString("%1-%2-%3-%4-%5").arg(GrupaID).arg(ParentID).arg(GrupaRB).arg(NazivGumba).arg(MaticnaID)
    GrupaTrazim = QString("grupa-%1").arg(grupeID);
    UcitajArtove();

   // qDebug() << "slot-GRB-GrupaID-GrupaMaticnaID-GrupaParentID: " << grupeRB << "-"  << grupeID << "-" << grupeMaticna_GID << "-" << grupeParent_GID;
    if (!q.exec(QString("select gn.rb,gn.maticna_grid,gn.parent_grid,gn.grupa_id,g.opis "
     "from grupe_norm gn left join grupe g on gn.grupa_id=g.id where gn.maticna_grid=%3 and gn.rb=%1 and gn.parent_grid=%2").arg(grupeRB).arg(grupeID).arg(grupeMaticna_GID)))
    {
        qDebug() << q.lastError() << q.lastQuery();
        return;
    }
    //qDebug() << q.lastQuery();
    grupeCistiLayout();
    grupeRB=grupeRB+1;
//QString NazivGumba, int GrupaID, int ParentID, int GrupaRB, int MaticnaID
    grupeLista->clear();
    while (q.next())
    {
        grupeNapraviGumb(q.value(q.record().indexOf("opis")).toString(),
                         q.value(q.record().indexOf("grupa_id")).toInt(),q.value(q.record().indexOf("parent_grid")).toInt(),
                         grupeRB,q.value(q.record().indexOf("maticna_grid")).toInt());
    }
    ui->scrollArea->setMaximumHeight(85);
    ui->lblGrupaParent->setText(grupeNaziv);
#if QT_VERSION < 0x050000

    ui->scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 20px; }");
#endif

}
void frmPregart::UcitajGrupe()
{
   // qDebug() << "UcitajGrupe";
    QSqlQuery q("",QSqlDatabase::database("baza"));

    if (!q.exec("select id,opis from grupe where glavna=1"))
        return;

    grupeLista = new QMap<QString,QString>;
    grupeLista->clear();
    while (q.next())
    {
        grupeNapraviGumb(q.value(q.record().indexOf("opis")).toString(),q.value(0).toInt(),0,0,q.value(0).toInt());
    }
    grupeID = 0;
    grupeMaticna_GID = 0;
    grupeRB = 0;
    grupeParent_GID=0;
    ui->scrollArea->setMaximumHeight(85);
    ui->lblGrupaParent->setText("Grupa-naziv");
#if QT_VERSION < 0x050000

    ui->scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 20px; }");
#endif

}
void frmPregart::grupeNapraviGumb(QString NazivGumba, int GrupaID, int ParentID, int GrupaRB, int MaticnaID)
{
    QPushButton *bt= new QPushButton();
    bt->setSizeIncrement(50,50);
    bt->setText(NazivGumba);
    bt->setObjectName(QString("g-%1-%2-%3-%4-%5").arg(GrupaID).arg(ParentID).arg(GrupaRB).arg(NazivGumba).arg(MaticnaID));
    grupeLista->insert(bt->objectName(),QString("%1;%2;%3;%4;%5").arg(GrupaID).arg(ParentID).arg(GrupaRB).arg(NazivGumba).arg(MaticnaID));
    ui->scrollLay->addWidget(bt);
    //connect(bt,SIGNAL(pressed()),this,SLOT(slotGrupe()));
    bt->installEventFilter(this);
}

void frmPregart::PosloziGrid()
{
//    ui->tableView->setColumnHidden();
}
void frmPregart::UcitajArtoveTop20()
{
    UcitajGrupe();
    model = new QSqlQueryModel();
    QString nSQL;
    nSQL = "select * from viz_rac_top20";
    model->setQuery(nSQL,QSqlDatabase::database("baza"));

    ui->tableView->setModel(model);
    int i;
    for (i=3;i<=model->columnCount();i++)
    {
        ui->tableView->setItemDelegateForColumn(i,new mojiHeaderi(this));
    }
    ui->tableView->setColumnWidth(2,400);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(3,100);
    ui->tableView->selectRow(0);
    ui->tableView->setFocus();
    //ui->tableView->columnWidth()
    //dbp.close();

}

void frmPregart::UcitajArtove()
{

    QTextStream out(stdout);
       out << "\nDeb-"+QString("UcitajArtove");

    //QSqlError err;

    /*
    QSqlDatabase dbp = QSqlDatabase::database("baza");
    if (!dbp.open())
    {
        out << "\nGreska db-"+dbp.lastError().text()+"\n";
        QMessageBox::warning(this, "Ne mogu otvorit bazu", "greska neka Otvaranje konekcije: " + dbp.lastError().text());
    }
    */

    model = new QSqlQueryModel();
    QString nSQL;
    if (StoTrazim != "")
    {
        nSQL = QString("(select * from viz_rac where artsif = '%1' and sid=%2 and cid=%3) union").arg(StoTrazim).arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt());
        if (qApp->property("Prog-PretragaArtLike").toString() == "1")
        {
            nSQL += QString(" (select * from viz_rac where naziv like '%1%'").arg(StoTrazim);
        }else
        {
            nSQL += QString(" (select * from viz_rac where naziv like '%%1%'").arg(StoTrazim);
        }
        nSQL += QString(" and sid=%1 and cid=%2)").arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt());
    }else
    {
        nSQL = QString("select * from viz_rac where sid=%1 and cid=%2").arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt());
    }
    if (GrupaTrazim != "")
    {
        QString Grupa = GrupaTrazim.replace("grupa-","");
        nSQL = QString("select * from viz_rac where grupaid=%3 and sid=%1 and cid=%2")
                .arg(qApp->property("Prog-SID").toInt()).arg(qApp->property("Prog-CID").toInt()).arg(Grupa);
    }
    nSQL.append(" limit 1000");
    //qDebug() << nSQL;
    model->setQuery(nSQL,QSqlDatabase::database("baza"));

    ui->tableView->setModel(model);
    int i;
    for (i=3;i<=model->columnCount();i++)
    {
        ui->tableView->setItemDelegateForColumn(i,new mojiHeaderi(this));
    }
    ui->tableView->setColumnWidth(2,400);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(3,100);
    ui->tableView->selectRow(0);
    ui->tableView->setFocus();
    //ui->tableView->columnWidth()
    //dbp.close();

}
bool frmPregart::eventFilter(QObject *target, QEvent *event)
{
    QString Pomoc = "F1-Pomoc\nEnter-potrvrda\n";
    QTextStream out(stdout);
    QKeyEvent *keyEvent = (QKeyEvent *)event;
    if (event->type() == QEvent::MouseButtonRelease)
    {
        //qDebug() << QString("%1").arg(target->objectName()).mid(0,2);
        if (QString("%1").arg(target->objectName()).mid(0,2) == "g-")
        {
            slotGrupe(target->objectName());
        }
        if (target == ui->btnGrupeBack)
        {
            grupaBack();
            return true;
        }
    }
    if (target == ui->tableView )
    {
        //qDebug() << event->type();
//#ifdef Q_OS_LINUX
            if (event->type() == QEvent::KeyPress)
/*#endif
#ifdef Q_OS_WIN
            if (event->type() == QEvent::KeyRelease)
#endif*/
            {
                switch (keyEvent->key())
                {
                    case Qt::Key_F1:
                        QMessageBox::information(this,"Pomoc " ,Pomoc,"OK");
                        return true;
                        break;
                    case Qt::Key_Enter:
                    case Qt::Key_Return:
                        if (ui->tableView->model()->rowCount()<1)
                        {
                            if (!StalnoUpaljen)
                            {
                                if (!mutex.tryLock())
                                    return false;

                                stisnutEnter();
                                mutex.unlock();
                                this->close();

                            }else
                            {
                                UpisiDirektArt();
                            }
                            return false;
                        }
                        stisnutEnter();
                        break;
                    case Qt::Key_Escape:
                        if (!StalnoUpaljen)
                            this->close();
                        break;
                }
            }
    }
    return QWidget::eventFilter(target,event);
}

void frmPregart::stisnutEnter()
{
    //UpisiDirektArt();
    //return;
    //if (qApp->property("Prog-OdabirArtDalje").toString() == "1")
    if (model->record(ui->tableView->currentIndex().row()).value("grupaid").toInt()<1)
        return;
    qDebug() << "Naruid" << NarucioUID;
    if (qApp->property("Prog-ArtPitaKolicinu").toString() == "1")
    {
        if (model->record(ui->tableView->currentIndex().row()).value("grupaid").toInt()<1)
            return;
        frmDodajStavku *fds;
        fds = new frmDodajStavku(this->parentWidget());
        this->close();
        fds->show();
        fds->ui->txtArtSifra->setText(model->record(ui->tableView->currentIndex().row()).value("artsif").toString());
        fds->ui->txtArtJedCijena->setText(model->record(ui->tableView->currentIndex().row()).value("PC").toString());
        fds->ui->txtArtNazivOriginal->setText(model->record(ui->tableView->currentIndex().row()).value("naziv").toString());
        fds->ui->txtArtPovratnaNaknada->setText(model->record(ui->tableView->currentIndex().row()).value("povratnanaknada").toString());
        fds->ui->txtArtKolicina->selectAll();
        fds->ui->txtArtKolicina->setFocus();
        fds->BrojStola = BrojStola;
        fds->NarID = NarID;
        fds->NarucioUID=0;//NarucioUID;

        fds->artID = model->record(ui->tableView->currentIndex().row()).value("id").toInt();
        fds->artGrupaID = model->record(ui->tableView->currentIndex().row()).value("grupaid").toInt();
        fds->artIspisKuhinja = model->record(ui->tableView->currentIndex().row()).value("ispis_kuhinja").toInt();
        fds->Suma();
    }else
    {
        //direktan upis bez kol
        clsIzracuni *IZRAC = new clsIzracuni(this);
        IZRAC->set_StavkaAID(model->record(ui->tableView->currentIndex().row()).value("id").toInt(),model->record(ui->tableView->currentIndex().row()).value("artsif").toString());
        IZRAC->set_StavkaArtNaziv(model->record(ui->tableView->currentIndex().row()).value("naziv").toString());
        IZRAC->set_StavkaKol(1);
        IZRAC->set_StavkaPC(model->record(ui->tableView->currentIndex().row()).value("PC").toDouble());
        IZRAC->set_StavkaPovratnaNaknada(model->record(ui->tableView->currentIndex().row()).value("povratnanaknada").toDouble());
        IZRAC->set_StavkaRabatK(0);
        IZRAC->set_StavkaRabatP(0);
        IZRAC->set_StavkaNarucioUID(0);//NarucioUID);
        IZRAC->set_StavkaNarudzba(0);
        IZRAC->set_StavkaPodaci(BrojStola,model->record(ui->tableView->currentIndex().row()).value("grupaid").toInt(),
                                model->record(ui->tableView->currentIndex().row()).value("ispis_kuhinja").toInt(),0,NarID);
        IZRAC->izracunMPC();
        if (!IZRAC->UpisStavkeUBazu())
        {
            QMessageBox::warning(this,"GRESKA","Greska kod upisa stavke u bazu","OK");
        }else
        {
            this->close();
        }
    }
}

void frmPregart::on_tableView_doubleClicked(const QModelIndex &index)
{
    return;
    /*
    if (!mutex.tryLock())
        return ;
    stisnutEnter();
    mutex.unlock();
    */
}

void frmPregart::on_tableView_clicked(const QModelIndex &index)
{
    if (!mutex.tryLock())
        return ;
    stisnutEnter();
    mutex.unlock();
}

void frmPregart::on_btnTop20_pressed()
{
    UcitajArtoveTop20();
}

void frmPregart::UpisiDirektArt()
{
    return;
    int AID = model->record(ui->tableView->currentIndex().row()).value("id").toInt();
    QString ArtSif = model->record(ui->tableView->currentIndex().row()).value("artsif").toString();
    QString ArtNaziv = model->record(ui->tableView->currentIndex().row()).value("naziv").toString();
    double ArtPovratna = model->record(ui->tableView->currentIndex().row()).value("povratnanaknada").toDouble();
    double ArtKol = 1;
    double ArtPC = model->record(ui->tableView->currentIndex().row()).value("PC").toDouble();
    double ArtGrupaID = model->record(ui->tableView->currentIndex().row()).value("grupaid").toInt();
    double ArtIspisKuhinja = model->record(ui->tableView->currentIndex().row()).value("ispis_kuhinja").toInt();
    emit VratiAID(AID,ArtSif,ArtNaziv,ArtKol,ArtPC,0,ArtPovratna,ArtGrupaID,ArtIspisKuhinja);
}
