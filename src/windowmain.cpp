#include "windowmain.h"

WindowMain::WindowMain(QWidget* parent) : QWidget(parent)
{
    qDebug() << "WindowMain::WindowMain(): Initializing object instance";

    // containing window properties
    qDebug() << "WindowMain::WindowMain(): Setting up window and widgets";
    setFixedSize(680, 440);
    setWindowTitle(PROGRAM_TITLE " " PROGRAM_VERSION);

    // set up widgets
    QFont normalFont("Helvetica", 12, QFont::Normal);
    QFont headingFont("Helvetica", 12, QFont::Bold);
    QFont titleLabelFont("Helvetica", 20, QFont::Bold);
    titleLabel = new QLabel(PROGRAM_TITLE, this);
    titleLabel->setGeometry(200, 10, 280, 40);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(titleLabelFont);

    tunerOpsBox = new QGroupBox("Tuner Operations", this);
    tunerOpsBox->setGeometry(20, 75, 300, 125);
    tunerOpsBox->setFont(headingFont);

    lastKnownTuneLabel = new QLabel("Last Known Tune:", tunerOpsBox);
    lastKnownTuneLabel->setGeometry(10, 30, 170, 40);
    lastKnownTuneLabel->setFont(normalFont);
    tunerOpsBox->setAlignment(Qt::AlignTop & Qt::AlignLeft);

    lastKnownTuneDataLabel = new QLabel("Not tuned yet", tunerOpsBox);
    lastKnownTuneDataLabel->setGeometry(160, 30, 140, 40);
    lastKnownTuneDataLabel->setFont(normalFont);
    tunerOpsBox->setAlignment(Qt::AlignTop & Qt::AlignLeft);

    memoryTuneButton = new QPushButton("Memory Tune", tunerOpsBox);
    memoryTuneButton->setGeometry(20, 70, 140, 40);
    memoryTuneButton->setFont(normalFont);

    fullTuneButton = new QPushButton("Full Tune", tunerOpsBox);
    fullTuneButton->setGeometry(170, 70, 100, 40);
    fullTuneButton->setFont(normalFont);

    tuneModeBox = new QGroupBox("Auto/Manual Control", this);
    tuneModeBox->setGeometry(360, 75, 300, 125);
    tuneModeBox->setFont(headingFont);

    autoTunerModeButton = new QPushButton("Auto", tuneModeBox);
    autoTunerModeButton->setGeometry(10, 70, 70, 40);
    autoTunerModeButton->setFont(normalFont);

    manualTunerModeButton = new QPushButton("Manual", tuneModeBox);
    manualTunerModeButton->setGeometry(90, 70, 90, 40);
    manualTunerModeButton->setFont(normalFont);

    bypassTunerButton = new QPushButton("Bypass", tuneModeBox);
    bypassTunerButton->setGeometry(190, 70, 90, 40);
    bypassTunerButton->setFont(normalFont);

    antennaSelectionBox = new QGroupBox("Antenna Selection", this);
    antennaSelectionBox->setGeometry(20, 210, 300, 125);
    antennaSelectionBox->setFont(headingFont);

    antennaSelectionLabel = new QLabel("Antenna:", antennaSelectionBox);
    antennaSelectionLabel->setGeometry(10, 30, 100, 40);
    antennaSelectionLabel->setFont(normalFont);

    antennaSelectionDataLabel = new QLabel("?", antennaSelectionBox);
    antennaSelectionDataLabel->setGeometry(100, 30, 40, 40);
    antennaSelectionDataLabel->setFont(normalFont);

    antennaToggleButton = new QPushButton("Toggle", antennaSelectionBox);
    antennaToggleButton->setGeometry(10, 75, 70, 40);
    antennaToggleButton->setFont(normalFont);

    otherControlsBox = new QGroupBox("Other Controls", this);
    otherControlsBox->setGeometry(360, 210, 300, 125);
    otherControlsBox->setFont(headingFont);

    configButton = new QPushButton("Config", otherControlsBox);
    configButton->setGeometry(10, 70, 70, 40);
    configButton->setFont(normalFont);

    aboutButton = new QPushButton("About", otherControlsBox);
    aboutButton->setGeometry(115, 70, 70, 40);
    aboutButton->setFont(normalFont);

    exitButton = new QPushButton("Exit", otherControlsBox);
    exitButton->setGeometry(220, 70, 70, 40);
    exitButton->setFont(normalFont);

    statusBox = new QGroupBox("Status", this);
    statusBox->setGeometry(20, 345, 640, 65);
    statusBox->setFont(headingFont);

    statusLabel = new QLabel("Nothing to report... yet", statusBox);
    statusLabel->setGeometry(10, 20, 620, 50);
    statusLabel->setFont(normalFont);

    // button actions
    connect(exitButton, SIGNAL(clicked()), this, SLOT(slotShutdown()));
    connect(antennaToggleButton, SIGNAL(clicked()), this, SLOT(slotToggleAntenna()));
}

void WindowMain::slotGetSerialDevice(QString chosenSerialDevice) {
    qDebug() << "WindowMain::slotGetSerialDevice(): Receiving serial device selection";
    this->serialDevice = chosenSerialDevice;
}

void WindowMain::slotShowWindowMain() {
    qDebug() << "WindowMain::slotShowWindowMain(): Receiving request to show this window";
    this->show();

    // initial tuner sync
    commLink = new CommLink(this->serialDevice);
    if(commLink->tunerSync()) {
        qDebug() << "WindowMain::slotShowWindowMain(): Tuner synced, updating status";
        statusLabel->setText("Tuner synced");
    }
    else {
        qDebug() << "WindowMain::slotShowWindowMain(): Tuner not synced, updating status";
        statusLabel->setText("Tuner not synced");
    }
}

void WindowMain::slotToggleAntenna() {
    qDebug() << "WindowMain::slotToggleAntenna(): Function called, toggling antenna";
    QString res = commLink->toggleAntenna();
    antennaSelectionDataLabel->setText(res);
    qDebug() << "WindowMain::slotToggleAntenna(): Result: '" << res << "'";
}

void WindowMain::slotShutdown() {
    qDebug() << "WindowMain::slotShutdown(): Function called, shutting down";
    commLink->close();
    QApplication::instance()->quit();
}
