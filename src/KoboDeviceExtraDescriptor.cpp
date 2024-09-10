#include "KoboDeviceExtraDescriptor.h"

// Thanks koreader for all the hardware info :
// https://github.com/koreader/koreader/blob/7ff40a341242abd586d2e6a49f3777465ab8950e/frontend/device/kobo/device.lua#L533

// Kobo Touch A/B:
KoboDeviceExtraDescriptor KoboTrilogyABEx
{
    .hasKeys = true,
    .frontlightSettings {.hasFrontLight = false},
};

// Kobo Touch C:
KoboDeviceExtraDescriptor KoboTrilogyCEx
{
    .hasKeys = true,
    .frontlightSettings {.hasFrontLight = false},
};

// Kobo Mini:
KoboDeviceExtraDescriptor KoboPixieEx
{
    .frontlightSettings {.hasFrontLight = false},
};

// Kobo Glo:
KoboDeviceExtraDescriptor KoboKrakenEx {};

// Kobo Glo HD:
KoboDeviceExtraDescriptor KoboAlyssumEx {};

// Kobo Touch 2.0:
KoboDeviceExtraDescriptor KoboPikaEx
{
    .frontlightSettings {.hasFrontLight = false},
};

// Kobo Aura:
KoboDeviceExtraDescriptor KoboPhoenixEx {};

// Kobo Aura HD:
KoboDeviceExtraDescriptor KoboDragonEx {};

// Kobo Aura H2O:
KoboDeviceExtraDescriptor KoboDahliaEx {};

// Kobo Aura H2O2:
KoboDeviceExtraDescriptor KoboSnowEx
{
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .frontlightDevWhite {"/sys/class/backlight/lm3630a_ledb"},
        .frontlightDevRed {"/sys/class/backlight/lm3630a_led"},
        .frontlightDevGreen {"/sys/class/backlight/lm3630a_leda"},
    },
};

// Kobo Aura H2O2, Rev2:
//- @fixme Check if the Clara fix actually helps here... (#4015)
KoboDeviceExtraDescriptor KoboSnowRev2Ex
{
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .frontlightDevWhite {"/sys/class/backlight/lm3630a_ledb"},
        .frontlightDevRed {"/sys/class/backlight/lm3630a_leda"},
    },
};

// Kobo Aura One:
KoboDeviceExtraDescriptor KoboDaylightEx
{
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .frontlightDevWhite {"/sys/class/backlight/lm3630a_led1b"},
        .frontlightDevRed {"/sys/class/backlight/lm3630a_led1a"},
        .frontlightDevGreen {"/sys/class/backlight/lm3630a_ledb"}
    },
};

// Kobo Aura second edition:
KoboDeviceExtraDescriptor KoboStarEx {};

// Kobo Aura second edition, Rev 2:
KoboDeviceExtraDescriptor KoboStarRev2Ex {};

// Kobo Clara HD:
KoboDeviceExtraDescriptor KoboNovaEx
{
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevMixer {"/sys/class/backlight/lm3630a_led/color"},
    },
};

// Kobo Forma:
// NOTE: Right now, we enforce Portrait orientation on startup to avoid getting touch coordinates wrong,
//       no matter the rotation we were started from (c.f., platform/kobo/koreader.sh).
// NOTE: For the FL, assume brightness is WO, and actual_brightness is RO!
//       i.e., we could have a real KoboPowerD:frontlightIntensityHW() by reading actual_brightness ;).
// NOTE: Rotation events *may* not be enabled if Nickel has never been brought up in that power cycle.
//       i.e., this will affect KSM users.
//       c.f., https://github.com/koreader/koreader/pull/4414#issuecomment-449652335
//       There's also a CM_ROTARY_ENABLE command, but which seems to do as much nothing as the STATUS one...
KoboDeviceExtraDescriptor KoboFrostEx
{
    .hasKeys = true,
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        // Warmth goes from 0 to 10 on the .device's side (our own internal scale is still normalized
        // to [0...100]) NOTE: Those three extra keys are *MANDATORY* if .frontlightDevMixer is set!
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevMixer {"/sys/class/backlight/tlc5947_bl/color"},
    },
};

// Kobo Libra:
// NOTE: Assume the same quirks as the Forma apply.
KoboDeviceExtraDescriptor KoboStormEx
{
    .hasKeys = true,
    .canToggleChargingLED = true,
    .frontlightSettings =
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevMixer {"/sys/class/backlight/lm3630a_led/color"},
    },
};

