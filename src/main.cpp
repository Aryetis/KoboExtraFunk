#include "KoboPlatformExtraIntegration.h"

class KoboPlatformIntegrationPlugin
{
public:
    QPlatformIntegration* create(const QString&, const QStringList&) override;
};

QPlatformIntegration* KoboPlatformIntegrationPlugin::create(const QString& system,
                                                            const QStringList& paramList)
{
    KoboPlatformIntegration
    if (!system.compare("kobo", Qt::CaseInsensitive))
        return new KoboPlatformIntegration(paramList);

    return 0;
}

#include "main.moc"
