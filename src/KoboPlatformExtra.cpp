#include "KoboPlatformExtra.h"

#include <QtEventDispatcherSupport/private/qgenericunixeventdispatcher_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <QtServiceSupport/private/qgenericunixservices_p.h>
#include <qpa/qplatforminputcontextfactory_p.h>
#include "koboplatformfunctions.h"
#include "KoboDeviceExtraDescriptor.h"

KoboPlatformExtra::KoboPlatformExtra() :
    koboExtraDescriptor(nullptr),
    kbdMgr(nullptr),
    koboKeyboard(nullptr),
    koboPwrButton(nullptr),
    koboAdditions(nullptr),
    debug(false)
{
    const char* debug_env_p = std::getenv("KOBO_EXTRA_FUNK_DEBUG");
    debug = (debug_env_p != nullptr && std::string{debug_env_p} == "TRUE");

    koboDescriptor = KoboPlatformFunctions::getKoboDeviceDescriptor();
    koboExtraDescriptor = DetermineExtraDescriptor(koboDescriptor);
    koboAdditions = new KoboPlatformAdditions(this, *koboExtraDescriptor);
    koboKeyboard = new KoboButtonIntegration(this, koboExtraDescriptor->ntxDev, debug);
    if (koboExtraDescriptor->powerDev != "")
        koboPwrButton = new KoboButtonIntegration(this, koboExtraDescriptor->powerDev, debug);
}

void KoboPlatformExtra::SetFrontlightLevelStatic(int val, int temp)
{
    Instance().koboAdditions->SetFrontlightLevel(val, temp);
}

int KoboPlatformExtra::GetBatteryLevelStatic()
{
    return Instance().koboAdditions->GetBatteryLevel();
}

bool KoboPlatformExtra::IsBatteryChargingStatic()
{
    return  Instance().koboAdditions->IsBatteryCharging();
}

KoboDeviceExtraDescriptor const* KoboPlatformExtra::getKoboDeviceExtraDescriptorStatic()
{
    return Instance().koboExtraDescriptor;
}

void KoboPlatformExtra::EnableWiFiConnectionStatic()
{
    Instance().wifiManager.EnableWiFiConnection();
}

void KoboPlatformExtra::DisableWiFiConnectionStatic()
{
    Instance().wifiManager.DisableWiFiConnection();
}

bool KoboPlatformExtra::IsWifiManagerBusy()
{
    return Instance().wifiManager.IsBusy();
}

bool KoboPlatformExtra::TestInternetConnectionStatic(int timeout/*=5*/)
{
    return Instance().wifiManager.TestInternetConnection(timeout);
}

void KoboPlatformExtra::SetStatusLedEnabledStatic(bool enabled)
{
    return  Instance().koboAdditions->SetStatusLedEnabled(enabled);
}

KoboWifiManager const& KoboPlatformExtra::GetKoboWifiManager()
{
    return Instance().wifiManager;
}
