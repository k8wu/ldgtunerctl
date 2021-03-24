#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <iostream>

#include "appconfig.h"

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
public:
    explicit WindowMain(QWidget* parent = nullptr);
public slots:
    void slotGetSerialDevice(QString chosenSerialDevice);
    void slotShowWindowMain();
};

#endif // WINDOWMAIN_H
