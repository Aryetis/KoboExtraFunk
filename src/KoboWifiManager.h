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
    void EnableWiFiConnection(bool obtainIp = true);
    void DisableWiFiConnection();
    void StopProcess();

private:
    void RunScript(QString scriptPath);
    void ExecuteShell(const char* command);

    QSharedPointer<QProcess> process;
};

#endif  // KOBOWIFIMANAGER_H
