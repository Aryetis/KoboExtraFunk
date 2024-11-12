#include "KoboWifiManager.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QThread>

KoboWifiManager::KoboWifiManager() :
    process(nullptr), state(WifiManagerState::Available), scriptsQueue()
{}

KoboWifiManager::~KoboWifiManager()
{
    if (process)
    {
        process->disconnect();
        StopProcess();
    }
}

bool KoboWifiManager::TestInternetConnection(int timeout)
{
    QString cmd = QString("ping -c 1 -q -W %1 1.1.1.1 2>&1 >/dev/null").arg(timeout);
    int res = system(cmd.toLocal8Bit());
    return res == 0;
}

void KoboWifiManager::EnableWiFiConnection(bool obtainIp /*= true*/)
{
    if (IsBusy())
    {
        qDebug() << "EnableWiFiConnection()::Request Terminated, WifiManager is busy" ;
        emit WifiManagerStateBusy(state);
        return;
    }

    state = WifiManagerState::Enabling;
    if (obtainIp)
        RunScript({":/scripts/enable-wifi.sh", ":/scripts/obtain-ip.sh"});
    else
        RunScript({":/scripts/enable-wifi.sh"});
}

void KoboWifiManager::DisableWiFiConnection()
{
    if (IsBusy())
    {
        qDebug() << "DisableWiFiConnection()::Request Terminated, WifiManager is busy" ;
        emit WifiManagerStateBusy(state);
        return;
    }

    state = WifiManagerState::Disabling;
    RunScript({":/scripts/disable-wifi.sh"});
}

void KoboWifiManager::StopProcess()
{
    if (process && process->state() != QProcess::NotRunning)
        process->terminate();
}

bool KoboWifiManager::IsBusy()
{
    return state != WifiManagerState::Available;
}

void KoboWifiManager::RunScript(std::initializer_list<QString> scriptPaths)
{
    scriptsQueue.append(scriptPaths);

    if (!scriptsQueue.empty())
    {
        QFile scriptFile(scriptsQueue.first());
        scriptFile.open(QIODevice::ReadOnly);
        QByteArray script = scriptFile.readAll();
        scriptFile.close();
        scriptsQueue.pop_front();

        ExecuteShell(script.data());
    }
    else
    {
        qDebug() << "RequestTerminated";
        emit RequestTerminated(state);
        state = WifiManagerState::Available;
    }
}

void KoboWifiManager::ExecuteShell(const char* scriptContent)
{
    if (!process)
    {
        process.reset(new QProcess());
        QObject::connect(process.data(), &QProcess::readyReadStandardOutput,
                         [&]() { qDebug() << process->readAllStandardOutput(); });
        QObject::connect(process.data(), &QProcess::readyReadStandardError,
                         [&]() { qDebug() << process->readAllStandardError(); });
        QObject::connect(process.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                         [&]() { RunScript({}); });
    }
    StopProcess();
    process->start("/bin/sh", QStringList() << "-c" << scriptContent << "\nexit\n");
}
