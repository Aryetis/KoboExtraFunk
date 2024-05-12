#ifndef KOBOWIFIMANAGER_H
#define KOBOWIFIMANAGER_H

#include <QProcess>
#include <QSharedPointer>

class KoboWifiManager : public QObject
{
    Q_OBJECT
public:
    KoboWifiManager();
    ~KoboWifiManager();

    bool TestInternetConnection(int timeout);
    void EnableWiFiConnection();
    void DisableWiFiConnection();
    void StopProcess();

private:
    void ExecuteShell(const char* command);

    QSharedPointer<QProcess> process;
};

#endif  // KOBOWIFIMANAGER_H
