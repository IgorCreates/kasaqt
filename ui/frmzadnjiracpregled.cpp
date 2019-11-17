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




#include "frmzadnjiracpregled.h"
#include "ui_frmzadnjiracpregled.h"
#include <QDebug>
#include <QKeyEvent>

frmZadnjiRacPregled::frmZadnjiRacPregled(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmZadnjiRacPregled)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(pressed()),this,SLOT(close()));
    ui->pushButton->setFocus();
    this->installEventFilter(this);
    ui->txtPlatio->installEventFilter(this);
    ui->txtUkpSuma->installEventFilter(this);
    ui->txtZaVratiti->installEventFilter(this);
    ui->pushButton->installEventFilter(this);
}

frmZadnjiRacPregled::~frmZadnjiRacPregled()
{
    delete ui;
}

void frmZadnjiRacPregled::on_pushButton_released()
{
    this->close();
}

void frmZadnjiRacPregled::Preracunaj()
{
    double ZaVrati;
    ZaVrati = ui->txtPlatio->text().toDouble()-ui->txtUkpSuma->text().toDouble();
    ui->txtZaVratiti->setText(QString("%L1").arg(ZaVrati,0,'f',2));
}

void frmZadnjiRacPregled::NovaSuma(double Suma)
{
    ui->txtUkpSuma->setText(QString("%L1").arg(Suma,0,'f',2));
    ui->txtPlatio->setText("0,00");
    Preracunaj();
    ui->pushButton->setFocus();
}

void frmZadnjiRacPregled::on_txtPlatio_textChanged(const QString &arg1)
{
    Preracunaj();
}

bool frmZadnjiRacPregled::eventFilter(QObject *target, QEvent *event)
{
    QKeyEvent *keyEvnt = (QKeyEvent *)event;
    /*
    if (event->type() == QEvent::KeyRelease)
    {
    }
    */

    if (event->type() == QEvent::KeyPress)
    {
        if (keyEvnt->key() == Qt::Key_Escape)
            this->close();
    }

    if (target == this)
    {
        if (event->type() == QEvent::FocusIn)
            ui->pushButton->setFocus();
    }
    if (target == ui->pushButton)
    {
        if (event->type() == QEvent::FocusIn)
            qDebug() << QString("DobioFocus");
    }


    return QWidget::eventFilter(target,event);
}
