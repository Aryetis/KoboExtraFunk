#ifndef KOBOPLATFORMEXTRAFUNCTIONS_H
#define KOBOPLATFORMEXTRAFUNCTIONS_H

#include <QByteArray>
#include <QtGui/QGuiApplication>


class KoboPlatformExtraFunctions
{
public:
    typedef int (*getBatteryLevelType)();
    static QByteArray getBatteryLevelIdentifier() { return QByteArrayLiteral("getBatteryLevel"); }

    static int getBatteryLevel()
    {
        auto func = reinterpret_cast<getBatteryLevelType>(
            QGuiApplication::platformFunction(getBatteryLevelIdentifier()));
        if (func)
            return func();

        return 0;
    }

    typedef bool (*isBatteryChargingType)();
    static QByteArray isBatteryChargingIdentifier() { return QByteArrayLiteral("isBatteryCharging"); }

    static bool isBatteryCharging()
    {
        auto func = reinterpret_cast<isBatteryChargingType>(
            QGuiApplication::platformFunction(isBatteryChargingIdentifier()));
        if (func)
            return func();

        return false;
    }

    typedef void (*setFrontlightLevelType)(int val, int temp);
    static QByteArray setFrontlightLevelIdentifier() { return QByteArrayLiteral("setFrontlightLevel"); }

    static void setFrontlightLevel(int val, int temp)
    {
        auto func = reinterpret_cast<setFrontlightLevelType>(
            QGuiApplication::platformFunction(setFrontlightLevelIdentifier()));
        if (func)
            func(val, temp);
    }
    typedef KoboDeviceDescriptor (*getKoboDeviceDescriptorType)();
    static QByteArray getKoboDeviceDescriptorIdentifier()
    {
        return QByteArrayLiteral("getKoboDeviceDescriptor");
    }

    static KoboDeviceDescriptor getKoboDeviceDescriptor()
    {
        auto func = reinterpret_cast<getKoboDeviceDescriptorType>(
            QGuiApplication::platformFunction(getKoboDeviceDescriptorIdentifier()));
        if (func)
            return func();

        return KoboDeviceDescriptor();
    }

    typedef bool (*testInternetConnectionType)(int timeout);
    static QByteArray testInternetConnectionIdentifier()
    {
        return QByteArrayLiteral("testInternetConnection");
    }

    static bool testInternetConnection(int timeout = 2)
    {
        auto func = reinterpret_cast<testInternetConnectionType>(
            QGuiApplication::platformFunction(testInternetConnectionIdentifier()));
        if (func)
            return func(timeout);

        return false;
    }

    typedef void (*enableWiFiConnectionType)();
    static QByteArray enableWiFiConnectionIdentifier() { return QByteArrayLiteral("enableWiFiConnection"); }

    static void enableWiFiConnection()
    {
        auto func = reinterpret_cast<enableWiFiConnectionType>(
            QGuiApplication::platformFunction(enableWiFiConnectionIdentifier()));
        if (func)
            func();
    }

    typedef void (*disableWiFiConnectionType)();
    static QByteArray disableWiFiConnectionIdentifier() { return QByteArrayLiteral("disableWiFiConnection"); }

    static void disableWiFiConnection()
    {
        auto func = reinterpret_cast<disableWiFiConnectionType>(
            QGuiApplication::platformFunction(disableWiFiConnectionIdentifier()));
        if (func)
            func();
    }
};
#endif // KOBOPLATFORMEXTRAFUNCTIONS_H
