TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lgmp
LIBS+= -L/usr/lib -lgmp

SOURCES += main.c


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-stribog-Desktop-Debug/release/ -lstribog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-stribog-Desktop-Debug/debug/ -lstribog
else:unix: LIBS += -L$$PWD/../build-stribog-Desktop-Debug/ -lstribog

INCLUDEPATH += $$PWD/../build-stribog-Desktop-Debug
DEPENDPATH += $$PWD/../build-stribog-Desktop-Debug
