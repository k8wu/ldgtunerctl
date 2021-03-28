#include "windowabout.h"

WindowAbout::WindowAbout(QWidget *parent) : QWidget(parent)
{
    // set fonts
    qDebug() << "WindowAbout::WindowAbout(): Initializing fonts";
    QFont normalFont("Helvetica", 12, QFont::Normal);
    QFont titleLabelFont("Helvetica", 16, QFont::Bold);

    // containing window properties
    qDebug() << "WindowAbout::WindowAbout(): Setting up main window";
    setFixedSize(320, 240);
    setWindowTitle("About...");

    // widgets
    qDebug() << "WindowAbout::WindowAbout(): Initializing widgets";
    programTitleLabel = new QLabel(PROGRAM_TITLE "\n" PROGRAM_VERSION, this);
    programTitleLabel->setGeometry(20, 0, 280, 80);
    programTitleLabel->setFont(titleLabelFont);
    programTitleLabel->setAlignment(Qt::AlignCenter);

    programInfoLabel = new QLabel("By " PROGRAM_AUTHOR"," PROGRAM_AUTHOR_CALLSIGN"\n" PROGRAM_AUTHOR_EMAIL"\n" PROGRAM_AUTHOR_WEBSITE, this);
    programInfoLabel->setGeometry(20, 80, 280, 100);
    programInfoLabel->setFont(normalFont);
    programInfoLabel->setAlignment(Qt::AlignCenter);

    okButton = new QPushButton("OK", this);
    okButton->setGeometry(135, 180, 50, 50);

    qDebug() << "WindowAbout::WindowAbout(): Initializing click action for OK button";
    connect(okButton, SIGNAL(clicked()), this, SLOT(hide()));
}
