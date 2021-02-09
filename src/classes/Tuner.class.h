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
  int TUNER_CPU_STATE_BYPASS = 0;
  int TUNER_CPU_STATE_AUTO = 1;
  int TUNER_CPU_STATE_MANUAL = 2;
  bool syncState;
  int cpuState;
  std::string rawCommand(const char *commandChar);
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
