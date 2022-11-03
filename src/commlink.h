#ifndef COMMLINK_H
#define COMMLINK_H

#include <QDebug>
#include <cstddef>
#include <QStringList>
#include <string>
#include <iostream>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "appconfig.h"

class CommLink
{
    // variables
    QString serialDevice; // name of the port
    QSerialPort serialPort; // instance of the port
    QString buffer;
    QString syncResponseString = "000000000000000AzAz"; // LDG standard sync response string

    // private methods
    bool setup();
    QString trx(QString command, unsigned int rxTimeout = 2000);
public:
    CommLink(QString serialDevice);
    bool tunerSync();
    QString doMemoryTune();
    QString doFullTune();
    QString setAuto();
    QString setManual();
    QString toggleBypass();
    QString toggleAntenna();
    void close();
    static QStringList enumerateDevices();
};

#endif // COMMLINK_H
