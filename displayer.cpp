#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QBoxLayout>
#include <iostream>
#include <memory>

#include "displayer.hpp"
#include "audio_input.hpp"
#include "circular_buffer.hpp"
#include "tone_utils.hpp"
#include "config.hpp"


Displayer::Displayer(std::shared_ptr<Config> conf, QWidget * parent) : QWidget(parent) {
	setWindowTitle("Trubadur");
    this->config = conf;

    this->noteBuffer = CircularBuffer<std::string>(config->getNoteBufferSize());

    auto vbox = new QVBoxLayout(this);

    QFont frequencyFont("Arial", 20, QFont::Bold);

    this->frequencyLabel = new QLabel("... / ...", this);
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
    std::string displayedString = std::to_string(real) + " / " + std::to_string(desired);
    this->frequencyLabel->setText(QString::fromStdString(displayedString));
}

void Displayer::setDefaultLabels() {
    this->frequencyLabel->setText("... / ...");
    this->toneLabel->setText("?");
}

bool Displayer::isBufferConsistent() {
    try {
        auto note = this->noteBuffer.getLast(1)[0];
        auto buffer = this->noteBuffer.getLast(this->config->getNoteBufferSize());
        for (auto n : buffer) {
            if (note != n) {
                return false;
            }
        }
        return true;
    } catch (std::invalid_argument& e) {
        return false;
    }
}

void Displayer::setNote(std::string& note) {
    this->toneLabel->setText(QString::fromStdString(note));
}

void Displayer::showPitch(float number) {
    std::pair<std::string, double> pitchNote = getClosestPitch(number, this->config->getConcertPitch());
    std::string note = pitchNote.first;
    this->noteBuffer.append(note);
    if (isBufferConsistent()) {
        this->setFrequencies(number, pitchNote.second);
        this->setNote(pitchNote.first);
    } else {
        setDefaultLabels();
    }
}
