#ifndef KOBOPLATFORMADDITIONS_H
#define KOBOPLATFORMADDITIONS_H

#include "KoboDeviceExtraDescriptor.h"
#include <QObject>

class KoboPlatformAdditions : public QObject
{
    Q_OBJECT

public:
    KoboPlatformAdditions(QObject *parent, const KoboDeviceExtraDescriptor &descriptor);

    int getBatteryLevel() const;
    bool isBatteryCharging() const;
    bool isUsbConnected() const;
    void setStatusLedEnabled(bool enabled);
    void setFrontlightLevel(int val, int temp);

private:
    void setNaturalBrightness(int brig, int temp);

    KoboDeviceExtraDescriptor descriptor;
};

#endif  // KOBOPLATFORMADDITIONS_H
