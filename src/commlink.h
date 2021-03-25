#ifndef COMMLINK_H
#define COMMLINK_H

#include <cstddef>
#include <QStringList>
#include <string>
#include <dirent.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

#include "appconfig.h"

class CommLink
{
    QString serialDevice;
public:
    CommLink(QString serialDevice);
    static QStringList enumerateDevices();
};

#endif // COMMLINK_H
