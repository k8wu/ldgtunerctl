/*
 * Tuner.class.cxx - commands and status for the tuner
 * Mike Phipps <qrz@k8wu.me>
 */

#include <cstddef>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include "CommLink.class.h"

class Tuner {
  // referenced communication link object
  CommLink *masterCommLink = new CommLink();

  // tuner command template
  std::string rawCommand(const char *commandChar, bool isSync = false) {
    // make one attempt unless we are trying to sync
    int attemptMax = 1;
    if(isSync) {
      attemptMax = 10;
    }

    // send the command and (hopefully) get a response
    std::string receivedString;
    for(int attempt = 0; attempt < attemptMax; attempt++) {
      masterCommLink->writeToDevice(commandChar);
      if(!masterCommLink->readFromDevice()) {
        std::cout << "Error while reading from serial device" << std::endl;
        return NULL;
      }
      receivedString.assign(masterCommLink->getBuffer());
      if(receivedString.length() > 0) {
        return receivedString;
      }
    }

    // no response
    return NULL;
  }

public:
  // associate the correct communications link object
  void setCommLink(CommLink *inputCommLink);

  // sync the tuner so we know that it can receive commands
  bool commSync();

  // set tuner bypass
  bool bypass();

  // set tuner to auto mode
  bool automatic();

  // set tuner to manual mode
  bool manual();

  // perform a memory or full tune
  char tune(bool fullTune = false);

  // toggle the antenna selection
  char toggleAntenna();
};

// actual class definitions for public methods
void Tuner::setCommLink(CommLink *inputCommLink) {
  masterCommLink = inputCommLink;
}

bool Tuner::commSync() {
  for(int attempt = 0; attempt < 5; attempt++) {
    const char *cmdChar = "Z";
    std::string receivedSyncString = rawCommand(cmdChar, true);
    if(receivedSyncString.length() > 0 && receivedSyncString.compare("000000000000000AzAz") == 0) {
      return true;
    }
  }

  // no response
  return false;
}

bool Tuner::bypass() {
  commSync();
  const char *cmdChar = "P";
  std::string receivedSyncString = rawCommand(cmdChar);
  if(receivedSyncString.length() > 0 && receivedSyncString.compare("P") == 0) {
    return true;
  }

  // no response
  return false;
}

bool Tuner::automatic() {
  commSync();
  const char *cmdChar = "C";
  std::string receivedSyncString = rawCommand(cmdChar);
  if(receivedSyncString.length() > 0 && receivedSyncString.compare("A") == 0) {
    return true;
  }

  // no response
  return false;
}

bool Tuner::manual() {
  commSync();
  const char *cmdChar = "M";
  std::string receivedSyncString = rawCommand(cmdChar);
  if(receivedSyncString.length() > 0 && receivedSyncString.compare("M") == 0) {
    return true;
  }

  // no response
  return false;
}

char Tuner::tune(bool fullTune /* = false */) {
  commSync();
  const char *commandChar = "T";
  if(fullTune) {
    commandChar = "F";
  }

  // send the command once, since we could be waiting a bit for the result
  std::string receivedString;
  masterCommLink->writeToDevice(commandChar);

  // try a few times to read any response
  for(int attempt = 0; attempt < 10; attempt++) {
    if(masterCommLink->readFromDevice()) {
      receivedString.assign(masterCommLink->getBuffer());
      if(receivedString.length() > 0) {
        char receivedByte = receivedString.c_str()[0];
        return receivedByte;
      }
      else {
        usleep(1000000);
      }
    }

    // sleep and try again if nothing is received
    else {
      usleep(1000000);
    }
  }

  // no response
  return 0x00;
}

char Tuner::toggleAntenna() {
  commSync();
  const char *commandChar = "A";

  // send the command once, since we could be waiting a bit for the result
  std::string receivedString;
  masterCommLink->writeToDevice(commandChar);

  // try a few times to read any response
  for(int attempt = 0; attempt < 10; attempt++) {
    if(masterCommLink->readFromDevice()) {
      receivedString.assign(masterCommLink->getBuffer());
      if(receivedString.length() > 0) {
        char receivedByte = receivedString.c_str()[0];
        return receivedByte;
      }
      else {
        usleep(500000);
      }
    }

    // sleep and try again if nothing is received
    else {
      usleep(500000);
    }
  }

  // no response
  return 0x00;
}
