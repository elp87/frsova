TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lgmp
LIBS+= -L/usr/lib -lgmp

SOURCES += main.c \
    LibSha1.c


INCLUDEPATH += $$PWD/../build-stribog-Desktop-Debug
DEPENDPATH += $$PWD/../build-stribog-Desktop-Debug

HEADERS += \
    data.h \
    LibSha1.h
