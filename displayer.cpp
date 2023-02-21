#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QBoxLayout>
#include <iostream>

#include "displayer.hpp"
#include "audio_input.hpp"


Displayer::Displayer(QWidget * parent) : QWidget(parent) {
    // TODO: add name setting from above
	setWindowTitle("Ladicka");
    auto vbox = new QVBoxLayout(this);

    QFont frequencyFont("Arial", 20, QFont::Bold);

    this->frequencyLabel = new QLabel("Frequency TBD", this);
    this->frequencyLabel->setMargin(20);
    this->frequencyLabel->setAlignment(Qt::AlignCenter);

    this->frequencyLabel->setFont(frequencyFont);

    QFont toneFont("Arial", 50, QFont::Bold);

    this->toneLabel = new QLabel("A#", this);
    this->toneLabel->setMargin(20);
    this->toneLabel->setAlignment(Qt::AlignCenter);

    this->toneLabel->setFont(toneFont);


    vbox->setAlignment(Qt::AlignHCenter);

    vbox->addWidget(this->toneLabel);
    vbox->addWidget(this->frequencyLabel);



    setLayout(vbox);
	setMinimumSize(200, 200);
}

void Displayer::showNumber(float number) {
    this->frequency = number;
    this->frequencyLabel->setNum(this->frequency);
}
