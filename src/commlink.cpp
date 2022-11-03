#include "commlink.h"

CommLink::CommLink(QString serialDevice)
{
    qDebug() << "CommLink::CommLink(): Initializing object instance";
    this->serialDevice = serialDevice;
}

bool CommLink::setup() {
    // internal method variables
    QString readBuffer;

    // define the port object
    this->serialPort.setPortName(serialDevice);
    if(this->serialPort.error() != QSerialPort::NoError) {
        qDebug() << "CommLink::setup(): Failed to get" << this->serialDevice << "referenced by name";
        return false;
    }

    // open the port
    this->serialPort.open(QIODevice::ReadWrite);
    if(this->serialPort.error() != QSerialPort::NoError) {
        qDebug() << "CommLink::setup(): Failed to open:" << this->serialDevice << "(" << this->serialPort.error() << ")";
        this->serialPort.close();
        return false;
    }

    // flush buffers and do a throwaway read just to be sure
    serialPort.flush();
    if(this->serialPort.error() != QSerialPort::NoError) {
        qDebug() << "CommLink::setup(): Failed to flush buffers";
        this->serialPort.close();
        return false;
    }
    readBuffer = this->serialPort.read(40);
    readBuffer.clear();
    qDebug() << "CommLink::setup(): Flushed buffers," << this->serialPort.bytesAvailable() << "input bytes," << this->serialPort.bytesToWrite() << "output bytes remaining";

    // set up the port
    this->serialPort.setBaudRate(QSerialPort::Baud38400);
    if(this->serialPort.error() != QSerialPort::NoError) {
        qDebug() << "CommLink::setup(): Could not set baud rate to 38400 (" << this->serialPort.error() << ")";
    }
    this->serialPort.setDataBits(QSerialPort::Data8);
    if(this->serialPort.error() != QSerialPort::NoError) {
        qDebug() << "CommLink::setup(): Could not set data bits to 8 (" << this->serialPort.error() << ")";
    }
    this->serialPort.setParity(QSerialPort::NoParity);
    if(this->serialPort.error() != QSerialPort::NoError) {
        qDebug() << "CommLink::setup(): Could not set parity to NONE (" << this->serialPort.error() << ")";
    }
    this->serialPort.setStopBits(QSerialPort::OneStop);
    if(this->serialPort.error() != QSerialPort::NoError) {
        qDebug() << "CommLink::setup(): Could not set stop bits to 1 (" << this->serialPort.error() << ")";
    }
    this->serialPort.setFlowControl(QSerialPort::NoFlowControl);
    if(this->serialPort.error() != QSerialPort::NoError) {
        qDebug() << "CommLink::setup(): Could not set flow control to NONE (" << this->serialPort.error() << ")";
    }

    qDebug() << "CommLink::setup(): Serial port setup complete";
    return true;
}

// rxTimeout is in ms
QString CommLink::trx(QString command, unsigned int rxTimeout) {
    QString readBuffer;
    qint64 res;
    int rxBytesWaiting;

    // do a dummy read, then clear buffers
    readBuffer = this->serialPort.read(40);
    readBuffer.clear();
    this->buffer.clear();
    qDebug() << "CommLink::trx(): Cleared buffers," << this->serialPort.bytesAvailable() << "output," << this->serialPort.bytesToWrite() << "input remaining";

    qDebug() << "CommLink::trx(): Writing command" << command;
    for(QString c : command) {
        qDebug() << "CommLink::trx(): Writing character and waiting up to 100 ms for it to go through";
        this->serialPort.write(c.toStdString().c_str());
        this->serialPort.waitForBytesWritten(100);
        if(this->serialPort.error() != QSerialPort::NoError) {
            qDebug() << "CommLink::trx(): Failed to write character";
            return "";
        }
        qDebug() << "CommLink::trx(): Wrote character:" << c;
    }
    qDebug() << "CommLink::trx(): Wrote command - now waiting up to" << rxTimeout << "ms for receive data";

    this->serialPort.waitForReadyRead(rxTimeout);
    if(this->serialPort.bytesAvailable() == 0) {
        qDebug() << "CommLink::trx(): No RX bytes waiting";
    }
    else {
        qDebug() << "CommLink::trx():" << this->serialPort.bytesAvailable() << "receive bytes waiting";
        do {
            readBuffer = this->serialPort.read(40);
            if(this->serialPort.error() != QSerialPort::NoError) {
                qDebug() << "CommLink::trx(): Failed to read";
                return "";
            }
            else if(readBuffer.length() == 0) {
                qDebug() << "CommLink::trx(): No more bytes to read";
            }
            qDebug() << "CommLink::trx(): Read" << readBuffer.length() << "byte(s):" << readBuffer;
            this->buffer.append(readBuffer);
            readBuffer.clear();
        } while(this->serialPort.bytesAvailable() > 0);
    }
    qDebug() << "CommLink::trx(): Total read buffer:" << this->buffer;
    return this->buffer;
}

