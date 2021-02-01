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




#include "frmloginrftag.h"
#include "ui_frmloginrftag.h"
#include <QtSql>
#include <QtGui>
#include "ui/frmtouch.h"

frmloginrftag::frmloginrftag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmloginrftag)
{
    ui->setupUi(this);
    UcitajFirmaInfo();
    this->setWindowTitle("Kasa login");
    ui->lblLogo->setPixmap(QPixmap(QString("%1/%2").arg(qApp->applicationDirPath()).arg("firma_logo.jpg")));

    //this->setStyleSheet("background-image: url(\"/tmp/srce_logo.jpg\");");
    //ui->frame->setStyleSheet("border-image: url(\"/tmp/srce_logo.jpg\" 0 0 0 0 stretch stretch;"
    //                         "border-width: 0px;");
    /*
    QPalette palette;
    QString appDir = QApplication::applicationDirPath();
    palette.setBrush(this->backgroundRole(),QBrush(QImage(appDir +"/srce_logo.jpg")));
    this->setPalette(palette);
    */
    ui->txtKorisnik->setStyleSheet("background-color: rgb(255, 255, 255);");
    this->setWindowState(Qt::WindowMaximized);
}

frmloginrftag::~frmloginrftag()
{
    delete ui;
}
/*
void frmloginrftag::eventFilter(QObject *target, QEvent *event)
{
    QKeyEvent *keyEvent = (QKeyEvent *)event;

    if (target->objectName() == "txtfirmainfo")
    {
        if (event->type() == QEvent::KeyRelease)
        {
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {

            }
        }
    }

    return QWidget::eventFilter(target,event);
}
*/

void frmloginrftag::UcitajFirmaInfo()
{
/*
    QTextEdit* txtFirmaInfo = new QTextEdit();
    QString poruka = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">";
            poruka += "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">";
            poruka += "p, li { white-space: pre-wrap; }";
            poruka += "</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">";
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600; font-style:italic; color:#3d74eb;\">Fiskal Kasa  Ver: %1</span></p>").arg(qApp->property("App_Verzija").toString());
            poruka += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">Program izrado : </span><span style=\" font-weight:600; font-style:italic;\">https://github.com/drjcro/kasaqt</span></p>";
            poruka += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	web: <a href=\"https://github.com/drjcro/kasaqt\"><span style=\" text-decoration: underline; color:#0000ff;\">https://github.com/drjcro/kasaqt</span></a></p>";
            poruka += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	mail: <a href=\"mailto:igor.jukic.76@gmail.com\"><span style=\" text-decoration: underline; color:#0000ff;\">igor.jukic.76@gmail.com</span></a></p>";
            poruka += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">OpenSource GNU GPL v3:</span></p>";
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	<span style=\" font-weight:600; font-style:italic;\">%1</span></p>").arg(qApp->property("Firma_Ime").toString());
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	OIB:%1</p>").arg(qApp->property("Firma_OIB").toString());
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	%1</p>").arg(QString("%1-%2").arg(qApp->property("Firma_Postanski").toString()).arg(qApp->property("Firma_Grad").toString()));
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; font-style:italic; text-decoration: underline;\">Compiled : %1-%2</span></p></body></html>").arg(QT_VERSION).arg(QT_VERSION_STR);

    txtFirmaInfo->setHtml(poruka);
    txtFirmaInfo->setObjectName("txtfirmainfo");
    txtFirmaInfo->setFocusPolicy(Qt::NoFocus);
    QLineEdit* txtRFid = new QLineEdit();
    QLabel* lbl1 = new QLabel();
    lbl1->setMaximumWidth(100);
    txtRFid->setMinimumWidth(300);
    txtRFid->setMaximumWidth(400);

    lbl1->setText("Korisnik:");

    txtRFid->installEventFilter(this);
    txtRFid->setFocus();
    */
}

void frmloginrftag::StisnutEnter(QString TxtPolje)
{
    if (TxtPolje.length() < 1 )
    {
        ui->txtKorisnik->setText("");
        return;
    }

    QSqlQuery q("",QSqlDatabase::database("baza"));
    if (!q.exec(QString("select k.puninaziv,k.oib,k.dozvole,kc.boja,k.sifra,k.id from korisnik k left join korisnik_boje kc on k.id=kc.uid where k.active='A' and k.rfid='%1'").arg(TxtPolje)))
    {
        ui->txtKorisnik->setText("");
        return;
    }

    qDebug() << q.record().count();// << q.lastQuery();

    if (q.first())
    {
        qApp->setProperty("KorisnikIme",q.value(q.record().indexOf("puninaziv")).toString());
        qApp->setProperty("KonobarOIB",q.value(q.record().indexOf("oib")).toString());
        qApp->setProperty("KonobarPrava",q.value(q.record().indexOf("dozvole")).toInt());
        qApp->setProperty("KorisnikSifra",q.value(q.record().indexOf("sifra")).toString());
        if (q.value(q.record().indexOf("boja")).toString() != "")
        {
            qApp->setProperty("KorisnikBoja",q.value(q.record().indexOf("boja")).toString());
        }else
        {
            qApp->setProperty("KorniskBoja","#FFFFFF");
        }

        qApp->setProperty("KonobarID",q.value(q.record().indexOf("id")).toInt());

        frmtouch *fp = new frmtouch();
        fp->setWindowModality(Qt::WindowModal);
        fp->setWindowFlags(Qt::WindowStaysOnTopHint);
        //fp->show();
        fp->showMaximized();

    }
    ui->txtKorisnik->setText("");
}

void frmloginrftag::on_txtKorisnik_returnPressed()
{
    qDebug() << ui->txtKorisnik->text();
    StisnutEnter(ui->txtKorisnik->text());
}
