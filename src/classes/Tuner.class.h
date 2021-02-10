/*
 * Tuner.class.h - controls tuner state (headers)
 * Mike Phipps <qrz@k8wu.me>
 */
#ifndef TUNER_CLASS_H
#define TUNER_CLASS_H

#include <cstddef>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include "CommLink.class.h"

class Tuner {
  CommLink *masterCommLink = new CommLink();
  std::string rawCommand(const char *commandChar, const char *expectedRresponseString, bool isSync = false);
  bool getSyncState();
  void setSyncState(bool newSyncState);
  int getCpuState();
  void setCpuState(bool newCpuState);

public:
  void setCommLink(CommLink *inputCommLink);
  bool commSync();
  bool bypass();
  bool automatic();
  bool manual();
  char tune(bool fullTune = false);
  char toggleAntenna();
};
#endif
