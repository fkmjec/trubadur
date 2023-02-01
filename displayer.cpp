#include "displayer.hpp"
#include <QMainWindow>

Displayer::Displayer(QWidget * parent) : QMainWindow(parent) {
    // TODO: add name setting from above
	setWindowTitle("Ladicka");

	setMinimumSize(200, 200);
}
