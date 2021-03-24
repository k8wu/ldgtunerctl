#include "commlink.h"

CommLink::CommLink(QString serialDevice)
{
    this->serialDevice = serialDevice;
}

QStringList CommLink::enumerateDevices() {
    // serial device naming for FreeBSD
#ifdef FREEBSD
    const char* serialDevicePrefixes[] = {
        "cua"
    };
#endif

    // serial device naming for macOS
#ifdef MACOS
    const char* serialDevicePrefixes[] = {
        "cu."
    };
#endif

    // serial device naming for Linux
#ifdef LINUX
    const char* serialDevicePrefixes[] = {
        "ttyS",
        "ttyACM",
        "ttyUSB"
    };
#endif
    QStringList devices = {};

    // get all character devices in /dev
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir("/dev")) != NULL) {
        // print all the files and directories within directory
        while((ent = readdir(dir)) != NULL) {
            if(ent->d_type == DT_CHR) {
                // check to see if this is actually a serial device
                bool isSerialDevice = false;
                for(const char* prefix : serialDevicePrefixes) {
                    if(strstr(ent->d_name, prefix)) {
                        isSerialDevice = true;
                    }
                }

                // if it passed, check if we can set basic serial options on it
                if(isSerialDevice) {
                    QString realDeviceName = "/dev/";
                    realDeviceName.insert(realDeviceName.length(), ent->d_name);
                    int potentialSerialPort = open(realDeviceName.toStdString().c_str(), O_RDWR | O_NONBLOCK);
                    if(potentialSerialPort >= 0) {
                        struct termios tty;
                        if(tcgetattr(potentialSerialPort, &tty) == 0) {
                            devices << realDeviceName;
                        }
                        close(potentialSerialPort);
                    }
                }
            }
        }
    closedir(dir);
  }

  // here's a list of what we found
  return devices;
}
