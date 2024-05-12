#-------------------------------------------------
# WORKING ON : - Restoring ButtonIntegration
#              - Fixing WifiManager / replacing it with a proper method
#              - Cleaning a bunch of stuff
#-------------------------------------------------

TARGET = KoboExtraFunk

# to disable symlinks & versioning that comes with usual library
# kobo /mnt/onboard is vfat => no symlink
CONFIG += plugin

include(../../../config.pri)

TEMPLATE = lib

DEFINES +=

QT +=  widgets \
    core-private gui-private \
    service_support-private eventdispatcher_support-private input_support-private

INCLUDEPATH += $$PWD/../qt5-kobo-platform-plugin/src \

SOURCES = \
    src/KoboButtonIntegration.cpp \
    src/KoboDeviceExtraDescriptor.cpp \
    src/KoboPlatformExtra.cpp \
    src/KoboWifiManager.cpp \
    src/KoboPlatformAdditions.cpp

HEADERS = \
    src/KoboButtonIntegration.h \
    src/KoboDeviceExtraDescriptor.h \
    src/KoboPlatformExtra.h \
    src/KoboWifiManager.h \
    src/KoboPlatformAdditions.h \

OTHER_FILES += \
    scripts/disable-wifi.sh \
    scripts/restore-wifi.sh \

target.path =/mnt/onboard/.adds/$${PROJECT_APP_NAME}/lib/
INSTALLS += target
