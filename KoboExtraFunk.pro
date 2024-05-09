TARGET = KoboExtraFunk
VERSION = 1.0.0

include(../../../config.pri)

TEMPLATE = lib

DEFINES += TEST_LIB\

QT += \

INCLUDEPATH += $$PWD/../qt5-kobo-platform-plugin/src \

SOURCES = \
    src/TestExtraFunk.cpp

HEADERS = \
    src/TestExtraFunk.h

target.path =/mnt/onboard/.adds/$${PROJECT_APP_NAME}/lib/
INSTALLS += target
