#ifndef KOBOPLATFORMEXTRA_H
#define KOBOPLATFORMEXTRA_H

#include "KoboButtonIntegration.h"
#include "KoboDeviceExtraDescriptor.h"
#include "KoboPlatformAdditions.h"
#include "KoboWifiManager.h"

class QEvdevKeyboardManager;

class KoboPlatformExtra : public QObject
{
    Q_OBJECT
public:
    static KoboPlatformExtra& Instance()
    {
        static KoboPlatformExtra instance;
        return instance;
    }

    static KoboDeviceExtraDescriptor const* getKoboDeviceExtraDescriptorStatic();
    static int GetBatteryLevelStatic();
    static bool IsBatteryChargingStatic();
    static void SetFrontlightLevelStatic(int val, int temp);
    static void EnableWiFiConnectionStatic();
    static void DisableWiFiConnectionStatic();
    static bool TestInternetConnectionStatic(int timeout);
    static void SetStatusLedEnabledStatic(bool enabled);

private:
    KoboPlatformExtra();

    KoboDeviceDescriptor koboDescriptor;
    KoboDeviceExtraDescriptor const* koboExtraDescriptor;
    KoboWifiManager wifiManager;
    QEvdevKeyboardManager *kbdMgr;
    KoboButtonIntegration *koboKeyboard, *koboPwrButton;
    KoboPlatformAdditions *koboAdditions;
    bool debug;
};

#endif  // KOBOPLATFORMEXTRA_H
