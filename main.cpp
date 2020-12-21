#include "gui.h"
#include "gtkmm/grid.h"
#include <gtkmm/application.h>
#include <set>

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    Othello othello;
    othello.set_property("resizable", false);
    return app->run(othello);
}
