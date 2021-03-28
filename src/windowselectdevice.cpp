#include "windowselectdevice.h"

WindowSelectDevice::WindowSelectDevice(QWidget *parent) : QWidget(parent)
{
    qDebug() << "WindowSelectDevice::WindowSelectDevice(): Initializing object instance";

    // containing window properties
    qDebug() << "WindowSelectDevice::WindowSelectDevice(): Setting up window and widgets";
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
    qDebug() << "WindowSelectDevice::handleSelectedDevice(): Querying QListView for selection";
    QModelIndex index = browser->currentIndex();
    this->setSerialDevice(index.data(Qt::DisplayRole).toString());
    qDebug() << "WindowSelectDevice::handleSelectedDevice(): Selected device = " << this->getSerialDevice();
    qDebug() << "WindowSelectDevice::handleSelectedDevice(): Hiding this window";
    this->hide();
    qDebug() << "WindowSelectDevice::handleSelectedDevice(): Sending serial device selection";
    emit sigSerialDeviceChosen(this->getSerialDevice());
    qDebug() << "WindowSelectDevice::handleSelectedDevice(): Sending request to show main window";
    emit sigShowWindowMain();
}
