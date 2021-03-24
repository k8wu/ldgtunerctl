#ifndef WINDOWSELECTDEVICE_H
#define WINDOWSELECTDEVICE_H

#include <QApplication>
#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <iostream>

#include "appconfig.h"
#include "commlink.h"

class WindowSelectDevice : public QWidget
{
    Q_OBJECT

    // widgets
    QListView* browser;
    QPushButton* chooseButton;
    QPushButton* exitButton;

    // variables
    QString serialDevice;
    CommLink* commLink;

signals:
    void sigSerialDeviceChosen(QString chosenSerialDevice);
    void sigShowWindowMain();
public:
    explicit WindowSelectDevice(QWidget* parent = nullptr);
    void setSerialDevice(QString serialDevice);
    QString getSerialDevice();
public slots:
    void handleSelectedDevice();

signals:

};

#endif // WINDOWSELECTDEVICE_H
