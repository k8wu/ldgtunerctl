TEMPLATE = app
TARGET = ldgtunerctl

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += \
    src/commlink.cpp \
    src/main.cpp \
    src/windowabout.cpp \
    src/windowmain.cpp \
    src/windowselectdevice.cpp

HEADERS += \
    src/appconfig.h \
    src/commlink.h \
    src/windowabout.h \
    src/windowmain.h \
    src/windowselectdevice.h

INCLUDEPATH += \
    /usr/include \
    /usr/local/include

QMAKE_LIBDIR_FLAGS += \
    -lserialport \
    -lusb

linux {
    DEFINES += LINUX=1
}

mac {
    DEFINES += MACOS=1
}

freebsd {
    DEFINES += FREEBSD=1
}
