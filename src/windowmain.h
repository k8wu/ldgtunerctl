#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QApplication>
#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <iostream>

#include "appconfig.h"
#include "commlink.h"

class WindowMain : public QWidget
{
    Q_OBJECT

    // widgets
    QLabel* titleLabel;
    QGroupBox* tunerOpsBox;
    QLabel* lastKnownTuneLabel;
    QLabel* lastKnownTuneDataLabel;
    QPushButton* memoryTuneButton;
    QPushButton* fullTuneButton;
    QGroupBox* tuneModeBox;
    QPushButton* autoTunerModeButton;
    QPushButton* manualTunerModeButton;
    QPushButton* bypassTunerButton;
    QGroupBox* antennaSelectionBox;
    QLabel* antennaSelectionLabel;
    QLabel* antennaSelectionDataLabel;
    QPushButton* antennaToggleButton;
    QGroupBox* otherControlsBox;
    QPushButton* configButton;
    QPushButton* aboutButton;
    QPushButton* exitButton;
    QGroupBox* statusBox;
    QLabel* statusLabel;

    // variables
    QString serialDevice;
    CommLink* commLink;
    QColor defaultButtonBackground;

public:
    explicit WindowMain(QWidget* parent = nullptr);
public slots:
    void slotGetSerialDevice(QString chosenSerialDevice);
    void slotShowWindowMain();
    void slotDoMemoryTune();
    void slotDoFullTune();
    void slotSetAuto();
    void slotSetManual();
    void slotToggleBypass();
    void slotToggleAntenna();
    void slotShutdown();
};

#endif // WINDOWMAIN_H
