#ifndef __DISPLAYER_HPP
#define __DISPLAYER_HPP

#include <gtkmm/window.h>
#include <gtkmm/label.h>

class Displayer : public Gtk::Window {
    public:
        Displayer();
        virtual ~Displayer();
    
    protected:
        Gtk::Label displayed_frequency;
};

#endif