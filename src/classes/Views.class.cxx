/*
 * Views.class.cxx - handles display of program and data
 * Mike Phipps <qrz@k8wu.me>
 */

#include "../appConfig.h"
#include "Tuner.class.h"
#include <iostream>
#include <string>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Select_Browser.H>

class Views {
  // we need a reference to the tuner object (defined upon object construction)
  Tuner* tuner;

  // FLTK widgets for serial port selection display
  Fl_Window* serialPortSelectionWindow = new Fl_Window(160, 280, "Select Serial Port");
  Fl_Select_Browser* serialPortBrowser = new Fl_Select_Browser(10, 10, 140, 150);
  Fl_Button* serialPortChooseButton = new Fl_Button(20, 170, 120, 40, "Choose Port");
  Fl_Button* serialPortQuitButton = new Fl_Button(20, 220, 120, 40, "Quit");

  // FLTK widgets for about display
  Fl_Window* aboutWindow = new Fl_Window(300, 140);
  Fl_Box* aboutTitleBox = new Fl_Box(10, 10, 280, 40);
  Fl_Box* aboutInfoBox = new Fl_Box(10, 50, 280, 80);

  // FLTK widgets for main display
  Fl_Window* mainWindow = new Fl_Window(680, 400);
  Fl_Box* mainTitleBox = new Fl_Box(40, 10, 600, 40, PROGRAM_TITLE);
  Fl_Box* mainDataBox = new Fl_Box(10, 60, 660, 320);
  Fl_Box* lastKnownTuneLabelBox = new Fl_Box(20, 70, 170, 40, "Last Known Tune:");
  Fl_Box* lastKnownTuneDataBox = new Fl_Box(190, 70, 120, 40, "Not tuned yet");
  Fl_Button* memoryTuneButton = new Fl_Button(20, 110, 140, 40, "Memory Tune");
  Fl_Button* fullTuneButton = new Fl_Button(170, 110, 100, 40, "Full Tune");
  Fl_Box* autoManualLabelBox = new Fl_Box(360, 70, 200, 40, "Auto/Manual Control");
  Fl_Button* tunerAutoModeButton = new Fl_Button(360, 110, 70, 40, "Auto");
  Fl_Button* tunerManualModeButton = new Fl_Button(440, 110, 90, 40, "Manual");
  Fl_Button* tunerBypassButton = new Fl_Button(540, 110, 90, 40, "Bypass");
  Fl_Box* antennaLabelBox = new Fl_Box(20, 160, 170, 40, "Antenna Selected:");
  Fl_Box* antennaDataBox = new Fl_Box(190, 160, 40, 40, "?");
  Fl_Button* antennaToggleButton = new Fl_Button(20, 200, 70, 40, "Toggle");
  Fl_Box* statusBox = new Fl_Box(20, 260, 640, 40, "Status not yet known...");
  Fl_Button* aboutButton = new Fl_Button(205, 320, 70, 40, "About");
  Fl_Button* mainExitButton = new Fl_Button(410, 320, 60, 40, "Exit");

  // variables for serial port selection display
  std::string selectedDevice;

  // callbacks for serial port selection display
  static void cbSerialPortSelectionDisplayChoosePortButton(Fl_Widget* widget, void* viewsObjRef) {
    int currentSelection = ((Views*) viewsObjRef)->serialPortBrowser->value();
    if(currentSelection > 0) {
      std::string currentSelectionText = ((Views*) viewsObjRef)->serialPortBrowser->text(currentSelection);
      ((Views*) viewsObjRef)->setSelectedDevice(currentSelectionText);
      ((Views*) viewsObjRef)->serialPortSelectionWindow->hide();
    }
  }

  static void cbSerialPortSelectionDisplayQuitButton(Fl_Widget* widget, void* viewsObjRef) {
    ((Views*) viewsObjRef)->serialPortSelectionWindow->hide();
  }

  // callbacks for main window widgets (static wrappers with dynamic methods)
  static void cbTune(Fl_Widget* widget, void* viewsObjRef) {
    // the calling button's label will determine the tuning operation
    bool fullTune = false;
    if(strcmp(((Fl_Button*) widget)->label(),  "Full Tune")) {
      fullTune = true;
    }
    std::string statusMessage = "Unknown (UNKN)";
    char res = ((Views*) viewsObjRef)->tuner->tune(fullTune);
    switch(res) {
      case 'T':
        ((Views*) viewsObjRef)->statusBox->label("Tune complete - good tune achieved");
        statusMessage.assign("Good ");
        break;

      case 'M':
        ((Views*) viewsObjRef)->statusBox->label("Tune complete - OK tune achieved");
        statusMessage.assign("OK ");
        break;

      case 'F':
        ((Views*) viewsObjRef)->statusBox->label("Tune failed");
        statusMessage.assign("Fail ");
        break;

      default:
        ((Views*) viewsObjRef)->statusBox->label("An unspecified error occurred during tuning");
        statusMessage.assign("Unknown ");
        break;
    }

    // finish the status message and send it to the status box
    statusMessage.insert(statusMessage.length(), (fullTune) ? "(Full)" : "(Auto)");
    ((Views*) viewsObjRef)->lastKnownTuneDataBox->copy_label(statusMessage.c_str());
  }

