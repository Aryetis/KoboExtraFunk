#include "KoboPlatformExtraIntegration.h"

#include <QtEventDispatcherSupport/private/qgenericunixeventdispatcher_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <QtServiceSupport/private/qgenericunixservices_p.h>
#include <qpa/qplatforminputcontextfactory_p.h>
#include "KoboPlatformExtraFunctions.h"

KoboPlatformIntegration::KoboPlatformIntegration(const QStringList &paramList) :
    m_paramList(paramList),
    kbdMgr(nullptr),
    koboKeyboard(nullptr),
    koboAdditions(nullptr),
    debug(false)
{
    koboExtraDescriptor = determineDevice();
}

void KoboPlatformIntegration::initialize()
{
    createInputHandlers();

    qWarning("kobofb: Finished initialization.");
}

QAbstractEventDispatcher *KoboPlatformIntegration::createEventDispatcher() const
{
    return createUnixEventDispatcher();
}

void KoboPlatformIntegration::createInputHandlers()
{

    for (const QString &arg : qAsConst(m_paramList))
    {
        if (arg.contains("debug"))
        {
            debug = true;
            break;
        }
    }

    koboKeyboard = new KoboButtonIntegration(this, koboExtraDescriptor.ntxDev, false);
    koboAdditions = new KoboPlatformAdditions(this, koboExtraDescriptor);
}

KoboDeviceExtraDescriptor *KoboPlatformIntegration::deviceDescriptor()
{
    return &koboExtraDescriptor;
}

QFunctionPointer KoboPlatformIntegration::platformFunction(const QByteArray &function) const
{
    if (function == KoboPlatformExtraFunctions::setFrontlightLevelIdentifier())
        return QFunctionPointer(setFrontlightLevelStatic);
    else if (function == KoboPlatformExtraFunctions::getBatteryLevelIdentifier())
        return QFunctionPointer(getBatteryLevelStatic);
    else if (function == KoboPlatformExtraFunctions::isBatteryChargingIdentifier())
        return QFunctionPointer(isBatteryChargingStatic);
    else if (function == KoboPlatformExtraFunctions::getKoboDeviceExtraDescriptorIdentifier())
        return QFunctionPointer(getKoboDeviceExtraDescriptorStatic);
    else if (function == KoboPlatformExtraFunctions::testInternetConnectionIdentifier())
        return QFunctionPointer(testInternetConnectionStatic);
    else if (function == KoboPlatformExtraFunctions::enableWiFiConnectionIdentifier())
        return QFunctionPointer(enableWiFiConnectionStatic);
    else if (function == KoboPlatformExtraFunctions::disableWiFiConnectionIdentifier())
        return QFunctionPointer(disableWiFiConnectionStatic);

    return 0;
}

void KoboPlatformIntegration::setFrontlightLevelStatic(int val, int temp)
{
    KoboPlatformIntegration *self =
        static_cast<KoboPlatformIntegration *>(QGuiApplicationPrivate::platformIntegration());

    self->koboAdditions->setFrontlightLevel(val, temp);
}

int KoboPlatformIntegration::getBatteryLevelStatic()
{
    KoboPlatformIntegration *self =
        static_cast<KoboPlatformIntegration *>(QGuiApplicationPrivate::platformIntegration());

    return self->koboAdditions->getBatteryLevel();
}

bool KoboPlatformIntegration::isBatteryChargingStatic()
{
    KoboPlatformIntegration *self =
        static_cast<KoboPlatformIntegration *>(QGuiApplicationPrivate::platformIntegration());

    return self->koboAdditions->isBatteryCharging();
}

KoboDeviceExtraDescriptor KoboPlatformIntegration::getKoboDeviceExtraDescriptorStatic()
{
    KoboPlatformIntegration *self =
        static_cast<KoboPlatformIntegration *>(QGuiApplicationPrivate::platformIntegration());

    return *self->deviceDescriptor();
}

void KoboPlatformIntegration::enableWiFiConnectionStatic()
{
    KoboPlatformIntegration *self =
        static_cast<KoboPlatformIntegration *>(QGuiApplicationPrivate::platformIntegration());

    self->wifiManager.enableWiFiConnection();
}

void KoboPlatformIntegration::disableWiFiConnectionStatic()
{
    KoboPlatformIntegration *self =
        static_cast<KoboPlatformIntegration *>(QGuiApplicationPrivate::platformIntegration());

    self->wifiManager.disableWiFiConnection();
}

bool KoboPlatformIntegration::testInternetConnectionStatic(int timeout)
{
    KoboPlatformIntegration *self =
        static_cast<KoboPlatformIntegration *>(QGuiApplicationPrivate::platformIntegration());

    return self->wifiManager.testInternetConnection(timeout);
}
