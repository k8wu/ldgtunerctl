#include "windowmain.h"

WindowMain::WindowMain(QWidget* parent) : QWidget(parent)
{
    qDebug() << "WindowMain::WindowMain(): Initializing object instance - loading global config object";
    config = &Config::getInstance();

    // containing window properties
    qDebug() << "WindowMain::WindowMain(): Setting up main window";
    setFixedSize(680, 440);
    setWindowTitle(PROGRAM_TITLE " " PROGRAM_VERSION);

    // set up widgets
    qDebug() << "WindowMain::WindowMain(): Setting up main window widgets";
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

    // the default button color isn't available otherwise
    qDebug() << "WindowMain::WindowMain(): Getting default button color";
    defaultButtonBackground = aboutButton->palette().color(QPalette::Button);

    // button actions
    qDebug() << "WindowMain::WindowMain(): Defining button actions";
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(slotShowWindowAbout()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(slotShutdown()));
    connect(memoryTuneButton, SIGNAL(clicked()), this, SLOT(slotDoMemoryTune()));
    connect(fullTuneButton, SIGNAL(clicked()), this, SLOT(slotDoFullTune()));
    connect(autoTunerModeButton, SIGNAL(clicked()), this, SLOT(slotSetAuto()));
    connect(manualTunerModeButton, SIGNAL(clicked()), this, SLOT(slotSetManual()));
    connect(bypassTunerButton, SIGNAL(clicked()), this, SLOT(slotToggleBypass()));
    connect(antennaToggleButton, SIGNAL(clicked()), this, SLOT(slotToggleAntenna()));
}

void WindowMain::setWindowPosition(QVector<qint16> thisWindowPosition) {
    if(thisWindowPosition.length() == 4) {
        int x = thisWindowPosition.at(0);
        int y = thisWindowPosition.at(1);
        int width = thisWindowPosition.at(2);
        int height = thisWindowPosition.at(3);
        setGeometry(x, y, width, height);
        windowPosition = thisWindowPosition;
        qDebug() << "WindowMain::slotGetWindowPosition(): Window position successfully set:" << x << y << width << height;
    }
    else {
        qDebug() << "WindowMain::slotGetWindowPosition(): Failed to set window position";
    }
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

        // write this into the configuration if it was not loaded from the config file already
        if(!config->getConfigLoadedFromFile()) {
            qDebug() << "WindowMain::slotShowWindowMain(): Copying known good serial device to config object";
            config->setSerialDevice(serialDevice);
            qDebug() << "WIndowMain::slotShowWindowMain(): Copying window position data to config object";
            config->setLastWindowPosition(geometry().left(), geometry().top(), geometry().width(), geometry().height());
            qDebug() << "WindowMain::slotShowWindowMain(): Writing config out now to file: " << config->getConfFileName();
            config->writeConfFile();
        }
    }
    else {
        qDebug() << "WindowMain::slotShowWindowMain(): Tuner not synced, updating status";
        statusLabel->setText("Tuner not synced");
    }
}

void WindowMain::slotDoMemoryTune() {
    qDebug() << "WindowMain::slotDoMemoryTune(): Function called, executing memory tune";
    QString res = commLink->doMemoryTune();
    qDebug() << "WindowMain::slotDoMemoryTune(): Result:" << res;
    if(res == "T") {
        lastKnownTuneDataLabel->setText("Good (Memory)");
    }
    else if(res == "M") {
        lastKnownTuneDataLabel->setText("OK (Memory)");
    }
    else if(res == "F") {
        lastKnownTuneDataLabel->setText("Fail (Memory)");
    }
    else {
        lastKnownTuneDataLabel->setText("Unknown (Memory)");
    }

    // bypass mode is turned off after tuning
    for(QPushButton* thisButton : { bypassTunerButton }) {
        QPalette pal = thisButton->palette();
        pal.setColor(QPalette::Button, defaultButtonBackground);
        thisButton->setAutoFillBackground(true);
        thisButton->setPalette(pal);
    }
}

