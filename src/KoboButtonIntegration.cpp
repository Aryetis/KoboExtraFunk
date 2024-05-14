#include "KoboButtonIntegration.h"
#include "einkenums.h"

#define EVENT_REPEAT 2
#define EVENT_PRESS 1
#define EVENT_RELEASE 0

#define OFF 0
#define ON 1

KoboButtonIntegration::KoboButtonIntegration(QObject *parent, const QString &inputDevice, bool debug)
    : QObject(parent), debug(debug), isInputCaptured(false)
{
    inputHandle = open(inputDevice.toStdString().c_str(), O_RDONLY);
    socketNotifier = new QSocketNotifier(inputHandle, QSocketNotifier::Read);
    connect(socketNotifier, &QSocketNotifier::activated, this, &KoboButtonIntegration::activity);
    socketNotifier->setEnabled(true);
    CaptureInput();
}

KoboButtonIntegration::~KoboButtonIntegration()
{
    ReleaseInput();
    delete socketNotifier;
    close(inputHandle);
}

void KoboButtonIntegration::CaptureInput()
{
    if (isInputCaptured || inputHandle == -1)
        return;

    if (debug)
        qDebug("KoboKb: Attempting to capture input...");

    int res = ioctl(inputHandle, EVIOCGRAB, ON);

    if (res == 0)
        isInputCaptured = true;
    else if (debug)
        qDebug() << "KoboKb: Capture keyboard input error:" << res;
}

void KoboButtonIntegration::ReleaseInput()
{
    if (!isInputCaptured || inputHandle == -1)
        return;

    if (debug)
        qDebug("KoboKb: attempting to release input...");

    if (ioctl(inputHandle, EVIOCGRAB, OFF) == 0)
        isInputCaptured = false;
    else if (debug)
        qDebug("KoboKb: release keyboard input: error");
}

void KoboButtonIntegration::activity(int)
{
    socketNotifier->setEnabled(false);

    input_event in;
    read(inputHandle, &in, sizeof(input_event));

    KoboKey code;

    if (KoboPhysicalKeyMap.contains(in.code))
    {
        code = KoboPhysicalKeyMap[in.code];

        QEvent::Type eventType = in.value == EVENT_PRESS ? QEvent::KeyPress : QEvent::KeyRelease;

        // EVENT_REPEAT is here to distinguish (using p_keyPressEvent->isAutoRepeat()) proper inputs and sleepcover spam
        QKeyEvent keyEvent(eventType, code, Qt::NoModifier, {}, in.value == EVENT_REPEAT);

        QObject *focusObject = qGuiApp->focusObject();
        if (focusObject)
            QGuiApplication::sendEvent(focusObject, &keyEvent);

        if (debug)
            qDebug() << "found focusobject:" << (focusObject != nullptr) << "in.type:" << in.type
                     << " | in.code: " << in.code << " | code:" << code << " | "
                     << (in.value ? "pressed" : "released");
    }

    socketNotifier->setEnabled(true);
}
