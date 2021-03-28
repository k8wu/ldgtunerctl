#include <QApplication>
#include <QDebug>
#include <iostream>

#include "appconfig.h"
#include "windowselectdevice.h"
#include "windowmain.h"
#include "windowabout.h"

int main(int argc, char **argv) {
    // initialize app
    qDebug() << "main(): Initializing app";
    QApplication app(argc, argv);

    // initialize all windows, but don't show anything yet
    qDebug() << "main(): Initializing WindowSelectDevice";
    WindowSelectDevice* windowSelectDevice = new WindowSelectDevice();
    qDebug() << "main(): Initializing WindowMain";
    WindowMain* windowMain = new WindowMain();
    qDebug() << "main(): Initializing WindowAbout";
    WindowAbout* windowAbout = new WindowAbout();

    // now we can connect things between the widgets together
    qDebug() << "main(): Initializing button actions for sub windows";
    QObject::connect(windowSelectDevice, SIGNAL(sigSerialDeviceChosen(QString)), windowMain, SLOT(slotGetSerialDevice(QString)));
    QObject::connect(windowSelectDevice, SIGNAL(sigShowWindowMain()), windowMain, SLOT(slotShowWindowMain()));
    QObject::connect(windowMain, SIGNAL(sigShowWindowAbout()), windowAbout, SLOT(show()));

    // start by showing the device selection window
    qDebug() << "main(): Showing WindowSelectDevice";
    windowSelectDevice->show();

    // loop and/or exit
    qDebug() << "main(): Looping";
    return app.exec();
}
