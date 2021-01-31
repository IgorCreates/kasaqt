#ifndef ISPISMALI_H
#define ISPISMALI_H
#include <QString>
#include <QDateTime>
#include <QtSql>


class ispisMali
{
public:
    ispisMali();
    void ispisVeliki(const QString &ImeReporta,const QString &Parametri);


    void IspisMaliPos(const QString &BrRacuna);
    void IspisMaliPosVrac(const QString &BrRacuna);
    void NewIsp(int BrRacuna);
    QString NewH();
    int VM(const QString &Polje,const int &PredvidenaDuljina);
    QString IspisHeader();
    void IspisStanjeKase(const QDateTime &DatumOd,const QDateTime &DatumDo);
    void IspisKrajDanaKorisnik(const QDateTime &DatumOd,const QDateTime &DatumDo,const int &KorisnikID,bool IspisiNaMali);
    QString printerBoldPrefix;
    QString printerBoldSuffix;
    QString printerLadicaPrefix;
    QString printerLadicaSuffix;
    QString printerRezacPrefix;
    QString printerRezacSuffix;
    QString printerResetPrefix;
    QString printerResetSuffix;
    char PrinterPort;
    int SirinaPapira;
    bool InozemniRacun;
    void UcitajPostavkePrintera();
    void IspisRNalog(const int &RnalogID);
    void SaljiNaPrinter(QString FileZaIspis);
    void SaljiNaESCPOS(QString FileZaIspis);
    QString ispisQRurl;
    QString ispisQRpath;

//private:
//    bool VratiMjesto(QString &Polje, int &PredvidenaDuljina) const;
private:
    QSqlDatabase dbP;
    QString PokretacJasper;
    QString PokretacJasperRunPath;
    int lenKolicina;
    int lenJedCijena;
    int lenUkpCijena;
    int lenSirNaziv;
    bool bRacunStavkaJedanRed;



};

#endif // ISPISMALI_H
