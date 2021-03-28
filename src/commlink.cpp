#include "commlink.h"

CommLink::CommLink(QString serialDevice)
{
    qDebug() << "CommLink::CommLink(): Initializing object instance";
    this->serialDevice = serialDevice;
}

bool CommLink::setup() {
    // internal method variables
    int res;
    char readBuffer[40];
    rxEventSet = (struct sp_event_set*) malloc(sizeof(rxEventSet));
    txEventSet = (struct sp_event_set*) malloc(sizeof(txEventSet));

    // setup for detecting when it's fine to write and/or read
    sp_new_event_set(&rxEventSet);
    sp_new_event_set(&txEventSet);

    // define the port object
    res = sp_get_port_by_name(serialDevice.toStdString().c_str(), &serialPort);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Failed to get " << this->serialDevice << " referenced by name";
        return false;
    }

    // open the port
    res = sp_open(serialPort, SP_MODE_READ_WRITE);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Failed to open: " << sp_last_error_message() << " (" << res << ")";
        sp_free_port(serialPort);
        return false;
    }

    // flush buffers and do a throwaway read just to be sure
    res = sp_flush(serialPort, SP_BUF_BOTH);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Failed to flush buffers";
        sp_close(serialPort);
        sp_free_port(serialPort);
        return false;
    }
    sp_nonblocking_read(serialPort, readBuffer, sizeof(readBuffer));
    memset(readBuffer, 0, sizeof(readBuffer));
    qDebug() << "CommLink::setup(): Flushed buffers, " << sp_input_waiting(serialPort) << " input bytes, " << sp_output_waiting(serialPort) << " output bytes remaining";

    // set up the port
    res = sp_set_baudrate(serialPort, 38400);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Could not set baud rate to 38400 (" << sp_last_error_message() << ")";
    }
    res = sp_set_bits(serialPort, 8);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Could not set data bits to 8 (" << sp_last_error_message() << ")";
    }
    res = sp_set_parity(serialPort, SP_PARITY_NONE);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Could not set parity to NONE (" << sp_last_error_message() << ")";
    }
    res = sp_set_stopbits(serialPort, 1);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Could not set stop bits to 1 (" << sp_last_error_message() << ")";
    }
    res = sp_set_flowcontrol(serialPort, SP_FLOWCONTROL_NONE);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Could not set flow control to NONE (" << sp_last_error_message() << ")";
    }

    res = sp_add_port_events(rxEventSet, serialPort, SP_EVENT_RX_READY);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Could not set receive event mask";
        sp_close(serialPort);
        sp_free_port(serialPort);
        return false;
    }

    res = sp_add_port_events(txEventSet, serialPort, SP_EVENT_TX_READY);
    if(res < 0) {
        qDebug() << "CommLink::setup(): Could not set transmit event mask";
        sp_close(serialPort);
        sp_free_port(serialPort);
        return false;
    }

    qDebug() << "CommLink::setup(): Serial port setup complete";
    return true;
}

QString CommLink::trx(QString command) {
    char readBuffer[40];
    int res;

    // do a dummy read, then clear buffers
    sp_nonblocking_read(serialPort, readBuffer, sizeof(readBuffer));
    memset(readBuffer, 0, sizeof(readBuffer));
    buffer.clear();

    qDebug() << "CommLink::trx(): Writing command '" << command << "'";
    sp_wait(txEventSet, 2000);
    res = sp_nonblocking_write(serialPort, command.toStdString().c_str(), command.length());
    if(res < 0) {
        qDebug() << "CommLink::trx(): Failed to write command";
        return "";
    }
    sp_drain(serialPort);
    qDebug() << "CommLink::trx(): Wrote " << res << " byte(s)";

    sp_wait(rxEventSet, 2000);
    if(sp_input_waiting(serialPort) == 0) {
        qDebug() << "CommLink::trx(): No RX bytes waiting";
    }
    else {
        do {
            res = sp_nonblocking_read(serialPort, readBuffer, sizeof(readBuffer));
            if(res < 0) {
                qDebug() << "CommLink::trx(): Failed to read";
                return "";
            }
            else if(res == 0) {
                qDebug() << "CommLink::trx(): No more bytes to read";
            }
            qDebug() << "CommLink::trx(): Read " << res << " byte(s): '" << readBuffer << "'";
            buffer.append(readBuffer);
            memset(readBuffer, 0, sizeof(readBuffer));
        } while(sp_input_waiting(serialPort) > 0);
    }
    qDebug() << "CommLink::trx(): Total read buffer: '" << buffer << "'";
    return buffer;
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
        buffer = trx(" Z Z");
        qDebug() << "CommLink::tunerSync(): Total read buffer: '" << buffer << "'";
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

QString CommLink::toggleAntenna() {
//    qDebug() << "CommLink::toggleAntenna(): Function called, syncing tuner";
//    if(!tunerSync()) {
//        qDebug() << "CommLink::toggleAntenna(): Failed to sync tuner";
//        return "";
//    }

    qDebug() << "CommLink::toggleAntenna(): Toggling antenna";
    buffer = trx(" Z A");
    qDebug() << "CommLink::toggleAntenna(): Response: '" << buffer << "'";
    return buffer;
}

void CommLink::close() {
    sp_free_event_set(rxEventSet);
    sp_free_event_set(txEventSet);
    sp_close(serialPort);
    sp_free_port(serialPort);
}

QStringList CommLink::enumerateDevices() {
    // get a list of serial devices that we can use
    QStringList serialPorts;
    struct sp_port **spList;
    enum sp_return spResult = sp_list_ports(&spList);
    if(spResult != SP_OK) {
        qDebug() << "CommLink::enumerateDevices(): Failed to list ports";
        return {};
    }

    // go through them and put them into the string list
    for(int i = 0; spList[i] != NULL; i++) {
        struct sp_port *spPort = spList[i];
        char *spPortName = sp_get_port_name(spPort);
        serialPorts.append(spPortName);
    }

    return serialPorts;
}
