#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from escpos import *
import os
import sys, getopt
import re

qrPath = ""
racPath = ""
printaj = False


def ispis():
    # p = printer.Usb(0x0419, 0x3c00,0,0x01,0x02)
    # p = printer.File("/dev/usb/lp0")
    p = printer.Dummy("/tmp/IS")
    if len(racPath) > 0:
        with open(racPath, 'r') as f:
            for l in f:
                text_type="NORMAL"
                align="left"
                width=1
                height=1
                
                nl = re.search(r'<c>(.*?)</c>', l)
                if nl:
                    l = nl[1] + "\n"
                    align="center"

                nl = re.search(r'<b>(.*?)</b>', l)
                if nl:
                    text_type="B"
                    height=2
                    l = nl[1] + "\n"

                p.set(text_type=text_type,align=align,width=width,height=height)
                p.text(l)
                p.set()


    if len(qrPath) > 0:
        p.set(align="center")
        p.image(qrPath)
        p.set()

    p.cut()


def usage():
    print(""" 
    Usage: 
       -q <path_to_qr_image> | --qr_path=<path_to_qr_image
       -r <path_to_racun> | --racun_path=<path_to_racun
       -h|--help 
    """)


try:
    opts, args = getopt.getopt(sys.argv[1:], "q:r:h",
                               ["qr_path=", "help", "racun_path="])
except getopt.GetoptError:
    usage()
    sys.exit(2)

for opt, arg in opts:
    if opt in ('-q', '--qr_path'):
        qrPath = arg
    elif opt in ('-r', '--racun_path'):
        racPath = arg
        printaj = True
    elif opt in ('-h', '--help'):
        usage()
        sys.exit(2)
    else:
        usage()
        sys.exit(2)

if printaj:
    ispis()
