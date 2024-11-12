#ifndef KOBOWIFIMANAGER_H
#define KOBOWIFIMANAGER_H

#include <QProcess>
#include <QSharedPointer>
#include <QTimer>

class KoboWifiManager : public QObject
{
    Q_OBJECT

public:
    enum WifiManagerState
    {
        Enabling,
        Disabling,
        Available
    };

signals:
    void RequestTerminated(WifiManagerState state);
    void WifiManagerStateBusy(WifiManagerState state);

public:
    KoboWifiManager();
    ~KoboWifiManager();

    bool TestInternetConnection(int timeout);
    void EnableWiFiConnection(bool obtainIp = true);
    void DisableWiFiConnection();
    void StopProcess();
    bool IsBusy();

private:
    void RunScript(std::initializer_list<QString> scriptPaths);
    void ExecuteShell(const char* scriptContent);

    QSharedPointer<QProcess> process;
    WifiManagerState state;
    QVector<QString> scriptsQueue;
};

#endif  // KOBOWIFIMANAGER_H
