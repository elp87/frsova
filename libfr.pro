TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lgmp
LIBS+= -L/usr/lib -lgmp

SOURCES += main.c

