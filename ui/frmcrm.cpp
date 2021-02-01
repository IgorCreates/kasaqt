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




#include "frmcrm.h"
#include "ui_frmcrm.h"
#include "ui/frmpregrazni.h"
#include <QMessageBox>
#include "ui/frmispisodabir.h"
#include "ui/frmrnalog.h"

frmcrm::frmcrm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmcrm)
{
    ui->setupUi(this);
    this->setWindowTitle("CRM");
    CistiVarijable();
    connect(ui->btnUpisIzmijena,SIGNAL(pressed()),this,SLOT(Upis()));
    ui->btnIzlaz->installEventFilter(this);
    ui->txtDolasciOpis->installEventFilter(this);
    ui->txtKupac->installEventFilter(this);
    ui->txtProdavac->installEventFilter(this);
    ui->btnKupacTrazi->installEventFilter(this);
    ui->btnProdavacTrazi->installEventFilter(this);
    PridruziRacunCRMu = false;
    _CRM_UnioID = qApp->property("KonobarID").toInt();
    connect(ui->btnUcitajPregled,SIGNAL(pressed()),this,SLOT(UcitajPregledDolazaka()));
    connect(ui->btnPridruziRacun,SIGNAL(pressed()),this,SLOT(on_btnPridruziRacun()));
    ui->txtKupac->setFocus();
    ui->btnPregDodajVezuRacun->setVisible(false);
    ui->btnPregMakniVezuRacun->setVisible(false);
    ui->btnPregBrisiCRM->setVisible(false);
    modelPregled = new QSqlQueryModel();
}

frmcrm::~frmcrm()
{
    delete ui;
}


void frmcrm::VratiID(QString StoVraca, int StoID, QList<QString> Lista)
{
    if (StoVraca == "crm-kupac")
    {
        _CRM_KupacID = StoID;
        QString KupacNaziv = QString("%1,%2-%3").arg(Lista[0]).arg(Lista[1]).arg(Lista[2]);
        ui->txtKupacNaziv->setText(KupacNaziv);
        ui->txtProdavac->setFocus();
    }else if (StoVraca == "crm-prodavac")
    {
        _CRM_ProdavacID = StoID;
        QString ProdavacNaziv = QString("%1,%2").arg(Lista[0]).arg(Lista[1]);
        ui->txtProdavacNaziv->setText(ProdavacNaziv);
        ui->txtDolasciOpis->setFocus();
    }
    //qDebug() << "Vratio " << StoVraca << StoID;
}

void frmcrm::CistiVarijable()
{
    _CRM_ID = 0;
    _CRM_KupacID = 0;
    _CRM_ProdavacID = 0;
    _CRM_UnioID = qApp->property("KonobarID").toInt();
    PridruziRacunCRMu = false;
    _CRM_OPIS.clear();
}

