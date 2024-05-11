#include "KoboPlatformExtraIntegration.h"

#include <QtEventDispatcherSupport/private/qgenericunixeventdispatcher_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <QtServiceSupport/private/qgenericunixservices_p.h>
#include <qpa/qplatforminputcontextfactory_p.h>
#include "KoboPlatformExtraFunctions.h"

KoboPlatformExtraIntegration::KoboPlatformExtraIntegration(const QStringList &paramList) :
    m_paramList(paramList),
    kbdMgr(nullptr),
    koboKeyboard(nullptr),
    koboAdditions(nullptr),
    debug(false)
{
    // koboExtraDescriptor = determineDevice(); // TODO NOW
    createInputHandlers();
}

// void KoboPlatformExtraIntegration::initialize()
// {
//     createInputHandlers();

//     qWarning("kobofb: Finished initialization.");
// }

// QAbstractEventDispatcher *KoboPlatformExtraIntegration::createEventDispatcher() const
// {
//     return createUnixEventDispatcher();
// }

void KoboPlatformExtraIntegration::createInputHandlers()
{
    for (const QString &arg : qAsConst(m_paramList))
    {
        if (arg.contains("debug"))
        {
            debug = true;
            break;
        }
    }

    koboKeyboard = new KoboButtonIntegration(this, koboExtraDescriptor.ntxDev, debug);
    koboAdditions = new KoboPlatformAdditions(this, koboExtraDescriptor);
}

KoboDeviceExtraDescriptor *KoboPlatformExtraIntegration::deviceDescriptor()
{
    return &koboExtraDescriptor;
}

QFunctionPointer KoboPlatformExtraIntegration::platformFunction(const QByteArray &function) const
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

void KoboPlatformExtraIntegration::setFrontlightLevelStatic(int val, int temp)
{
    KoboPlatformExtraIntegration *self =
        static_cast<KoboPlatformExtraIntegration *>(QGuiApplicationPrivate::platformIntegration());

    self->koboAdditions->setFrontlightLevel(val, temp);
}

int KoboPlatformExtraIntegration::getBatteryLevelStatic()
{
    KoboPlatformExtraIntegration *self =
        static_cast<KoboPlatformExtraIntegration *>(QGuiApplicationPrivate::platformIntegration());

    return self->koboAdditions->getBatteryLevel();
}

bool KoboPlatformExtraIntegration::isBatteryChargingStatic()
{
    KoboPlatformExtraIntegration *self =
        static_cast<KoboPlatformExtraIntegration *>(QGuiApplicationPrivate::platformIntegration());

    return self->koboAdditions->isBatteryCharging();
}

KoboDeviceExtraDescriptor KoboPlatformExtraIntegration::getKoboDeviceExtraDescriptorStatic()
{
    KoboPlatformExtraIntegration *self =
        static_cast<KoboPlatformExtraIntegration *>(QGuiApplicationPrivate::platformIntegration());

    return *self->deviceDescriptor();
}

void KoboPlatformExtraIntegration::enableWiFiConnectionStatic()
{
    KoboPlatformExtraIntegration *self =
        static_cast<KoboPlatformExtraIntegration *>(QGuiApplicationPrivate::platformIntegration());

    self->wifiManager.enableWiFiConnection();
}

void KoboPlatformExtraIntegration::disableWiFiConnectionStatic()
{
    KoboPlatformExtraIntegration *self =
        static_cast<KoboPlatformExtraIntegration *>(QGuiApplicationPrivate::platformIntegration());

    self->wifiManager.disableWiFiConnection();
}

bool KoboPlatformExtraIntegration::testInternetConnectionStatic(int timeout)
{
    KoboPlatformExtraIntegration *self =
        static_cast<KoboPlatformExtraIntegration *>(QGuiApplicationPrivate::platformIntegration());

    return self->wifiManager.testInternetConnection(timeout);
}
