#ifndef KOBODEVICEEXTRADESCRIPTOR_H
#define KOBODEVICEEXTRADESCRIPTOR_H

#include <QString>

struct FrontlightSettings
{
    bool hasFrontLight = true;
    int frontlightMin = 0;
    int frontlightMax = 100;
    bool hasNaturalLight = false;
    bool hasNaturalLightMixer = false;
    bool naturalLightInverted = false;
    int naturalLightMin = 0;
    int naturalLightMax = 100;

    QString frontlightDevWhite = "/sys/class/backlight/mxc_msp430.0/brightness";
    QString frontlightDevMixer = "/sys/class/backlight/lm3630a_led/color";
    QString frontlightDevRed = "/sys/class/backlight/lm3630a_led1a";
    QString frontlightDevGreen = "/sys/class/backlight/lm3630a_ledb";
};

struct KoboDeviceExtraDescriptor
{
    bool hasKeys = false;
    bool canToggleChargingLED = false;
    FrontlightSettings frontlightSettings;
    // Standard sysfs path to the battery directory
    QString batterySysfs = "/sys/class/power_supply/mc13892_bat";
    // Stable path to the NTX input device (buttons)
    QString ntxDev = "/dev/input/event0";
    // Stable path to the Power Button input device
    QString powerDev = "null";
};

#endif // KOBODEVICEEXTRADESCRIPTOR_H
