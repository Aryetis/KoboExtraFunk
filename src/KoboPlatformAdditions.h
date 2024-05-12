#ifndef KOBOPLATFORMADDITIONS_H
#define KOBOPLATFORMADDITIONS_H

#include "KoboDeviceExtraDescriptor.h"
#include <QObject>

class KoboPlatformAdditions : public QObject
{
    Q_OBJECT

public:
    KoboPlatformAdditions(QObject *parent, const KoboDeviceExtraDescriptor &descriptor);

    int GetBatteryLevel() const;
    bool IsBatteryCharging() const;
    bool IsUsbConnected() const;
    void SetStatusLedEnabled(bool enabled);
    void SetFrontlightLevel(int val, int temp);

private:
    void SetNaturalBrightness(int brig, int temp);

    KoboDeviceExtraDescriptor extraDescriptor;
};

#endif  // KOBOPLATFORMADDITIONS_H
