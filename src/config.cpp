#include "config.h"

// private constructor - disallows object creation
Config::Config() {

}

// static - ensures that we always hand out the same instance, since we can create one here in the class
Config& Config::getInstance() {
    qDebug() << "Config::getInstance(): Static configuration instance requested";
    static Config instance;
    return instance;
}

QString Config::getConfFileName() {
    return confFileName;
}

bool Config::parseConfFile(QString thisConfFileName) {
    // try opening the file first
    confFileName = thisConfFileName;
    QFile confFile(confFileName);
    if(!confFile.open(QIODevice::ReadOnly)) {
        configLoadedFromFile = false;
        qDebug() << "Config::parseConfFile(): Could not open config file (" << confFile.errorString() << ")";
        return false;
    }

    // read all data in one go
    QString confData = confFile.readAll();
    confFile.close();
    qDebug() << "Config::parseConfFile(): Data read from file";

    // go line by line
    for(QString confLine : confData.split("\n")) {
        qDebug() << "Config::parseConfFile(): Reading next line (" << confLine << ")";
        // we should check for a "=" first before trying to split, or else it will bomb
        if(confLine.indexOf("=") >= 0) {
            QStringList confLineData = confLine.split("=");
            QString confLineKey = confLineData[0].trimmed();
            QString confLineValue = confLineData[1].trimmed();
            qDebug() << "Config::parseConfFile(): Key: " << confLineKey << ", value: " << confLineValue;

            // check for known configuration keys
            if(confLineKey == "SerialDevice") {
                // this will be a string - easy
                serialDevice = confLineValue;
                qDebug() << "Config::parseConfFile(): Processed SerialDevice";
            }
            else if(confLineKey == "LastWindowPosition") {
                // this will be four space separated values
                QStringList thisWindowPositionList = confLineValue.split(" ");
                if(thisWindowPositionList.length() < 4) {
                    qDebug() << "Config::parseConfFile(): Invalid number of window positions - not processing";
                    configLoadedFromFile = false;
                    return false;
                }
                else {
                    lastWindowPosition = {};
                    for(QString windowPosition : thisWindowPositionList) {
                        lastWindowPosition.push_back(windowPosition.toInt());
                    }
                    qDebug() << "Config::parseConfFile(): Processed LastWindowPosition";
                }
            }
        }
        else {
            qDebug() << "Config::parseConfFile(): Line does not contain valid configuration data";
        }
    }

    // last check to make sure that the variables that should be set are actually set
    if(serialDevice.length() == 0 || lastWindowPosition.length() == 0) {
        qDebug() << "Config::parseConfFile(): Not all of the configuration was loaded";
        configLoadedFromFile = false;
        return false;
    }

    // getting here means everything is OK
    qDebug() << "Config::parseConfFile(): Successfully loaded config from file";
    configLoadedFromFile = true;
    return true;
}

bool Config::writeConfFile() {
    // try opening the file first
    QFile confFile(confFileName);
    if(!confFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Config::writeConfFile(): Could not open config file (" << confFile.errorString() << ")";
        return false;
    }

    // assemble the config file based on the current state of this object's variables
    QTextStream ts(&confFile);
    ts << "# Generated by ldgtunerctl on " << QDateTime::currentDateTime().toString(Qt::TextDate) << "\n";
    ts << "SerialDevice = " << serialDevice << "\n";
    ts << "LastWindowPosition =";
    for(qint16 windowPosition : lastWindowPosition) {
        ts << " " << windowPosition;
    }
    ts << "\n";

    // file written - close it
    confFile.close();
    qDebug() << "Config::writeConfFile(): Configuration written successfully to:" << confFile.fileName();
    return true;
}

bool Config::getConfigLoadedFromFile() {
    return configLoadedFromFile;
}

QString Config::getSerialDevice() {
    return serialDevice;
}

void Config::setSerialDevice(QString thisSerialDevice) {
    serialDevice = thisSerialDevice;
}

QVector<qint16> Config::getLastWindowPosition() {
    return lastWindowPosition;
}

// we might write a vector into this object...
void Config::setLastWindowPosition(QVector<qint16> thisWindowPosition) {
    lastWindowPosition = thisWindowPosition;
}

// ... or we might write the coordinates themselves
void Config::setLastWindowPosition(int x, int y, int width, int height) {
    lastWindowPosition.clear();
    for(int val : {x, y, width, height}) {
        lastWindowPosition.push_back(val);
    }
}