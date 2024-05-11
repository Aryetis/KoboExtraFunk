TARGET = KoboExtraFunk

# disable symlinks & versioning
CONFIG += plugin

include(../../../config.pri)

TEMPLATE = lib

DEFINES += TEST_LIB\

QT += \

INCLUDEPATH += $$PWD/../qt5-kobo-platform-plugin/src \

SOURCES = \
    src/KoboButtonIntegration.cpp \
    src/KoboDeviceExtraDescriptor.cpp \
    src/KoboPlatformExtraFunctions.cpp \
    src/KoboWifiManager.cpp \

HEADERS = \
    src/KoboButtonIntegration.h \
    src/KoboDeviceExtraDescriptor.h \
    src/KoboPlatformExtraFunctions.h \
    src/KoboWifiManager.h \


OTHER_FILES += \
    scripts/disable-wifi.sh \
    scripts/restore-wifi.sh \

target.path =/mnt/onboard/.adds/$${PROJECT_APP_NAME}/lib/
INSTALLS += target
