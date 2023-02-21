#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QBoxLayout>
#include <iostream>

#include "displayer.hpp"
#include "audio_input.hpp"
#include "tone_utils.hpp"

// FIXME: into a config object
const double CONCERT_PITCH = 440.0;

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

    this->toneLabel = new QLabel("?", this);
    this->toneLabel->setMargin(20);
    this->toneLabel->setAlignment(Qt::AlignCenter);

    this->toneLabel->setFont(toneFont);


    vbox->setAlignment(Qt::AlignHCenter);

    vbox->addWidget(this->toneLabel);
    vbox->addWidget(this->frequencyLabel);

    setLayout(vbox);
	setMinimumSize(200, 200);
}

void Displayer::setFrequencies(double real, double desired) {
    // FIXME: this Qstring crap is awful
    std::string displayedString = std::to_string(real) + " / " + std::to_string(desired);
    this->frequencyLabel->setText(QString::fromStdString(displayedString));
}

void Displayer::setNote(std::string& note) {
    this->toneLabel->setText(QString::fromStdString(note));
}

void Displayer::showPitch(float number) {
    std::pair<std::string, double> pitchNote = getClosestPitch(number, CONCERT_PITCH);
    this->setFrequencies(number, pitchNote.second);
    this->setNote(pitchNote.first);
}
