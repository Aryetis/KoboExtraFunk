#include "KoboDeviceExtraDescriptor.h"

// Kobo Touch A/B:
KoboDeviceExtraDescriptor KoboTrilogyAB
{
    .hasKeys = true,
    .frontlightSettings {.hasFrontLight = false},
};

// Kobo Touch C:
KoboDeviceExtraDescriptor KoboTrilogyC
{
    .hasKeys = true,
    .frontlightSettings {.hasFrontLight = false},
};

// Kobo Mini:
KoboDeviceExtraDescriptor KoboPixie
{
    .frontlightSettings {.hasFrontLight = false},
};

// Kobo Glo:
KoboDeviceExtraDescriptor KoboKraken {};

// Kobo Glo HD:
KoboDeviceExtraDescriptor KoboAlyssum {};

// Kobo Touch 2.0:
KoboDeviceExtraDescriptor KoboPika
{
    .frontlightSettings {.hasFrontLight = false},
};

// Kobo Aura:
KoboDeviceExtraDescriptor KoboPhoenix {};

// Kobo Aura HD:
KoboDeviceExtraDescriptor KoboDragon {};

// Kobo Aura H2O:
KoboDeviceExtraDescriptor KoboDahlia {};

// Kobo Aura H2O2:
KoboDeviceExtraDescriptor KoboSnow
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
KoboDeviceExtraDescriptor KoboSnowRev2
{
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .frontlightDevWhite {"/sys/class/backlight/lm3630a_ledb"},
        .frontlightDevRed {"/sys/class/backlight/lm3630a_leda"},
    },
};

// Kobo Aura One:
KoboDeviceExtraDescriptor KoboDaylight
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
KoboDeviceExtraDescriptor KoboStar {};

// Kobo Aura second edition, Rev 2:
KoboDeviceExtraDescriptor KoboStarRev2 {};

// Kobo Clara HD:
KoboDeviceExtraDescriptor KoboNova
{
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevWhite {"/sys/class/backlight/mxc_msp430.0/brightness"},
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
KoboDeviceExtraDescriptor KoboFrost
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
        .frontlightDevWhite {"/sys/class/backlight/mxc_msp430.0/brightness"},
        .frontlightDevMixer {"/sys/class/backlight/tlc5947_bl/color"},
    },
};

// Kobo Libra:
// NOTE: Assume the same quirks as the Forma apply.
KoboDeviceExtraDescriptor KoboStorm
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
        .frontlightDevWhite {"/sys/class/backlight/mxc_msp430.0/brightness"},
        .frontlightDevMixer {"/sys/class/backlight/lm3630a_led/color"},
    },
};

// Kobo Nia:
KoboDeviceExtraDescriptor KoboLuna
{
    .canToggleChargingLED = true,
};

// Kobo Elipsa
KoboDeviceExtraDescriptor KoboEuropa
{
    .canToggleChargingLED = true,
    .batterySysfs {"/sys/class/power_supply/battery"},
};

// Kobo Sage
KoboDeviceExtraDescriptor KoboCadmus
{
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = false,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevWhite {"/sys/class/backlight/mxc_msp430.0/brightness"},
        .frontlightDevMixer {"/sys/class/leds/aw99703-bl_FL1/color"},
    },
    .batterySysfs {"/sys/class/power_supply/battery"},
};

// Kobo Libra 2
KoboDeviceExtraDescriptor KoboIo
{
    .canToggleChargingLED = true,
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevWhite {"/sys/class/backlight/mxc_msp430.0/brightness"},
        .frontlightDevMixer {"/sys/class/leds/aw99703-bl_FL1/color"},
    },
    .batterySysfs = "/sys/class/power_supply/battery",
};

// Kobo Clara 2E
KoboDeviceExtraDescriptor KoboGoldfinch
{
    .frontlightSettings
    {
        .hasNaturalLight = true,
        .hasNaturalLightMixer = true,
        .naturalLightInverted = true,
        .naturalLightMin = 0,
        .naturalLightMax = 10,
        .frontlightDevWhite {"/sys/class/backlight/mxc_msp430.0/brightness"},
        .frontlightDevMixer {"/sys/class/leds/aw99703-bl_FL1/color"},
    },
    .batterySysfs {"/sys/class/power_supply/battery"},
    .powerDev {"/dev/input/by-path/platform-bd71828-pwrkey-event"},
};
