#include <QApplication>
#include <QDebug>
#include <iostream>

#include "appconfig.h"
#include "windowselectdevice.h"
#include "windowmain.h"

int main(int argc, char **argv) {
    // initialize app
    qDebug() << "main(): Initializing app";
    QApplication app(argc, argv);

    // initialize all windows, but don't show anything yet
    qDebug() << "main(): Initializing WindowSelectDevice";
    WindowSelectDevice* windowSelectDevice = new WindowSelectDevice();
    WindowMain* windowMain = new WindowMain();

    // now we can connect things between the widgets together
    QObject::connect(windowSelectDevice, SIGNAL(sigSerialDeviceChosen(QString)), windowMain, SLOT(slotGetSerialDevice(QString)));
    QObject::connect(windowSelectDevice, SIGNAL(sigShowWindowMain()), windowMain, SLOT(slotShowWindowMain()));

    // start by showing the device selection window
    qDebug() << "main(): Showing WindowSelectDevice";
    windowSelectDevice->show();

    // loop and/or exit
    qDebug() << "main(): Looping";
    return app.exec();
}
