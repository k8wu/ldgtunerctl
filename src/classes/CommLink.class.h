/*
 * CommLink.class.h - handles serial communications (headers)
 * Mike Phipps <qrz@k8wu.me>
 */
#ifndef COMMLINK_CLASS_H
#define COMMLINK_CLASS_H

#include <cstddef>
#include <stdint.h>
#include <vector>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>

class CommLink {
  std::string buffer, errorMessage;
  int deviceDescriptor;
  void setBuffer(std::string data);
  void setErrorMessage(std::string data);
  int getDeviceDescriptor();
  void setDeviceDescriptor(int newDescriptor);

public:
  std::string getBuffer();
  std::string getErrorMessage();
  std::vector<std::string> enumerateDevices();
  bool setupDevice(std::string devicePath);
  bool readFromDevice();
  bool writeToDevice(std::string dataToWrite);
  void closeDevice();
};
#endif
