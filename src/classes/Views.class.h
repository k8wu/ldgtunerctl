/*
 * Views.class.h - handles display of program and data (headers)
 * Mike Phipps <qrz@k8wu.me>
 */
#ifndef _VIEWS_CLASS_H
#define _VIEWS_CLASS_H
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
  Tuner* tuner;
  Fl_Window* serialPortSelectionWindow = new Fl_Window(160, 280, "Select Serial Port");
  Fl_Select_Browser* serialPortBrowser = new Fl_Select_Browser(10, 10, 140, 150);
  Fl_Button* serialPortChooseButton = new Fl_Button(20, 170, 120, 40, "Choose Port");
  Fl_Button* serialPortQuitButton = new Fl_Button(20, 220, 120, 40, "Quit");
  Fl_Window* aboutWindow = new Fl_Window(300, 140);
  Fl_Box* aboutTitleBox = new Fl_Box(10, 10, 280, 40);
  Fl_Box* aboutInfoBox = new Fl_Box(10, 50, 280, 80);
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
  std::string selectedDevice;
  static void cbSerialPortSelectionDisplayChoosePortButton(Fl_Widget* widget, void* viewsObjRef);
  static void cbSerialPortSelectionDisplayQuitButton(Fl_Widget* widget, void* viewsObjRef);
  static void cbTune(Fl_Widget* widget, void* viewsObjRef);
  static void cbSetAutoMode(Fl_Widget* widget, void* viewsObjRef);
  static void cbSetManualMode(Fl_Widget* widget, void* viewsObjRef);
  static void cbSetBypassMode(Fl_Widget* widget, void* viewsObjRef);
  static void cbAntennaToggle(Fl_Widget* widget, void* viewsObjRef);
  static void cbAbout(Fl_Widget* widget, void* viewsObjRef);
  static void cbExitProgram(Fl_Widget* widget, void* viewsObjRef);

public:
  void setTuner(Tuner* inputTuner);
  std::string getSelectedDevice();
  void setSelectedDevice(std::string inputSelectedDevice);
  int serialPortSelectionDisplay(std::vector<std::string> detectedDevices);
  int mainDisplay();
};
#endif
