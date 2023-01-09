#!/usr/bin/python2
import fisk
import lxml.etree as et
from datetime import date, timedelta
import sys
from timeit import default_timer as timer
import getopt,os
import json


VER="1.0.2"

def otvori_json(file, zki=False):
  with open(file) as json_file:
    data = json.load(json_file)
    BrRac = data['BrRac']
    del data['BrRac']
# ovo vrati ---- samo je maknuto radi firme koja nije u sustavu PDVa
#    Pdv = data['PDV']['Porezi']
#    del data['PDV']
    '''
    decode FISKAL_2.p12 with
    openssl pkcs12 -in FISKAL_2.p12 -out certificate.pem -nodes
    edit certificate.pem and remove certs leave only key ---BEGIN -> END---
    openssl.exe rsa -in certificate.key -des3 -out passkey.pem
    '''
    fisk.FiskInit.init('passkey.pem', "Sifra101", 'certificate.pem',True)
    racun = fisk.Racun(data = {"Oib": str(data['Oib']),
              "USustPdv": str(data['USustPdv']),
              "DatVrijeme": str(data['DatVrijeme']),
              "BrRac": fisk.BrRac(BrRac),
#              "Pdv": [fisk.Porez(Pdv)],
              "IznosUkupno": str(data['IznosUkupno']),
              "NacinPlac": str(data['NacinPlac']),
              "OibOper": str(data['OibOper']),
              "OznSlijed": str(data['OznSlijed']),
              "NakDost": str(data['NakDost'])})
# ovo vrati ---- samo je maknuto radi firme koja nije u sustavu PDVa
#    racun.BrRac = BrRac
#    racun.Porez = Pdv
    print "ZKI:" + racun.ZastKod

    #create Request and send it to server (DEMO) and print reply
    if (zki==False):
        racunZahtjev = fisk.RacunZahtjev(racun)
        racun_reply = racunZahtjev.execute()
        if(racun_reply != False):
            print "JIR:" + racun_reply
        else:
            errors = racunZahtjev.get_last_error()
            print "RacunZahtjev reply errors:"
            for error in errors:
                print error

    #fiskpy deinitialization - maybe not needed but good for correct garbage cleaning
    fisk.FiskInit.deinit()


def return_help():
    print '''
VER: {}
Usage: p3.py json.file
    -h --help  -- this screen
    -z --zki   -- generate zk from json file
    -s --send  -- generate zk and jir
    '''.format(VER)

def read_argv(argv):
    try:
        opts, args = getopt.getopt(argv,"h:z:s:",["","z=","s="])
    except getopt.GetoptError:
        return_help()
        sys.exit(2)
    for opt, arg in opts:
        if opt in ('-h','--help'):
            return_help()
            sys.exit(0)
        elif opt in ('-z','--zki'):
            otvori_json(sys.argv[2],True)
        elif opt in ('-s','--send'):
            otvori_json(sys.argv[2],False)
        else:
            return_help()
            sys.ext(2)


if __name__ == "__main__":
  timer_start = timer()
  if (len(sys.argv)<=1):
      return_help()
      sys.exit(2)
  read_argv(sys.argv[1:])
  print("VER:" + VER)
  print("DoneIn:" + str(timer() - timer_start) + "seconds")
