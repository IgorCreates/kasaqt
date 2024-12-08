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




#include "stoodabir.h"
#include "ui_stoodabir.h"
#include <QtGui>
#include "ui/dragwidget.h"
#include <QDragEnterEvent>
#include <QMimeData>

#include <QDragLeaveEvent>
#include <QDragMoveEvent>
//#include <QDragResponseEvent>

stoodabir::stoodabir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stoodabir)
{
    ui->setupUi(this);
    setAcceptDrops(true);

}

stoodabir::~stoodabir()
{
    delete ui;
}
void stoodabir::dragEnterEvent(QDragEnterEvent *event)
{
    //event->accept();
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void stoodabir::dragMoveEvent(QDragMoveEvent *event)
{
    //event->accept();
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void stoodabir::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void stoodabir::dropEvent(QDropEvent *event)
{
    //event->mimeData();

    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;

        QLabel *newIcon = new QLabel(this);
        newIcon->setPixmap(pixmap);
        newIcon->move(event->pos() - offset);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }

}

void stoodabir::mousePressEvent(QMouseEvent *event)
{
    try {

        /*
        QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
        if (!child)
            return;
        */
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
        if (!child)
            return;
        QPixmap pixmap = *child->pixmap();
#elif QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        QLabel *child = qobject_cast<QLabel*>(childAt(event->pos()));
        if (child != nullptr && child) {
            // Safely use the QLabel
        } else {
            qDebug() << "Pointer is null. No QLabel found at this position.";
        }
#endif


        //QPixmap pixmap = child->pixmap();

        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << pixmap << QPoint(event->pos() - child->pos());
    //! [1]

    //! [2]
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/x-dnditemdata", itemData);
    //! [2]

    //! [3]
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(event->pos() - child->pos());
    //! [3]

        QPixmap tempPixmap = pixmap;
        QPainter painter;
        painter.begin(&tempPixmap);
        painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
        painter.end();

        child->setPixmap(tempPixmap);

        if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
            child->close();
        } else {
            child->show();
            child->setPixmap(pixmap);
        }
    }
    catch ( ... )
    {
        return;
    }
}


void stoodabir::on_btnSnimiPostavke_released()
{
    QSettings settings;
    //snimi postavke
    if (ui->btnSnimiPostavke->text() == "Postavke")
    {
        QMainWindow *mw = new QMainWindow();
        QHBoxLayout *horizontalLayout = new QHBoxLayout;
        horizontalLayout->addWidget(new DragWidget);
        horizontalLayout->addWidget(new DragWidget);

        mw->setLayout(horizontalLayout);
        mw->setWindowTitle(QObject::tr("Odabir ikone"));
        mw->show();
        ui->btnSnimiPostavke->setText("Snimi Postavke");
    }else if (ui->btnSnimiPostavke->text() == "Snimi Postavke")
    {
        for(int index(0); index < ui->grPregled->items().count(); index++)
        {
            /*
           QWidget * child = ui->grPregled->itemAt(index);  //myLayout->itemAt(index)->widget();
           if(child)
           {   //Then it must really be a widget and not a layout

              settings.beginGroup(QString("gumbic-%1/").arg(QString::number(index)));
                 child->setGeometry(settings.value("geometry").toRect());
              settings.setValue("pos",child->pos());
              settings.setValue("size",child->size());
              settings.setValue("name",child->objectName());
              settings.endGroup();
           }
           */
        }
        ui->btnSnimiPostavke->setText("Postavke");
    }

}