  static void cbSetAutoMode(Fl_Widget* widget, void* viewsObjRef) {
    ((Views*) viewsObjRef)->tuner->automatic();
    ((Views*) viewsObjRef)->tunerAutoModeButton->color(2);
    ((Views*) viewsObjRef)->tunerAutoModeButton->redraw();
    ((Views*) viewsObjRef)->tunerManualModeButton->color(47);
    ((Views*) viewsObjRef)->tunerManualModeButton->redraw();
    ((Views*) viewsObjRef)->tunerBypassButton->color(47);
    ((Views*) viewsObjRef)->tunerBypassButton->redraw();
    ((Views*) viewsObjRef)->statusBox->label("Tuner is now in automatic tuning mode");
  }

  static void cbSetManualMode(Fl_Widget* widget, void* viewsObjRef) {
    ((Views*) viewsObjRef)->tuner->manual();
    ((Views*) viewsObjRef)->tunerAutoModeButton->color(47);
    ((Views*) viewsObjRef)->tunerAutoModeButton->redraw();
    ((Views*) viewsObjRef)->tunerManualModeButton->color(2);
    ((Views*) viewsObjRef)->tunerManualModeButton->redraw();
    ((Views*) viewsObjRef)->tunerBypassButton->color(47);
    ((Views*) viewsObjRef)->tunerBypassButton->redraw();
    ((Views*) viewsObjRef)->statusBox->label("Tuner is now in manual tuning mode");
  }

  static void cbSetBypassMode(Fl_Widget* widget, void* viewsObjRef) {
    ((Views*) viewsObjRef)->tuner->bypass();
    ((Views*) viewsObjRef)->tunerAutoModeButton->color(47);
    ((Views*) viewsObjRef)->tunerAutoModeButton->redraw();
    ((Views*) viewsObjRef)->tunerManualModeButton->color(47);
    ((Views*) viewsObjRef)->tunerManualModeButton->redraw();
    ((Views*) viewsObjRef)->tunerBypassButton->color(2);
    ((Views*) viewsObjRef)->tunerBypassButton->redraw();
    ((Views*) viewsObjRef)->statusBox->label("Tuner is now in bypass mode");
  }

  static void cbAntennaToggle(Fl_Widget* widget, void* viewsObjRef) {
    char res = ((Views*) viewsObjRef)->tuner->toggleAntenna();
    std::string resString;
    resString.push_back(res);
    ((Views*) viewsObjRef)->antennaDataBox->copy_label(resString.c_str());
    ((Views*) viewsObjRef)->statusBox->label("Antenna toggled");
  }

  static void cbAbout(Fl_Widget* widget, void* viewsObjRef) {
    std::string aboutTitleText = "About ";
    aboutTitleText.insert(aboutTitleText.length(), PROGRAM_TITLE);
    ((Views*) viewsObjRef)->aboutWindow->label(aboutTitleText.c_str());
    std::string aboutTitleBoxText = PROGRAM_TITLE;
    aboutTitleBoxText.insert(aboutTitleBoxText.length(), "\nv");
    aboutTitleBoxText.insert(aboutTitleBoxText.length(), PROGRAM_VERSION);
    ((Views*) viewsObjRef)->aboutTitleBox->copy_label(aboutTitleBoxText.c_str());
    ((Views*) viewsObjRef)->aboutTitleBox->labelsize(20);
    ((Views*) viewsObjRef)->aboutTitleBox->labelfont(FL_BOLD + FL_ITALIC);
    std::string aboutInfoBoxString = "(c) ";
    aboutInfoBoxString.insert(aboutInfoBoxString.length(), PROGRAM_AUTHOR);
    aboutInfoBoxString.insert(aboutInfoBoxString.length(), ", ");
    aboutInfoBoxString.insert(aboutInfoBoxString.length(), PROGRAM_AUTHOR_CALLSIGN);
    aboutInfoBoxString.insert(aboutInfoBoxString.length(), "\n");
    aboutInfoBoxString.insert(aboutInfoBoxString.length(), PROGRAM_AUTHOR_EMAIL);
    ((Views*) viewsObjRef)->aboutInfoBox->copy_label(aboutInfoBoxString.c_str());
    ((Views*) viewsObjRef)->aboutInfoBox->labelsize(16);
    ((Views*) viewsObjRef)->aboutWindow->show();
  }

