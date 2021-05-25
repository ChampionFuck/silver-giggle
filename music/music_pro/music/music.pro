TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c

HEADERS += \
    bmp_show.h \
    head.h \
    touch.h

LIBS += \
        -lpthread
