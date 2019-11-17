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




//#include <QtGui/QApplication>
#include <QApplication>
//#include <QtGui>
#include "ui/frmlogin.h"
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QTranslator>


//#include <ui/frmtipkovnica.h>

//#include <QtGui>
//#include "ui/frmtartodabir.h"
//#include ""
//#include <QUuid>
void LoadLang(QString lang){

    QTranslator * qt_translator = new QTranslator;
    QString fst = QApplication::applicationDirPath() + QString("/languages/%1.qm").arg(lang);
    if (!qt_translator->load(fst))
    {
        delete(qt_translator);
    }else {
        qApp->installTranslator(qt_translator);
    }
}

void myMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    QString dt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //QString txt = QString("[%1] ").arg(dt);
    switch (type) {
    case QtDebugMsg:
        txt = QString("%1-Debug: %2").arg(dt).arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("%1-Warning: %2").arg(dt).arg(msg);
    break;
    case QtCriticalMsg:
        txt = QString("%1-Critical: %2").arg(dt).arg(msg);
    break;
    case QtFatalMsg:
        txt = QString("%1-Fatal: %2").arg(dt).arg(msg);
        abort();
    }
    QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream ts(&outFile);
    ts << txt << endl;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString VerProga = " os_1.2";

    qApp->setQuitOnLastWindowClosed(true);
    qDebug() << QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    qDebug() << QT_VERSION << " "  << QT_VERSION_STR << " " << qVersion();

#if QT_VERSION > 0x050000
    app.setOrganizationName("KasaQT DEMO");
    app.setApplicationName("FiskalKasa");
    app.setApplicationVersion(VerProga);
    app.setApplicationDisplayName("FiskalKasa");
    qApp->setProperty("App_Verzija",app.applicationVersion());

    QCommandLineParser parser;
    QCommandLineOption arg_verzija("verzija","Verzija:",qApp->property("App_Verzija").toString());
    parser.addOption(arg_verzija);
    QCommandLineOption arg_opcija_deb("debug","Pokrenuti debug log");
    parser.addOption(arg_opcija_deb);
    QCommandLineOption arg_opcija_touch("touch","Pokrenuti program u touch formi");
    parser.addOption(arg_opcija_touch);
    QCommandLineOption art_opcija_rftag("rftag","Dodana opcija za login preko RFTag kartica/narukvica/privjeska");
    parser.addOption(art_opcija_rftag);
    QCommandLineOption arg_opcija_narudzba("narudzba","Pokrenuti program u touch da radi samo narudzbu na sank");
    parser.addOption(arg_opcija_narudzba);
    QCommandLineOption arg_opcija_help = parser.addHelpOption();


    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
     QStringList arguments = QCoreApplication::arguments();
     if (!parser.parse(arguments)) {
         qWarning() << parser.helpText() << '\n' << parser.errorText();
         exit(1);
     }
     if (parser.isSet(arg_opcija_help)) {
         qWarning() << parser.helpText();
         exit(0);
     }
     if (parser.isSet(arg_verzija))
     {
         //QDebug() << app.applicationName() << " " << app.applicationVersion();
         qWarning() << app.applicationName() << " " << app.applicationVersion();
         exit(0);
     }
     if (parser.isSet(arg_opcija_deb))
     {
         //qInstallMessageHandler(myMessageHandler());
     }
     if (parser.isSet(arg_opcija_touch))
     {
         qApp->setProperty("Prog-GUIizgled","touch");
     }
     if (parser.isSet(art_opcija_rftag))
         qApp->setProperty("Prog-LOGIN","RFTAG");
     if (parser.isSet(arg_opcija_narudzba))
         qApp->setProperty("Prog-GUInarudzba","1");

/*
     if (parser.positionalArguments().count() == 1)
         source = QUrl::fromLocalFile(parser.positionalArguments().first());
*/

#endif

#if QT_VERSION < 0x050000
    qApp->setProperty("App_Verzija",VerProga);

    for (int i = 1; i < qApp->arguments().count(); ++i)
    {
        const QString param = qApp->argv()[i];

        if (param == "--verzija")
        {
            qDebug() << "Verzija: " << qApp->property("App_Verzija").toString();
            qApp->deleteLater();
            return 0;
        }
        if (param == "--debug")
        {
            qInstallMsgHandler(myMessageHandler);
            //qInstallMsgHandler(myMessageHandler());
        }
        if (param =="--touch")
        {
            qApp->setProperty("Prog-GUIizgled","touch");
        }
        if (param == "--help")
        {
            qDebug() << "Arg:\n--help-pomoc\n--debug-ukljucuje debug u log\n--verzija-verzija programa\n--touch-touch gui\n--lang lang_hr|en|...\n";
            qApp->deleteLater();
            return 0;
        }
        if (param == "--rftag")
        {
            qApp->setProperty("Prog-LOGIN","RFTAG");
        }
        if (param == "--narudzba")
            qApp->setProperty("Prog-GUInarudzba","1");
		if (param == "--lang")
		{
			int nextParam=i+1;
			LoadLang(qApp->argv()[nextParam]);
		}
			
    }
#endif

    //QString bb = "ZKI : bbd743f6fd3abe1ba2a145dff71c80d7\r\nRacunZahtjev reply errors:\r\nOIB iz poruke zahtjeva nije jednak OIB-u iz certifikata.\r\n('Done in ', 0.2754233, 'seconds')\r\nJIR is: c59de779-28d1-4714-9141-1bd2c10b852f";
//    QString bb = "ZKI : 8507c9d0ab7ae82288c0e989efbc10c8\r\nJIR is: d940a715-463b-4de6-96e9-1bd2c10b4d52\r\n";
//    QStringList bL = bb.split("\r\n");

//    QFile file("test.txt");
//    if (file.open(QIODevice::ReadWrite))
//    {
//        QTextStream stream(&file);
//        stream <<  bb <<endl;
//    }

//    //qDebug() << bb;
//    //qDebug() << bL;
//    foreach (QString i, bL) {
//        //qDebug() << "List items = " << i;
//        if (i.contains("ZKI"))
//        {
//            qDebug() << i.split(":");
//            qDebug() << QString(i.split(":")[1]).trimmed();
//        }
//        if (i.contains("JIR"))
//        {
//            qDebug() << i.split(":");
//            qDebug() << QString(i.split(":")[1]).trimmed();
//        }
//    }

    frmLogin w;
    w.show();
    w.PromjenaKorisnikaP(false);
    return app.exec();

}