void WindowMain::slotDoFullTune() {
    qDebug() << "WindowMain::slotDoFullTune(): Function called, executing full tune";
    QString res = commLink->doMemoryTune();
    qDebug() << "WindowMain::slotDoFullTune(): Result:" << res;
    if(res == "T") {
        lastKnownTuneDataLabel->setText("Good (Full)");
    }
    else if(res == "M") {
        lastKnownTuneDataLabel->setText("OK (Full)");
    }
    else if(res == "F") {
        lastKnownTuneDataLabel->setText("Fail (Full)");
    }
    else {
        lastKnownTuneDataLabel->setText("Unknown (Full)");
    }

    // bypass mode is turned off after tuning
    for(QPushButton* thisButton : { bypassTunerButton }) {
        QPalette pal = thisButton->palette();
        pal.setColor(QPalette::Button, defaultButtonBackground);
        thisButton->setAutoFillBackground(true);
        thisButton->setPalette(pal);
    }
}

void WindowMain::slotSetAuto() {
    qDebug() << "WindowMain::slotSetAuto(): Function called, setting auto mode";
    QString res = commLink->setAuto();
    qDebug() << "WindowMain::slotSetAuto(): Result:" << res;
    if(res == "A") {
        // turn the Auto button background green
        QPalette pal = autoTunerModeButton->palette();
        pal.setColor(QPalette::Button, QColor(Qt::green));
        autoTunerModeButton->setAutoFillBackground(true);
        autoTunerModeButton->setPalette(pal);

        // this usually means that the other two in this group go back to default
        for(QPushButton* thisButton : { manualTunerModeButton, bypassTunerButton }) {
            pal = thisButton->palette();
            pal.setColor(QPalette::Button, defaultButtonBackground);
            thisButton->setAutoFillBackground(true);
            thisButton->setPalette(pal);
        }
    }
}

void WindowMain::slotSetManual() {
    qDebug() << "WindowMain::slotSetManual(): Function called, setting auto mode";
    QString res = commLink->setManual();
    qDebug() << "WindowMain::slotSetManual(): Result:" << res;
    if(res == "M") {
        // turn the Manual button background green
        QPalette pal = manualTunerModeButton->palette();
        pal.setColor(QPalette::Button, QColor(Qt::green));
        manualTunerModeButton->setAutoFillBackground(true);
        manualTunerModeButton->setPalette(pal);

        // this usually means that the other two in this group go back to default
        for(QPushButton* thisButton : { autoTunerModeButton, bypassTunerButton }) {
            pal = thisButton->palette();
            pal.setColor(QPalette::Button, defaultButtonBackground);
            thisButton->setAutoFillBackground(true);
            thisButton->setPalette(pal);
        }
    }
}

void WindowMain::slotToggleBypass() {
    qDebug() << "WindowMain::slotToggleBypass(): Function called, setting bypass mode";
    QString res = commLink->toggleBypass();
    qDebug() << "WindowMain::slotToggleBypass(): Result:" << res;
    if(res == "P") {
        // turn the Bypass button green
        QPalette pal = bypassTunerButton->palette();
        pal.setColor(QPalette::Button, QColor(Qt::green));
        bypassTunerButton->setAutoFillBackground(true);
        bypassTunerButton->setPalette(pal);
        bypassTunerButton->update();
    }
}

void WindowMain::slotToggleAntenna() {
    qDebug() << "WindowMain::slotToggleAntenna(): Function called, toggling antenna";
    QString res = commLink->toggleAntenna();
    antennaSelectionDataLabel->setText(res);
    qDebug() << "WindowMain::slotToggleAntenna(): Result:" << res;
}

void WindowMain::slotShowWindowAbout() {
    qDebug() << "WindowMain::slotShowWindowAbout(): Function called, sending to main";
    emit(sigShowWindowAbout());
}

void WindowMain::slotShutdown() {
    qDebug() << "WindowMain::slotShutdown(): Function called - writing config and shutting down";
    commLink->close();
    config->setLastWindowPosition(windowPosition);
    config->writeConfFile();
    QApplication::instance()->quit();
}
