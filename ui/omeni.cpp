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




#include "omeni.h"
#include "ui_omeni.h"

oMeni::oMeni(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::oMeni)
{
    ui->setupUi(this);
    ui->txtOmeni->setReadOnly(true);
}

oMeni::~oMeni()
{
    delete ui;
}

void oMeni::Prikazi()
{


    QString poruka = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">";
            poruka += "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">";
            poruka += "p, li { white-space: pre-wrap; }";
            poruka += "</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">";
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600; font-style:italic; color:#3d74eb;\">Fiskal Kasa  Ver: %1</span></p>").arg(qApp->property("App_Verzija").toString());
            poruka += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">Program izrado : </span><span style=\" font-weight:600; font-style:italic;\">https://bitbucket.org/kasaqt/kasaqt</span></p>";
            poruka += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	web: <a href=\"https://bitbucket.org/kasaqt/kasaqt\"><span style=\" text-decoration: underline; color:#0000ff;\">https://bitbucket.org/kasaqt/kasaqt</span></a></p>";
            poruka += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">Za potrebe dan na koristenje:</span></p>";
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	<span style=\" font-weight:600; font-style:italic;\">%1</span></p>").arg(qApp->property("Firma_Ime").toString());
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	OIB:%1</p>").arg(qApp->property("Firma_OIB").toString());
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">	%1</p>").arg(QString("%1-%2").arg(qApp->property("Firma_Postanski").toString()).arg(qApp->property("Firma_Grad").toString()));
            poruka += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; font-style:italic; text-decoration: underline;\">Compiled : %1-%2</span></p></body></html>").arg(QT_VERSION).arg(QT_VERSION_STR);

    ui->txtOmeni->setHtml(poruka);
}
