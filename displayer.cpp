#include "displayer.hpp"

Displayer::Displayer() : displayed_frequency("125") {
    // TODO: add name setting from above
    set_title("Ladicka");
    set_child(displayed_frequency);
}

Displayer::~Displayer() {
}