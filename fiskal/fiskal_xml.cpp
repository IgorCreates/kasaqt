#include "fiskal_xml.h"
#include "QApplication"
#include "QDomNode"
#include "QDebug"
#include <QLibrary>

extern "C"

fiskal_XML::fiskal_XML()
{

}
void fiskal_XML::fisk_echo_req()
{
    qDebug() << "Lib Start";
    QLibrary lib("/tmp/r/Raverus.FiskalizacijaDEV.COM.dll");
    lib.load();

    qDebug() << lib.isLoaded();
    if (!lib.load())
    {
        qDebug() << "\nGreska dll-load";
        qDebug() << lib.errorString();
    }
    typedef void (*MyProto)();
    MyProto myFun = (MyProto) lib.resolve("TestCom");
    if (myFun)
    {
        qDebug() << "Greska - fun";
    }else
    {
        myFun();
    }

}
