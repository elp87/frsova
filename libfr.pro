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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Загрузки/CryptLib/release/ -lcrypt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Загрузки/CryptLib/debug/ -lcrypt
else:unix: LIBS += -L$$PWD/../../Загрузки/CryptLib/ -lcrypt

INCLUDEPATH += $$PWD/../../Загрузки/CryptLib
DEPENDPATH += $$PWD/../../Загрузки/CryptLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Загрузки/CryptLib/release/libcrypt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Загрузки/CryptLib/debug/libcrypt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Загрузки/CryptLib/release/crypt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Загрузки/CryptLib/debug/crypt.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../Загрузки/CryptLib/libcrypt.a
