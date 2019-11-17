import fisk
import lxml.etree as et
from datetime import date, timedelta
import sys
from timeit import default_timer as timer
import json


def otvori_json(file):
  with open(file) as json_file:
    data = json.load(json_file)
    BrRac = data['BrRac']
    Pdv = data['PDV']['Porezi']
    del data['BrRac']
    del data['PDV']
    fisk.FiskInit.init('passkey.pem', "sifra101", 'amplituda.crt',True)
    racun = fisk.Racun(data = {"Oib": str(data['Oib']),
              "USustPdv": str(data['USustPdv']),
              "DatVrijeme": str(data['DatVrijeme']),
              "BrRac": fisk.BrRac(BrRac),
              "Pdv": [fisk.Porez(Pdv)],
              "IznosUkupno": str(data['IznosUkupno']),
              "NacinPlac": str(data['NacinPlac']),
              "OibOper": str(data['OibOper']),
              "OznSlijed": str(data['OznSlijed']),
              "NakDost": str(data['NakDost'])})
#    racun.BrRac = BrRac
#    racun.Porez = Pdv
    print "ZKI : " + racun.ZastKod

    #create Request and send it to server (DEMO) and print reply
    racunZahtjev = fisk.RacunZahtjev(racun)
    racun_reply = racunZahtjev.execute()
    if(racun_reply != False):
        print "JIR is: " + racun_reply
    else:
        errors = racunZahtjev.get_last_error()
        print "RacunZahtjev reply errors:"
        for error in errors:
            print error

    #fiskpy deinitialization - maybe not needed but good for correct garbage cleaning
    fisk.FiskInit.deinit()

if __name__ == "__main__":
  timer_start = timer()
  otvori_json(sys.argv[1])
  print("DoneIn:" + timer() - timer_start + "seconds")