// Kobo Nia:
KoboDeviceExtraDescriptor KoboLunaEx
{
    .canToggleChargingLED = true,
};

// Kobo Elipsa
KoboDeviceExtraDescriptor KoboEuropaEx
{
    .canToggleChargingLED = true,
    .batterySysfs {"/sys/class/power_supply/battery"},
};

// Kobo Sage
KoboDeviceExtraDescriptor KoboCadmusEx
{
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = false,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevMixer {"/sys/class/leds/aw99703-bl_FL1/color"},
    },
    .batterySysfs {"/sys/class/power_supply/battery"},
};

// Kobo Libra 2
KoboDeviceExtraDescriptor KoboIoEx
{
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevMixer {"/sys/class/leds/aw99703-bl_FL1/color"},
    },
    .batterySysfs = "/sys/class/power_supply/battery",
};

// Kobo Clara 2E
KoboDeviceExtraDescriptor KoboGoldfinchEx
{
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevMixer {"/sys/class/leds/aw99703-bl_FL1/color"},
    },
    .batterySysfs {"/sys/class/power_supply/battery"},
    .powerDev {"/dev/input/by-path/platform-bd71828-pwrkey-event"},
};

// Kobo Libra Colour
KoboDeviceExtraDescriptor KoboMonzaEx
{
    .hasKeys = true,
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
    },
    .batterySysfs {"/sys/class/power_supply/bd71827_bat"},
    .powerDev {"/dev/input/by-path/platform-bd71828-pwrkey.6.auto-event"},
};

// Kobo Kobo Clara BW
KoboDeviceExtraDescriptor KoboSpaBWEx
{
    .hasKeys = true,
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
    },
    .batterySysfs {"/sys/class/power_supply/bd71827_bat"},
.powerDev {"/dev/input/by-path/platform-bd71828-pwrkey.6.auto-event"}, // to be confirmed
};

// Kobo Clara Colour
KoboDeviceExtraDescriptor KoboSpaColourEx
{
    .hasKeys = true,
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
    },
    .batterySysfs {"/sys/class/power_supply/bd71827_bat"},
.powerDev {"/dev/input/by-path/platform-bd71828-pwrkey.6.auto-event"}, // to be confirmed
};


KoboDeviceExtraDescriptor KoboUnknownEx {};

namespace
{
    std::map<KoboDevice, KoboDeviceExtraDescriptor> extraMap
    {
        {KoboDevice::KoboTouchAB, KoboTrilogyABEx},
        {KoboDevice::KoboTouchC, KoboTrilogyCEx},
        {KoboDevice::KoboMini, KoboPixieEx},
        {KoboDevice::KoboGlo, KoboKrakenEx},
        {KoboDevice::KoboGloHD, KoboAlyssumEx},
        {KoboDevice::KoboTouch2, KoboPikaEx},
        {KoboDevice::KoboAura, KoboPhoenixEx},
        {KoboDevice::KoboAuraHD, KoboDragonEx},
        {KoboDevice::KoboAuraH2O, KoboDahliaEx},
        {KoboDevice::KoboAuraH2O2_v1, KoboSnowEx},
        {KoboDevice::KoboAuraH2O2_v2, KoboSnowRev2Ex},
        {KoboDevice::KoboAuraOne, KoboDaylightEx},
        {KoboDevice::KoboAura2, KoboStarEx},
        {KoboDevice::KoboAura2_v2, KoboStarRev2Ex},
        {KoboDevice::KoboClaraHD, KoboNovaEx},
        {KoboDevice::KoboForma, KoboFrostEx},
        {KoboDevice::KoboLibraH2O, KoboStormEx},
        {KoboDevice::KoboNia, KoboLunaEx},
        {KoboDevice::KoboElipsa, KoboEuropaEx},
        {KoboDevice::KoboSage, KoboCadmusEx},
        {KoboDevice::KoboLibra2, KoboIoEx},
        {KoboDevice::KoboClara2E, KoboGoldfinchEx},
        {KoboDevice::KoboLibraColour, KoboMonzaEx},
        {KoboDevice::KoboClaraBW, KoboSpaBWEx},
        {KoboDevice::KoboClaraColour, KoboSpaColourEx},
        {KoboDevice::Unknown, KoboUnknownEx}
    };
}

KoboDeviceExtraDescriptor const* DetermineExtraDescriptor(KoboDeviceDescriptor const& koboDesc)
{
    if (extraMap.find(koboDesc.device) != extraMap.end())
        return &extraMap[koboDesc.device];
    else
        return &KoboUnknownEx;
}
