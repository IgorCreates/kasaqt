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




#ifndef FRMISPISODABIR_H
#define FRMISPISODABIR_H

#include <QDialog>
#include <QtCore>
#include <QTextDocument>
#include <QtGui>
#if QT_VERSION > 0x050000
#include <QPlainTextEdit>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QLineEdit>
#endif


namespace Ui {
class frmIspisOdabir;
}

class frmIspisOdabir : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmIspisOdabir(QWidget *parent = 0);
    ~frmIspisOdabir();
    int RacunID;
    int NarID;
    QList<int> RacuniID;
    bool RacuniOdDo;
    QString RacTipRacuna; //da li je racun iz rac1 ili otpremnica iz vrac
    void ProvjeraOpcija();
    //void ispisA4stanjeTemplate(QDateTime DatumStart,QDateTime DatumEnd,int KasaID);
    void ispisA4templateStanje(QDateTime DatumStart,QDateTime DatumEnd,int KasaID,QString MaliVeliki);

    static void IzvrtiSQL(QDateTime DatumStart,QDateTime DatumEnd,int KasaID);
    void ispisPOS_poFile(QString PathDoDatoteke);
    QString IspisMaliSto;
    QTextDocument *doc;



private slots:
    void on_btnPOS_released();
    void on_btnA4_released();
    void StartA4Ispis();
    void StopA4Ispis();
    void IspisPreview(QPrinter *printer);
    void IspisPreviewTT(QPrinter *printer);

    void PokaziSaljiMejl();
    void SaljiMejl();
    void mailSent(QString);
    void print(QPrinter *printer);




private:
    Ui::frmIspisOdabir *ui;
    void ispisA4template(QString Sto, int RID);
    void ispisA4templateTT(QString Sto, int RID);
    void ispisHTMLtemplate(int RID,QString Sto);
    void ispisQRcreate(int RID);
    QString regEx(QString text, QString pattern);


    QTextDocument *DokumentZaIspisA4;
    QPainter *painter;
    QString ispisVratiHtmlContent(int RID);
    QString ispisVratiHtmlContentMali(int RID);
    QString ispisVratiHtmlContentStanje(const QDateTime &DatumStart,const QDateTime &DatumEnd,const int &KasaID);
    QString ispisVratiHtmlContentNarudzba(int RID);

    QString ispisQR_path;
    QString ispisQR_url;
    void  ispisTestPOS(int RID);


    QPrinter *prnPrinter;
    QString prnImeFajla;
    QLineEdit *txtMailTO;
    QLineEdit *txtMailSubject;
    QPlainTextEdit *txtMailPoruka;
    QWidget *mm;
    QPrintPreviewDialog *pw;



signals:
    void Ridovi_Promjena(int TrenValue);
    void SignalOdjava();


};

#endif // FRMISPISODABIR_H
