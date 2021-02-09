/*
 * main.cxx - entry point for ldgtunerctl
 * Mike Phipps <qrz@k8wu.me>
 */

#include "appConfig.h"
#include <string>
#include <vector>
#include "classes/Views.class.h"
#include "classes/CommLink.class.h"
#include "classes/Tuner.class.h"
#include <iostream>

int main(int argc, char **argv) {
  // get a list of serial ports
  CommLink* commLink = new CommLink();
  std::vector<std::string> detectedDevices = commLink->enumerateDevices();
  if(detectedDevices.empty()) {
    std::cout << "Devices not detected" << std::endl;
    return 1;
  }

  // open a dialog and let the user choose one of the devices
  Views *views = new Views();
  views->serialPortSelectionDisplay(detectedDevices);

  // if the user did not choose a port, they probably wanted to exit
  if(views->getSelectedDevice().length() == 0) {
    return 0;
  }

  // set up the chosen device
  if(!commLink->setupDevice(views->getSelectedDevice())) {
    std::cout << "Device " << views->getSelectedDevice() << " failed setup" << std::endl;
    return 2;
  }
  else {
    std::cout << "Using device " << views->getSelectedDevice() << std::endl;
  }

  // set up a tuner object if the serial stuff was fine and we have a valid port
  Tuner* masterTuner = new Tuner();
  masterTuner->setCommLink(commLink);
  if(!masterTuner->commSync()) {
    std::cout << "Failed to get synced with the tuner" << std::endl;
    commLink->closeDevice();
    return 3;
  }

  // set the tuner and call the main display
  views->setTuner(masterTuner);
  views->mainDisplay();

  // if we ever get here...
  commLink->closeDevice();
  std::cout << "Program ending normally" << std::endl;
  return 0;
}
