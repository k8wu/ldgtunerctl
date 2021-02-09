/*
 * CommLink.class.cxx - handles serial communications
 * Mike Phipps <qrz@k8wu.me>
 */

#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>

class CommLink {
  // internal variables
  std::string buffer, errorMessage;
  int deviceDescriptor;

  // internal methods
  void setBuffer(std::string data) {
    buffer.assign(data);
  }

  void setErrorMessage(std::string data) {
    errorMessage.assign(data);
  }

  int getDeviceDescriptor() {
    return deviceDescriptor;
  }

  void setDeviceDescriptor(int newDescriptor) {
    if(newDescriptor) {
      deviceDescriptor = newDescriptor;
    }
  }
public:
  std::string getBuffer();
  std::string getErrorMessage();
  std::vector<std::string> enumerateDevices();
  bool setupDevice(std::string devicePath);
  bool readFromDevice();
  bool writeToDevice(std::string dataToWrite);
  void closeDevice();
};

// actual class definitions for public methods
std::string CommLink::getBuffer() {
  return buffer;
}

std::string CommLink::getErrorMessage() {
  return errorMessage;
}

std::vector<std::string> CommLink::enumerateDevices() {
  std::vector<std::string> devices = {};

  // get all character devices in /dev
  DIR *dir;
  struct dirent *ent;
  if((dir = opendir("/dev")) != NULL) {
    // print all the files and directories within directory
    while((ent = readdir(dir)) != NULL) {
      if(ent->d_type == DT_CHR) {
        std::string realDeviceName = "/dev/";
        realDeviceName.insert(realDeviceName.length(), ent->d_name);

        // try to open this device and get termios (nonblock because macOS hangs otherwise)
        int potentialSerialPort = open(realDeviceName.c_str(), O_RDWR | O_NONBLOCK);
        if(potentialSerialPort >= 0) {
          struct termios tty;
          if(tcgetattr(potentialSerialPort, &tty) == 0) {
            // maybe a serial device, maybe not - let the user decide later
            devices.push_back(realDeviceName);
          }
          close(potentialSerialPort);
        }
      }
    }
    closedir(dir);
  }

  // here's a list of what we found
  return devices;
}

bool CommLink::setupDevice(std::string devicePath) {
  // open serial port or give up
  int serialPort = open(devicePath.c_str(), O_RDWR);
  if(serialPort < 0) {
    errorMessage.assign(strerror(errno));
    return false;
  }
  struct termios tty;

  // see if the defaults work, and bail if not
  if(tcgetattr(serialPort, &tty) != 0) {
    errorMessage.assign(strerror(errno));
    return false;
  }

  // set 38400 8N1, no flow control
  cfsetispeed(&tty, B38400); // set both input/output to 38400
  cfsetospeed(&tty, B38400);
  tty.c_cflag &= ~CSIZE;     // 8 bits per byte
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~PARENB;    // no parity
  tty.c_cflag &= ~CSTOPB;    // 1 stop bit
  tty.c_cflag &= ~CRTSCTS;   // no flow control

  // the following are a bunch of flags that prevent data mangling
  tty.c_cflag |= CREAD | CLOCAL;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;

  // do not block on data reception
  tty.c_cc[VTIME] = 3;
  tty.c_cc[VMIN] = 0;

  // see if the settings work, and bail if not
  if(tcsetattr(serialPort, TCSANOW, &tty) != 0) {
    errorMessage.assign(strerror(errno));
    return false;
  }

  // good enough - save the file descriptor ID and return
  deviceDescriptor = serialPort;
  return true;
}

bool CommLink::readFromDevice() {
  // do a direct read to a temporary buffer
  char tempBuffer[256];
  memset(tempBuffer, 0x00, sizeof(tempBuffer));
  int numBytesRead = read(deviceDescriptor, &tempBuffer, sizeof(tempBuffer));
  if(numBytesRead < 0) {
    errorMessage.assign(strerror(errno));
    buffer.assign("");
    return false;
  }
  else if(numBytesRead == 0) {
    errorMessage.assign("No bytes read");
    buffer.assign("");
    return true;
  }
  buffer.assign(tempBuffer);
  errorMessage.assign("");
  return true;
}

bool CommLink::writeToDevice(std::string dataToWrite) {
  // can't do anything with an empty string
  if(dataToWrite.length() == 0) {
    errorMessage.assign("Empty write string given - not writing");
    return false;
  }

  // wake up the device
  char wakeUpArray[] = "    ";
  write(deviceDescriptor, wakeUpArray, sizeof(wakeUpArray));
  usleep(50000);

  // write the actual data
  char dataCharArray[dataToWrite.length()];
  strncpy(dataCharArray, dataToWrite.c_str(), dataToWrite.length());
  write(deviceDescriptor, dataCharArray, sizeof(dataCharArray));
  buffer.assign("");
  errorMessage.assign("");
  // usleep(200000);
  return true;
}

void CommLink::closeDevice() {
  buffer.assign("");
  errorMessage.assign("");
  close(deviceDescriptor);
}
