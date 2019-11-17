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




#include "frmtipkovnica.h"
#include "ui_frmtipkovnica.h"
#include "QSignalMapper"
#include "QtGui"

frmTipkovnica::frmTipkovnica(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmTipkovnica)
{
    ui->setupUi(this);
    UcitajSve();
    this->set_IzgledTipkovnice("Tipkovnica");

}

frmTipkovnica::~frmTipkovnica()
{
    delete ui;
}


void frmTipkovnica::UcitajSve()
{

    for (int i = 0; i < NumDigitButtons; ++i) {
        digitButtons[i] = createButton(QString::number(i));
    }

    if (this->get_IzgledTipkovnice() == "Izmijena")
    {
        QSignalMapper *signalMapper = new QSignalMapper(this);
        connect(signalMapper, SIGNAL(mapped(const QString &)),
                this, SIGNAL(clicked(const QString &)));
        connect(signalMapper,SIGNAL(mapped(QString)),
                this,SIGNAL(VratiTipku(QString)));

        //this->setText(this->get_IzgledTipkovnice());

        display = new QLineEdit("0");
        display->setReadOnly(false);
        display->setAlignment(Qt::AlignRight);
        display->setMaxLength(15);
        QFont font = display->font();
        font.setPointSize(font.pointSize() + 12);
        display->setFont(font);

        Button *pointButton = createButton(tr(","));
        //connect(pointButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(QString",")));

        Button *backspaceButton = createButton(tr("Backspace"));
        backspaceButton->setIcon(QIcon(":/ikone/ikone/backspace.png"));
        backspaceButton->setIconSize(QSize(22,22));
    //    connect(backspaceButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Backspace)));

        Button *clearButton = createButton(tr("Clear"));
        //connect(clearButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Clear)));

        Button *minusButton = createButton(tr("-"));
    //    connect(minusButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Minus)));

        Button *plusButton = createButton(tr("+"));
    //    connect(plusButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Plus)));

        Button *equalButton = createButton(tr("="));
        //connect(equalButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Equal)));

        Button *enterButton = createButton("ENTER");

        Button *izlazButton = createButton("IZLAZ");
        connect(izlazButton,SIGNAL(clicked(bool)),this,SLOT(close()));

        QGridLayout *mainLayout = new QGridLayout();
        mainLayout->addWidget(display, 0, 0, 1, 6);
        mainLayout->addWidget(backspaceButton, 1, 0, 1, 2);
        mainLayout->addWidget(clearButton, 1, 2, 1, 2);
        mainLayout->addWidget(enterButton,1,5,3,1);
        mainLayout->addWidget(izlazButton,4,5,5,1);

        for (int i = 1; i < NumDigitButtons; ++i) {
            int row = ((9 - i) / 3) + 2;
            int column = ((i - 1) % 3) + 1;
            mainLayout->addWidget(digitButtons[i], row, column);
            //connect(digitButtons[i],SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_[i])));
            connect(digitButtons[i],SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
            signalMapper->setMapping(digitButtons[i], QString("%1").arg(i));
        }

        mainLayout->addWidget(digitButtons[0], 5, 1);
        connect(digitButtons[0],SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(digitButtons[0], "0");
        mainLayout->addWidget(pointButton, 5, 2);
        mainLayout->addWidget(minusButton, 3, 4);
        mainLayout->addWidget(plusButton, 4, 4);
        mainLayout->addWidget(equalButton, 5, 4);

        ui->verticalLayout->addLayout(mainLayout);
        this->setWindowTitle(tr("Tipkovnica"));


    }else
    {
        //display->setText(this->get_IzgledTipkovnice());

        QSignalMapper *signalMapper = new QSignalMapper(this);
        connect(signalMapper, SIGNAL(mapped(const QString &)),
                this, SIGNAL(clicked(const QString &)));
        connect(signalMapper,SIGNAL(mapped(QString)),
                this,SIGNAL(VratiTipku(QString)));

        Button *pointButton = createButton(tr(","));
        //connect(pointButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(QString",")));
        connect(pointButton,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(pointButton, ",");

        Button *backspaceButton = createButton(tr("Backspace"));
        backspaceButton->setIcon(QIcon(":/ikone/ikone/backspace.png"));
        backspaceButton->setIconSize(QSize(22,22));
    //    connect(backspaceButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Backspace)));
        connect(backspaceButton,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(backspaceButton, "BACKSPACE");

        Button *clearButton = createButton(tr("Clear"));
        //connect(clearButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Clear)));
        connect(clearButton,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(clearButton, "CLEAR");

        Button *minusButton = createButton(tr("-"));
    //    connect(minusButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Minus)));
        connect(minusButton,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(minusButton, "-");

        Button *plusButton = createButton(tr("+"));
    //    connect(plusButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Plus)));
        connect(plusButton,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(plusButton, "+");

        Button *equalButton = createButton(tr("="));
        //connect(equalButton,SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_Equal)));
        connect(equalButton,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(equalButton, "=");

        Button *enterButton = createButton("ENTER");
        connect(enterButton,SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(enterButton,"ENTER");
        connect(enterButton,SIGNAL(clicked(bool)),this,SLOT(close()));

        Button *izlazButton = createButton("IZLAZ");
        connect(izlazButton,SIGNAL(clicked(bool)),this,SLOT(UbijenaSlot()));
        connect(izlazButton,SIGNAL(clicked(bool)),this,SLOT(close()));

        QGridLayout *mainLayout = new QGridLayout();
        //mainLayout->setSizeConstraint(QLayout::SetFixedSize);

        QLabel* lbl1 = new QLabel("TIPK");
    //    mainLayout->addWidget(display, 0, 0, 1, 6);
        mainLayout->addWidget(lbl1,0,0,1,6);
        mainLayout->addWidget(backspaceButton, 1, 0, 1, 2);
        mainLayout->addWidget(clearButton, 1, 2, 1, 2);
        mainLayout->addWidget(enterButton,1,5,3,1);
        mainLayout->addWidget(izlazButton,4,5,5,1);

        for (int i = 1; i < NumDigitButtons; ++i) {
            int row = ((9 - i) / 3) + 2;
            int column = ((i - 1) % 3) + 1;
            mainLayout->addWidget(digitButtons[i], row, column);
            //connect(digitButtons[i],SIGNAL(pressed()),this,SLOT(StisnutaTipka(Qt::Key_[i])));
            connect(digitButtons[i],SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
            signalMapper->setMapping(digitButtons[i], QString("%1").arg(i));
        }

        mainLayout->addWidget(digitButtons[0], 5, 1);
        connect(digitButtons[0],SIGNAL(clicked(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(digitButtons[0], "0");
        mainLayout->addWidget(pointButton, 5, 2);
        mainLayout->addWidget(minusButton, 3, 4);
        mainLayout->addWidget(plusButton, 4, 4);
        mainLayout->addWidget(equalButton, 5, 4);

        ui->verticalLayout->addLayout(mainLayout);
        this->setWindowTitle(tr("Tipkovnica"));
    }
}

void frmTipkovnica::ResetSvega()
{
    this->set_IzgledTipkovnice("Skeniranje");
    //display->setText("0");
}

Button * frmTipkovnica::createButton(const QString &text) //, const char *member)
{
    Button *button = new Button(text);
    //connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void frmTipkovnica::StisnutaTipka(QString Tipka)
{
    emit VratiTipku(Tipka);
}
void frmTipkovnica::UbijenaSlot()
{
    emit Ubijena();
}
