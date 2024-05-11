#ifndef KOBOPLATFORMEXTRAINTEGRATION_H
#define KOBOPLATFORMEXTRAINTEGRATION_H

#include <qpa/qplatformintegration.h>
#include <qpa/qplatformnativeinterface.h>
#include <QtCore/QRegularExpression>
#include "KoboButtonIntegration.h"
#include "KoboDeviceExtraDescriptor.h"
#include "KoboPlatformAdditions.h"
#include "KoboWifiManager.h"

class QAbstractEventDispatcher;
class QFbVtHandler;
class QEvdevKeyboardManager;

class KoboPlatformExtraIntegration /*: public QPlatformIntegration, public QPlatformNativeInterface*/
{
public:
    explicit KoboPlatformExtraIntegration(const QStringList &paramList);
    ~KoboPlatformExtraIntegration() = default;

    // void initialize() override;
    // QAbstractEventDispatcher *createEventDispatcher() const override;
    // QFunctionPointer platformFunction(const QByteArray &function) const override;
    KoboDeviceExtraDescriptor *deviceDescriptor();

private:
    static KoboDeviceExtraDescriptor getKoboDeviceExtraDescriptorStatic();
    void createInputHandlers();
    static int getBatteryLevelStatic();
    static bool isBatteryChargingStatic();
    static void setFrontlightLevelStatic(int val, int temp);
    static void enableWiFiConnectionStatic();
    static void disableWiFiConnectionStatic();
    static bool testInternetConnectionStatic(int timeout);

    QStringList m_paramList;
    KoboDeviceExtraDescriptor koboExtraDescriptor;
    KoboWifiManager wifiManager;
    QEvdevKeyboardManager *kbdMgr;
    KoboButtonIntegration *koboKeyboard;
    KoboPlatformAdditions *koboAdditions;
    bool debug;
};

#endif  // KOBOPLATFORMEXTRAINTEGRATION_H
