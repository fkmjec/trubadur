#include <gtkmm.h>
#include <displayer.hpp>

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.examples.base");

  return app->make_window_and_run<Displayer>(argc, argv);
  
}