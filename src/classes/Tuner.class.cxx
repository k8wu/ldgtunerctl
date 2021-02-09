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

  // some handy definitions
  int TUNER_CPU_STATE_BYPASS = 0;
  int TUNER_CPU_STATE_AUTO = 1;
  int TUNER_CPU_STATE_MANUAL = 2;

  // state variables
  bool syncState;  // sync state
  int cpuState;    // 0 = passthru/bypass, 1 = auto tuning, 2 = manual tuning

  // tuner command template
  std::string rawCommand(const char *commandChar) {
    // keep sending the command to the tuner until it responds or times out
    std::string receivedString;
    for(int attempt = 0; attempt < 10; attempt++) {
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

  // get or change sync state
  bool getSyncState() {
    return syncState;
  }

  void setSyncState(bool newSyncState) {
    syncState = newSyncState;
  }

  // get or change CPU state
  int getCpuState() {
    return cpuState;
  }

  void setCpuState(int newCpuState) {
    cpuState = newCpuState;
  }

public:
  // the constructor is where we get the communications link object
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
  // reset sync state since we need to do it again anyway
  setSyncState(false);
  for(int attempt = 0; attempt < 5; attempt++) {
    const char *cmdChar = "Z";
    std::string receivedSyncString = rawCommand(cmdChar);
    if(receivedSyncString.length() > 0 && receivedSyncString.compare("000000000000000AzAz") == 0) {
      setSyncState(true);
      return true;
    }
  }

  // no response
  return false;
}

bool Tuner::bypass() {
  for(int attempt = 0; attempt < 5; attempt++) {
    const char *cmdChar = "P";
    std::string receivedSyncString = rawCommand(cmdChar);
    if(receivedSyncString.length() > 0 && receivedSyncString.compare("P") == 0) {
      setCpuState(TUNER_CPU_STATE_BYPASS);
      return true;
    }
  }

  // no response
  return false;
}

bool Tuner::automatic() {
  for(int attempt = 0; attempt < 5; attempt++) {
    const char *cmdChar = "C";
    std::string receivedSyncString = rawCommand(cmdChar);
    if(receivedSyncString.length() > 0 && receivedSyncString.compare("A") == 0) {
      setCpuState(TUNER_CPU_STATE_AUTO);
      return true;
    }
  }

  // no response
  return false;
}

bool Tuner::manual() {
  for(int attempt = 0; attempt < 5; attempt++) {
    const char *cmdChar = "M";
    std::string receivedSyncString = rawCommand(cmdChar);
    if(receivedSyncString.length() > 0 && receivedSyncString.compare("M") == 0) {
      setCpuState(TUNER_CPU_STATE_MANUAL);
      return true;
    }
  }

  // no response
  return false;
}

char Tuner::tune(bool fullTune /* = false */) {
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
