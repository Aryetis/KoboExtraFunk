#include "KoboWifiManager.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QThread>

KoboWifiManager::KoboWifiManager() : process(nullptr) {}

KoboWifiManager::~KoboWifiManager()
{
    if (process)
    {
        process->disconnect();
        StopProcess();
    }
}

void KoboWifiManager::ExecuteShell(const char* command)
{
    if (!process)
    {
        process.reset(new QProcess());
        QObject::connect(process.data(), &QProcess::readyReadStandardOutput,
                         [&]() { qDebug() << process->readAllStandardOutput(); });

        QObject::connect(process.data(), &QProcess::readyReadStandardError,
                         [&]() { qDebug() << process->readAllStandardError(); });
    }

    StopProcess();
    process->start("/bin/sh", {}, QProcess::ReadWrite);
    process->waitForStarted();

    process->write(command);
    process->write("\nexit\n");
    process->waitForFinished();
}

void KoboWifiManager::StopProcess()
{
    if (process)
        if (process->state() != QProcess::NotRunning)
            process->close();
}

bool KoboWifiManager::TestInternetConnection(int timeout)
{
    QString cmd = QString("ping -c 1 -q -W %1 1.1.1.1 2>&1 >/dev/null").arg(timeout);
    int res = system(cmd.toLocal8Bit());
    return res == 0;
}

void KoboWifiManager::RunScript(QString scriptPath)
{
    QFile scriptFile(scriptPath);
    scriptFile.open(QIODevice::ReadOnly);
    QByteArray script = scriptFile.readAll();
    scriptFile.close();

    ExecuteShell(script.data());
}

void KoboWifiManager::EnableWiFiConnection(bool obtainIp /*= true*/)
{
    RunScript(":/scripts/enable-wifi.sh");
    if (obtainIp)
        RunScript(":/scripts/obtain-ip.sh");
}

void KoboWifiManager::DisableWiFiConnection()
{
    RunScript(":/scripts/disable-wifi.sh");
}
