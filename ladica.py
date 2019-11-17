#!/usr/bin/python

#
#import shutil
#shutil.copy2('/opt/CashDrawer.txt', '/tmp/ispLadica.txt')

#srp-350 27,112,0,50,250
#kod = chr(27)+chr(112)+chr(0)+chr(50)+chr(250)

kodr = [27,112,0,50,250]
#kodr = [27,109]

kod = ""
for korr in kodr:
#    print "Dec=",korr,",hex=",hex(korr)[2:]
    kod += chr(korr)

#print "kod=",kod
f = open("/tmp/ispLadica.txt", "w+")
f.seek(0)
f.write(kod)
f.close()
