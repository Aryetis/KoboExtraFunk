#ifndef KOBOWIFIMANAGER_H
#define KOBOWIFIMANAGER_H

#include <QProcess>
#include <QSharedPointer>

class KoboWifiManager
{
public:
    KoboWifiManager();
    ~KoboWifiManager();

    bool testInternetConnection(int timeout);

    void enableWiFiConnection();
    void disableWiFiConnection();

    void stopProcess();

private:
    void executeShell(const char* command);

    QSharedPointer<QProcess> process;
};

#endif  // KOBOWIFIMANAGER_H
