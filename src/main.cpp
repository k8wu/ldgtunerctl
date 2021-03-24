#include <QApplication>
#include <iostream>

#include "appconfig.h"
#include "windowselectdevice.h"
#include "windowmain.h"

int main(int argc, char **argv) {
    // initialize app
    DEBUG && std::cout << "main(): Initializing app" << std::endl;
    QApplication app(argc, argv);

    // initialize all windows, but don't show anything yet
    DEBUG && std::cout << "main(): Initializing WindowSelectDevice" << std::endl;
    WindowSelectDevice* windowSelectDevice = new WindowSelectDevice();
    WindowMain* windowMain = new WindowMain();

    // now we can connect things between the widgets together
    QObject::connect(windowSelectDevice, SIGNAL(sigSerialDeviceChosen(QString)), windowMain, SLOT(slotGetSerialDevice(QString)));
    QObject::connect(windowSelectDevice, SIGNAL(sigShowWindowMain()), windowMain, SLOT(slotShowWindowMain()));

    // start by showing the device selection window
    DEBUG && std::cout << "main(): Showing WindowSelectDevice" << std::endl;
    windowSelectDevice->show();

    // loop and/or exit
    DEBUG && std::cout << "main(): Looping" << std::endl;
    return app.exec();
}
