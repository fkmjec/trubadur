#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include "displayer.hpp"

Displayer::Displayer(QWidget * parent) : QWidget(parent) {
    // TODO: add name setting from above
	setWindowTitle("Ladicka");
    auto grid = new QGridLayout(this);
    this->shown_text = new QLabel("Tady bude frekvence", this);
    grid->addWidget(this->shown_text, 0, 1, 1, 1);
    setLayout(grid);
	setMinimumSize(200, 200);
}

void Displayer::show_number(double number) {
    this->frequency = number;
    this->shown_text->setNum(this->frequency);
}
