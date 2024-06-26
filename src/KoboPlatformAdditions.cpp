#include "KoboPlatformAdditions.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <cmath>

QString str_from_file(const QString& fileName)
{
    QString result;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        result = in.readLine().trimmed();
    }
    return result;
}

int int_from_file(const QString& fileName)
{
    return str_from_file(fileName).toInt();
}

static void write_light_value(const QString& file, int value)
{
    QFile f(file);
    if (f.open(QIODevice::WriteOnly))
    {
        f.write(QString::number(value).toLatin1());
        f.flush();
        f.close();
    }
}

static void set_light_value(const QString& dir, int value)
{
    write_light_value(dir + "/bl_power", value > 0 ? 31 : 0);
    write_light_value(dir + "/brightness", value);
}

static bool ntx_io_write(int port, int value)
{
    int fd = -1, retval = -1;
    if ((fd = ::open("/dev/ntx_io", O_RDWR)) != -1)
    {
        retval = ioctl(fd, port, value);
        close(fd);
    }
    return retval != -1;
}

static int ntx_io_read(int port)
{
    int fd = -1, retval = -1, output;
    if ((fd = ::open("/dev/ntx_io", O_RDWR)) != -1)
    {
        retval = ioctl(fd, port, &output);
        close(fd);
    }

    if (retval == -1)
        return -1;

    return output;
}

KoboPlatformAdditions::KoboPlatformAdditions(QObject* parent, const KoboDeviceExtraDescriptor& descriptor_)
    : QObject(parent), extraDescriptor(descriptor_)
{ }

int KoboPlatformAdditions::GetBatteryLevel() const
{
    return int_from_file(extraDescriptor.batterySysfs + "/capacity");
}

bool KoboPlatformAdditions::IsBatteryCharging() const
{
    return str_from_file(extraDescriptor.batterySysfs + "/status") == "Charging";
}

bool KoboPlatformAdditions::IsUsbConnected() const
{
    return ntx_io_read(108) == 1;
}

void KoboPlatformAdditions::SetStatusLedEnabled(bool enabled)
{
    ntx_io_write(101, enabled ? 1 : 0);
}

void KoboPlatformAdditions::SetFrontlightLevel(int val, int temp)
{
    if (!extraDescriptor.frontlightSettings.hasFrontLight)
        return;

    val = qMax(extraDescriptor.frontlightSettings.frontlightMin, qMin(val, extraDescriptor.frontlightSettings.frontlightMax));
    temp = qMax(extraDescriptor.frontlightSettings.naturalLightMin,
                qMin(temp, extraDescriptor.frontlightSettings.naturalLightMax));
    if (extraDescriptor.frontlightSettings.hasNaturalLight)
        SetNaturalBrightness(val, temp);
    else
        ntx_io_write(241, val);
}

void KoboPlatformAdditions::SetNaturalBrightness(int brig, int temp)
{
    QString fWhite = extraDescriptor.frontlightSettings.frontlightDevWhite,
        fRed = extraDescriptor.frontlightSettings.frontlightDevRed,
        fGreen = extraDescriptor.frontlightSettings.frontlightDevGreen,
        fMixer = extraDescriptor.frontlightSettings.frontlightDevMixer;

    if (extraDescriptor.frontlightSettings.naturalLightInverted)
    {
        temp = extraDescriptor.frontlightSettings.naturalLightMax - temp;
    }

    if (extraDescriptor.frontlightSettings.hasNaturalLightMixer)
    {
        if (fWhite != "")
            write_light_value(fWhite, brig);
        if (fMixer != "")
            write_light_value(fMixer, temp);
    }
    else
    {
        static const int white_gain = 25;
        static const int red_gain = 24;
        static const int green_gain = 24;
        static const int white_offset = -25;
        static const int red_offset = 0;
        static const int green_offset = -65;
        static const double exponent = 0.25;
        double red = 0.0, green = 0.0, white = 0.0;
        if (brig > 0)
        {
            white = std::min(white_gain * pow(brig, exponent) *
                                     pow(extraDescriptor.frontlightSettings.naturalLightMax - temp, exponent) +
                                 white_offset,
                             255.0);
        }
        if (temp > 0)
        {
            red = std::min(red_gain * pow(brig, exponent) * pow(temp, exponent) + red_offset, 255.0);
            green = std::min(green_gain * pow(brig, exponent) * pow(temp, exponent) + green_offset, 255.0);
        }
        white = std::max(white, 0.0);
        red = std::max(red, 0.0);
        green = std::max(green, 0.0);

        if (fWhite != "")
            set_light_value(fWhite, floor(white));
        if (fRed != "")
            set_light_value(fRed, floor(red));
        if (fGreen != "")
            set_light_value(fGreen, floor(green));
    }
}
