#
# This file is part of KasaQT, an open-source PointOfSale
#
# (C) Igor Jukic & Ante Biocic 2012-2016
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Contact e-mail: Igor Jukic <igor.jukic.76@gmail.com>
#
#

#https://github.com/KasaQT/kasaqt


#-------------------------------------------------
#
# Project created by QtCreator 2012-09-05T14:58:07
#
#-------------------------------------------------

QT       += core gui network \
            sql xml svg printsupport


TARGET = KasaQT
TEMPLATE = app


SOURCES += main.cpp\
    ui/frmkasa.cpp \
    ui/frmpregart.cpp \
    ui/frmzakljuci.cpp \
    ui/frmdodajstavku.cpp \
    ui/frmpopup_preg.cpp \
    ui/mojiheaderi.cpp \
    ui/frmkrajdana.cpp \
    ui/frmlogin.cpp \
    ui/frmodabirbaza.cpp \
    Ispis/ispismali.cpp \
    overlay.cpp \
    ui/frmopcije.cpp \
    ui/frmpregdokum.cpp \
    ui/frmtartodabir.cpp \
    ui/frmtouch.cpp \
    ui/classizracuni.cpp \
    ui/frmfiskalporuke.cpp \
    ui/frmprinteropcije.cpp \
    ui/frmispiskuhinja.cpp \
    ui/frmispisdelstolova.cpp \
    ui/frmrnalog.cpp \
    ui/frmkupac.cpp \
    ui/frmodabirstavke.cpp \
    ui/frmzaprimanjerobe.cpp \
    ui/frmispisodabir.cpp \
    ui/frmrazduzisklad.cpp \
    ui/clsizracuni.cpp \
    ui/frmcrm.cpp \
    ui/frmstolodvajanje.cpp \
    ui/frmpregrazni.cpp \
    ui/frmcrmpregled.cpp \
    ui/frmtipkovnica.cpp \
    ui/button.cpp \
    ui/QProgressIndicator.cpp \
    ui/smtp.cpp \
    ui/mailprozor.cpp \
    ui/omeni.cpp \
    ui/frmloginrftag.cpp \
    ui/stoodabir.cpp \
    ui/dragwidget.cpp \
    ui/frmzadnjiracpregled.cpp

HEADERS  += \
    ui/frmkasa.h \
    bazaSqlite.h \
    ui/frmpregart.h \
    ui/frmzakljuci.h \
    ui/frmdodajstavku.h \
    ui/baze.h \
    ui/frmpopup_preg.h \
    ui/mojiheaderi.h \
    ui/frmkrajdana.h \
    ui/frmlogin.h \
    ui/frmodabirbaza.h \
    Ispis/ispismali.h \
    overlay.h \
    ui/frmopcije.h \
    ui/frmpregdokum.h \
    ui/frmtartodabir.h \
    ui/frmtouch.h \
    ui/classizracuni.h \
    ui/frmfiskalporuke.h \
    ui/frmprinteropcije.h \
    ui/frmispiskuhinja.h \
    ui/frmispisdelstolova.h \
    ui/frmrnalog.h \
    ui/frmkupac.h \
    ui/frmodabirstavke.h \
    ui/frmzaprimanjerobe.h \
    ui/frmispisodabir.h \
    ui/frmrazduzisklad.h \
    ui/clsizracuni.h \
    ui/frmcrm.h \
    ui/frmstolodvajanje.h \
    ui/frmpregrazni.h \
    ui/frmcrmpregled.h \
    ui/frmtipkovnica.h \
    ui/button.h \
    ui/QProgressIndicator.h \
    ui/smtp.h \
    ui/mailprozor.h \
    ui/omeni.h \
    ui/frmloginrftag.h \
    ui/stoodabir.h \
    ui/dragwidget.h \
    ui/frmzadnjiracpregled.h

FORMS    += \
    ui/frmkasa.ui \
    ui/frmpregart.ui \
    ui/frmzakljuci.ui \
    ui/frmdodajstavku.ui \
    ui/frmpopup_preg.ui \
    ui/frmkrajdana.ui \
    ui/frmlogin.ui \
    ui/frmodabirbaza.ui \
    ui/frmopcije.ui \
    ui/frmpregdokum.ui \
    ui/frmtartodabir.ui \
    ui/frmfiskalporuke.ui \
    ui/frmprinteropcije.ui \
    ui/frmispiskuhinja.ui \
    ui/frmispisdelstolova.ui \
    ui/frmrnalog.ui \
    ui/frmkupac.ui \
    ui/frmodabirstavke.ui \
    ui/frmzaprimanjerobe.ui \
    ui/frmispisodabir.ui \
    ui/frmrazduzisklad.ui \
    ui/frmcrm.ui \
    ui/frmstolodvajanje.ui \
    ui/frmpregrazni.ui \
    ui/frmcrmpregled.ui \
    ui/frmtouch.ui \
    ui/frmtipkovnica.ui \
    ui/mailprozor.ui \
    ui/omeni.ui \
    ui/frmloginrftag.ui \
    ui/stoodabir.ui \
    ui/frmzadnjiracpregled.ui

#Release:DESTDIR = release
#Release:OBJECTS_DIR = release/.obj
#Release:MOC_DIR = release/.moc
#Release:RCC_DIR = release/.rcc
#Release:UI_DIR = release/.ui

#Debug:DESTDIR = debug
#Debug:OBJECTS_DIR = debug/.obj
#Debug:MOC_DIR = debug/.moc
#Debug:RCC_DIR = debug/.rcc
#Debug:UI_DIR = debug/.ui

#
#CONFIG(debug, debug|release) {
#    DESTDIR = build/debug
#} else {
#    DESTDIR = build/release
#}
#*/
#RC_FILE = app_ikona.rc
#ICON = logo.png


#--za touch i qsint libove --
#INCLUDEPATH += QSINT_ROOT/include
##// for release version
#LIBS += -LQSINT_BUILD_ROOT/lib -lqsint
##// for debug version
#LIBS += -LQSINT_BUILD_ROOT/lib -lqsintd
#



OTHER_FILES += \
    p.py \
    op.db

RESOURCES += \
    kasares.qrc



#unix|win32: LIBS += -ltableprinter
#win32: {
#    LIBS += ..\..\lib\tableprinter.lib
#    LIBS += $$[QT_INSTALL_LIBS]\QtGui.lib
#    LIBS += $$[QT_INSTALL_LIBS]\QtCore.lib
#}

#unix:  LIBS += -L/tmp/ -ltableprinter

message (LIBS: $$LIBS)


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qsint/lib/release/ -lQSCore
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qsint/lib/debug/ -lQSCore
#else:unix: LIBS += -L$$PWD/qsint/lib/ -lQSCore

#INCLUDEPATH += $$PWD/qsint/include
#DEPENDPATH += $$PWD/qsint/include

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/qsint/lib/release/libQSCore.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/qsint/lib/debug/libQSCore.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/qsint/lib/release/QSCore.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/qsint/lib/debug/QSCore.lib
#else:unix: PRE_TARGETDEPS += $$PWD/qsint/lib/libQSCore.a

TRANSLATIONS = languages/lang_hr.ts  languages/lang_en.ts

DISTFILES += \
    fiskal.py
