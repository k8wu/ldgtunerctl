#include "windowselectdevice.h"

WindowSelectDevice::WindowSelectDevice(QWidget *parent) : QWidget(parent)
{
    DEBUG && std::cout << "WindowSelectDevice::WindowSelectDevice(): Initializing object instance" << std::endl;

    // containing window properties
    DEBUG && std::cout << "WindowSelectDevice::WindowSelectDevice(): Setting up window and widgets" << std::endl;
    setFixedSize(240, 320);
    setWindowTitle("Select Serial Port");

    // set up the other widgets
    browser = new QListView(this);
    browser->setGeometry(10, 10, 140, 150);
    chooseButton = new QPushButton("Choose Port", this);
    chooseButton->setGeometry(20, 170, 120, 40);
    exitButton = new QPushButton("Quit", this);
    exitButton->setGeometry(20, 220, 120, 40);


    // populate the listview with serial devices
    QStringList serialDevices = CommLink::enumerateDevices();
    QStringListModel* browserModel = new QStringListModel(this);
    browserModel->setStringList(serialDevices);
    browser->setModel(browserModel);

    // define signal/slot connections
    connect(chooseButton, SIGNAL(clicked()), this, SLOT(handleSelectedDevice()));
    connect(exitButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
}

void WindowSelectDevice::setSerialDevice(QString serialDevice) {
    this->serialDevice = serialDevice;
}

QString WindowSelectDevice::getSerialDevice() {
    return this->serialDevice;
}

void WindowSelectDevice::handleSelectedDevice() {
    DEBUG && std::cout << "WindowSelectDevice::handleSelectedDevice(): Querying QListView for selection" << std::endl;
    QModelIndex index = browser->currentIndex();
    this->setSerialDevice(index.data(Qt::DisplayRole).toString());
    DEBUG && std::cout << "WindowSelectDevice::handleSelectedDevice(): Selected device = " << this->getSerialDevice().toStdString() << std::endl;
    DEBUG && std::cout << "WindowSelectDevice::handleSelectedDevice(): Hiding this window" << std::endl;
    this->hide();
    DEBUG && std::cout << "WindowSelectDevice::handleSelectedDevice(): Sending serial device selection" << std::endl;
    emit sigSerialDeviceChosen(this->getSerialDevice());
    DEBUG && std::cout << "WindowSelectDevice::handleSelectedDevice(): Sending request to show main window" << std::endl;
    emit sigShowWindowMain();
}