void frmcrm::Upis()
{
    _CRM_UnioID = qApp->property("KonobarID").toInt();
    if (_CRM_KupacID == 0)
    {
        QMessageBox::warning(this,tr("CRM-Greska"),tr("Nemate popunjenog kupca\nnije moguce upisati"),"OK");
        return;
    }
    QString OpisPoruka="";
    if (ui->txtDolasciOpis->toPlainText().length() == 0)
    {
        OpisPoruka = "Niste upisali nikakav opis, da li zelite upisati izmijene bez opisa?";
    }else
    {
        OpisPoruka = "Da li zelite upisati izmijene";
    }
    int ret = QMessageBox::information(this,tr("CRM-Upis"),OpisPoruka,
                                       QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (ret != QMessageBox::Yes)
        return;
    //bool DobroUpisan = true;

    QString qveri = "insert into crm (id,datum_unosa,kupacid,prodavacid,unioid,kupac_naziv,prodavac_naziv) values(";
    qveri += QString("0,now(),'%1','%2','%3','%4','%5')").arg(_CRM_KupacID).arg(_CRM_ProdavacID).arg(_CRM_UnioID)
            .arg(ui->txtKupacNaziv->text()).arg(ui->txtProdavacNaziv->text());
    QSqlQuery q("",QSqlDatabase::database("baza"));
    if (!q.exec(qveri))
    {
        QMessageBox::warning(this,tr("CRM-Upis"),tr("Greska kod upisa u bazu"),"OK");
        qDebug() << "CRM-Greska-upisa-" << q.lastError() << q.lastQuery();
//        DobroUpisan = false;
        return;
    }
    _CRM_ID = q.lastInsertId().toInt();

    if (!q.exec(QString("update crm c,korisnik k set c.unio_naziv=concat(k.naziv,'-',k.puninaziv) where c.id=%1 and c.unioid=k.id and c.unioid=%2").arg(_CRM_ID).arg(_CRM_UnioID)))
        qDebug() << "greska " << q.lastError() << q.lastQuery();
    qveri = QString("insert into crmopis (crmid,opis) values(%1,'%2')").arg(_CRM_ID).arg(_CRM_OPIS);
    if (!q.exec(qveri))
    {
        QMessageBox::warning(this,tr("CRM-Upis"),tr("Greska kod upisa u bazu"),"OK");
        qDebug() << "CRM-Greska-upisa-" << q.lastError() << q.lastQuery();
//        DobroUpisan = false;
        return;
    }
    QMessageBox::information(this,tr("CRM-UPIS"),tr("Upisano"),"OK");
    ui->txtKupac->setText("");
    ui->txtKupacNaziv->setText("");
    ui->txtProdavac->setText("");
    ui->txtProdavacNaziv->setText("");
    ui->txtDolasciOpis->clear();
    //ui->btnPridruziRacun->setIcon(QIcon::addFile());
    if (PridruziRacunCRMu)
    {
        emit VratiCRMID(_CRM_ID);
    }
    this->close();
}

void frmcrm::FirmaTrazi()
{
    frmpregrazni *fp = new frmpregrazni(this);
    fp->show();
    fp->_frmPregledSto ="crm-kupac";
    QString qveri = "select id,naziv,adresa,mjesto from company ";//order by naziv";
    if (ui->txtKupac->text() != "" )
        qveri += QString(" where naziv like '%%1%' or oib like '%%1' ").arg(ui->txtKupac->text());
    qveri += " order by naziv";
    fp->set_Qveri(qveri);
    fp->UcitajFormu();

    connect(fp,SIGNAL(VratiStoTrazi(QString,int,QList<QString>)),this,SLOT(VratiID(QString,int,QList<QString>)));
}
void frmcrm::ProdavacTrazi()
{
    frmpregrazni *fp = new frmpregrazni(this);
    fp->show();
    fp->_frmPregledSto ="crm-prodavac";
    QString qveri = "select id,naziv,puninaziv from korisnik ";//order by naziv";
    if (ui->txtProdavac->text() != "" )
        qveri += QString(" where naziv like '%%1%' or puninaziv like '%%1' ").arg(ui->txtProdavac->text());
    qveri += " order by naziv";
    fp->set_Qveri(qveri);
    fp->UcitajFormu();
    connect(fp,SIGNAL(VratiStoTrazi(QString,int,QList<QString>)),this,SLOT(VratiID(QString,int,QList<QString>)));
}

void frmcrm::UcitajPregledDolazaka()
{
    if (_CRM_KupacID == 0)
        return;
    ui->btnPregBrisiCRM->setVisible(false);
    ui->btnPregDodajVezuRacun->setVisible(false);
    ui->btnPregMakniVezuRacun->setVisible(false);
    QString qveri = QString("select c.*,co.opis,concat(r.br,'/',r.oznpp,'/',r.kasaid) rnbr from "
                            " crm c left join crmopis co on c.id=co.crmid left join crmrac cr on c.id=cr.crmid "
                            " left join rac1 r on cr.rid=r.id where c.kupacid=%1 order by datum_unosa desc").arg(_CRM_KupacID);
    modelPregled->setQuery(qveri,QSqlDatabase::database("baza"));
    if (modelPregled->record().count()<1)
        return;
    ui->dgPregledDolazaka->setModel(modelPregled);
    ui->dgPregledDolazaka->setColumnHidden(modelPregled->record().indexOf("kupacid"),true);
    ui->dgPregledDolazaka->setColumnHidden(modelPregled->record().indexOf("prodavacid"),true);
    ui->dgPregledDolazaka->setColumnHidden(modelPregled->record().indexOf("unioid"),true);
    ui->dgPregledDolazaka->setColumnHidden(modelPregled->record().indexOf("id"),true);
    QItemSelectionModel *sm = ui->dgPregledDolazaka->selectionModel();
    connect(sm, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
    this, SLOT(on_dgPregledDolazaka_clicked(QModelIndex)));

}

bool frmcrm::eventFilter(QObject *target, QEvent *event)
{
    QKeyEvent *keyEvent = (QKeyEvent *)event;

    if (target == ui->pgPregledDolazaka)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            //qDebug() << "Jesam";
            UcitajPregledDolazaka();
        }
    }
    if (target == ui->btnKupacTrazi)
    {
        if (event->type() == QEvent::MouseButtonRelease)
        {
            FirmaTrazi();
        }
    }
    if (target == ui->txtKupac)
    {
        if (event->type() == QEvent::KeyRelease)
        {
            if (keyEvent->key() == Qt::Key_F2)
                FirmaTrazi();
        }
    }
    if (target == ui->btnProdavacTrazi)
    {
        if (event->type() == QEvent::MouseButtonRelease)
        {
            ProdavacTrazi();
        }
    }
    if (target == ui->txtProdavac)
    {
        if (event->type() == QEvent::KeyRelease)
        {
            if (keyEvent->key() == Qt::Key_F2)
                ProdavacTrazi();
        }
    }
    if (target == ui->txtDolasciOpis)
    {
        if (event->type() == QEvent::FocusOut)
        {
            _CRM_OPIS = ui->txtDolasciOpis->toPlainText();
        }
    }

    if (target == ui->btnIzlaz)
    {
        if (event->type() == QEvent::MouseButtonRelease)
        {
            int ret = QMessageBox::information(this,tr("CRM-Izlaz"),tr("Da li zelite zatvoriti CRM?"),
                                               QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
            if (ret == QMessageBox::Yes)
            {
                this->close();
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

void frmcrm::on_dgPregledDolazaka_doubleClicked(const QModelIndex &index)
{
    frmcrmpregled *fpc = new frmcrmpregled(this);
    fpc->show();
    fpc->setWindowTitle(tr("CRM-Pregled stavke"));
    fpc->UcitaFormu(ui->dgPregledDolazaka->model()->data(ui->dgPregledDolazaka->model()->index(ui->dgPregledDolazaka->currentIndex().row(),0)).toInt());

}

void frmcrm::on_btnPridruziRacun()
{
//    QIcon *ic = new QIcon();

    if (ui->btnPridruziRacun->text() == "Racun pridruzen")
    {
        ui->btnPridruziRacun->setIcon(QIcon("ikone/list_add.svg"));
        ui->btnPridruziRacun->setText("Pridruzi ovaj racun nakon unosa");
        ui->btnPridruziRacun->setStyleSheet("");
        PridruziRacunCRMu = false;
    }else{
        ui->btnPridruziRacun->setIcon(QIcon("ikone/button_ok.svg"));
        ui->btnPridruziRacun->setText("Racun pridruzen");
        ui->btnPridruziRacun->setStyleSheet("background-color: rgb(0, 255, 127);");
        PridruziRacunCRMu = true;
    }
}

void frmcrm::on_btnUcitajPregDok_pressed()
{
    if (_CRM_KupacID == 0)
        return;
    QString qveri = QString("select id,kid kupac,\"RNalog\" tip_dok,datumz datum_dok,dok br_dok,0 placeno from rnalog1 where kid=%1"
                            " union all select id,idk kupac,\"MRacun\" tip_dok,datumr datum_dok,br br_dok,sum placeno from rac1 where idk=%1 "
                            " union all (select r.id,r.idk kupac,\"MRacun\" tip_dok,r.datumr datum_dok,r.br br_dok,r.sum placeno from rac1 r "
                            " left join crmrac c on r.id=c.rid left join crm on c.crmid=crm.id  where crm.kupacid=%1) order by datum_dok desc;").arg(_CRM_KupacID);
    QSqlQueryModel *qm = new QSqlQueryModel();
    qm->setQuery(qveri,QSqlDatabase::database("baza"));
    ui->dgPregDok->setModel(qm);
    ui->dgPregDok->setColumnHidden(qm->record().indexOf("id"),true);
    ui->dgPregDok->setColumnHidden(qm->record().indexOf("kupac"),true);
}

void frmcrm::on_dgPregledDolazaka_clicked(const QModelIndex &index)
{
    int crmid = ui->dgPregledDolazaka->model()->data(ui->dgPregledDolazaka->model()->index(ui->dgPregledDolazaka->currentIndex().row(),0)).toInt();
    ProvjeriVezuNaRacune(crmid);
}

void frmcrm::ProvjeriVezuNaRacune(int crmid)
{
    int CurRow = ui->dgPregledDolazaka->currentIndex().row();
    int CurCol = modelPregled->record().indexOf("rnbr");
    QString BrRac = ui->dgPregledDolazaka->model()->data(ui->dgPregledDolazaka->model()->index(CurRow,CurCol)).toString();

    if (BrRac != "")
    {
        ui->btnPregDodajVezuRacun->setVisible(false);
        ui->btnPregMakniVezuRacun->setVisible(true);
        ui->btnPregMakniVezuRacun->setText(QString("Maknuti vezu-RN_br:%1").arg(BrRac));
    }else
    {
        ui->btnPregDodajVezuRacun->setVisible(true);
        ui->btnPregMakniVezuRacun->setVisible(false);
    }
    if (ui->dgPregledDolazaka->model()->rowCount() < 1)
    {
        ui->btnPregBrisiCRM->setVisible(false);
    }else
    {
        ui->btnPregBrisiCRM->setVisible(true);
    }
}
void frmcrm::PridruziRacunCRMfunkcija()
{

    if (ui->dgPregledDolazaka->model()->rowCount() < 1)
        return;
    int crmid = ui->dgPregledDolazaka->model()->data(ui->dgPregledDolazaka->model()->index(ui->dgPregledDolazaka->currentIndex().row(),0)).toInt();

    int ret = QMessageBox::information(this,tr("Dodjela CRM-ID na racun"),tr("Da li zelite dodati ovaj CRM na racun u kreiranju?"),
                                       QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (ret != QMessageBox::Yes)
        return;
    //_CRM_ID = q.value(q.record().indexOf("crmid")).toInt();
    _CRM_ID = crmid;
    emit VratiCRMID(_CRM_ID);
    this->close();
}

void frmcrm::MakniRacunCRMfunkcija()
{
    if (modelPregled->record().count()<1)
        return;
    int crmid = ui->dgPregledDolazaka->model()->data(ui->dgPregledDolazaka->model()->index(ui->dgPregledDolazaka->currentIndex().row(),0)).toInt();

    int ret = QMessageBox::information(this,tr("Micanje CRM-ID sa racuna"),tr("Da li zelite maknuti vezu CRM-racun?"),
                                       QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
    if (ret != QMessageBox::Yes)
        return;
    //_CRM_ID = q.value(q.record().indexOf("crmid")).toInt();
    _CRM_ID = crmid;
    QSqlQuery q("",QSqlDatabase::database("baza"));
    if (!q.exec(QString("delete from crmrac where crmid=%1").arg(crmid)))
    {
        QMessageBox::warning(this,tr("CRM"),tr("Greska kod brisanja veze CRM-racun"),"OK");
        return;
    }else{
        QMessageBox::information(this,tr("CRM"),tr("Veza maknuta"),"OK");
    }
    ProvjeriVezuNaRacune(crmid);
}


void frmcrm::on_btnPregDodajVezuRacun_pressed()
{
    PridruziRacunCRMfunkcija();
}

void frmcrm::on_btnPregMakniVezuRacun_pressed()
{
    MakniRacunCRMfunkcija();
}

void frmcrm::on_btnPregBrisiCRM_pressed()
{
    int crmid = ui->dgPregledDolazaka->model()->data(ui->dgPregledDolazaka->model()->index(ui->dgPregledDolazaka->currentIndex().row(),0)).toInt();
    if (crmid < 1)
        return;
    int ret = QMessageBox::warning(this,tr("CRM"),tr("Da li zelite obrisati CRM i sve veze?"),QMessageBox::Yes|QMessageBox::Default,
                                   QMessageBox::No|QMessageBox::Escape);
    if (ret != QMessageBox::Yes)
        return;

    QSqlQuery q("",QSqlDatabase::database("baza"));

    q.exec(QString("delete from crmrac where crmid=%1").arg(crmid));
    q.exec(QString("delete from crmopis where crmid=%1").arg(crmid));
    q.exec(QString("delete from crm where id=%1").arg(crmid));
    UcitajPregledDolazaka();
    ui->dgPregledDolazaka->selectRow(0);

}

void frmcrm::on_dgPregDok_doubleClicked(const QModelIndex &index)
{



    if (ui->dgPregDok->model()->data(ui->dgPregDok->model()->index(ui->dgPregDok->currentIndex().row(),2)).toString() == "RNalog")
    {
        frmrnalog *frn = new frmrnalog(this);
        frn->IspisA4(ui->dgPregDok->model()->data(ui->dgPregDok->model()->index(ui->dgPregDok->currentIndex().row(),0)).toInt());
        frn->close();
    }
    if (ui->dgPregDok->model()->data(ui->dgPregDok->model()->index(ui->dgPregDok->currentIndex().row(),2)).toString() == "MRacun")
    {
        frmIspisOdabir *fis = new frmIspisOdabir(this);
        fis->RacunID = ui->dgPregDok->model()->data(ui->dgPregDok->model()->index(ui->dgPregDok->currentIndex().row(),0)).toInt();
        fis->show();
    }

}
