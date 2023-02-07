#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <iostream>

#include "displayer.hpp"
#include "audio_input.hpp"

Displayer::Displayer(QWidget * parent) : QWidget(parent) {
    // TODO: add name setting from above
	setWindowTitle("Ladicka");
    auto grid = new QGridLayout(this);
    this->shownText = new QLabel("Tady bude frekvence", this);
    grid->addWidget(this->shownText, 0, 1, 1, 1);
    setLayout(grid);
	setMinimumSize(200, 200);
}

void Displayer::showNumber(float number) {
    this->frequency = number;
    this->shownText->setNum(this->frequency);
}
