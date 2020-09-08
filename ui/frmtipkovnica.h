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




#ifndef FRMTIPKOVNICA_H
#define FRMTIPKOVNICA_H

#include <QWidget>
#include <ui/button.h>
#include <QGridLayout>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>


class Button;
namespace Ui {
class frmTipkovnica;
}

class frmTipkovnica : public QWidget
{
    Q_OBJECT

public:
    explicit frmTipkovnica(QWidget *parent = 0);
    ~frmTipkovnica();
    void UcitajSve();
    void ResetSvega();

    void set_IzgledTipkovnice(QString Kako)
    {
        _IzgledTipkovnice = Kako;
    }
    QString get_IzgledTipkovnice()
    {
        return _IzgledTipkovnice;
    }

private slots:
    void StisnutaTipka(QString Tipka); //(Qt::Key Tipka);
    void UbijenaSlot();

private:
    Ui::frmTipkovnica *ui;

    void PrazniSve();
    enum { NumDigitButtons = 10 };
    Button *digitButtons[NumDigitButtons];
    QLineEdit *display;
    Button *createButton(const QString &text);
    QString _IzgledTipkovnice;
signals:
    void VratiTipku(QString Tipka); //(Qt::Key Tipka);
    void clicked(QString Tipka);
    void Ubijena();
};

#endif // FRMTIPKOVNICA_H