bool CommLink::tunerSync() {
    // set up the port
    qDebug() << "CommLink::tunerSync(): Calling setup routines";
    if(!setup()) {
        qDebug() << "CommLink::tunerSync(): Setup failed";
        return false;
    }

    // loop a few times until we get synced, or we run out of attempts
    for(int syncAttempt = 1; syncAttempt <= 5; syncAttempt++) {
        buffer = trx(" Z");
        qDebug() << "CommLink::tunerSync(): Total read buffer:" << buffer;
        if(buffer == syncResponseString) {
            qDebug() << "CommLink::tunerSync(): Tuner is synced";
            return true;
        }
        else {
            qDebug() << "CommLink::tunerSync(): Tuner is not synced - trying again if there are any attempts left";
        }
    }

    // sorry, no sync
    qDebug() << "CommLink::tunerSync(): Failed to get synced after several attempts";
    return false;
}

// we will override the timeout for these next two methods, because tuning can take several seconds
QString CommLink::doMemoryTune() {
    qDebug() << "CommLink::doMemoryTune(): Doing memory tune";
    buffer = trx(" T", 15000);
    qDebug() << "CommLink::doMemoryTune(): Response:" << buffer;
    return buffer;
}

QString CommLink::doFullTune() {
    qDebug() << "CommLink::doFullTune(): Doing full tune";
    buffer = trx(" F", 15000);
    qDebug() << "CommLink::doFullTune(): Response:" << buffer;
    return buffer;
}

QString CommLink::setAuto() {
    qDebug() << "CommLink::setAuto(): Setting automatic tuning mode";
    buffer = trx(" C");
    qDebug() << "CommLink::setAuto(): Response:" << buffer;
    return buffer;
}

QString CommLink::setManual() {
    qDebug() << "CommLink::setManual(): Setting manual tuning mode";
    buffer = trx(" M");
    qDebug() << "CommLink::setManual(): Response:" << buffer;
    return buffer;
}

QString CommLink::toggleBypass() {
    qDebug() << "CommLink::toggleBypass(): Toggling bypass mode";
    buffer = trx(" P");
    qDebug() << "CommLink::toggleBypass(): Response:" << buffer;
    return buffer;
}

QString CommLink::toggleAntenna() {
//    qDebug() << "CommLink::toggleAntenna(): Function called, syncing tuner";
//    if(!tunerSync()) {
//        qDebug() << "CommLink::toggleAntenna(): Failed to sync tuner";
//        return "";
//    }

    qDebug() << "CommLink::toggleAntenna(): Toggling antenna";
    buffer = trx(" A");
    qDebug() << "CommLink::toggleAntenna(): Response:" << buffer;
    return buffer;
}

void CommLink::close() {
    this->serialPort.close();
}

QStringList CommLink::enumerateDevices() {
    // quick and easy enumeration
    QStringList serialPorts;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "CommLink::enumerateDevices(): Added serial port: " << info.portName();
        serialPorts.append(info.portName());
    }

    return serialPorts;
}
