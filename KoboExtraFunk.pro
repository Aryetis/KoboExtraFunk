#-------------------------------------------------
# WORKING ON : - Fixing WifiManager / replacing it with a proper method cf QNDB :
#                   https://github.com/shermp/NickelDBus/blob/ab7ac0c7fee308b776a1359bb90898a21edfb54d/src/ndb/NDBDbus.h#L109C12-L109C21
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
    scripts/*.sh \

target.path =/mnt/onboard/.adds/$${PROJECT_APP_NAME}/lib/
INSTALLS += target

RESOURCES += \
    Ressources.qrc
