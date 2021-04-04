#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QDebug>

class Config
{
    // private constructor - disallows object creation
    Config();

    // private variables
    QString confFileName;
    bool configLoadedFromFile;
    QString serialDevice;
    QVector<qint16> lastWindowPosition;
public:
    Config(Config const&) = delete;
    void operator=(Config const&) = delete;
    static Config& getInstance();
    QString getConfFileName();
    bool parseConfFile(QString thisConfFileName);
    bool writeConfFile();
    bool getConfigLoadedFromFile();
    QString getSerialDevice();
    void setSerialDevice(QString thisSerialDevice);
    QVector<qint16> getLastWindowPosition();
    void setLastWindowPosition(QVector<qint16> thisWindowPosition);
    void setLastWindowPosition(int x, int y, int width, int height);
};

#endif // CONFIG_H
