TARGET = KoboExtraFunk

# disable symlinks & versioning
CONFIG += plugin

include(../../../config.pri)

TEMPLATE = lib

DEFINES += TEST_LIB\

QT +=  widgets \
    core-private gui-private \
    service_support-private eventdispatcher_support-private input_support-private

INCLUDEPATH += $$PWD/../qt5-kobo-platform-plugin/src \

SOURCES = \
    src/KoboButtonIntegration.cpp \
    src/KoboDeviceExtraDescriptor.cpp \
    src/KoboWifiManager.cpp \
    src/KoboPlatformExtraIntegration.cpp \
    src/KoboPlatformAdditions.cpp \

HEADERS = \
    src/KoboButtonIntegration.h \
    src/KoboDeviceExtraDescriptor.h \
    src/KoboPlatformExtraFunctions.h \
    src/KoboWifiManager.h \
    src/KoboPlatformExtraIntegration.h \
    src/KoboPlatformAdditions.h \


OTHER_FILES += \
    scripts/disable-wifi.sh \
    scripts/restore-wifi.sh \

target.path =/mnt/onboard/.adds/$${PROJECT_APP_NAME}/lib/
INSTALLS += target