  static void cbExitProgram(Fl_Widget* widget, void* viewsObjRef) {
    // this needs to be a lot more graceful when more code gets written
    exit(0);
  }

public:
  // set tuner reference object
  void setTuner(Tuner* inputTuner);

  // getter/setter methods for selected device (needed outside this class)
  std::string getSelectedDevice();

  void setSelectedDevice(std::string inputSelectedDevice);

  // serial port selection display
  int serialPortSelectionDisplay(std::vector<std::string> detectedDevices);

  // setup for main window
  int mainDisplay();
};

// actual class definitions for public methods
void Views::setTuner(Tuner* inputTuner) {
  tuner = inputTuner;
}

std::string Views::getSelectedDevice() {
  return selectedDevice;
}

void Views::setSelectedDevice(std::string inputSelectedDevice) {
  selectedDevice = inputSelectedDevice;
}

int Views::serialPortSelectionDisplay(std::vector<std::string> detectedDevices) {
  serialPortChooseButton->labelsize(16);
  serialPortChooseButton->callback(cbSerialPortSelectionDisplayChoosePortButton, this);
  serialPortQuitButton->labelsize(16);
  serialPortQuitButton->callback(cbSerialPortSelectionDisplayQuitButton, this);

  // populate the browser
  for(std::string detectedDevice : detectedDevices) {
    serialPortBrowser->add(detectedDevice.c_str());
  }

  serialPortSelectionWindow->show();
  return Fl::run();
}

int Views::mainDisplay() {
  // draw the root window
  std::string mainWindowString = PROGRAM_TITLE;
  mainWindowString.insert(mainWindowString.length(), " v");
  mainWindowString.insert(mainWindowString.length(), PROGRAM_VERSION);
  mainWindow->label(mainWindowString.c_str());

  // title box
  mainTitleBox->labelfont(FL_BOLD);
  mainTitleBox->labelsize(24);

  // data box
  mainDataBox->box(FL_BORDER_BOX);

  // status of last known tune
  lastKnownTuneLabelBox->align(FL_ALIGN_INSIDE + FL_ALIGN_LEFT);
  lastKnownTuneLabelBox->labelfont(FL_BOLD);
  lastKnownTuneLabelBox->labelsize(16);
  lastKnownTuneDataBox->align(FL_ALIGN_INSIDE + FL_ALIGN_LEFT);
  lastKnownTuneDataBox->labelsize(16);

  // memory tune button
  memoryTuneButton->color(47);
  memoryTuneButton->labelsize(16);
  memoryTuneButton->callback(cbTune, this);

  // full tune button
  fullTuneButton->color(47);
  fullTuneButton->labelsize(16);
  fullTuneButton->callback(cbTune, this);

  // auto/manual label box and buttons
  autoManualLabelBox->align(FL_ALIGN_INSIDE + FL_ALIGN_LEFT);
  autoManualLabelBox->labelfont(FL_BOLD);
  autoManualLabelBox->labelsize(16);
  tunerAutoModeButton->color(47);
  tunerAutoModeButton->labelsize(16);
  tunerAutoModeButton->callback(cbSetAutoMode, this);
  tunerManualModeButton->color(47);
  tunerManualModeButton->labelsize(16);
  tunerManualModeButton->callback(cbSetManualMode, this);
  tunerBypassButton->color(47);
  tunerBypassButton->labelsize(16);
  tunerBypassButton->callback(cbSetBypassMode, this);

  // antenna label box and buttons
  antennaLabelBox->align(FL_ALIGN_INSIDE + FL_ALIGN_LEFT);
  antennaLabelBox->labelfont(FL_BOLD);
  antennaLabelBox->labelsize(16);
  antennaToggleButton->color(47);
  antennaToggleButton->labelsize(16);
  antennaToggleButton->callback(cbAntennaToggle, this);

  // status box
  statusBox->box(FL_BORDER_BOX);
  statusBox->align(FL_ALIGN_INSIDE + FL_ALIGN_LEFT);
  statusBox->labelsize(16);

  // about button
  aboutButton->labelsize(18);
  aboutButton->callback(cbAbout, this);

  // exit button
  mainExitButton->labelsize(18);
  mainExitButton->callback(cbExitProgram, this);

  // stop drawing and listen for input
  mainWindow->end();
  mainWindow->show();
  return Fl::run();
}
