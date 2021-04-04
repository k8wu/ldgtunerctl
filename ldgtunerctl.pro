TEMPLATE = app
TARGET = ldgtunerctl

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += \
    src/commlink.cpp \
    src/config.cpp \
    src/main.cpp \
    src/windowabout.cpp \
    src/windowmain.cpp \
    src/windowselectdevice.cpp

HEADERS += \
    src/appconfig.h \
    src/commlink.h \
    src/config.h \
    src/windowabout.h \
    src/windowmain.h \
    src/windowselectdevice.h

unix:!macx: CONFIG += link_pkgconfig
unix:!macx: PKGCONFIG += libserialport
macx: INCLUDEPATH += /usr/local/include
macx: LIBS += -L/usr/local/lib -lserialport -lusb
freebsd: PKGCONFIG += libusb-1.0
