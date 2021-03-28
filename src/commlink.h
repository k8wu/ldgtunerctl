#ifndef COMMLINK_H
#define COMMLINK_H

extern "C" {
    #include <libserialport.h>
}

#include <QDebug>
#include <cstddef>
#include <QStringList>
#include <string>
#include <iostream>

#include "appconfig.h"

class CommLink
{
    // variables
    QString serialDevice;       // UNIX device path
    struct sp_port* serialPort; // libserialport instance
    struct sp_event_set* rxEventSet;
    struct sp_event_set* txEventSet;
    QString buffer;
    QString syncResponseString = "000000000000000AzAz"; // LDG standard sync response string

    // private methods
    bool setup();
    QString trx(QString command);
public:
    CommLink(QString serialDevice);
    bool tunerSync();
    QString toggleAntenna();
    void close();
    static QStringList enumerateDevices();
};

#endif // COMMLINK_H
