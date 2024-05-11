#ifndef KOBOBUTTONINTEGRATION_H
#define KOBOBUTTONINTEGRATION_H

#include <fcntl.h>
#include <linux/input.h>
#include <qevent.h>
#include <qguiapplication.h>
#include <unistd.h>

#include <QDebug>
#include <QSocketNotifier>

class KoboButtonIntegration : public QObject
{
    Q_OBJECT

public:
    KoboButtonIntegration(QObject* parent = nullptr, const QString& inputDevice = "/dev/input/event0", bool debug = false);
    ~KoboButtonIntegration();

private slots:
    void activity(int);

private:
    void captureInput();
    void releaseInput();

    int inputHandle;
    QSocketNotifier* socketNotifier;
    bool debug;
    bool isInputCaptured;
};

#endif // KOBOBUTTONINTEGRATION_H
