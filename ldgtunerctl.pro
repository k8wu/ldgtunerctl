TEMPLATE = app
TARGET = ldgtunerctl

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += \
    src/commlink.cpp \
    src/main.cpp \
    src/windowmain.cpp \
    src/windowselectdevice.cpp

HEADERS += \
    src/appconfig.h \
    src/commlink.h \
    src/windowmain.h \
    src/windowselectdevice.h

DEFINES += \
    MACOS=1
