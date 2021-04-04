#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <iostream>

#include "appconfig.h"
#include "config.h"
#include "windowselectdevice.h"
#include "windowmain.h"
#include "windowabout.h"

// global defaults
QString debugFile;
bool debugShow = false;
QString confFile = QDir::homePath() + "/.config/ldgtunerctl.conf";

void debugMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& str) {
    QString logLevel;

    // prepend the date and log level
    QString datetime = QDateTime::currentDateTime().toString(Qt::TextDate);
    switch (type) {
    case QtDebugMsg:
        logLevel = QString("[Debug*] ");
        break;
    case QtWarningMsg:
        logLevel = QString("[Warning] ");
        break;
    case QtInfoMsg:
        logLevel = QString("[Info] ");
        break;
    case QtCriticalMsg:
        logLevel = QString("[Critical] ");
        break;
    case QtFatalMsg:
        logLevel = QString("[Fatal] ");
    }

    // if there is a file given, write to it
    if(debugFile.length() > 0) {
        QFile outFile(debugFile);
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << datetime << ": " << logLevel << str << "\n";
        outFile.close();
    }

    // write to stdout as well if that option is enabled
    if(debugShow) {
        std::cout << datetime.toStdString() << ": " << logLevel.toStdString() << str.toStdString() << std::endl;
    }
}

int main(int argc, char **argv) {
    // initialize app
    QApplication app(argc, argv);
    app.setApplicationName(PROGRAM_TITLE);
    app.setApplicationVersion(PROGRAM_VERSION);
    qInstallMessageHandler(debugMessageHandler);

    // check for command line arguments
    QCommandLineParser parser;
    parser.setApplicationDescription("Control program for the LDG AT600ProII and AT1000ProII tuners");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption debugToStdOutOption(QStringList() << "debug-print" << "p", "Write debug output to stdout");
    parser.addOption(debugToStdOutOption);
    QCommandLineOption debugToFileOption(QStringList() << "debug-file" << "f", "Write debug output to specified file", "debugfile");
    parser.addOption(debugToFileOption);
    QCommandLineOption confOption(QStringList() << "conf" << "c", "Specify configuration file to use other than default", "conffile");
    parser.addOption(confOption);

    // process those command line arguments
    parser.process(app);
    if(parser.isSet(debugToStdOutOption)) {
        debugShow = true;
    }
    if(parser.value(debugToFileOption).length() > 0) {
        debugFile = parser.value(debugToFileOption);
    }
    if(parser.value(confOption).length()) {
        confFile = parser.value(confOption);
    }

    // set up config instance
    Config* config = &Config::getInstance();
    if(!config->parseConfFile(confFile)) {
        qDebug() << "main(): Could not parse config file (or it doesn't exist)";
    }

    // initialize all windows, but don't show anything yet
    qDebug() << "main(): Initializing WindowSelectDevice";
    WindowSelectDevice* windowSelectDevice = new WindowSelectDevice();
    qDebug() << "main(): Initializing WindowMain";
    WindowMain* windowMain = new WindowMain();
    qDebug() << "main(): Initializing WindowAbout";
    WindowAbout* windowAbout = new WindowAbout();

    // now we can connect things between the widgets together
    qDebug() << "main(): Initializing button actions for sub windows";
    QObject::connect(windowSelectDevice, SIGNAL(sigSerialDeviceChosen(QString)), windowMain, SLOT(slotGetSerialDevice(QString)));
    QObject::connect(windowSelectDevice, SIGNAL(sigShowWindowMain()), windowMain, SLOT(slotShowWindowMain()));
    QObject::connect(windowMain, SIGNAL(sigShowWindowAbout()), windowAbout, SLOT(show()));

    // if config is loaded successfully, go straight to the main window
    if(config->getConfigLoadedFromFile()) {
        qDebug() << "main(): Config was loaded - showing WindowMain";
        windowMain->slotGetSerialDevice(config->getSerialDevice());
        windowMain->setWindowPosition(config->getLastWindowPosition());
        windowMain->slotShowWindowMain();
    }
    else {
        qDebug() << "main(): Config was not loaded - showing WindowSelectDevice";
        windowSelectDevice->show();
    }

    // loop and/or exit
    qDebug() << "main(): Looping";
    return app.exec();
}
