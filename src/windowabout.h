#ifndef WINDOWABOUT_H
#define WINDOWABOUT_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include "appconfig.h"

class WindowAbout : public QWidget
{
    Q_OBJECT

    // widgets
    QLabel* programTitleLabel;
    QLabel* programInfoLabel;
    QPushButton* okButton;
public:
    explicit WindowAbout(QWidget *parent = nullptr);

signals:

};

#endif // WINDOWABOUT_H
