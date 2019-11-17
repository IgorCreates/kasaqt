#ifndef FISKAL_XML_H
#define FISKAL_XML_H

#include <QString>

class fiskal_XML
{
public:
    fiskal_XML();
    void fisk_echo_req();
    void fisk_echo_res();
    void fisk_poslovniProstor_req();
    void fisk_poslovniProstor_res();
    void fisk_racuni_req();
    void fisk_racuni_res();
};

#endif // FISKAL_XML_H
